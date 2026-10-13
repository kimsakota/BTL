# Kiểm tra nguồn xv6 làm nền cho đề tài 19

Ngày kiểm tra: 13/10/2026.

**Kết luận:** thư mục xv6 được cung cấp phù hợp về mã nguồn để phát triển Slab allocator and per-CPU free lists. Chưa xác nhận build/boot trên môi trường hiện tại.

## Nguồn và phiên bản

- Thư mục: `D:\2. FullStack\2026.1\Hệ điều hành\Excecise\4\xv6-riscv`.
- Remote: `https://github.com/mit-pdos/xv6-riscv.git`.
- Branch: `riscv`.
- HEAD và remote-tracking `origin/riscv` tại thời điểm kiểm tra: `06aad25c735fd3159bdfae5680be4eab7b1668b2`.
- Đã xác nhận commit này có trong [repo MIT chính thức](https://github.com/mit-pdos/xv6-riscv/commit/06aad25c735fd3159bdfae5680be4eab7b1668b2).

## Vì sao phù hợp?

| Thành phần được kiểm tra | Kết quả |
|---|---|
| `kernel/kalloc.c` | Allocator trang gốc: một free list và một `kmem.lock` |
| `kernel/riscv.h` | `PGSIZE = 4096` |
| `kernel/param.h` | `NCPU = 8` |
| `kernel/file.c` | Pool `struct file` tĩnh, có thể làm ứng viên chuyển sang cấp phát động |
| `kernel/fs.c` | Bảng in-memory inode tĩnh |
| `kernel/bio.c` | Bảng buffer tĩnh |
| Makefile | Có target `kernel/kernel`, `fs.img`, `qemu`, `clean`; nhận `CPUS` |
| Tìm kiếm slab/per-CPU | Chưa thấy implementation slab, kmalloc hay stealing trong source đã kiểm tra |
| Thay đổi chưa commit ở kernel | Không có |

Các đường mã dùng để kiểm tra: [kalloc.c](<D:/2. FullStack/2026.1/Hệ điều hành/Excecise/4/xv6-riscv/kernel/kalloc.c>), [param.h](<D:/2. FullStack/2026.1/Hệ điều hành/Excecise/4/xv6-riscv/kernel/param.h>), [Makefile](<D:/2. FullStack/2026.1/Hệ điều hành/Excecise/4/xv6-riscv/Makefile>).

## Các khác biệt local cần giữ lại khi tạo nền BTL

Git báo ba file modified:

1. Makefile: thêm dấu nháy quanh `-ffile-prefix-map=$(CURDIR)=.`. Sửa đổi này phù hợp với đường dẫn workspace có dấu cách.
2. `test-xv6.py`: thay đổi mode từ `100755` sang `100644`, không có thay đổi nội dung được Git báo cáo.
3. `user/usys.pl`: thay đổi mode tương tự, không có thay đổi nội dung được Git báo cáo.

`core.autocrlf=true`; đây là thiết lập có thể làm newline đổi khi Git checkout. Khi đưa source vào môi trường Linux, kiểm tra LF và mode thực thi. Makefile gọi `perl user/usys.pl`, nên script đó không cần được chạy trực tiếp cho bước tạo syscall stubs.

Các sản phẩm build có sẵn như `kernel/kernel`, `mkfs/mkfs`, `user/_usertests` và `fs.img` chỉ chứng minh có file từ lần build trước. Chưa dùng chúng để khẳng định source hiện tại build/boot thành công trên máy đang kiểm tra.

## Cách sử dụng cho đồ án

Đã tạo bản source riêng tại root BTL ngày 13/10/2026: nhập 81 file source/cấu hình được Git theo dõi, giữ sửa đổi Makefile xử lý dấu cách và chuẩn hóa newline LF. Giữ thư mục Exercise làm nguồn tham chiếu. Workspace và tasks BTL trỏ vào root chứa Makefile mới nhập. Metadata ở `docs/xv6-baseline.json`.

Chỉ lấy source và file cấu hình cần thiết; tạo lại `.o`, binary kernel, user programs và filesystem image bằng toolchain đã xác định. Không dùng sẵn `fs.img` của bài tập làm dữ liệu baseline cho đồ án.

Phiên bản này có `sleep_prepare()` và `vmfault()`; khi học code hoặc tích hợp thay đổi phải bám đúng API hiện có. Không giả định mọi hướng dẫn xv6 cũ dùng cùng hàm hoặc cùng ngữ nghĩa.

Makefile yêu cầu QEMU từ 7.2 và sử dụng `bc` khi kiểm tra phiên bản, cùng Perl để tạo syscall stubs. Chưa có `kalloctest.c` hoặc `kernel/kcsan.c` được Git theo dõi trong bản này; test của lock lab không tự có sẵn.

Thứ tự xác nhận runtime: có Linux/WSL 2 và toolchain → build mới → boot QEMU 1 CPU → chạy `usertests -q` → thử 2/4/8 CPU → ghi baseline trước khi sửa allocator.
