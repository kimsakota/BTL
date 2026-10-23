# Trạng thái hiện tại

Cập nhật: 23/10/2026.

## Project status

- Giai đoạn: Đang hoàn thiện hồ sơ báo cáo W01 về cấu trúc dự án và dùng GitHub. Minh/Khánh đã được thêm vào repo theo Linh xác nhận. PR #2/#4/#8/#9 đã merge; main và origin/main ở 5c5fb99. Lượt sửa lỗi hiện tại nằm trên codex/fix-w01-evidence: gỡ xung đột stash, giữ hai ảnh minh-chung và xóa hai ảnh linh-chung đúng yêu cầu. Chưa có slab/per-CPU hoặc benchmark mới.
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
| CST-006 | Allocator vẫn một list/lock chung, có kfreepage_count/prototype/caller. Chưa có per-CPU/slab. CI main53872b8 đã qua usertests/crash; chưa có benchmark hoặc test đặc thù cho allocator mới | Cao về source và CI baseline | source hiện tại; GitHub run39212699734; L023 | Source/test mới | 19/10/2026 |
| CST-007 | WSL có Ubuntu 26.04.1 LTS, version 2; Make/RISC-V GCC/QEMU đã tìm thấy. Linh đã cài clang-format, kiểm tra trực tiếp thấy Ubuntu clang-format 21.1.8. Chưa build/boot/test local trong lượt kiểm tra | Cao về phát hiện công cụ | wsl/os-release/command -v và clang-format --version; L017 | Công cụ đổi hoặc trước build/run | 19/10/2026 |
| CST-008 | Bộ nhớ project-memory Light được tích hợp ở agent/, nối qua AGENTS.md | Cao | agent/README.md và các file bộ nhớ | Đổi bộ khung memory | 13/10/2026 |
| CST-009 | Git repo BTL có default branch origin/main và tag xv6-baseline local; tag chưa được push lên origin ở lần kiểm tra 19/10 | Cao | Git local, GitHub API ref và git ls-remote --tags origin; L016 | Push tag hoặc đổi nền source | 19/10/2026 |
| CST-010 | Mã dự án 26118, số đề tài 19; nhóm gồm Linh 20233495, Minh 20233528, Khánh 20224019; người dùng là Linh | Cao | Ảnh và yêu cầu trực tiếp 19/10; LOGBOOK L015 | Người dùng đính chính | 19/10/2026 |
| CST-011 | Run main39212699734 tại head53872b8 đã completed/success: linux format/build/usertests/crash success; macos build/job success. Run PR39210447048 tại head2df489d cũng success. Chưa có per-CPU/slab hoặc benchmark; không có kalloctest.c/kcsan.c MIT lab trong BTL | Cao tại lúc kiểm tra | GitHub run và jobs; L023 | Source/run mới | 19/10/2026 |
| CST-012 | Tuần 19–25/10/2026 mới khởi động nhóm; phần Linh đang học là học trước/chuẩn bị cá nhân, không tự tính là kết quả nhóm W01 | Cao | Người dùng trả lời trực tiếp ngày 19/10; LOGBOOK L015 | Người dùng đính chính hoặc có kết quả W01 thật | 19/10/2026 |
| CST-013 | Main local/origin/main ở 5c5fb99 (PR #9), hai ghi chú Minh đã khôi phục và ba xung đột đã gỡ. Đã commit và push codex/fix-w01-evidence theo yêu cầu rõ của Linh, thiết lập upstream và đối chiếu remote SHA. Nhánh sửa giữ đúng 2 ảnh minh-chung; GitHub main chưa nhận phần sửa qua PR/merge | Cao tại lúc kiểm tra | Git local/remote refs; LOGBOOK L031 | Commit/push/merge hoặc đổi working tree | 23/10/2026 |
| CST-014 | Cấu hình lần đọc 19/10: main-workflow active, PR 0 approvals, resolve conversations, linear history, chặn xóa/force push; checks linux/macos integration_id15368, strict=false; chỉ squash. Linh đã xác nhận thêm Minh/Khánh, Minh có ghi chú và PR onboarding trên main. Chưa tái kiểm tra Settings hoặc xác nhận đầy đủ Khánh/board | Cao về cấu hình tại ngày đọc và bằng chứng nhóm; không suy ra cấu hình hiện tại đã đổi | L025, L031; user xác nhận và source ghi chú Minh | Cấu hình/thành viên đổi | 23/10/2026 |

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
| Current priority | Phần sửa W01 đã commit và publish trên codex/fix-w01-evidence: README ngắn 3 phần, giữ hai ảnh minh-chung và xóa hai linh-chung. Còn PR/merge vào main và tổng hợp đủ đóng góp nhóm để nộp báo cáo tuần. Linh cho phép tự commit/push riêng lần sửa này, không thành quyền auto-commit các bài học sau | Yêu cầu Linh 23/10; source và Git; L031 | PR/merge sửa lỗi và đủ đóng góp nhóm | 23/10/2026 |
| Build và runtime | Local chỉ xác nhận style, chưa chạy local build/test. CI main39212699734 tại 53872b8 completed/success: linux format/build/usertests/crash và macos build đạt. Đây là baseline có hàm đếm trang, chưa phải perCPU/slab hoặc benchmark | Plugin GitHub run/jobs; L023 | Source/run mới hoặc thực hành local | 19/10/2026 |
| Learning progress | Đã xác nhận đọc xong Lesson 0; Lesson 1–3 đã soạn. Hàm đếm đã vào main và CI baseline đạt; chưa suy ra Linh đã hoàn thành các bài thực hành khác | LOGBOOK L011–L014, L023; source và CI | Sau bài học tiếp theo | 19/10/2026 |
| Next implementation step | Chưa phải việc đang giao: sau khi Linh muốn trở lại học, P02 là pool/mảng rồi M1 giữ pool 0/count; M2 local path sau. Hiện chỉ làm workflow GitHub ba người, không sửa C thay hoặc yêu cầu làm bài C | docs/lessons/03-per-cpu-free-lists.md; yêu cầu Linh L026 | Người dùng quay lại học C | 19/10/2026 |

## Current constraints

- Source tham chiếu: `D:\2. FullStack\2026.1\Hệ điều hành\Excecise\4\xv6-riscv`.
- Source đó có sửa Makefile xử lý dấu cách và hai khác biệt mode script; cần ghi nhận khi tạo baseline.
- API page `kfree` của xv6 khác ý nghĩa object free trong đề tài; lộ trình đặt tên nằm trong tài liệu kiến trúc.
- Các mở rộng buddy, reclaim và adaptive stealing mới là hướng khảo sát, chưa có kết quả.

## Open questions

- Overview/rubric, giờ nộp chính thức, hạn cuối kỳ và ngày from/to thực tế của W01; tuần khởi động đã xác nhận 19–25/10/2026.
- Đóng góp và minh chứng W01 của cả ba, quỹ giờ, URL design/board và permalink nộp chưa đủ. Minh dùng tài khoản Minhlth và có ghi chú onboarding tự báo cáo môi trường/test; chưa đối chiếu raw log Minh trong lượt này. Không mời lại thành viên đã có hoặc nhận đã hoàn thành bài kỹ thuật từ việc đọc tài liệu.
- Cách giảng viên định lượng sai khác 25% và cumulative progress; quy ước nội bộ trong tài liệu chỉ là đề xuất.
- Cách môn học tính ít nhất 3 cấu trúc kernel chuyển động và quy định tên API cuối.
- Hiệu quả của batch stealing/adaptive policy chỉ có thể kết luận sau thực nghiệm.
