# Điểm tiếp tục

Cập nhật: 23/10/2026. Checkpoint này và LOGBOOK L029 thay các nhận định workflow cũ ngày 19/10; CURRENT_STATE.md còn một số trạng thái lịch sử cần đối chiếu source/Git mới trước khi dùng.

## Việc đang làm

- Linh xác nhận W01 mới tạo cấu trúc dự án và dùng GitHub, hỏi báo cáo/minh chứng rồi yêu cầu lưu trong Git theo tuần. Không ghi đã thiết kế/triển khai slab/per-CPU hoặc có benchmark.
- Đã tạo local docs/bao-cao/W01/ với README mục lục, thư mục linh/minh/khanh/chung có README để điền đóng góp, ảnh/log, link PR và TODO. Báo cáo tổng hợp .md/.pdf sau này đặt tại W01. Tuần sau tạo W02 cùng cấu trúc, không copy kết quả cũ thành kết quả mới.
- Đã bổ sung quy tắc lưu/đặt tên, gửi PR bằng Desktop và nộp báo cáo trong docs/bao-cao/README.md, nối từ README root và sổ tay. Giữ nguyên bản nháp cũ và các link đến nó. Chưa có ảnh/log mới hoặc bản nộp cuối; các README mới là khung chờ từng người điền.
- Kiểm tra 8 Markdown và 44 target tương đối hiện có đạt, 5 file W01 mới không có trailing whitespace, git diff --check đạt. Link docs/Huong-dan-thanh-vien.md thiếu từ trước không được sửa trong việc này. Không build/test vì chỉ sửa tài liệu.

## Bước tiếp theo

- Linh tạo nhánh codex/docs-weekly-evidence trong Desktop, mang các thay đổi tài liệu theo nhánh nếu được hỏi; xem Changes và tự chọn file thuộc việc này, commit/push rồi mở PR vào main. Chưa tự stage/commit/push hoặc tạo branch/worktree thay người dùng.
- Sau merge, Minh/Khánh pull main, mỗi người tạo nhánh theo việc/tuần, cập nhật thư mục cá nhân và gửi PR. Linh dùng các đầu ra để tổng hợp đủ 5 mục báo cáo, chốt ngày thực tế và TODO người phụ trách đã nhận. Nháp cũ còn placeholder/thông tin cũ, không nộp nguyên trạng.
- Báo cáo vẫn cần đính kèm reply trong thread môn học, không chỉ up ảnh lên GitHub. Ghi tên file, ngày giờ Asia/Saigon và permalink nộp trong chung/README.md khi có thật.

## Dữ kiện cần giữ đúng

- Linh đã xác nhận thêm Minh và Khánh; không mời lại. Chưa xác nhận đầy đủ clone/review/board. Ưu tiên workflow nhóm trước khi tự chuyển sang bài C.
- PR #2 đã merge thành 3baf33f, có tài liệu nhóm và snapshot references/VstHelper. Không upload snapshot lần nữa; giữ Git riêng và dirty work của repo VstHelper gốc.
- Kiểm tra trước trong chat: main/origin/main c9b0128, commit summary (#3) của Minhlth chỉ thêm docs/onboarding/minh.md còn rỗng. PR thử chưa tự chứng minh đã có khảo sát kỹ thuật. Chưa kiểm tra remote mới hơn tracking ref.
- Worktree phụ đã dọn; lần kiểm tra 20/10 chỉ còn BTL ổ D:. Backup cũ tại C:/Users/KimSakota/.codex/backups/BTL-member-guide-20261020-juf2onp2/saved-worktree.zip. Không tạo thêm worktree cho việc lưu ảnh.
- CI baseline 53872b8 run39212699734 đã xác nhận linux format/build/usertests/crash và macos build đạt ở lần đọc trước. Không có test local mới; không áp kết quả cũ cho revision khác. Source vẫn list/lock chung, có kfreepage_count, chưa slab/per-CPU.
- Bảo vệ main lần đọc 19/10: PR, 0 approvals, linux/macos và squash. Chưa tái kiểm tra Settings; tăng 1 approval khi có reviewer thực hiện được.
- Template báo cáo có thông tin chung, summary 200–300 từ/cumulative progress, technical activities, đóng góp đủ ba người, code review (algorithm/source/testing), TODO (issues/next tasks/milestone). Mẫu không quy định số ảnh; khuyến nghị ảnh repo local và PR có nội dung kèm link.
