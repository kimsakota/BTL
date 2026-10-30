# Nguyễn An Khánh - Nhiệm vụ W02

- MSSV: 20224019.
- Tài khoản GitHub: `ankhanh2594`.
- Issue: #19 (N03). Khoảng ngày làm việc thực tế: 24/10/2026, 30/10/2026.
- Trạng thái: Partial — xong baseline, test matrix, harness trang (11 ca); ca object chờ S02.

## Việc đã làm và kết quả

| Task | Đã làm gì, kết quả cụ thể | Ngày | Đầu ra |
|---|---|---|---|
| E01 | Build/boot baseline tại `578b89f`, `CPUS=3`: boot tới `$`, `free pages after kinit: 32735`, `forktest` → `fork test OK` | 24/10/2026 | `2026-10-24-baseline-boot.txt`, `2026-10-24-env.txt` |
| T01 | Test matrix trang (11 ca) và object (6 ca, chưa chạy), log schema, điều kiện dừng | 30/10/2026 | [`docs/tests/test-matrix-allocator.md`](../../../tests/test-matrix-allocator.md) |
| T02 | Harness trang `kernel/kalloctest.c` tại `66d5874`, `CPUS=1`: pass=11, fail=0 | 30/10/2026 | `2026-10-30-kt-run.txt`, `2026-10-30-manifest.txt` |
| T02 | Tự kiểm tra bằng `KALLOCTEST_INJECT=1`: pass=8, fail=3 đúng tại P03, P04, P05b | 30/10/2026 | `2026-10-30-kt-inject.txt` |

Ghi nhận: thêm harness làm kernel lớn hơn, số trang trống lúc boot từ 32735 xuống 32734. Harness so với số đếm lúc bắt đầu nên không báo nhầm thành leak.

## Khó khăn và TODO kỳ sau

- Khó khăn:
  - Ca object O01–O06 chưa chạy vì S02 chưa merge vào `main`.
  - Hook trong `Makefile`, `defs.h`, `main.c` chờ nhóm duyệt cách ghép.
  - Chưa chạy nhiều CPU.
- TODO : viết code T03 khi có S02; chạy lại harness trang trên bản per-CPU với nhiều CPU; cần nhóm xác nhận trước.

