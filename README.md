# BTL: Slab allocator and per-CPU free lists trên xv6

Đề tài 19, mã dự án 26118, môn Hệ điều hành. Nhóm: Nguyễn Kim Linh (20233495), Lê Trần Hiếu Minh (20233528), Nguyễn An Khánh (20224019).

## Trạng thái

Đã nhập mã xv6 riêng vào root BTL từ source Exercise, commit nền `06aad25c735fd3159bdfae5680be4eab7b1668b2`, giữ sửa đổi Makefile xử lý đường dẫn có dấu cách. Kiểm tra source ngày 19/10/2026: allocator vẫn dùng một list/lock chung; đã có hàm kfreepage_count, khai báo và lời gọi sau kinit. Chưa triển khai slab/per-CPU lists và chưa có kết quả build/boot/test được đối chiếu trong các lượt soạn bài/nghiên cứu quy trình.

Đã chuẩn bị quy trình nhóm ba người, backlog và báo cáo W01 dạng nháp theo PDF mẫu. Linh xác nhận tuần 19–25/10/2026 mới khởi động dự án nhóm; bài học/code thử trước đó là chuẩn bị cá nhân. Phân công là đề xuất; ngày chốt báo cáo, deadline cuối kỳ và đóng góp Minh/Khánh chưa được xác nhận. Linh muốn tham gia sâu và hiểu toàn bộ hệ thống.

Kiểm tra môi trường ngày 13/10/2026: Windows có VS Code, Git và `wsl.exe`; chưa có distro WSL. Chưa tìm thấy Make, RISC-V GCC hoặc QEMU trong Windows PATH. Chưa kiểm tra toolchain Linux vì chưa có distro.

## Tổ chức dự án

Giữ cây thư mục tiêu chuẩn của xv6 tại root BTL: `kernel/`, `user/`, `mkfs/` và `Makefile`. Phần bổ sung của đề tài nằm cạnh mã gốc, được chia theo trách nhiệm bằng các module `.c/.h`.

- [Hướng dẫn kiến trúc và dựng dự án](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Kien-truc-du-an.md>).
- [Kết quả kiểm tra nguồn xv6](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Nguon-xv6.md>).
- [Bản nghiên cứu đề tài 19](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Nghien-cuu-de-tai-19.md>).
- [Quy trình làm việc nhóm 26118](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Quy-trinh-nhom-26118.md>).
- [Kế hoạch và backlog có phân công, phụ thuộc, tiêu chí](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Ke-hoach-va-backlog-26118.md>).
- [Quy tắc báo cáo và nối TODO giữa các tuần](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/bao-cao/README.md>).
- [Bản nháp báo cáo W01 cần xác nhận dữ kiện trước khi nộp](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/bao-cao/26118-Progress-Report-W01-DRAFT.md>).
- [Workspace VS Code](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/BTL.code-workspace>).
- [Bộ nhớ agent và lịch sử làm việc](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/agent/README.md>).
- [Lộ trình vừa học vừa code](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Lo-trinh-hoc.md>).
- [Bài 0: hàm, header và liên kết](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/00-ham-header-va-lien-ket.md>).
- [Bài 1: bộ nhớ, con trỏ, struct và allocator trang xv6](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/01-baseline-va-kalloc.md>).
- [Bài 2: race condition, spinlock, CPU ID và trạng thái ngắt](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/02-race-spinlock-va-cpu.md>).
- [Bài 3: thiết kế và tự viết free list theo CPU](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/03-per-cpu-free-lists.md>).

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
