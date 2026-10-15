# Điểm tiếp tục

Cập nhật: 15/10/2026.

## Checkpoint mới nhất

### Đã làm

- Người dùng đã chốt đề tài 19 và bắt đầu học lý thuyết.
- Hoàn thành nghiên cứu, kiến trúc dự kiến và workspace BTL.
- Kiểm tra source xv6 ở Exercise, xác nhận phù hợp về mã nguồn.
- Tích hợp bộ nhớ agent từ project-memory-skill Light và lưu snapshot hai chat liên quan.
- Người dùng chọn tự gõ code, assistant hướng dẫn/kiểm tra. Đã nhập source riêng vào BTL, tạo lộ trình và Bài 1; Git baseline lưu bằng commit/tag local.
- Người dùng làm rõ: phải tự thiết kế và viết C, hiểu mọi reference/phụ thuộc. Đã bỏ lời giải hoàn chỉnh của bài tập và đổi sang dạy/giao từng bước nhỏ.
- Người dùng xác nhận đã đọc xong Lesson 0 và yêu cầu bài kế tiếp đầy đủ trong Markdown. Đã viết lại/mở rộng Lesson 1 có nghiên cứu nguồn chính, lý thuyết chi tiết, source map và bài tập từng chặng; không sửa implementation.
- Ngày 15/10 người dùng yêu cầu tạo bài tiếp theo; đã soạn Lesson 2 đầy đủ về race, spinlock/atomic, ngắt, noff/intena, CPU ID, deadlock và cơ sở per-CPU. Có bài phân tích A–F và hợp đồng helper quan sát tự viết.

### Đúng ở hiện tại

- Root BTL có source kernel/Makefile và Git repo local, tag nền xv6-baseline.
- Chưa triển khai slab/per-CPU lists hoặc chạy benchmark.
- Source tham chiếu: `D:\2. FullStack\2026.1\Hệ điều hành\Excecise\4\xv6-riscv`, commit `06aad25c735fd3159bdfae5680be4eab7b1668b2`.
- Môi trường lần kiểm tra 13/10/2026 chưa có distro WSL; chưa kiểm tra lại cài đặt trong lượt soạn Lesson 2 ngày 15/10. Chưa có kết quả build/boot/test tại BTL.
- Kiểm tra source/diff 15/10: kalloc.c và proc.h có thêm chú thích, chưa đổi phần xử lý allocator. Thư mục .vs/ hiện có, không xử lý trong lượt soạn tài liệu.

### Tiếp tục theo nhiệm vụ

- File mới nhất: docs/lessons/02-race-spinlock-va-cpu.md, soạn 15/10/2026. Đọc theo chặng A–E; bảng noff/intena dùng rc_sstatus đúng source hiện tại. Lesson 3 dự kiến free list theo CPU, chưa tạo trong lượt này.
- Người dùng đã xác nhận đọc xong Lesson 0. Lesson 1/2 có đầy đủ tài liệu; chưa suy ra hoàn thành bài code/baseline khi chưa có output. Nếu gửi bài, kiểm tra đúng chặng; nếu yêu cầu bài mới, tạo bài Markdown đầy đủ thay vì lặp lại kiến thức cũ trong chat.
- Không tự thêm/sửa implementation hoặc cung cấp toàn bộ lời giải để gõ theo. Giao một bước nhỏ, người dùng viết, assistant đọc và gợi ý sửa để người dùng thực hiện.
- Quy tắc mới: cuối mỗi phần có thay đổi code đã hoàn thành, gợi ý chú thích về lý do/trách nhiệm/invariant/ownership/khóa/ngắt, chỉ ra phần diff và kiểm tra thật, đề xuất commit message và nhắc người dùng tự commit rồi push GitHub. Không tự stage/commit/push khi chưa được yêu cầu rõ. Lesson 1/2 và lộ trình đã có phần chốt mốc.
- Nếu người dùng muốn kiểm tra bộ nhớ: mở `CURRENT_STATE.md`, `LOGBOOK.md`, `history/INDEX.md`.

### Nguồn cần tin

`CURRENT_STATE.md` → tài liệu liên quan trong `docs/` → mã nguồn/kết quả mới nhất. Transcript chỉ cần đọc khi tra cứu nguyên văn hoặc xác minh lịch sử.
