# Điểm tiếp tục

Cập nhật: 19/10/2026. Trạng thái trước và bằng chứng theo mốc nằm ở LOGBOOK.md; không cần nạp transcript mặc định.

## Checkpoint mới nhất

- Linh đã squash merge PR #1 thành 53872b8895e82f7baa453c077a0a5255b7310c79, title chore: add page-count diagnostics and team workflow, rồi chuyển GitHub Desktop về main và Pull origin. Local/origin main cùng commit ở lần đọc mới.
- CI main run39212699734 completed/success: linux qua format/build/usertests/crash, macos qua build. Run PR39210447048 tại head2df489d cũng success. Chưa chạy build/test local trong các lượt kiểm tra này; chưa có per-CPU/slab/benchmark.
- Bảo vệ main cho solo đã hoàn thành và đọc lại: ruleset main-workflow id25416827, active, target default branch, bypass rỗng; PR với 0 approvals, resolve conversations, linear history, chặn xóa/force push. Required checks linux và macos đều có integration ID15368; strict up-to-date tắt. Repo chỉ cho squash ở lần đọc gần nhất. Không yêu cầu Linh tạo lại ruleset hoặc thêm lại checks đã có.
- Chưa thêm Minh/Khánh vào GitHub; chưa xác nhận board/issues hoặc push tag xv6-baseline. Các việc này chưa tự động là điều kiện phải làm trước khi học tiếp.

## Bước nhỏ tiếp theo

- Có 4 file tài liệu/bộ nhớ chưa commit: CURRENT_STATE, LOGBOOK, RECOVERY_NOTES và docs/Thiet-lap-GitHub-26118.md. Hướng dẫn Desktop tạo nhánh codex/docs-main-workflow từ main, commit riêng 4 file với message docs: record verified main workflow, Push origin rồi Create Pull Request. Không tự stage/commit/push/merge.
- Kernel/kalloc.c có một dòng chú thích Linh tự thêm, chưa đổi logic ở lần đọc; VstHelper/ untracked có Git riêng. Giữ cả hai ngoài commit tài liệu; không sửa/format C thay Linh và không đặt chỉnh chú thích thành điều kiện tiếp tục.
- Sau chốt tài liệu, quay lại Lesson 3 P02: Linh tự khai báo kiểu pool/mảng; review từng thành phần rồi init, alloc/free/count ở M1 vẫn dùng pool 0. M2 mới chọn CPU và giới hạn CPUS=1; chưa stealing nên không khẳng định hỗ trợ nhiều CPU.
- Dùng GitHub Desktop cho hướng dẫn Git, tên nút cụ thể; chỉ dùng terminal khi người dùng yêu cầu hoặc thao tác không có trong Desktop. Với việc code, Linh tự thiết kế/viết, assistant giải thích và đọc diff; không cung cấp implementation đầy đủ.

## Bối cảnh học và nhóm

- Đề tài 19, dự án 26118: Nguyễn Kim Linh 20233495 (người dùng), Lê Trần Hiếu Minh 20233528, Nguyễn An Khánh 20224019. Trao đổi tiếng Việt, từng bước nhỏ. Linh muốn hiểu toàn bộ; phân công module trong tài liệu là đề xuất chưa được cả nhóm xác nhận.
- Tuần 19–25/10/2026 mới khởi động nhóm; học/code thử trước là chuẩn bị cá nhân của Linh. Không tự ghi là thành quả W01 của cả nhóm hoặc gán đóng góp cho hai bạn.
- Lesson 0 đã được xác nhận đọc xong; Lesson 1–3 đã soạn. Source main vẫn allocator một list/lock, có kfreepage_count, prototype và caller sau kinit; không giữ kết luận cũ chưa có hàm đếm.
- WSL2 có Ubuntu 26.04.1, make/RISC-V GCC/QEMU, clang-format21.1.8. Lỗi APT index universe rỗng đã được Linh xử lý và formatter đã cài, không quay lại sửa môi trường đã xong.
- Báo cáo nháp docs/bao-cao/26118-Progress-Report-W01-DRAFT.* chưa đủ dữ kiện để nộp: thiếu from/to thực tế, đóng góp Minh/Khánh, link design/board và xác nhận nội dung. Phải báo cáo mỗi tuần; >14 ngày là mất báo cáo. Cách tính sai khác 25%, giờ nộp chính thức và hạn cuối kỳ chưa rõ.

Đọc CURRENT_STATE.md cho sự thật/giới hạn hiện hành, tài liệu và source cho phần đang làm, LOGBOOK.md nếu cần truy nguyên. Không tạo checkpoint chỉ để ghi lại mỗi lần bấm nút khi quyết định và trạng thái có ý nghĩa chưa đổi.
