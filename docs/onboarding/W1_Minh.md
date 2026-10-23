# Ghi chú onboarding — Lê Trần Hiếu Minh

- Tài khoản GitHub: <điền_username_github_của_bạn_vào_đây>
- Ngày cập nhật: 23/10/2026
- Đã đọc:
  - `README.md`, `docs/Quy-trinh-nhom-26118.md`, `docs/Ke-hoach-va-backlog-26118.md`
  - Chuỗi bài giảng `docs/lessons/00-ham-header-va-lien-ket.md`, `01-baseline-va-kalloc.md`, `02-race-spinlock-va-cpu.md`
- Công cụ đã xác nhận:
  - Hệ điều hành: WSL2 (Ubuntu Linux)
  - Toolchain: `riscv64-linux-gnu-gcc`, `riscv64-linux-gnu-ld`, `gdb-multiarch`, `qemu-system-riscv64`, `python3`
- Kiểm tra đã chạy và kết quả:
  - Đã chạy `make clean` và `make qemu` trên máy cá nhân: hệ thống khởi động thành công vào dấu nhắc `$` của `xv6` (`hart 0`, `hart 1`, `hart 2` starting).
  - Đã chạy thử `ls`, `forktest` trên QEMU đạt kết quả `fork test OK`.
- Câu hỏi/trở ngại:
  - Đã giải quyết xong lỗi đường dẫn thư mục khi chạy `make` trong WSL và nắm rõ cơ chế ép kiểu con trỏ trong `freerange()` cũng như cấu trúc `struct run` của `kalloc.c`. Hiện không còn trở ngại về môi trường.
- Bước tiếp theo muốn nhận:
  - Nhận task khởi động `K02` / `S01`: Viết tài liệu thiết kế Slab Allocator một class (layout metadata/slots, alignment/capacity, luồng allocate/free/OOM trên trang 4KB của `xv6`).