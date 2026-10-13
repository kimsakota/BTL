# Điểm tiếp tục

Cập nhật: 13/10/2026.

## Checkpoint mới nhất

### Đã làm

- Người dùng đã chốt đề tài 19 và bắt đầu học lý thuyết.
- Hoàn thành nghiên cứu, kiến trúc dự kiến và workspace BTL.
- Kiểm tra source xv6 ở Exercise, xác nhận phù hợp về mã nguồn.
- Tích hợp bộ nhớ agent từ project-memory-skill Light và lưu snapshot hai chat liên quan.
- Người dùng chọn tự gõ code, assistant hướng dẫn/kiểm tra. Đã nhập source riêng vào BTL, tạo lộ trình và Bài 1; Git baseline lưu bằng commit/tag local.

### Đúng ở hiện tại

- Root BTL có source kernel/Makefile và Git repo local, tag nền xv6-baseline.
- Chưa triển khai slab/per-CPU lists hoặc chạy benchmark.
- Source tham chiếu: `D:\2. FullStack\2026.1\Hệ điều hành\Excecise\4\xv6-riscv`, commit `06aad25c735fd3159bdfae5680be4eab7b1668b2`.
- Đã kiểm tra lại: vẫn chưa có distro WSL. Chưa build/boot/test tại BTL; code bài tập chưa được gõ.

### Tiếp tục theo nhiệm vụ

- Tiếp tục Bài 1 trong docs/lessons/01-baseline-va-kalloc.md: môi trường → baseline boot/test → người dùng tự gõ hàm đếm trang → assistant đọc diff và kiểm tra.
- Không tự thêm code bài tập thay người dùng. Chuẩn bị nguồn/tài liệu và kiểm tra vẫn có thể thực hiện trong phạm vi được yêu cầu.
- Nếu người dùng muốn kiểm tra bộ nhớ: mở `CURRENT_STATE.md`, `LOGBOOK.md`, `history/INDEX.md`.

### Nguồn cần tin

`CURRENT_STATE.md` → tài liệu liên quan trong `docs/` → mã nguồn/kết quả mới nhất. Transcript chỉ cần đọc khi tra cứu nguyên văn hoặc xác minh lịch sử.
