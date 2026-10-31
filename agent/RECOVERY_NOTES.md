# Điểm tiếp tục

Cập nhật: 24/10/2026. Đối chiếu source/Git mới trước khi dùng trạng thái lịch sử trong LOGBOOK L031.

## GitHub Issues và Projects đã triển khai

- Linh yêu cầu triển khai ngay, đã tạo thật ngày 24/10: https://github.com/users/kimsakota/projects/1 (public, linked kimsakota/BTL). View1 Cây công việc đã bật hierarchy; view3 Bảng tiến độ nhóm cột theo Status gồm Todo/Doing/Review/Done. View2 thử ban đầu đã bỏ, chỉ còn hai view sử dụng.
- Milestone W02 số1; parent Issue16; sub-issue17 N01 gán kimsakota, 18 N02 gán Minhlth, 19 N03 gán ankhanh2594, 20 N04 gán cả ba. Checklist, reviewer, ngày bắt đầu 26/10 và hạn nội bộ đã ghi. Cả năm Issue open/Todo, không tick công việc kỹ thuật đã đạt.
- Minh và Khánh được cấp WRITER trên Project mới qua mutation thành công. Quan hệ cha–con, option/status/date, repo link và hierarchy public page đã kiểm tra. Không gửi tin riêng sang Teams/chat khác.
- GitHub connector đã kết nối, get_profile xác nhận kimsakota và create/update Issue dùng được. Token Git GCM thiếu project; Linh hoàn tất OAuth GitHub CLI (API đã xác nhận) để cấu hình Projects. CLI portable tại C:/Users/KimSakota/.codex/tmp/BTL-issues-projects-20261024/github-cli/bin/gh.exe, nguồn cli/cli và SHA256 đã verify. Không ghi token/device code vào repo.
- CUA và Computer Use runtime lỗi khởi tạo, không có UI action; đã làm qua API/CLI. Không nói chờ cấp quyền nữa: đăng nhập Projects đã thành công và bảng đã hoàn tất.
- Local lúc kết thúc đang nhánh update@c782bb5 do người dùng chuyển, giữ nguyên tất cả thay đổi tài liệu/DOCX; cập nhật docs/Thiet-lap-GitHub-26118.md, backlog/quy trình và checkpoint. Không tự commit/push hoặc sửa C/h/S. Tiếp theo mở Issue17 cho Linh, thành viên xác nhận nhận việc và cập nhật Doing khi làm thật.

## Việc tiếp theo đã được yêu cầu

- Linh yêu cầu phân việc W02 cho cả ba và kế hoạch hoàn thành trong 8 tuần, đồng thời muốn tự code và hiểu hết. Đã cập nhật docs/Ke-hoach-va-backlog-26118.md và docs/Quy-trinh-nhom-26118.md: W01 19–25/10 đến W08 07–13/12, Linh đã trả lời async xác nhận khoảng 10–12 giờ/người/tuần. Kế hoạch đã đổi giả định thành quỹ giờ xác nhận, gồm 6–7 giờ code/test, 2 giờ học, 1–2 giờ review/ghép và 1 giờ báo cáo. Phân công kỹ thuật vẫn chờ các thành viên nhận task.
- Phân công đề xuất: Linh trang/stealing/buffer/tích hợp, Minh slab/file, Khánh harness/inode/đo. W02 gói N01 Linh E01/P01/P02 + G02; N02 Minh review S01 rồi S02 một class; N03 Khánh hoàn tất môi trường và code test; N04 review/báo cáo. Issue16–20 và Project1 đã triển khai theo yêu cầu mới; chưa tạo nhánh code/PR, chưa coi gán việc là thành viên đã xác nhận nhận việc.
- Mục tiêu W03 lõi trang/object hoạt động; W05 đủ ba client; W06 khóa chức năng; W07 dữ liệu; W08 dự phòng/bàn giao. Buddy/adaptive/magazine để phần tùy chọn. Linh hiểu toàn bộ qua đọc PR, giải thích lại, chạy test và ghép thật, không nhận thay tác quyền hai bạn.
- VstHelper áp dụng nguyên lý bin/class, metadata/owner, slot và tái sử dụng ở tầng slab; chưa là code C hiện thực. Kế hoạch có bảng đối chiếu và thay đổi cần cho xv6. Chú ý S01: README Minh mới nói header32 byte/con trỏ nhúng, còn thiết kế hiện tại dùng field/index khác; cần review/chốt một layout trước S02.
- Đã fetch origin/main439e89b, đọc hồ sơ Minh mới; local vẫn c782bb5 vì working tree có báo cáo/bộ nhớ đang sửa. Không tự pull đè thay đổi. Việc này chỉ sửa tài liệu/bộ nhớ, không C/h/S, không commit/push.

