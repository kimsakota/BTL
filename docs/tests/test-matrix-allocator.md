# Test matrix allocator (T01) và harness trang (T02) — Khánh

Issue: #19 (N03). Bản đầu 30/10/2026; cập nhật 31/10/2026 theo review PR #21 (chặn ca không an toàn, kiểm tra địa chỉ trong P07, sửa mô tả cờ build). Viết trước khi API object (S02) được chốt.

## 1. Cách kích hoạt

Harness tầng trang nằm ở `kernel/kalloctest.c`. Chỉ khi bật cờ `KALLOCTEST`, `Makefile` mới biên dịch và link `kalloctest.o`, và `main.c` mới gọi `kalloctest()`. Build mặc định không chứa harness, kernel không đổi kích thước.

```bash
make clean
make qemu CPUS=1 KALLOCTEST=1                       # chạy thật
make clean
make qemu CPUS=1 KALLOCTEST=1 KALLOCTEST_INJECT=1   # tự kiểm tra: các ca bắt được lỗi
make clean
make qemu CPUS=1 KALLOCTEST=1 KALLOCTEST_INJECT=2   # tự kiểm tra: đường chặn NULL/địa chỉ sai/trùng
```

Đổi cờ thì phải `make clean`, vì object cũ không tự biên dịch lại khi chỉ đổi cờ.

- Harness chạy trên CPU 0 ngay sau `kinit()`, trước khi CPU khác khởi động và trước mọi cấp phát khác (trạng thái yên), nên số trang đếm được là chính xác.
- Mọi so sánh số trang dùng `before = kfreepage_count()` đếm lúc bắt đầu, không viết cứng số trang lúc boot, vì số này đổi theo kích thước kernel (bật harness làm kernel lớn hơn, số trang trống có thể giảm 1).
- W02 chỉ chạy `CPUS=1`. Nhiều CPU và stealing là W03 (P05 trong backlog).

## 2. Quy tắc an toàn của harness

- Địa chỉ hợp lệ (`kt_valid`): khác 0, chia hết `PGSIZE`, nằm trong `[PGROUNDUP(end), PHYSTOP)`.
- Trang được test "sở hữu" khi hợp lệ (P01) và không trùng trang khác (P03). Trang trùng chỉ giữ bản đầu tiên.
- Nếu có trang sai hoặc trùng (`kt_unsafe`), ca ghi vào trang (P04) bị bỏ qua và tính FAIL (`note=skipped_unsafe`).
- Khi dọn dẹp chỉ `kfree` trang đang sở hữu, không bao giờ `kfree` địa chỉ sai hoặc trả một trang hai lần.
- P07 kiểm tra địa chỉ trước khi ghi con trỏ nối danh sách; gặp địa chỉ sai thì dừng vòng lặp, trả các trang đã lấy, bỏ qua P07c.

## 3. Ca kiểm thử tầng trang (`kalloc`/`kfree`)

N = 64 trang sống cùng lúc. Cột cuối là kết quả chế độ thường (`INJECT=0`).

| ID | Kiểm tra | Cách làm | Expected | Điều kiện FAIL | Kết quả |
|---|---|---|---|---|---|
| P01 | Căn lề và phạm vi | Cấp N trang, kiểm `kt_valid` | Cả N trang hợp lệ | Có trang NULL/lệch/ngoài phạm vi | PASS |
| P02 | Junk fill khi cấp | Đọc 4096 byte mỗi trang sở hữu | Mọi byte = 5 | Có byte ≠ 5, hoặc thiếu trang sở hữu | PASS |
| P03 | Không cấp trùng | So từng cặp trang sở hữu | 0 cặp trùng | Có cặp trùng | PASS |
| P04 | Ghi/đọc pattern | Ghi `(i << 32) \| j` vào mọi ô 8 byte, ghi hết rồi mới đọc | N trang nguyên vẹn | Có trang bị đè, hoặc bị bỏ qua vì không an toàn | PASS |
| P05a | Đếm sau cấp | `kfreepage_count()` | `before − N` | Khác | PASS |
| P05b | Bảo toàn sau trả (leak) | Trả trang sở hữu rồi đếm | `before` | Khác (thiếu = leak) | PASS |
| P06 | Junk fill khi trả | Cấp 1, trả, đọc byte 8..4095 | Đều = 1 (8 byte đầu là con trỏ `next`) | Có byte ≠ 1, hoặc trang cấp ra không hợp lệ | PASS |
| P07a | Cấp đến hết (OOM) | `kalloc` đến khi trả 0 | Số trang cấp được = `before` | Khác, hoặc dừng sớm vì địa chỉ sai | PASS |
| P07b | Địa chỉ khi OOM | `kt_valid` từng trang trước khi ghi | 0 trang sai | > 0 | PASS |
| P07c | Hết bộ nhớ | `kalloc` thêm 1 lần | Trả 0 | ≠ 0, hoặc bị bỏ qua vì P07b | PASS |
| P07d | Bảo toàn sau OOM | Trả hết rồi đếm | `before` | Khác | PASS |

