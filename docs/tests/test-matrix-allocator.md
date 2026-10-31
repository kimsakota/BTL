# Test matrix allocator (T01) và harness trang (T02) — Khánh

Issue: #19 (N03). Bản đầu 30/10/2026. Cập nhật 31/10/2026 theo review PR #21 lần 1 (chặn ca không an toàn, kiểm tra địa chỉ trong P07, sửa mô tả cờ build) và lần 2 (chống trang trùng trong P07, ghép `main` sau #22/#23, đổi tên API object).

## 1. Cách kích hoạt

Harness tầng trang nằm ở `kernel/kalloctest.c`. Chỉ khi bật cờ `KALLOCTEST`, `Makefile` mới biên dịch và link `kalloctest.o`, và `main.c` mới gọi `kalloctest()`. Build mặc định không chứa harness.

```bash
make clean
make qemu CPUS=1                                    # mặc định: không có harness
make clean
make qemu CPUS=1 KALLOCTEST=1                       # chạy thật
make clean
make qemu CPUS=1 KALLOCTEST=1 KALLOCTEST_INJECT=1   # tự kiểm tra: các ca bắt được lỗi
make clean
make qemu CPUS=1 KALLOCTEST=1 KALLOCTEST_INJECT=2   # tự kiểm tra: đường chặn NULL/địa chỉ sai/trùng
make clean
make qemu CPUS=1 KALLOCTEST=1 KALLOCTEST_INJECT=3   # tự kiểm tra: trang trùng trong P07
```

Đổi cờ thì phải `make clean`, vì object cũ không tự biên dịch lại khi chỉ đổi cờ.

- Harness chạy trên CPU 0 ngay sau `kinit()`, trước `kmalloc_init()`/`slab_test()` và trước khi CPU khác khởi động. Đây là trạng thái yên: chưa ai khác lấy trang, nên số trang đếm được là chính xác.
- Mọi so sánh số trang dùng `before = kfreepage_count()` đếm lúc bắt đầu, không viết cứng số trang lúc boot, vì số này đổi theo kích thước kernel.
- W02 chỉ chạy `CPUS=1`. Nhiều CPU và stealing là W03 (P05 trong backlog).

## 2. Quy tắc an toàn của harness

- Địa chỉ hợp lệ (`kt_valid`): khác 0, chia hết `PGSIZE`, nằm trong `[PGROUNDUP(end), PHYSTOP)`.
- Trang được test "sở hữu" khi hợp lệ (P01) và không trùng trang khác (P03). Trang trùng chỉ giữ bản đầu tiên.
- Nếu có trang sai hoặc trùng (`kt_unsafe`), ca ghi vào trang (P04) bị bỏ qua và tính FAIL (`note=skipped_unsafe`).
- Khi dọn dẹp chỉ `kfree` trang đang sở hữu, không bao giờ `kfree` địa chỉ sai hoặc trả một trang hai lần.
- P07 không ghi gì vào các trang đã cấp. Trang đã cấp được đánh dấu bằng bitmap 1 bit/trang (4 KB) nằm ngoài trang. Mỗi trang phải hợp lệ và chưa có bit; gặp trang sai hoặc trùng thì dừng vòng lặp và bỏ qua P07c. Dọn dẹp duyệt bitmap, mỗi trang trả đúng một lần.

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
| P07a | Cấp đến hết (OOM) | `kalloc` đến khi trả 0 | Số trang cấp được = `before` | Khác, hoặc dừng sớm vì trang sai/trùng | PASS |
| P07b | Địa chỉ khi OOM | `kt_valid` từng trang | 0 trang sai | > 0 | PASS |
| P07e | Không trùng khi OOM | Kiểm bit của trang trong bitmap trước khi đánh dấu | 0 trang trùng | > 0 | PASS |
| P07c | Hết bộ nhớ | `kalloc` thêm 1 lần | Trả 0 | ≠ 0, hoặc bị bỏ qua vì P07b/P07e | PASS |
| P07d | Bảo toàn sau OOM | Trả mỗi trang có bit đúng một lần rồi đếm | `before` | Khác | PASS |

P02 và P06 kiểm tra hành vi junk 5/1 của `kalloc.c`. Nếu bản per-CPU đổi giá trị junk thì cập nhật expected, không xoá ca.

## 4. Tự kiểm tra harness (fault injection)

Lỗi giả lập chỉ tác động lên dữ liệu của harness, trang thật luôn được trả lại, nên kernel vẫn boot bình thường và `free_after` bằng `free_before`.

