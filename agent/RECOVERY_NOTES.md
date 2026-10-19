# Điểm tiếp tục

Cập nhật: 19/10/2026. Trạng thái trước và bằng chứng theo mốc nằm ở LOGBOOK.md; không cần nạp transcript mặc định.

## Checkpoint mới nhất

- Baseline main sau PR #1 là 53872b8895e82f7baa453c077a0a5255b7310c79. Linh đã commit/push tài liệu lên codex/docs-main-workflow và tạo PR #2; PR còn open/not merged ở lần đọc mới, headff8a36a. Trước lượt sửa mới local/origin nhánh docs cùng head, chỉ VstHelper untracked; không giữ nhận định cũ có 4 file chưa commit.
- CI main run39212699734 completed/success: linux qua format/build/usertests/crash, macos qua build. Run PR39210447048 tại head2df489d cũng success. Chưa chạy build/test local trong các lượt kiểm tra này; chưa có per-CPU/slab/benchmark.
- Bảo vệ main cho solo đã hoàn thành và đọc lại: ruleset main-workflow id25416827, active, target default branch, bypass rỗng; PR với 0 approvals, resolve conversations, linear history, chặn xóa/force push. Required checks linux và macos đều có integration ID15368; strict up-to-date tắt. Repo chỉ cho squash ở lần đọc gần nhất. Không yêu cầu Linh tạo lại ruleset hoặc thêm lại checks đã có.
- Theo Linh, Minh và Khánh chưa được thêm vào repo. Chưa có username/profile xác nhận; đã hỏi qua async. Connector không hỗ trợ endpoint list collaborators/invitations ở lần thử mới, nên không giả định trạng thái từ API. Chưa xác nhận board/issues hoặc push tag xv6-baseline.

## Bước nhỏ tiếp theo

- **Ưu tiên đã sửa theo yêu cầu Linh:** đang hướng dẫn GitHub cho cả nhóm, không chuyển sang bài C. Cần mời/accept Minh và Khánh, clone, chia sẻ lesson, nhận issue/board, thử PR/review rồi mới tiếp tục công việc kỹ thuật theo nhóm.
- Steering mới: Linh muốn up VstHelper và hỏi branch/main. Đã chuẩn bị references/VstHelper để chia sẻ snapshot 43 file nguồn/tài liệu (~1,2 MB), thêm README/SNAPSHOT.json với provenance/dirty status/hash. Repo VstHelper gốc có origin ShiftSort@7236e72 và thay đổi chưa commit, đã giữ nguyên .git/HEAD/status và ignore /VstHelper/ tại BTL. Không xóa/move repo gốc. Chưa stage/commit/push. Chốt docs trong PR #2 trước, rồi main cập nhật → nhánh codex/reference-vsthelper → chỉ .gitignore và snapshot → PR vào main. Đây là chuẩn bị snapshot cho yêu cầu chia sẻ, không mặc định thư viện đã publish.
- Sổ tay docs/So-tay-GitHub-nhom.md và docs/lessons/README.md mới nằm ở local; chưa publish. Đã sửa 94 link nội bộ trong 6 Markdown thành tương đối; cập nhật README, CONTRIBUTING, templates, kế hoạch/quy trình và bộ nhớ. Các file mới/sửa chưa tự commit/push. PR #2 hiện có là PR workflow, chưa merge; không tự tạo nhánh/commit để tránh hỏi người dùng.
- Bước ngay: có username chính xác thì Linh mời qua Settings → Collaborators → Add people, hai bạn accept và clone bằng Desktop. Mỗi người thử một PR ghi chú onboarding; khi có reviewer sẵn sàng mới tăng approvals lên 1. Project có quyền riêng: thêm hai bạn với Write trên board. K01–K04 và phân công trong sổ tay là đề xuất cần nhận, không là thành quả đã làm.
- VstHelper gốc có Git riêng đã được ignore; không stage clone gốc. Chỉ references/VstHelper là bản chuẩn bị chia sẻ. User đã commit một chú thích kalloc.c vào PR #2 và sửa trailing whitespace thành ff8a36a; không sửa/format C thay Linh hoặc đặt chỉnh chú thích thành điều kiện cộng tác.
- Khi Linh muốn quay lại học sau, P02 là pool/mảng rồi init/alloc/free/count M1 dùng pool 0; M2 local-only CPUS=1, sau đó mới stealing. Hiện không giao bài này.
- Dùng GitHub Desktop cho hướng dẫn Git, tên nút cụ thể; chỉ dùng terminal khi người dùng yêu cầu hoặc thao tác không có trong Desktop. Với việc code, Linh tự thiết kế/viết, assistant giải thích và đọc diff; không cung cấp implementation đầy đủ.

## Bối cảnh học và nhóm

- Đề tài 19, dự án 26118: Nguyễn Kim Linh 20233495 (người dùng), Lê Trần Hiếu Minh 20233528, Nguyễn An Khánh 20224019. Trao đổi tiếng Việt, từng bước nhỏ. Linh muốn hiểu toàn bộ; phân công module trong tài liệu là đề xuất chưa được cả nhóm xác nhận.
- Tuần 19–25/10/2026 mới khởi động nhóm; học/code thử trước là chuẩn bị cá nhân của Linh. Không tự ghi là thành quả W01 của cả nhóm hoặc gán đóng góp cho hai bạn.
- Lesson 0 đã được xác nhận đọc xong; Lesson 1–3 đã soạn. Source main vẫn allocator một list/lock, có kfreepage_count, prototype và caller sau kinit; không giữ kết luận cũ chưa có hàm đếm.
- WSL2 có Ubuntu 26.04.1, make/RISC-V GCC/QEMU, clang-format21.1.8. Lỗi APT index universe rỗng đã được Linh xử lý và formatter đã cài, không quay lại sửa môi trường đã xong.
- Báo cáo nháp docs/bao-cao/26118-Progress-Report-W01-DRAFT.* chưa đủ dữ kiện để nộp: thiếu from/to thực tế, đóng góp Minh/Khánh, link design/board và xác nhận nội dung. Phải báo cáo mỗi tuần; >14 ngày là mất báo cáo. Cách tính sai khác 25%, giờ nộp chính thức và hạn cuối kỳ chưa rõ.

Đọc CURRENT_STATE.md cho sự thật/giới hạn hiện hành, tài liệu và source cho phần đang làm, LOGBOOK.md nếu cần truy nguyên. Không tạo checkpoint chỉ để ghi lại mỗi lần bấm nút khi quyết định và trạng thái có ý nghĩa chưa đổi.
