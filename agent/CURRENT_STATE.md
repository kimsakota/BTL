# Trạng thái hiện tại

Cập nhật: 15/10/2026.

## Project status

- Giai đoạn: Đã soạn Lesson 0/1 và Lesson 2 theo yêu cầu bài tiếp theo. Lesson 2 tập trung đồng bộ/CPU/ngắt trước per-CPU lists. Chưa có kết quả runtime/bài làm để xác nhận thực hành.
- Mục tiêu: đề tài 19, Slab allocator and per-CPU free lists trên xv6; nối kiến thức từ BaoKhoaHoc/VstHelper.
- Chủ dự án: người dùng trong chat này.
- Đối tượng sử dụng: nhóm làm BTL/NCKH sinh viên; thông tin nhân lực và deadline chưa có.

## Stable facts

| ID | Statement | Confidence | Source | Revisit when | Last checked |
|---|---|---|---|---|---|
| CST-001 | Người dùng đã chốt đề tài 19 | Cao | LOGBOOK.md, L001; chat gốc | Người dùng đổi đề tài | 13/10/2026 |
| CST-002 | BTL có tài liệu, workspace và task cấu hình | Cao | README.md và các file hiện có | Cấu trúc dự án thay đổi | 13/10/2026 |
| CST-003 | Source xv6 phù hợp nằm ở thư mục Exercise được người dùng cung cấp | Cao | docs/Nguon-xv6.md | Import hoặc đổi baseline | 13/10/2026 |
| CST-004 | Baseline nguồn được kiểm tra là commit 06aad25c735fd3159bdfae5680be4eab7b1668b2 của mit-pdos/xv6-riscv | Cao | docs/Nguon-xv6.md | HEAD nguồn thay đổi | 13/10/2026 |
| CST-005 | Source đó dùng free list trang chung, PGSIZE=4096, NCPU=8; chưa có slab/stealing trong phần đã kiểm tra | Cao | docs/Nguon-xv6.md và kernel nguồn | Thay allocator hoặc phiên bản | 13/10/2026 |
| CST-006 | Đã nhập 81 file source/cấu hình xv6 vào BTL; source allocator vẫn một list/lock chung. Diff hiện tại kalloc.c/proc.h thêm chú thích; chưa có hàm đếm, per-CPU lists hoặc slab | Cao | docs/xv6-baseline.json, source kalloc.c và git diff kalloc.c/proc.h | Bắt đầu viết implementation | 15/10/2026 |
| CST-007 | Lần kiểm tra môi trường cho thấy chưa có distro WSL; chưa xác nhận build/boot hiện tại | Cao tại thời điểm kiểm tra | README.md, LOGBOOK.md L004 | Trước build/run hoặc sau cài môi trường | 13/10/2026 |
| CST-008 | Bộ nhớ project-memory Light được tích hợp ở agent/, nối qua AGENTS.md | Cao | agent/README.md và các file bộ nhớ | Đổi bộ khung memory | 13/10/2026 |
| CST-009 | Git repo local đã khởi tạo ở BTL, branch main; baseline được lưu bằng commit và tag xv6-baseline | Cao | Git repo BTL và LOGBOOK.md L007 | Thay nền source | 13/10/2026 |

## Active operating decisions

