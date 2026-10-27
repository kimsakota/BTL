# Trạng thái hiện tại

Cập nhật: 24/10/2026.

## Quyết định vận hành W02 — 27/10/2026

- Linh chốt không tạo file/hồ sơ `docs/bao-cao/W02` riêng trong repo.
- Issue N01–N04 và các PR là nơi làm việc chính: mỗi buổi cập nhật comment có revision, lệnh, kết quả, link PR/log và blocker; review ghi ngay trên PR/Issue.
- Linh tổng hợp W02 bằng permalink comment/PR/log để chốt W03. Chỉ tạo file nộp ngoài GitHub nếu môn học bắt buộc, không dùng file đó làm sổ tiến độ.
- Comment nhận việc đầu tiên của Linh đã có trên Issue #17; chưa có comment W02 nào trên Issue #20.

## Project status

- Giai đoạn: Đã triển khai GitHub Issues/Projects theo yêu cầu Linh ngày 24/10: Project public số1, cây W02 #16 với sub-issue #17–#20, milestone W02, board Todo/Doing/Review/Done. Linh/Minh/Khánh đã được gán đúng việc; Minh/Khánh có WRITER. Tất cả Todo, chưa nhận là đã thực hiện code. Local đang ở nhánh update@c782bb5 với báo cáo/tài liệu chưa commit; không tự đổi nhánh hoặc push. Kế hoạch W02 và 8 tuần vẫn theo docs/Ke-hoach-va-backlog-26118.md.
- Mục tiêu: đề tài 19, Slab allocator and per-CPU free lists trên xv6; nối kiến thức từ BaoKhoaHoc/VstHelper.
- Người dùng trong chat này: Nguyễn Kim Linh; muốn làm nhiều và hiểu toàn bộ dự án.
- Nhóm dự án 26118: Nguyễn Kim Linh 20233495, Lê Trần Hiếu Minh 20233528, Nguyễn An Khánh 20224019. Linh xác nhận khoảng 10–12 giờ/người/tuần ngày 24/10; chưa xác nhận deadline chính thức và việc nhận toàn bộ phân công kỹ thuật của hai bạn.

## Stable facts

| ID | Statement | Confidence | Source | Revisit when | Last checked |
|---|---|---|---|---|---|
| CST-001 | Người dùng đã chốt đề tài 19 | Cao | LOGBOOK.md, L001; chat gốc | Người dùng đổi đề tài | 13/10/2026 |
| CST-002 | BTL có tài liệu, workspace và task cấu hình | Cao | README.md và các file hiện có | Cấu trúc dự án thay đổi | 13/10/2026 |
| CST-003 | Source xv6 phù hợp nằm ở thư mục Exercise được người dùng cung cấp | Cao | docs/Nguon-xv6.md | Import hoặc đổi baseline | 13/10/2026 |
| CST-004 | Baseline nguồn được kiểm tra là commit 06aad25c735fd3159bdfae5680be4eab7b1668b2 của mit-pdos/xv6-riscv | Cao | docs/Nguon-xv6.md | HEAD nguồn thay đổi | 13/10/2026 |
| CST-005 | Source đó dùng free list trang chung, PGSIZE=4096, NCPU=8; chưa có slab/stealing trong phần đã kiểm tra | Cao | docs/Nguon-xv6.md và kernel nguồn | Thay allocator hoặc phiên bản | 13/10/2026 |
| CST-006 | Allocator vẫn một list/lock chung, có kfreepage_count/prototype/caller. Chưa có per-CPU/slab. CI main53872b8 đã qua usertests/crash; chưa có benchmark hoặc test đặc thù cho allocator mới | Cao về source và CI baseline | source hiện tại; GitHub run39212699734; L023 | Source/test mới | 19/10/2026 |
| CST-007 | WSL có Ubuntu 26.04.1 LTS, version 2; Make/RISC-V GCC/QEMU đã tìm thấy. Linh đã cài clang-format, kiểm tra trực tiếp thấy Ubuntu clang-format 21.1.8. Chưa build/boot/test local trong lượt kiểm tra | Cao về phát hiện công cụ | wsl/os-release/command -v và clang-format --version; L017 | Công cụ đổi hoặc trước build/run | 19/10/2026 |
| CST-008 | Bộ nhớ project-memory Light được tích hợp ở agent/, nối qua AGENTS.md | Cao | agent/README.md và các file bộ nhớ | Đổi bộ khung memory | 13/10/2026 |
| CST-009 | Git repo BTL có default branch origin/main và tag xv6-baseline local; tag chưa được push lên origin ở lần kiểm tra 19/10 | Cao | Git local, GitHub API ref và git ls-remote --tags origin; L016 | Push tag hoặc đổi nền source | 19/10/2026 |
| CST-010 | Mã dự án 26118, số đề tài 19; nhóm gồm Linh 20233495, Minh 20233528, Khánh 20224019; người dùng là Linh | Cao | Ảnh và yêu cầu trực tiếp 19/10; LOGBOOK L015 | Người dùng đính chính | 19/10/2026 |
| CST-011 | Run main39212699734 tại head53872b8 đã completed/success: linux format/build/usertests/crash success; macos build/job success. Run PR39210447048 tại head2df489d cũng success. Chưa có per-CPU/slab hoặc benchmark; không có kalloctest.c/kcsan.c MIT lab trong BTL | Cao tại lúc kiểm tra | GitHub run và jobs; L023 | Source/run mới | 19/10/2026 |
| CST-012 | Tuần 19–25/10/2026 mới khởi động nhóm; phần Linh đang học là học trước/chuẩn bị cá nhân, không tự tính là kết quả nhóm W01 | Cao | Người dùng trả lời trực tiếp ngày 19/10; LOGBOOK L015 | Người dùng đính chính hoặc có kết quả W01 thật | 19/10/2026 |
| CST-013 | Local update@c782bb5 có DOCX W01 và tài liệu đang sửa, chưa commit/push. origin/main439e89b sau fetch cũ. GitHub Project1 và Issues16–20 đã tạo thật, không cần commit source để chúng xuất hiện | Cao tại lúc kiểm tra | Git local; GraphQL/REST và public Project | Commit/merge hoặc đổi working tree/Project | 24/10/2026 |
| CST-014 | Settings main lần đọc 19/10 giữ như lịch sử, chưa tái audit. Ngày 24/10 xác nhận assignees repo gồm kimsakota, Minhlth, ankhanh2594; Project1 linked BTL/public, Minh/Khánh được cấp WRITER. Không coi đã gán Issue là đã nhận/hoàn thành | Cao về API đã đọc/ghi; Settings cũ chỉ có giá trị tại ngày đọc | L025/L031; GitHub API và Project1 | Cấu hình/quyền/thành viên đổi | 24/10/2026 |

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
| OP-010 | Ưu tiên hoàn thiện quy trình ba người: mời/accept, clone, chia sẻ lesson, nhận task/board và review. Không tự chuyển sang bài C khi Linh còn hỏi cách tổ chức GitHub | Linh đính chính assistant chuyển sang Lesson 3 quá sớm và yêu cầu quy trình thực hành chung | Yêu cầu trực tiếp; LOGBOOK L026; AGENTS.md | Nhóm vận hành được hoặc Linh yêu cầu chuyển bài | 19/10/2026 |

