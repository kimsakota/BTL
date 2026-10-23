# Điểm tiếp tục

Cập nhật: 23/10/2026. Nguồn chuẩn hiện tại: CURRENT_STATE.md và LOGBOOK L031. Các checkpoint cũ là lịch sử, không áp lại trạng thái nhánh/PR hoặc tên ảnh đã đổi.

## Trạng thái sau sửa lỗi local

- Linh đã yêu cầu assistant tự xử lý, commit và push lần sửa này, xóa hai ảnh linh-chung. Quyền này chỉ cho lần sửa hiện tại; chưa yêu cầu merge hoặc auto-commit bài học sau.
- Main và origin/main đã được đồng bộ ở 5c5fb99, có PR #9. Commit local b00fe36 từng xóa nhầm hai ghi chú Minh đã lưu trong backup; hai ghi chú W1_Minh.md và minh.md được giữ đúng như origin/main.
- Phần sửa nằm trên một nhánh codex/fix-w01-evidence từ main mới. Ba xung đột Markdown do áp stash đã được gỡ; README của Linh dùng bản ngắn 3 phần, dẫn đúng 01-minh-chung.png và 02-minh-chung.png. Hai ảnh 01-linh-chung.png/02-linh-chung.png được xóa theo yêu cầu; không sửa ảnh giữ lại hoặc implementation.
- Đã lưu working files, raw index, 9 stage blobs và full history bundle trong C:/Users/KimSakota/.codex/backups/BTL-conflicts-20261023-aa7t56qa/. Stash gây xung đột65a2e92 đã được sao lưu và bỏ để tránh áp lại; stash5221c62 chứa hướng dẫn thành viên từ trước vẫn giữ nguyên.
- Đã push thành công codex/fix-w01-evidence và thiết lập upstream origin/codex/fix-w01-evidence. Commit sửa nội dung là 8b49680; sau đó Linh có fix và revert fix, không thay đổi kết quả cuối của bản sửa. Remote đã được đối chiếu đúng HEAD. Main trên GitHub vẫn ở 5c5fb99; chỉ nhánh sửa đã hết hai ảnh trùng, cần PR/merge để main nhận phần sửa. Chưa xác nhận CI mới; không force-push.

## Bước tiếp theo

- Nhánh sửa đã publish, không yêu cầu Linh commit/publish lại khi không có thay đổi mới. Có thể mở PR tại https://github.com/kimsakota/BTL/compare/main...codex/fix-w01-evidence?expand=1 để đưa phần sửa vào main; chưa tạo PR thay người dùng trong lượt push này. Không dùng lại PR #8/#9 vì đã merge.
- Sau merge, về main và Fetch/Pull trước việc mới. Kết thúc công việc bằng commit/push hoặc giữ thay đổi trên đúng nhánh; không mang các sửa README/bộ nhớ chưa commit sang main cũ bằng stash.
- Tiếp tục lấy đóng góp/ảnh/log thật của cả ba và tổng hợp báo cáo W01. Theo Linh, tuần đầu chủ yếu tạo cấu trúc dự án và dùng GitHub. Chưa có bản báo cáo cuối được nộp; GitHub lưu minh chứng không thay reply đính kèm báo cáo trong thread môn học.

## Ngữ cảnh nhóm và giới hạn

- Đề tài 19 trên xv6, mã 26118; Linh 20233495, Minh 20233528, Khánh 20224019. Minh/Khánh đã được thêm vào repo theo xác nhận của Linh; không mời lại. Ưu tiên workflow nhóm khi người dùng đang hỏi GitHub.
- Tài liệu nhóm và snapshot references/VstHelper đã có trên main. Không upload lại snapshot hoặc xóa Git/dirty work của clone VstHelper gốc.
- Minh dùng tài khoản Minhlth, có ghi chú onboarding/môi trường/test trên main. Đây là tự báo cáo của Minh; ảnh Linh xem diff đó không chứng minh test do Linh chạy. Khánh/board/đóng góp hoàn chỉnh còn cần bằng chứng.
- CI baseline53872b8 run39212699734 từng xác nhận linux format/build/usertests/crash và macos build đạt. Không có build/test mới trong sửa tài liệu này, chưa có slab/per-CPU hoặc benchmark. Không đổi workflow chỉ vì hai sự kiện push/pull_request cùng chạy CI.
- Template báo cáo giữ thông tin chung và 5 mục: summary 200–300 từ/cumulative progress, activities, đóng góp đủ ba người, code review (algorithm/source/testing), TODO (issues/next tasks/milestone). Mẫu không ấn định số ảnh.