| ID | Operating decision | Why it is active now | Source | Revisit when | Last checked |
|---|---|---|---|---|---|
| OP-001 | Trao đổi bằng tiếng Việt, giải thích theo từng bước học | Người dùng đang học nền tảng | LOGBOOK.md L002 | Người dùng yêu cầu khác | 13/10/2026 |
| OP-002 | Giữ cấu trúc nhỏ, chia module C theo trách nhiệm trong xv6 | Người dùng yêu cầu không thừa thãi; khung hiện tại theo hướng này | docs/Kien-truc-du-an.md | Có nhu cầu module độc lập thực tế | 13/10/2026 |
| OP-003 | Tách sự thật đã kiểm chứng khỏi hướng thiết kế và giả thuyết | Đồ án chưa có implementation/benchmark | DOCS_GUIDE.md | Luôn áp dụng khi ghi kết quả | 13/10/2026 |
| OP-004 | Dùng tài liệu Light và snapshot local, không chạy memory service nền | Phù hợp yêu cầu thêm thư mục agent cho dự án nhỏ | LOGBOOK.md L006 | Cần hooks/tìm kiếm memory phức tạp | 13/10/2026 |
| OP-005 | Người dùng tự thiết kế/viết toàn bộ phần C; assistant dạy từng thành phần, giải thích reference và kiểm tra, không đưa lời giải hoàn chỉnh hoặc sửa implementation thay | Người dùng làm rõ mục tiêu hiểu và tự viết, không chỉ gõ theo code mẫu | LOGBOOK.md L008 | Người dùng yêu cầu rõ cách làm khác | 13/10/2026 |
| OP-006 | Khi chuyển bài, soạn Markdown đầy đủ kiến thức và chi tiết từng phần; thực hành chia chặng nhỏ, không giữ người dùng ở Lesson 0 khi họ đã xác nhận đọc xong | Người dùng yêu cầu tạo Lesson 1 đầy đủ, có nghiên cứu và source reference | LOGBOOK.md L011 | Người dùng đổi cách học | 14/10/2026 |
| OP-007 | Cuối một phần hướng dẫn có thay đổi code, gợi ý chú thích giải thích lý do/invariant/ownership/khóa/ngắt khi phù hợp; nêu diff/kiểm tra, gợi ý commit message và nhắc người dùng tự commit/push GitHub. Không tự stage/commit/push khi chưa được yêu cầu rõ | Người dùng yêu cầu ghi nhớ quy tắc commit sau từng phần code và bổ sung chú thích/nhắc nhở | LOGBOOK.md L013, AGENTS.md, docs/Lo-trinh-hoc.md | Người dùng đổi quy tắc hoặc yêu cầu rõ thao tác Git | 15/10/2026 |

## Current context

| Area | Current value | Source | Revisit when | Last checked |
|---|---|---|---|---|
| Current priority | Đọc Lesson 2 theo chặng A–E: race/critical section → atomic/spinlock → push_off/pop_off → CPU ID → deadlock/per-CPU. Bài phân tích A–F; helper quan sát là phần tự viết nếu thực hành | docs/lessons/02-race-spinlock-va-cpu.md, LOGBOOK.md L012 | Người dùng gửi bài hoặc yêu cầu bài tiếp theo | 15/10/2026 |
| Main blocker for build | Chưa có distro WSL và chưa có toolchain Linux được xác nhận | Kiểm tra lại trong L007 | Hoàn tất môi trường | 13/10/2026 |
| Learning progress | Đã xác nhận đọc xong Lesson 0; Lesson 1/2 đã soạn. Yêu cầu tạo Lesson 2 không là bằng chứng đã hoàn thành code/runtime Lesson 1; chưa đánh giá bài làm | LOGBOOK.md L011, L012 | Sau bài học tiếp theo | 15/10/2026 |
| Next implementation step | Khi người dùng thực hành, kiểm tra bài nhỏ/fn đếm Lesson 1 hoặc helper ngắt Lesson 2 theo điểm họ chọn. Lesson 3 dự kiến tự thiết kế pool theo CPU; trước sửa allocator cần baseline và hiểu guard/lock/ownership | docs/lessons/01-baseline-va-kalloc.md, docs/lessons/02-race-spinlock-va-cpu.md | Bước tự viết được kiểm tra | 15/10/2026 |

## Current constraints

- Source tham chiếu: `D:\2. FullStack\2026.1\Hệ điều hành\Excecise\4\xv6-riscv`.
- Source đó có sửa Makefile xử lý dấu cách và hai khác biệt mode script; cần ghi nhận khi tạo baseline.
- API page `kfree` của xv6 khác ý nghĩa object free trong đề tài; lộ trình đặt tên nằm trong tài liệu kiến trúc.
- Các mở rộng buddy, reclaim và adaptive stealing mới là hướng khảo sát, chưa có kết quả.

## Open questions

- Overview/rubric, deadline và số người làm đồ án.
- Cách môn học tính ít nhất 3 cấu trúc kernel chuyển động và quy định tên API cuối.
- Hiệu quả của batch stealing/adaptive policy chỉ có thể kết luận sau thực nghiệm.
