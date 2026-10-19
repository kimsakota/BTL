# BTL: Slab allocator and per-CPU free lists trên xv6

Đề tài 19, mã dự án 26118, môn Hệ điều hành. Nhóm: Nguyễn Kim Linh (20233495), Lê Trần Hiếu Minh (20233528), Nguyễn An Khánh (20224019).

## Trạng thái

Đã nhập mã xv6 riêng vào root BTL từ source Exercise, commit nền `06aad25c735fd3159bdfae5680be4eab7b1668b2`, giữ sửa đổi Makefile xử lý đường dẫn có dấu cách. Kiểm tra source ngày 19/10/2026: allocator vẫn dùng một list/lock chung; đã có hàm kfreepage_count, khai báo và lời gọi sau kinit. Chưa triển khai slab/per-CPU lists.

Kiểm tra GitHub ngày 19/10: [PR #1](https://github.com/kimsakota/BTL/pull/1) đã nhận commit sửa format `58be186`, chưa merge. [Run của PR](https://github.com/kimsakota/BTL/actions/runs/39209117349) đã completed/success: Linux qua format, build kernel/fs.img, usertests và crash test; macOS build thành công. Đây là kiểm tra baseline hiện tại, chưa có per-CPU/slab hoặc benchmark. Chi tiết cấu hình đề xuất ở [kiểm tra GitHub](docs/Thiet-lap-GitHub-26118.md).

Đã chuẩn bị quy trình nhóm ba người, backlog và báo cáo W01 dạng nháp theo PDF mẫu. Linh xác nhận tuần 19–25/10/2026 mới khởi động dự án nhóm; bài học/code thử trước đó là chuẩn bị cá nhân. Phân công là đề xuất; ngày chốt báo cáo, deadline cuối kỳ và đóng góp Minh/Khánh chưa được xác nhận. Linh muốn tham gia sâu và hiểu toàn bộ hệ thống.

Kiểm tra môi trường ngày 19/10/2026: WSL 2 có Ubuntu 26.04.1 LTS; tìm thấy Make, RISC-V GCC và QEMU. Linh đã cài formatter, kiểm tra trực tiếp xác nhận Ubuntu clang-format 21.1.8. Đây chưa phải kết quả format/build/boot/test local.

## Tổ chức dự án

Giữ cây thư mục tiêu chuẩn của xv6 tại root BTL: `kernel/`, `user/`, `mkfs/` và `Makefile`. Phần bổ sung của đề tài nằm cạnh mã gốc, được chia theo trách nhiệm bằng các module `.c/.h`.

- [Hướng dẫn kiến trúc và dựng dự án](docs/Kien-truc-du-an.md).
- [Kết quả kiểm tra nguồn xv6](docs/Nguon-xv6.md).
- [Bản nghiên cứu đề tài 19](docs/Nghien-cuu-de-tai-19.md).
- [Quy trình làm việc nhóm 26118](docs/Quy-trinh-nhom-26118.md).
- [Kiểm tra GitHub và cấu hình theo giai đoạn một người rồi ba người](docs/Thiet-lap-GitHub-26118.md).
- [Cách đóng góp và mở PR](CONTRIBUTING.md).
- [Kế hoạch và backlog có phân công, phụ thuộc, tiêu chí](docs/Ke-hoach-va-backlog-26118.md).
- [Quy tắc báo cáo và nối TODO giữa các tuần](docs/bao-cao/README.md).
- [Bản nháp báo cáo W01 cần xác nhận dữ kiện trước khi nộp](docs/bao-cao/26118-Progress-Report-W01-DRAFT.md).
- [Workspace VS Code](BTL.code-workspace).
- [Bộ nhớ agent và lịch sử làm việc](agent/README.md).
- [Lộ trình vừa học vừa code](docs/Lo-trinh-hoc.md).
- [Bài 0: hàm, header và liên kết](docs/lessons/00-ham-header-va-lien-ket.md).
- [Bài 1: bộ nhớ, con trỏ, struct và allocator trang xv6](docs/lessons/01-baseline-va-kalloc.md).
- [Bài 2: race condition, spinlock, CPU ID và trạng thái ngắt](docs/lessons/02-race-spinlock-va-cpu.md).
- [Bài 3: thiết kế và tự viết free list theo CPU](docs/lessons/03-per-cpu-free-lists.md).

Các liên kết trên dùng đường dẫn tương đối để mở được từ GitHub hoặc clone ở máy khác. Một số bài học cũ vẫn còn đường dẫn local; việc đổi các liên kết đó được theo dõi trong tài liệu kiểm tra GitHub.

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
