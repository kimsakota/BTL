# Trạng thái hiện tại

Cập nhật: 19/10/2026.

## Project status

- Giai đoạn: Đã soạn Lesson 0–3, quy trình nhóm/báo cáo và kiểm tra GitHub. Source đã có hàm đếm trang; Lesson 3 hướng dẫn tự đổi state/local path theo CPU. CI macOS đã build kernel/fs.img; chưa có kết quả runtime được đối chiếu.
- Mục tiêu: đề tài 19, Slab allocator and per-CPU free lists trên xv6; nối kiến thức từ BaoKhoaHoc/VstHelper.
- Người dùng trong chat này: Nguyễn Kim Linh; muốn làm nhiều và hiểu toàn bộ dự án.
- Nhóm dự án 26118: Nguyễn Kim Linh 20233495, Lê Trần Hiếu Minh 20233528, Nguyễn An Khánh 20224019. Đã có nhân lực; chưa xác nhận deadline, quỹ giờ và phân công với hai bạn.

## Stable facts

| ID | Statement | Confidence | Source | Revisit when | Last checked |
|---|---|---|---|---|---|
| CST-001 | Người dùng đã chốt đề tài 19 | Cao | LOGBOOK.md, L001; chat gốc | Người dùng đổi đề tài | 13/10/2026 |
| CST-002 | BTL có tài liệu, workspace và task cấu hình | Cao | README.md và các file hiện có | Cấu trúc dự án thay đổi | 13/10/2026 |
| CST-003 | Source xv6 phù hợp nằm ở thư mục Exercise được người dùng cung cấp | Cao | docs/Nguon-xv6.md | Import hoặc đổi baseline | 13/10/2026 |
| CST-004 | Baseline nguồn được kiểm tra là commit 06aad25c735fd3159bdfae5680be4eab7b1668b2 của mit-pdos/xv6-riscv | Cao | docs/Nguon-xv6.md | HEAD nguồn thay đổi | 13/10/2026 |
| CST-005 | Source đó dùng free list trang chung, PGSIZE=4096, NCPU=8; chưa có slab/stealing trong phần đã kiểm tra | Cao | docs/Nguon-xv6.md và kernel nguồn | Thay allocator hoặc phiên bản | 13/10/2026 |
| CST-006 | Allocator vẫn một list/lock chung, có kfreepage_count/prototype/caller. Chưa có per-CPU/slab. CI của checkpoint 58be186 đã qua usertests/crash; chưa có benchmark hoặc test đặc thù cho allocator mới | Cao về source và CI baseline | source hiện tại; GitHub run39209117349; L022 | Source/test mới | 19/10/2026 |
| CST-007 | WSL có Ubuntu 26.04.1 LTS, version 2; Make/RISC-V GCC/QEMU đã tìm thấy. Linh đã cài clang-format, kiểm tra trực tiếp thấy Ubuntu clang-format 21.1.8. Chưa build/boot/test local trong lượt kiểm tra | Cao về phát hiện công cụ | wsl/os-release/command -v và clang-format --version; L017 | Công cụ đổi hoặc trước build/run | 19/10/2026 |
| CST-008 | Bộ nhớ project-memory Light được tích hợp ở agent/, nối qua AGENTS.md | Cao | agent/README.md và các file bộ nhớ | Đổi bộ khung memory | 13/10/2026 |
| CST-009 | Git repo BTL có default branch origin/main và tag xv6-baseline local; tag chưa được push lên origin ở lần kiểm tra 19/10 | Cao | Git local, GitHub API ref và git ls-remote --tags origin; L016 | Push tag hoặc đổi nền source | 19/10/2026 |
| CST-010 | Mã dự án 26118, số đề tài 19; nhóm gồm Linh 20233495, Minh 20233528, Khánh 20224019; người dùng là Linh | Cao | Ảnh và yêu cầu trực tiếp 19/10; LOGBOOK L015 | Người dùng đính chính | 19/10/2026 |
| CST-011 | Run39209117349 tại head58be186 đã completed/success: Linux format/build/usertests/crash success; macOS build/job success. Chưa có per-CPU/slab hoặc benchmark; không có kalloctest.c/kcsan.c MIT lab trong BTL | Cao tại lúc kiểm tra | GitHub run và jobs; L022 | Source/run mới | 19/10/2026 |
| CST-012 | Tuần 19–25/10/2026 mới khởi động nhóm; phần Linh đang học là học trước/chuẩn bị cá nhân, không tự tính là kết quả nhóm W01 | Cao | Người dùng trả lời trực tiếp ngày 19/10; LOGBOOK L015 | Người dùng đính chính hoặc có kết quả W01 thật | 19/10/2026 |
| CST-013 | Nhánh review local/origin đều 58be186, ahead/behind 0/0; PR #1 head đã nhận commit format, còn mở/chưa merge, tổng 2 commit/20 file. Main vẫn c829ab9. Tài liệu và VstHelper còn ngoài commit | Cao tại lúc kiểm tra | Plugin GitHub và Git local; L021 | Nhánh/PR/working tree thay đổi | 19/10/2026 |
| CST-014 | Repo public, main.protected=false/rulesets rỗng ở lần audit; kiểm tra tiếp sau bước General thấy chỉ squash merge còn bật, merge commit/rebase tắt. Chưa đọc lại auto-delete và rulesets sau bước này; Linh chưa thêm cộng tác viên | Cao về trường đã đọc | Plugin GitHub và yêu cầu trực tiếp; L016–L017 | Cấu hình/thành viên đổi | 19/10/2026 |