| Chế độ | Giả lập | Expected |
|---|---|---|
| `INJECT=1` | P03: tăng bộ đếm trùng; P04: đè 1 ô trong trang 3; P05b: tạm giữ 1 trang | 3 FAIL: P03, P04, P05b |
| `INJECT=2` | P01: trang 2 thành NULL, trang 4 trùng trang 3, trang 6 lệch 8 byte; P07: trang thứ 11 lệch 8 byte | 7 FAIL: P01, P02, P03, P04 (bỏ qua), P07a, P07b, P07c (bỏ qua). Kernel boot tới shell |
| `INJECT=3` | P07: lần cấp thứ 11 trả lại trang vừa cấp | 3 FAIL: P07a (dừng ở 10), P07e (1), P07c (bỏ qua). P07d PASS, kernel boot tới shell |

## 5. Ca kiểm thử tầng object (slab, T03) — chưa chạy

S02 đã merge vào `main` (#23). API: `kmalloc_init()`, `kmalloc(uint64 size)` nhận `1..64`, `kmfree(void *)`. Các ca dưới là bước tiếp theo (T03), áp dụng cùng quy tắc an toàn ở mục 2.

| ID | Kiểm tra | Cách làm | Expected | Trạng thái |
|---|---|---|---|---|
| O01 | Căn lề object | `kmalloc` nhiều lần | Địa chỉ đúng biên slot theo thiết kế S01/S02 | Chưa chạy, T03 |
| O02 | Không cấp trùng | So các object đang sống | 0 cặp trùng, không chồng lấn `[p, p+size)` | Chưa chạy, T03 |
| O03 | Pattern | Ghi mẫu riêng từng object, đọc lại | Không object nào bị đè, header slab không bị đè | Chưa chạy, T03 |
| O04 | Reuse | `kmfree` 1 object rồi `kmalloc` lại | Slot được tái sử dụng, số trang dùng không tăng | Chưa chạy, T03 |
| O05 | Hết chỗ trong 1 slab | Cấp đúng `capacity` object | Lần thứ `capacity + 1` lấy trang mới | Chưa chạy, T03 |
| O06 | Giữ trang qua chu kỳ | Đếm `kfreepage_count()` qua các chu kỳ cấp/trả | Không tốn thêm trang khi cấp lại | Chưa chạy, T03 |

## 6. Log schema

Mỗi dòng của harness bắt đầu bằng `KT|`, lọc bằng `grep -a '^KT|'`.

```text
KT|begin|rev=<git describe>|cpus=<CPUS>|inject=<0/1/2/3>|n=<N>|free_before=<số trang>
KT|case=<ID>|expected=<mô tả>|want=<giá trị mong đợi>|actual=<giá trị thật>|result=<PASS/FAIL>
KT|case=<ID>|expected=safe_input|result=FAIL|note=skipped_unsafe
KT|end|pass=<số>|fail=<số>|free_after=<số trang>|summary=<ALL_PASS/HAS_FAIL>
```

Manifest đi kèm mỗi lần chạy: người chạy, ngày, revision, OS, toolchain, QEMU, lệnh, `CPUS`. `rev` có đuôi `-dirty` nghĩa là chạy trên code chưa commit, không dùng làm kết quả nộp.

## 7. Điều kiện dừng

- PASS toàn bộ: dòng `KT|end` có `pass=12|fail=0`, và kernel boot tiếp tới `init: starting sh`.
- FAIL: có dòng `result=FAIL`, hoặc kernel `panic`, hoặc không xuất hiện `KT|end` sau 60 giây.
- `INJECT=1` phải ra đúng 3 FAIL; `INJECT=2` đúng 7 FAIL; `INJECT=3` đúng 3 FAIL; cả ba vẫn boot tới shell. Khác đi nghĩa là harness hỏng.

## 8. Yêu cầu ghép file dùng chung 

Nhánh này thêm vài dòng vào file dùng chung, đều nằm sau cờ `KALLOCTEST`:

- `Makefile`: khối `ifdef KALLOCTEST` thêm `$K/kalloctest.o` vào `OBJS` và các cờ `-DKALLOCTEST`, `-DKALLOCTEST_INJECT`, `-DKT_CPUS`, `-DKT_REV`. Build mặc định không biên dịch, không link harness.
- `kernel/defs.h`: khai báo `void kalloctest(int);`.
- `kernel/main.c`: gọi `kalloctest(KALLOCTEST_INJECT)` ngay sau `kinit()`, trước `kmalloc_init()`/`slab_test()`, trong `#ifdef KALLOCTEST`.
