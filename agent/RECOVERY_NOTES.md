# Điểm tiếp tục

Cập nhật: 19/10/2026.

## Checkpoint mới nhất

### Tổ chức nhóm và báo cáo ngày 19/10

- Người dùng là Nguyễn Kim Linh 20233495; nhóm 26118 còn Lê Trần Hiếu Minh 20233528 và Nguyễn An Khánh 20224019. Số đề tài trong workspace là 19, không phải mã điền báo cáo.
- Linh muốn làm nhiều/hiểu toàn bộ, nhưng chia phần riêng cho đủ ba người. Đã nghiên cứu và viết docs/Quy-trinh-nhom-26118.md, docs/Ke-hoach-va-backlog-26118.md và docs/bao-cao/README.md. Phương án đề xuất Linh page/buffer/tích hợp, Minh slab/file, Khánh test/inode; chưa coi nhóm đã nhận việc.
- Linh xác nhận tuần này mới khởi động nhóm (19–25/10/2026); Lesson/code thử trước là học trước cá nhân, không tính mặc định là kết quả nhóm W01. Plan có K01–K04 cho W1, N01–N04 là TODO W2 có điều kiện; đề xuất tổng hợp nội bộ 24/10, chưa phải giờ nộp chính thức.
- Đã đọc chat được tham chiếu bằng read_thread, ảnh và hai PDF local; báo cáo nháp có đủ năm mục và summary 251 từ. File Markdown/PDF tại docs/bao-cao/26118-Progress-Report-W01-DRAFT.*; chưa sẵn sàng nộp vì thiếu ngày from/to thực tế, đóng góp thực của hai bạn, URL design/board và xác nhận nội dung của Linh.
- Thông báo yêu cầu ít nhất một báo cáo mỗi tuần; >14 ngày là mất báo cáo, không phải chu kỳ nộp cho phép. Sai khác 25% chưa rõ cách tính, tài liệu chỉ đưa quy ước nội bộ. Hạn cuối kỳ/giờ nộp chính thức còn thiếu.
- Source 19/10 vẫn single-list có hàm đếm, chưa per-CPU/slab; không có kalloctest.c/kcsan.c của MIT lab. Chưa build/boot/test/benchmark, không sửa C/h/S/Makefile, không tự stage/commit/push hay gửi báo cáo.
- Có mẫu task/PR trong .github để nhóm dùng sau khi tự push; chưa tạo issue/board/PR từ xa. Bước tiếp theo theo yêu cầu mới: chốt dữ kiện báo cáo hoặc quay lại Lesson 3 dưới đây.

### Đã làm

- Người dùng đã chốt đề tài 19 và bắt đầu học lý thuyết.
- Hoàn thành nghiên cứu, kiến trúc dự kiến và workspace BTL.
- Kiểm tra source xv6 ở Exercise, xác nhận phù hợp về mã nguồn.
- Tích hợp bộ nhớ agent từ project-memory-skill Light và lưu snapshot hai chat liên quan.
- Người dùng chọn tự gõ code, assistant hướng dẫn/kiểm tra. Đã nhập source riêng vào BTL, tạo lộ trình và Bài 1; Git baseline lưu bằng commit/tag local.
- Người dùng làm rõ: phải tự thiết kế và viết C, hiểu mọi reference/phụ thuộc. Đã bỏ lời giải hoàn chỉnh của bài tập và đổi sang dạy/giao từng bước nhỏ.
- Người dùng xác nhận đã đọc xong Lesson 0 và yêu cầu bài kế tiếp đầy đủ trong Markdown. Đã viết lại/mở rộng Lesson 1 có nghiên cứu nguồn chính, lý thuyết chi tiết, source map và bài tập từng chặng; không sửa implementation.
- Ngày 15/10 người dùng yêu cầu tạo bài tiếp theo; đã soạn Lesson 2 đầy đủ về race, spinlock/atomic, ngắt, noff/intena, CPU ID, deadlock và cơ sở per-CPU. Có bài phân tích A–F và hợp đồng helper quan sát tự viết.
- Ngày 17/10 đã tạo Lesson 3 đầy đủ: pool/mảng struct, ownership, init, guard, local free/alloc, count nhiều pool, bài A–I và mốc M1/M2 với chú thích/commit.

### Đúng ở hiện tại

- Root BTL có source kernel/Makefile và Git repo local, tag nền xv6-baseline.
- Chưa triển khai slab/per-CPU lists hoặc chạy benchmark.
- Source tham chiếu: `D:\2. FullStack\2026.1\Hệ điều hành\Excecise\4\xv6-riscv`, commit `06aad25c735fd3159bdfae5680be4eab7b1668b2`.
- Môi trường lần kiểm tra 13/10/2026 chưa có distro WSL; chưa kiểm tra lại cài đặt trong lượt soạn Lesson 3 ngày 17/10. Chưa có kết quả build/boot/test được đối chiếu tại BTL.
- Source/diff 17/10 đã có kfreepage_count, prototype defs.h và caller ngay sau kinit trong main.c. Allocator vẫn một list/lock chung; không giữ kết luận cũ “chưa có hàm đếm”. Không sửa hoặc chốt Git thay người dùng.

### Tiếp tục theo nhiệm vụ

- File mới nhất: docs/lessons/03-per-cpu-free-lists.md, soạn 17/10/2026. M1 refactor state thành mảng nhưng giữ pool 0 chung; M2 chọn local theo CPU, chưa stealing và giới hạn kiểm chứng CPUS=1. Lesson 4 bổ sung remote allocation trước tiêu chí nhiều CPU.
- Lesson 3 cập nhật cả hàm đếm: quét từng pool giữ khóa tương ứng, không giữ nhiều lock. Tổng chính xác ở boot yên; runtime đọc lần lượt không là snapshot đồng thời.
- Người dùng xác nhận đọc xong Lesson 0. Lesson 1–3 có tài liệu đầy đủ; chưa suy ra hoàn thành code/baseline khi chưa có output. Nếu gửi bài thì kiểm tra đúng chặng; nếu yêu cầu bài mới thì tạo Markdown đầy đủ.
- Không tự thêm/sửa implementation hoặc cung cấp toàn bộ lời giải để gõ theo. Giao một bước nhỏ, người dùng viết, assistant đọc và gợi ý sửa để người dùng thực hiện.
- Quy tắc mới: cuối mỗi phần có thay đổi code đã hoàn thành, gợi ý chú thích về lý do/trách nhiệm/invariant/ownership/khóa/ngắt, chỉ ra phần diff và kiểm tra thật, đề xuất commit message và nhắc người dùng tự commit rồi push GitHub. Không tự stage/commit/push khi chưa được yêu cầu rõ. Lesson 1/2 và lộ trình đã có phần chốt mốc.
- Nếu người dùng muốn kiểm tra bộ nhớ: mở `CURRENT_STATE.md`, `LOGBOOK.md`, `history/INDEX.md`.

### Nguồn cần tin

`CURRENT_STATE.md` → tài liệu liên quan trong `docs/` → mã nguồn/kết quả mới nhất. Transcript chỉ cần đọc khi tra cứu nguyên văn hoặc xác minh lịch sử.
