# Điểm tiếp tục

Cập nhật: 19/10/2026.

## Checkpoint mới nhất

- Cập nhật sau khi Linh báo CI xong: run39209117349 đã completed/success, cả Linux (format/build/usertests/crash) và macOS success tại head58be186. PR #1 còn mở, 2 commits/20 files, chưa merge. Bước Desktop tiếp theo: bỏ chọn VstHelper, commit riêng 11 file docs/quy trình (gồm memory, templates, AGENTS, README, CONTRIBUTING và hướng dẫn), rồi Push origin. Chỉ các docs cần chốt; source format đã commit/push và CI baseline đạt. Không tự thao tác Git ghi/merge. Chưa có perCPU/slab/benchmark.

- Mới nhất sau bấm Push origin: đã xác nhận remote và PR #1 nhận 58be186, local/origin 0/0; PR mở, 2 commit/20 file. Desktop Fetch origin và 12 Changes còn lại là bình thường vì các file đó chưa commit. Run PR mới 39209117349: Linux format + build success, usertests đang chạy, crash pending; macOS job success. Không kết luận toàn CI/runtime đã pass. Khi tiếp tục đọc trạng thái run mới trước hướng dẫn merge; không yêu cầu push lại hoặc commit 12 file chỉ để tab Changes biến mất.

- Ưu tiên mới nhất: Linh yêu cầu hướng dẫn GitHub Desktop, đã lưu AGENTS/OP-009. Ảnh UI và Git xác nhận commit 58be186 style(kernel): fix formatting đã chứa đúng 5 file, local ahead origin 1; bước tiếp bấm Push origin phía trên. Tab Changes còn 12 file tài liệu/VstHelper ngoài commit; không dùng nút Commit 12 files để push commit format. Chưa xác nhận push/CI mới. Không tự thao tác Git ghi thay người dùng; không quay lại yêu cầu chỉnh chú thích.

### Tiếp tục sau bước General ngày 19/10

- Mới nhất sau “Đã format”: git diff đúng 5 file bio.c/kalloc.c/main.c/spinlock.c/string.c; chỉ định dạng, chưa thấy đổi logic. git diff --check và clang-format --dry-run --Werror trên kernel/user/mkfs đạt. Chú thích dài của acquire/release trong kfree vẫn ở cuối dòng nên formatter tách lời gọi; gợi ý chuyển lên dòng riêng. Index chưa stage gì; hướng dẫn Linh tự stage/commit/push đúng 5 file này, giữ docs/VstHelper ngoài commit format. Chưa chạy build/test hoặc xác nhận CI mới.
- Mới nhất: Linh báo cài formatter xong; kiểm tra trực tiếp `wsl --exec clang-format --version` thấy Ubuntu clang-format 21.1.8 (6ubuntu1). Chưa có diff C mới, chưa chạy format/build/test. Bước tiếp Linh tự đưa chú thích dài lên dòng riêng, chạy make fmt và báo để assistant đọc diff trước commit format riêng.
- Đã đọc lại GitHub: squash bật, merge commit/rebase tắt; chưa kiểm tra lại các setting khác. CI chưa có run mới và source format chưa đổi.
- Môi trường local đã khác lần 13/10: WSL2 có Ubuntu 26.04.1, make/RISC-V GCC/QEMU tìm thấy; thiếu clang-format. CI đã dùng gói clang-format 21.1.6. Bước nhỏ tiếp theo hướng dẫn Linh cài formatter trong Ubuntu, tự chỉnh chú thích dài/format, gửi diff để review trước khi chốt commit riêng. Không tự sửa C hoặc cài công cụ thay người dùng trong lượt này.
- Cài clang-format vướng cache APT lỗi: Linh chạy apt update chỉ Hit, index resolute/universe Packages vẫn 0 byte. InRelease yêu cầu bản giải nén 76.792.699 byte; HEAD HTTPS Packages.xz trả 200 và 15.960.824 byte. Nguồn universe đã bật, không cần thêm kho. Bước hướng dẫn tiếp: Linh xóa đúng file index rỗng /var/lib/apt/lists/archive.ubuntu.com_ubuntu_dists_resolute_universe_binary-amd64_Packages rồi apt update/policy để tải lại. Chưa xóa cache/cài gói thay người dùng; sudo -n yêu cầu xác thực tương tác.

### Kiểm tra GitHub tiếp ngày 19/10