P02 và P06 kiểm tra hành vi riêng của `kalloc.c` gốc (junk 5/1). Nếu bản per-CPU đổi giá trị junk thì cập nhật expected, không xoá ca.

## 4. Tự kiểm tra harness (fault injection)

Lỗi giả lập chỉ tác động lên dữ liệu của harness, trang thật luôn được trả lại, nên kernel vẫn boot bình thường và `free_after` bằng `free_before`.

| Chế độ | Giả lập | Expected |
|---|---|---|
| `INJECT=1` | P03: tăng bộ đếm trùng; P04: đè 1 ô trong trang 3; P05b: tạm giữ 1 trang | Đúng 3 FAIL: P03, P04, P05b. Các ca khác PASS |
| `INJECT=2` | P01: trang 2 thành NULL, trang 4 trùng trang 3, trang 6 lệch 8 byte; P07: trang thứ 11 lệch 8 byte | 7 FAIL: P01 (62/64), P02 (62/64), P03 (1 cặp), P04 bị bỏ qua, P07a (dừng ở 10), P07b (1), P07c bị bỏ qua. PASS: P05a, P05b, P06, P07d. Kernel boot tới shell |

## 5. Ca kiểm thử tầng object (slab, T03) — chưa chạy

API S02 chưa có trên `main`. Trên nhánh `codex/slab-allocator_page` (chưa merge), Minh khai báo `slab_init(uint32 obj_size)`, `slab_alloc(void)`, `slab_free(void *)`. Các ca dưới viết theo API đó, sẽ cập nhật nếu API chốt khác. Khi viết code sẽ áp dụng cùng quy tắc an toàn ở mục 2.

| ID | Kiểm tra | Cách làm | Expected | Trạng thái |
|---|---|---|---|---|
| O01 | Căn lề object | `slab_alloc` nhiều lần | Địa chỉ đúng alignment trong thiết kế S01 | Chưa chạy, chờ S02 |
| O02 | Không cấp trùng | So các object đang sống | 0 cặp trùng, không chồng lấn `[p, p+size)` | Chưa chạy, chờ S02 |
| O03 | Pattern | Ghi mẫu riêng từng object, đọc lại | Không object nào bị đè, metadata slab không bị đè | Chưa chạy, chờ S02 |
| O04 | Reuse | Trả 1 object rồi cấp lại | Slot được tái sử dụng, số trang dùng không tăng | Chưa chạy, chờ S02 |
| O05 | Hết chỗ trong 1 slab | Cấp đúng `capacity` object | Lần thứ `capacity + 1` lấy trang mới hoặc trả 0 theo thiết kế | Chưa chạy, chờ S02 |
| O06 | Bảo toàn trang | Đếm `kfreepage_count()` trước/sau chu kỳ cấp/trả | Về lại ban đầu sau khi trả hết (nếu thiết kế trả trang rỗng) | Chưa chạy, chờ S02 |

## 6. Log schema

Mỗi dòng của harness bắt đầu bằng `KT|`, lọc bằng `grep -a '^KT|'`.

```text
KT|begin|rev=<git describe>|cpus=<CPUS>|inject=<0/1/2>|n=<N>|free_before=<số trang>
KT|case=<ID>|expected=<mô tả>|want=<giá trị mong đợi>|actual=<giá trị thật>|result=<PASS/FAIL>
KT|case=<ID>|expected=safe_input|result=FAIL|note=skipped_unsafe
KT|end|pass=<số>|fail=<số>|free_after=<số trang>|summary=<ALL_PASS/HAS_FAIL>
```

Manifest đi kèm mỗi lần chạy: người chạy, ngày, revision, OS, toolchain, QEMU, lệnh, `CPUS`. `rev` có đuôi `-dirty` nghĩa là chạy trên code chưa commit, không dùng làm kết quả nộp.

## 7. Điều kiện dừng

- PASS toàn bộ: có dòng `KT|end` với `fail=0`, và kernel boot tiếp tới `init: starting sh`.
- FAIL: có dòng `result=FAIL`, hoặc kernel `panic`, hoặc không xuất hiện `KT|end` sau 60 giây.
- `INJECT=1` phải ra đúng 3 FAIL; `INJECT=2` phải ra đúng 7 FAIL và vẫn boot tới shell. Khác đi nghĩa là harness hỏng.

## 8. Yêu cầu ghép file dùng chung (gửi Linh)

Nhánh này thêm vài dòng vào file dùng chung, đều nằm sau cờ `KALLOCTEST`:

- `Makefile`: khối `ifdef KALLOCTEST` thêm `$K/kalloctest.o` vào `OBJS` và các cờ `-DKALLOCTEST`, `-DKALLOCTEST_INJECT`, `-DKT_CPUS`, `-DKT_REV`. Build mặc định không biên dịch, không link harness.
- `kernel/defs.h`: khai báo `void kalloctest(int);`.
- `kernel/main.c`: gọi `kalloctest(KALLOCTEST_INJECT)` ngay sau `kinit()`, trong `#ifdef KALLOCTEST`.