## Active operating decisions

| ID | Operating decision | Why it is active now | Source | Revisit when | Last checked |
|---|---|---|---|---|---|
| OP-001 | Trao đổi bằng tiếng Việt, giải thích theo từng bước học | Người dùng đang học nền tảng | LOGBOOK.md L002 | Người dùng yêu cầu khác | 13/10/2026 |
| OP-002 | Giữ cấu trúc nhỏ, chia module C theo trách nhiệm trong xv6 | Người dùng yêu cầu không thừa thãi; khung hiện tại theo hướng này | docs/Kien-truc-du-an.md | Có nhu cầu module độc lập thực tế | 13/10/2026 |
| OP-003 | Tách sự thật đã kiểm chứng khỏi hướng thiết kế và giả thuyết | Đồ án chưa có implementation/benchmark | DOCS_GUIDE.md | Luôn áp dụng khi ghi kết quả | 13/10/2026 |
| OP-004 | Dùng tài liệu Light và snapshot local, không chạy memory service nền | Phù hợp yêu cầu thêm thư mục agent cho dự án nhỏ | LOGBOOK.md L006 | Cần hooks/tìm kiếm memory phức tạp | 13/10/2026 |
| OP-005 | Người dùng tự thiết kế/viết toàn bộ phần C; assistant dạy từng thành phần, giải thích reference và kiểm tra, không đưa lời giải hoàn chỉnh hoặc sửa implementation thay | Người dùng làm rõ mục tiêu hiểu và tự viết, không chỉ gõ theo code mẫu | LOGBOOK.md L008 | Người dùng yêu cầu rõ cách làm khác | 13/10/2026 |
| OP-006 | Khi chuyển bài, soạn Markdown đầy đủ kiến thức và chi tiết từng phần; thực hành chia chặng nhỏ, không giữ người dùng ở Lesson 0 khi họ đã xác nhận đọc xong | Người dùng yêu cầu tạo Lesson 1 đầy đủ, có nghiên cứu và source reference | LOGBOOK.md L011 | Người dùng đổi cách học | 14/10/2026 |
| OP-007 | Cuối phần code nêu diff/kiểm tra thật, gợi ý commit và nhắc người dùng tự commit/push; không tự stage/commit/push. Chú thích chỉ gợi ý khi cần, không bắt buộc và không trì hoãn mốc đã đạt để chỉnh trình bày comment | Linh giữ quy tắc tự commit/push, làm rõ không cần coi trọng chú thích | LOGBOOK L013, L019; AGENTS.md | Người dùng đổi quy tắc hoặc yêu cầu rõ thao tác Git | 19/10/2026 |
| OP-008 | Linh muốn tham gia sâu và hiểu toàn bộ, đồng thời chia việc độc lập cho đủ ba người. Owner theo module + review/giải thích chéo là phương án đề xuất, chưa được nhóm chốt | Yêu cầu tổ chức nhóm và báo cáo tuần | LOGBOOK.md L015; docs/Quy-trinh-nhom-26118.md | Nhóm xác nhận hoặc đổi vai trò | 19/10/2026 |
| OP-009 | Hướng dẫn Git bằng GitHub Desktop, chỉ rõ tên/vị trí nút; không mặc định giao lệnh Git terminal. Agent vẫn có thể đọc Git local để kiểm tra | Người dùng yêu cầu dùng GitHub Desktop và gửi ảnh UI | LOGBOOK L020; AGENTS.md | Người dùng đổi công cụ | 19/10/2026 |

