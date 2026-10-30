# Test matrix allocator (T01) và harness trang (T02) — Khánh

Issue: #19 (N03). Viết ngày 30/10/2026, trước khi API object (S02) được chốt.

## 1. Cách kích hoạt

Harness tầng trang nằm ở `kernel/kalloctest.c`, chỉ được biên dịch và gọi khi bật cờ `KALLOCTEST`. Build mặc định không thay đổi.

```bash
make clean
make qemu CPUS=1 KALLOCTEST=1                       # chạy thật
make clean
make qemu CPUS=1 KALLOCTEST=1 KALLOCTEST_INJECT=1   # tự kiểm tra: cố tình gây lỗi
```

- Harness chạy trên CPU 0 ngay sau `kinit()`, trước khi CPU khác khởi động và trước mọi cấp phát khác (trạng thái yên), nên số trang đếm được là chính xác.
- Mọi so sánh số trang dùng `before = kfreepage_count()` đếm lúc bắt đầu, không viết cứng số trang lúc boot, vì số này đổi theo kích thước kernel (thêm harness đã làm số trang từ 32735 xuống 32734).
- `KALLOCTEST_INJECT=1` cố tình gây 3 lỗi (cấp trùng trên bản sao, đè 1 ô nhớ, giữ lại 1 trang) để chứng minh harness báo `FAIL` khi sai. Lỗi giả được dọn ngay sau khi đo, hệ thống vẫn boot bình thường.
- W02 chỉ chạy `CPUS=1`. Nhiều CPU và stealing là W03 (P05 trong backlog).

## 2. Ca kiểm thử tầng trang (`kalloc`/`kfree`)

N = 64 trang sống cùng lúc. Kết quả chạy 30/10 tại revision `66d5874`.

| ID | Kiểm tra | Cách làm | Expected | Điều kiện FAIL | Kết quả 30/10 |
|---|---|---|---|---|---|
| P01 | Căn lề và phạm vi | Cấp N trang | Mọi địa chỉ ≠ 0, chia hết `PGSIZE`, trong `[end, PHYSTOP)` | Có trang lệch hoặc ngoài phạm vi | PASS |
| P02 | Junk fill khi cấp | Đọc toàn bộ 4096 byte mỗi trang | Mọi byte = 5 | Có byte ≠ 5 | PASS |
| P03 | Không cấp trùng | So từng cặp trong N trang sống | 0 cặp trùng | Có cặp trùng | PASS |
| P04 | Ghi/đọc pattern | Ghi `(i << 32) \| j` vào mọi ô 8 byte, ghi hết rồi mới đọc | N trang nguyên vẹn | Có trang bị đè | PASS |
| P05a | Đếm sau cấp | `kfreepage_count()` | `before − N` | Khác | PASS |
| P05b | Bảo toàn sau trả (leak) | Trả cả N trang rồi đếm | `before` | Khác (thiếu = leak) | PASS |
| P06 | Junk fill khi trả | Cấp 1, trả, đọc byte 8..4095 | Đều = 1 (8 byte đầu là con trỏ `next`) | Có byte ≠ 1 | PASS |
| P07a | Cấp đến hết (OOM) | `kalloc` đến khi trả 0 | Số trang cấp được = `before` | Khác | PASS |
| P07b | Địa chỉ khi OOM | Kiểm từng trang | 0 trang lệch/ngoài phạm vi | > 0 | PASS |
| P07c | Hết bộ nhớ | `kalloc` thêm 1 lần | Trả 0 | ≠ 0 | PASS |
| P07d | Bảo toàn sau OOM | Trả hết rồi đếm | `before` | Khác | PASS |

P02 và P06 kiểm tra hành vi riêng của `kalloc.c` gốc (junk 5/1). Nếu bản per-CPU đổi giá trị junk thì cập nhật expected, không xoá ca.

## 3. Ca kiểm thử tầng object (slab, T03) — chưa chạy

API S02 chưa có trên `main`. Trên nhánh `codex/slab-allocator_page` (chưa merge), Minh khai báo `slab_init(uint32 obj_size)`, `slab_alloc(void)`, `slab_free(void *)`. Các ca dưới viết theo API đó, sẽ cập nhật nếu API chốt khác.

| ID | Kiểm tra | Cách làm | Expected | Trạng thái |
|---|---|---|---|---|
| O01 | Căn lề object | `slab_alloc` nhiều lần | Địa chỉ đúng alignment trong thiết kế S01 | Chưa chạy, chờ S02 |
| O02 | Không cấp trùng | So các object đang sống | 0 cặp trùng, không chồng lấn `[p, p+size)` | Chưa chạy, chờ S02 |
| O03 | Pattern | Ghi mẫu riêng từng object, đọc lại | Không object nào bị đè, metadata slab không bị đè | Chưa chạy, chờ S02 |
| O04 | Reuse | Trả 1 object rồi cấp lại | Slot được tái sử dụng, số trang dùng không tăng | Chưa chạy, chờ S02 |
| O05 | Hết chỗ trong 1 slab | Cấp đúng `capacity` object | Lần thứ `capacity + 1` lấy trang mới hoặc trả 0 theo thiết kế | Chưa chạy, chờ S02 |
| O06 | Bảo toàn trang | Đếm `kfreepage_count()` trước/sau chu kỳ cấp/trả | Về lại ban đầu sau khi trả hết (nếu thiết kế trả trang rỗng) | Chưa chạy, chờ S02 |

## 4. Log schema

Mỗi dòng của harness bắt đầu bằng `KT|`, lọc bằng `grep -a '^KT|'`.

```text
KT|begin|rev=<git describe>|cpus=<CPUS>|inject=<0/1>|n=<N>|free_before=<số trang>
KT|case=<ID>|expected=<mô tả>|want=<giá trị mong đợi>|actual=<giá trị thật>|result=<PASS/FAIL>
KT|end|pass=<số>|fail=<số>|free_after=<số trang>|summary=<ALL_PASS/HAS_FAIL>
```

Manifest đi kèm mỗi lần chạy: người chạy, ngày, revision, OS, toolchain, QEMU, lệnh, `CPUS`. `rev` có đuôi `-dirty` nghĩa là chạy trên code chưa commit, không dùng làm kết quả nộp.

## 5. Điều kiện dừng

- PASS toàn bộ: có dòng `KT|end` với `fail=0`, và kernel boot tiếp tới `init: starting sh`.
- FAIL: có dòng `result=FAIL`, hoặc kernel `panic`, hoặc không xuất hiện `KT|end` sau 60 giây.
- Chế độ inject phải ra đúng 3 FAIL (P03, P04, P05b). Ra 0 FAIL nghĩa là harness hỏng.

## 6. Yêu cầu ghép file dùng chung 

Nhánh này thêm vài dòng vào file dùng chung, đều nằm sau cờ `KALLOCTEST` nên build mặc định không đổi:

- `Makefile`: thêm `$K/kalloctest.o` vào `OBJS`; khối `ifdef KALLOCTEST` thêm `-DKALLOCTEST`, `-DKALLOCTEST_INJECT`, `-DKT_CPUS`, `-DKT_REV`.
- `kernel/defs.h`: khai báo `void kalloctest(int);`.
- `kernel/main.c`: gọi `kalloctest(KALLOCTEST_INJECT)` ngay sau `kinit()`, trong `#ifdef KALLOCTEST`.

Nhóm check lại giúp mình xem có giữ cách gắn này không hay ghép theo cách khác.