## Báo cáo W01

- Linh xác nhận cả ba đã hoàn tất phần khởi động, yêu cầu tạo file DOCX. Đã tạo docs/bao-cao/W01/26118-Progress-Report-W01-2026-10-24.docx, kỳ 19–24/10/2026; 5 mục theo template, summary 261 từ, đóng góp cả ba, TODO dự kiến W02 và phụ lục minh chứng.
- Đã render qua Microsoft Word thành PDF QA rồi kiểm tra cả 4 ảnh trang; kiểm tra tên/MSSV, 13 hyperlink (8 đích file theo commit), hai ảnh nhúng và DOCX đọc lại được. Renderer đóng gói thiếu LibreOffice trên Windows nên dùng Word export; PDF/PNG/builder chỉ ở temp, không là file nộp. Không chạy xv6 test trong việc tạo tài liệu.
- Báo cáo và cập nhật tài liệu hiện local, chưa stage/commit/push; Linh chưa yêu cầu công bố báo cáo. Cần nhóm đọc lại và chốt TODO W02 trước nộp. Chưa ghi nhận gửi báo cáo trong thread môn học.

## Đầu ra của thành viên và Git

- Bản local đã đồng bộ c782bb5 ở lượt trước; remote mới439e89b thêm hồ sơ Minh, source allocator không đổi trong diff. Tại c782bb5: PR #10 sửa ảnh, #11 đổi ghi chú Linh thành W1_Linh.md, #13 thêm docs/slab_allocator_design.md của Minh. Không còn bốn ảnh trùng; giữ đúng 01-minh-chung.png và 02-minh-chung.png.
- Minh có W1_Minh.md, tự ghi đã build/boot, ls/forktest và nhận S01; thiết kế một class đã vào main. Chưa đối chiếu raw log cùng revision, không gán kết quả của Minh cho Linh.
- Khánh dùng ankhanh2594; hồ sơ được đọc từ codex/khanh-onboarding@7ddc0e2, có W1_Khanh.md, README W01 và hai ảnh. PR #15 xuất hiện trong hồ sơ/ảnh; không dùng trạng thái ảnh thay live state và không tự merge. Khánh ghi rõ thiếu toolchain RISC-V/chưa chạy make qemu; đăng ký K03/T01.
- Linh có W1_Linh.md, hồ sơ W01 và ảnh PR/History; tổ chức kho mã, cộng tác, tài liệu và báo cáo. Cả hai thành viên đã được thêm theo xác nhận trước; không mời lại.
- Chưa có implementation per-CPU/slab, ba client động hoặc benchmark. Mốc M0 chung chưa đủ log/môi trường của cả ba. TODO W02 là kế hoạch dự kiến, không là việc đã nghiệm thu.

## Cách theo dõi W02 đã chốt

- Linh không muốn tạo file/hồ sơ `docs/bao-cao/W02` riêng. W02 được theo dõi trực tiếp bằng comment trên Issue N01–N04 và review/PR; mỗi comment ghi đầu ra, revision, lệnh, kết quả, blocker và liên kết bằng chứng.
- Linh tổng hợp bằng permalink GitHub để chốt W03. Báo cáo W01 đã có không thay đổi; chỉ tạo file nộp ngoài GitHub nếu môn học bắt buộc.

## Giữ an toàn công việc trước

- Chỉ một worktree BTL ổ D, không tạo thêm. Giữ repo VstHelper gốc và dirty work riêng; snapshot tham khảo đã có trên main.
- Backup xử lý conflict cũ: C:/Users/KimSakota/.codex/backups/BTL-conflicts-20261023-aa7t56qa/. Stash65a2e92 gây xung đột đã sao lưu và bỏ; stash5221c62 cũ vẫn giữ, không áp lại khi không có yêu cầu.
- Người dùng tự viết C và tự commit/push bài học theo AGENTS.md. Quyền tự commit/push trước đây chỉ cho lần sửa Git/ảnh, không tự mở rộng sang báo cáo hoặc bài code mới.
