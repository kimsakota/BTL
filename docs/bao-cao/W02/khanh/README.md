# Nguyễn An Khánh - Đóng góp W02

- MSSV: 20224019.
- Tài khoản GitHub: `ankhanh2594`.
- Issue: #19 (N03). PR: #21. Khoảng ngày làm việc thực tế: 24/10/2026, 30–31/10/2026.
- Trạng thái: Partial — xong baseline, test matrix, harness trang (11 ca) đã sửa theo review; ca object chờ S02.

## Việc đã làm và kết quả

| Task | Đã làm gì, kết quả cụ thể | Ngày | Đầu ra |
|---|---|---|---|
| E01 | Build/boot baseline tại `578b89f`, `CPUS=3`: boot tới `$`, `free pages after kinit: 32735`, `forktest` → `fork test OK` | 24/10/2026 | `2026-10-24-baseline-boot.txt`, `2026-10-24-env.txt` |
| T01 | Test matrix trang (11 ca) và object (6 ca, chưa chạy), quy tắc an toàn, log schema, điều kiện dừng | 30–31/10/2026 | [`docs/tests/test-matrix-allocator.md`](../../../tests/test-matrix-allocator.md) |
| T02 | Bản đầu tại `66d5874`, `CPUS=1`: pass=11, fail=0; inject=1: đúng 3 FAIL | 30/10/2026 | `2026-10-30-kt-run.txt`, `2026-10-30-kt-inject.txt`, `2026-10-30-manifest.txt` |
| T02 | Sửa theo review PR #21: chặn ca không an toàn, chỉ trả trang sở hữu, P07 kiểm địa chỉ trước khi ghi, harness chỉ link khi bật cờ. Chạy lại tại `8d9d2bf`, `CPUS=1`: inject=0 pass=11 fail=0; inject=1 đúng 3 FAIL; inject=2 đúng 7 FAIL và kernel vẫn boot tới shell | 31/10/2026 | `2026-10-31-kt-run.txt`, `2026-10-31-kt-inject1.txt`, `2026-10-31-kt-inject2.txt`, `2026-10-31-manifest.txt` |

Ghi nhận: bật harness làm kernel lớn hơn, số trang trống lúc boot có thể giảm 1. Harness so với số đếm lúc bắt đầu nên không báo nhầm thành leak.

## Khó khăn và TODO tiếp theo

- Khó khăn:
  - Ca object O01–O06 chưa chạy vì S02 chưa merge vào `main`.
  - Hook trong `Makefile`, `defs.h`, `main.c` chờ nhóm duyệt cách ghép.
  - Chưa chạy nhiều CPU.
- TODO: viết code T03 khi có S02; chạy lại harness trang trên bản per-CPU với nhiều CPU; cần nhóm xác nhận lại.