## Current context

| Area | Current value | Source | Revisit when | Last checked |
|---|---|---|---|---|
| Current priority | Issues + Projects đã triển khai: https://github.com/users/kimsakota/projects/1; tree view1, board view3, cha W02 #16, N01 Linh #17, N02 Minh #18, N03 Khánh #19, N04 cả ba #20. Tất cả Todo, có ngày bắt đầu 26/10, hạn 30/10 cho code và 31/10 cho tổng hợp; milestone1 tới 01/11. Tiếp theo từng người xác nhận nhận việc và cập nhật theo tiến độ; Linh bắt đầu E01/P01/P02, không chờ học hết lesson | Yêu cầu Linh triển khai; API remote đã verify; quỹ 10–12 giờ/người/tuần | Nhận task, thay đổi scope hoặc kết quả code | 24/10/2026 |
| Build và runtime | Local chỉ xác nhận style, chưa chạy local build/test. CI main39212699734 tại 53872b8 completed/success: linux format/build/usertests/crash và macos build đạt. Đây là baseline có hàm đếm trang, chưa phải perCPU/slab hoặc benchmark | Plugin GitHub run/jobs; L023 | Source/run mới hoặc thực hành local | 19/10/2026 |
| Learning progress | Đã xác nhận đọc xong Lesson 0; Lesson 1–3 đã soạn. Hàm đếm đã vào main và CI baseline đạt; chưa suy ra Linh đã hoàn thành các bài thực hành khác | LOGBOOK L011–L014, L023; source và CI | Sau bài học tiếp theo | 19/10/2026 |
| Next implementation step | Linh có thể bắt đầu P02 sau baseline và review API ngắn: struct/mảng pool, init mọi khóa, ban đầu dùng pool0 và cập nhật count. P03 local-only là bước trung gian CPUS=1; P04/P05 hoàn thiện nhiều CPU ở W03. Không chờ học hết lesson và không tự viết C thay Linh | Lesson 3; kế hoạch W02; yêu cầu Linh 24/10 | Kết quả P02 hoặc nhóm đổi phạm vi | 24/10/2026 |

## Current constraints

- Source tham chiếu: `D:\2. FullStack\2026.1\Hệ điều hành\Excecise\4\xv6-riscv`.
- Source đó có sửa Makefile xử lý dấu cách và hai khác biệt mode script; cần ghi nhận khi tạo baseline.
- API page `kfree` của xv6 khác ý nghĩa object free trong đề tài; lộ trình đặt tên nằm trong tài liệu kiến trúc.
- Các mở rộng buddy, reclaim và adaptive stealing mới là hướng khảo sát, chưa có kết quả.

## Open questions

- Overview/rubric, giờ nộp chính thức, hạn cuối kỳ và ngày from/to thực tế của W01; tuần khởi động đã xác nhận 19–25/10/2026.
- Linh xác nhận cả ba đã xong onboarding; Minh có thiết kế S01 và tự báo cáo môi trường, Khánh vẫn ghi thiếu toolchain. Project/Issue URL đã có; còn cần các thành viên xác nhận nhận task, thu log cùng revision và permalink nộp báo cáo.
- Cách giảng viên định lượng sai khác 25% và cumulative progress; quy ước nội bộ trong tài liệu chỉ là đề xuất.
- Cách môn học tính ít nhất 3 cấu trúc kernel chuyển động và quy định tên API cuối.
- Hiệu quả của batch stealing/adaptive policy chỉ có thể kết luận sau thực nghiệm.
