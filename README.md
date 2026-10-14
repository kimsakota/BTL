# BTL: Slab allocator and per-CPU free lists trên xv6

Đề tài 19, môn Hệ điều hành.

## Trạng thái

Đã nhập mã xv6 riêng vào root BTL từ source Exercise, commit nền `06aad25c735fd3159bdfae5680be4eab7b1668b2`, giữ sửa đổi Makefile xử lý đường dẫn có dấu cách. Source kernel hiện vẫn là allocator gốc. Chưa triển khai slab/per-CPU lists, chưa có môi trường Linux để build và chưa xác nhận boot/test tại BTL.

Kiểm tra môi trường ngày 13/10/2026: Windows có VS Code, Git và `wsl.exe`; chưa có distro WSL. Chưa tìm thấy Make, RISC-V GCC hoặc QEMU trong Windows PATH. Chưa kiểm tra toolchain Linux vì chưa có distro.

## Tổ chức dự án

Giữ cây thư mục tiêu chuẩn của xv6 tại root BTL: `kernel/`, `user/`, `mkfs/` và `Makefile`. Phần bổ sung của đề tài nằm cạnh mã gốc, được chia theo trách nhiệm bằng các module `.c/.h`.

- [Hướng dẫn kiến trúc và dựng dự án](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Kien-truc-du-an.md>).
- [Kết quả kiểm tra nguồn xv6](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Nguon-xv6.md>).
- [Bản nghiên cứu đề tài 19](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Nghien-cuu-de-tai-19.md>).
- [Workspace VS Code](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/BTL.code-workspace>).
- [Bộ nhớ agent và lịch sử làm việc](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/agent/README.md>).
- [Lộ trình vừa học vừa code](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Lo-trinh-hoc.md>).
- [Bài 0: hàm, header và liên kết](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/00-ham-header-va-lien-ket.md>).
- [Bài 1: bộ nhớ, con trỏ, struct và allocator trang xv6](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/01-baseline-va-kalloc.md>).

Các đường dẫn trên trỏ đến workspace hiện tại. Khi chuyển dự án sang vị trí khác, dùng các file cùng tên trong `docs/`.

## Mở và chạy sau khi hoàn tất thiết lập

Mở `BTL.code-workspace` trong VS Code. Khuyến nghị làm việc bằng WSL 2 và extension WSL khi toolchain Linux đã sẵn sàng.

Tác vụ trong `.vscode/tasks.json`:

| Task | Lệnh Make | Cách sử dụng |
|---|---|---|
| `xv6: build` | `make kernel/kernel fs.img` | `Ctrl+Shift+B` |
| `xv6: run` | `make qemu CPUS=1/2/4/8` | `Tasks: Run Task`, chọn số CPU |
| `xv6: clean` | `make clean` | Xóa sản phẩm build theo Makefile |

Trên Windows, task gọi Make thông qua distro WSL mặc định; trong VS Code WSL, task gọi Make trực tiếp. Task không tự cài phần mềm, tải xv6 hoặc chạy khi mở workspace.

Target dựa trên Makefile xv6 chính thức đã khảo sát. Nếu starter môn học dùng target khác, cập nhật task theo Makefile của starter.