- Linh yêu cầu kiểm tra repo/quy trình/commit; hiện chưa thêm cộng tác viên. Đã dùng plugin GitHub kiểm tra live kimsakota/BTL và đối chiếu local. Local đang ở codex/linh-review-existing-changes@7a61ccf, cùng origin; main@c829ab9 có 4 commit; PR #1 đang mở, 17 file. Tài liệu/code chuẩn bị đã được checkpoint lên nhánh trước lượt audit này, không còn là toàn bộ thay đổi chưa commit như checkpoint trước.
- Lịch sử thẳng nhưng có 2 commit tên dấu chấm và scope trộn. Đề xuất giữ main, áp dụng task/PR/squash từ đây và tách checkpoint nếu bàn giao; chưa rewrite/force push/commit/merge. Tag xv6-baseline chỉ local, origin chưa có tag. VstHelper/ untracked có Git riêng, không tự stage.
- Bằng chứng mới thay nhận định chưa đối chiếu CI: run 39202934361, macOS build kernel/fs.img thành công trên PR merge ref; Linux format fail ở bio.c/kalloc.c/main.c/spinlock.c/string.c nên build/usertests/crash skipped. Chưa có runtime/benchmark. Không tự format/sửa C thay Linh.
- Main.protected=false và rulesets=[]; merge commit/squash/rebase đều bật, auto-delete branch tắt. Settings đề xuất: squash, PR vào main, 0 required approvals khi solo → 1 khi có reviewer, require CI sau khi ổn định. Chưa áp dụng settings hoặc mời ai.
- Đã chuẩn bị docs/Thiet-lap-GitHub-26118.md và CONTRIBUTING.md; sửa link README thành tương đối, templates solo/team và cập nhật quy trình/bộ nhớ. Chưa commit/push; chưa sửa workflow/C/h/S/Makefile. PR #1 đã attach vào chat để tham chiếu.
- Khi tiếp tục, đọc checklist GitHub; nếu người dùng muốn tách/commit/push/đổi settings thì xác định yêu cầu cụ thể đã được cho phép. Việc học và báo cáo W1 phía dưới vẫn giữ bối cảnh chuẩn bị cá nhân/nhóm.

### Tổ chức nhóm và báo cáo ngày 19/10

- Người dùng là Nguyễn Kim Linh 20233495; nhóm 26118 còn Lê Trần Hiếu Minh 20233528 và Nguyễn An Khánh 20224019. Số đề tài trong workspace là 19, không phải mã điền báo cáo.
- Linh muốn làm nhiều/hiểu toàn bộ, nhưng chia phần riêng cho đủ ba người. Đã nghiên cứu và viết docs/Quy-trinh-nhom-26118.md, docs/Ke-hoach-va-backlog-26118.md và docs/bao-cao/README.md. Phương án đề xuất Linh page/buffer/tích hợp, Minh slab/file, Khánh test/inode; chưa coi nhóm đã nhận việc.
- Linh xác nhận tuần này mới khởi động nhóm (19–25/10/2026); Lesson/code thử trước là học trước cá nhân, không tính mặc định là kết quả nhóm W01. Plan có K01–K04 cho W1, N01–N04 là TODO W2 có điều kiện; đề xuất tổng hợp nội bộ 24/10, chưa phải giờ nộp chính thức.
- Đã đọc chat được tham chiếu bằng read_thread, ảnh và hai PDF local; báo cáo nháp có đủ năm mục và summary 251 từ. File Markdown/PDF tại docs/bao-cao/26118-Progress-Report-W01-DRAFT.*; chưa sẵn sàng nộp vì thiếu ngày from/to thực tế, đóng góp thực của hai bạn, URL design/board và xác nhận nội dung của Linh.
- Thông báo yêu cầu ít nhất một báo cáo mỗi tuần; >14 ngày là mất báo cáo, không phải chu kỳ nộp cho phép. Sai khác 25% chưa rõ cách tính, tài liệu chỉ đưa quy ước nội bộ. Hạn cuối kỳ/giờ nộp chính thức còn thiếu.
- Ở lượt soạn quy trình trước audit GitHub: source vẫn single-list có hàm đếm, chưa per-CPU/slab; không có kalloctest.c/kcsan.c MIT lab, chưa đối chiếu build/boot/test. Bằng chứng CI mới xem mục kiểm tra GitHub ở trên.
- Mẫu task/PR đã có trong nhánh checkpoint PR #1, chưa vào main. Bước theo yêu cầu báo cáo: chốt dữ kiện W01 hoặc quay lại Lesson 3 dưới đây.

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