## Current context

| Area | Current value | Source | Revisit when | Last checked |
|---|---|---|---|---|
| Current priority | CI checkpoint đã xanh hoàn toàn, PR #1 còn mở. Hướng dẫn Desktop bỏ tick VstHelper, commit riêng 11 file tài liệu/quy trình còn lại và push. Sau đó review PR/config trước hướng dẫn squash merge; không tự stage/commit/push/merge | LOGBOOK L022; Git và GitHub | Sau commit/push tài liệu hoặc người dùng đổi bước | 19/10/2026 |
| Build và runtime | Local chỉ xác nhận style, chưa chạy local build/test. CI run39209117349 completed/success: Linux format/build/usertests/crash đều success; macOS build success. Kết quả gắn với head58be186 và PR merge ref7e9ea622, chưa phải source perCPU/slab hay benchmark | Plugin GitHub run/jobs; L022 | Source/run mới hoặc thực hành local | 19/10/2026 |
| Learning progress | Đã xác nhận đọc xong Lesson 0; Lesson 1–3 đã soạn. Code hàm đếm đã có nhưng chưa xác nhận build/boot/test; không đồng nhất source với thực hành đã pass | LOGBOOK.md L011–L014, source hiện tại | Sau bài học tiếp theo | 17/10/2026 |
| Next implementation step | Kiểm tra/chốt hàm đếm nếu chưa làm. Người dùng tự khai báo pool/mảng, ghép M1 giữ pool 0 và count; kiểm tra rồi tự viết M2 local path. Không sửa C hoặc commit/push thay | docs/lessons/03-per-cpu-free-lists.md mục 11 | Bước tự viết được kiểm tra | 17/10/2026 |

## Current constraints

- Source tham chiếu: `D:\2. FullStack\2026.1\Hệ điều hành\Excecise\4\xv6-riscv`.
- Source đó có sửa Makefile xử lý dấu cách và hai khác biệt mode script; cần ghi nhận khi tạo baseline.
- API page `kfree` của xv6 khác ý nghĩa object free trong đề tài; lộ trình đặt tên nằm trong tài liệu kiến trúc.
- Các mở rộng buddy, reclaim và adaptive stealing mới là hướng khảo sát, chưa có kết quả.

## Open questions

- Overview/rubric, giờ nộp chính thức, hạn cuối kỳ và ngày from/to thực tế của W01; tuần khởi động đã xác nhận 19–25/10/2026.
- Đóng góp thật Minh/Khánh trong kỳ, quỹ giờ và GitHub usernames; URL design/board và ngày/permalink nộp chưa có.
- Cách giảng viên định lượng sai khác 25% và cumulative progress; quy ước nội bộ trong tài liệu chỉ là đề xuất.
- Cách môn học tính ít nhất 3 cấu trúc kernel chuyển động và quy định tên API cuối.
- Hiệu quả của batch stealing/adaptive policy chỉ có thể kết luận sau thực nghiệm.
