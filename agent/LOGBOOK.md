# Nhật ký dự án

Cập nhật: 19/10/2026. Lưu kết quả có ý nghĩa; transcript nằm riêng trong `history/`.

## L015 - 19/10/2026 - Nghiên cứu quy trình nhóm 26118 và báo cáo tuần

- type: user-context / research / documentation
- summary: Người dùng xác nhận tên Linh, muốn tham gia nhiều/hiểu toàn bộ và chia phần riêng cho ba thành viên như cách làm việc thực tế; cung cấp ảnh đề tài, thông báo, template PDF và sample PDF.
- facts: Mã 26118, đề tài 19; Nguyễn Kim Linh 20233495, Lê Trần Hiếu Minh 20233528, Nguyễn An Khánh 20224019. Báo cáo ít nhất mỗi tuần, cùng thread môn học, mọi người có đóng góp, nối TODO trước và cho phép sai khác 25%; template có năm phần, summary 200–300 từ.
- user clarification: Trong lúc nghiên cứu, Linh xác nhận tuần này mới bắt đầu dự án nhóm, phần đang học là học trước. W1 theo lịch là 19–25/10/2026; không gộp Lesson/code/tài liệu chuẩn bị cá nhân thành kết quả nhóm W01. Đã tách task khởi động K01–K04 khỏi TODO W2 N01–N04 có điều kiện đầu vào.
- method: Đọc chat được chỉ định qua read_thread; đọc memory/kiến trúc/source/Git/CI; trích và render hai PDF; xem ảnh; kiểm tra tài liệu chính MIT lock lab, Bonwick/USENIX, GitHub Flow/Projects/CODEOWNERS và QEMU MTTCG.
- observation: Source vẫn list/lock chung với kfreepage_count/prototype/caller; chưa perCPU/slab. Không có user/kalloctest.c hoặc kernel/kcsan.c. CI file có nhưng không kiểm chứng pipeline. Baseline tag e287830, HEAD c829ab9, working tree có thay đổi từ trước; remote kimsakota/BTL.
- proposal: Một repo/ba clone, nhánh ngắn theo task, API nhỏ, PR/review chéo. Linh page/buffer/tích hợp, Minh slab/file, Khánh test/inode; Linh học toàn luồng bằng review/teach-back/reproduce. Slab phát triển trên page gốc để song song; ba client chính file/inode/buf, pipe thử trước. Chưa được nhóm chốt phân công.
- outputs: docs/Quy-trinh-nhom-26118.md; docs/Ke-hoach-va-backlog-26118.md; docs/bao-cao/README.md; bản nháp W01 Markdown/PDF; mẫu task/PR local .github; liên kết README và bộ nhớ.
- report status: PDF ba trang, summary 251 từ theo whitespace; kiểm tra lại bản render sau khi cập nhật bối cảnh khởi động. Ngày from/to thực tế, đóng góp thực Minh/Khánh, xác nhận phần của Linh và URL design/board còn thiếu; nhãn DRAFT rõ ràng, không nộp thay nhóm. Không lấy tiến độ/test/công cụ nhóm mẫu làm dữ kiện nhóm này.
- validation: Kiểm tra 10 Markdown, 21 liên kết local, code fences và đối chiếu ID K01–K04/N01–N04; summary 251 từ, PDF đủ mục/MSSV, ba trang, text bounds hợp lệ; xem cả ba PNG bản cuối. git diff --check tài liệu đạt. Diff kernel/user/mkfs/Makefile giống đầu lượt và SHA256 kalloc.c/defs.h/main.c/Makefile giữ nguyên. Không chạy build/boot/test/benchmark.
- limitations: Đã biết tuần khởi động nhưng chưa biết giờ nộp chính thức/hạn cuối kỳ; đề xuất tổng hợp nội bộ 24/10 chỉ là lịch nhóm. W01 còn là khung chờ đầu ra cuối tuần. Công thức sai khác 25% chưa do giảng viên xác nhận; không tự gán phần trăm tiến độ. Mẫu/plan không chứng minh ba thành viên đã đóng góp.
- actions not performed: Không sửa implementation C/h/S/Makefile, không stage/commit/push, không tạo board/issue/PR hoặc gửi tin/báo cáo ra ngoài.
- next action: Nhận kỳ/hạn và contributions thật, chỉnh bản nháp theo đúng thời gian; hoặc tiếp tục tự code Lesson 3 khi Linh quay lại học.
- should promote to current state?: yes, chỉ thông tin nhóm/yêu cầu/đầu ra, phân công giữ nhãn đề xuất.

## L014 - 17/10/2026 - Tạo Lesson 3 về free list theo CPU

- type: documentation / observation
- summary: Người dùng yêu cầu file Markdown Lesson 3; giữ cách dạy đầy đủ, tự viết C, gợi ý chú thích và nhắc commit/push từng phần.
- source update: Kalloc.c đã có kfreepage_count với lock bao vòng duyệt, defs.h có prototype, main.c gọi/in ngay sau kinit. Đây là bằng chứng source, chưa xác nhận runtime; thay kết luận cũ chưa có hàm đếm.
- result: Tạo docs/lessons/03-per-cpu-free-lists.md với pool/mảng struct, lifetime tên khóa, init trước freerange, ownership/invariant, guard/local free-alloc, count nhiều pool và bản đồ reference. Có bài A–I và mốc M0/M1/M2 để người dùng tự code/check/commit.
- design for lesson: M1 đổi state thành mảng nhưng giữ pool 0 chung. M2 local-only chưa stealing, chỉ đặt tiêu chí CPUS=1; local-empty không là global OOM. Lesson 4 thêm remote allocation trước kiểm chứng nhiều CPU.
- count semantics: Giữ một khóa mỗi lần quét pool. Boot yên cho tổng giải thích theo end/PHYSTOP; runtime không coi tổng nhiều thời điểm là snapshot đồng thời. Chưa thêm counter chung hoặc giữ đồng thời nhiều khóa.
- sources: Source BTL/Makefile và kiến trúc, MIT lock lab/xv6 book, WG14 N1570. Thiết kế M1/M2 là hướng dẫn cho BTL, không phải implementation đã nhập.
- changes: Lesson 3, liên kết từ Lesson 2/README/lộ trình và bộ nhớ; không sửa C/header/assembly/Makefile, không stage/commit/push.
- validation: Kiểm tra 7 tài liệu, 68 liên kết local/số dòng, code fences, thứ tự mục 0–15 và 3 điểm source của hàm đếm/khai báo/caller; git diff --check tài liệu đạt. Diff kernel/user/mkfs/Makefile sau soạn bài giống snapshot đầu lượt, giữ nguyên phần C người dùng đã viết.
- limitations: Chưa kiểm tra lại cài môi trường; chưa build/boot/test. Chưa có source per-CPU/slab. Việc tạo tài liệu không tự hoàn thành các mốc code.
- next action: Người dùng kiểm tra/chốt hàm đếm nếu chưa làm, rồi tự viết pool/mảng và lần lượt M1/M2; assistant kiểm tra, gợi ý chú thích/message và nhắc tự commit/push.
- should promote to current state?: yes

## L013 - 15/10/2026 - Gợi ý chú thích và nhắc commit sau từng phần code

- type: user-decision / documentation
- summary: Người dùng yêu cầu thêm quy tắc: với phần hướng dẫn có sửa code, sau khi xong một phần người dùng sẽ commit lên GitHub; assistant thêm gợi ý chú thích và lời nhắc.
- decision: Mỗi checkpoint code có ý nghĩa nêu thay đổi/kiểm tra thật, gợi ý chú thích giải thích lý do và hợp đồng, đề xuất commit message phù hợp, chỉ rõ file/hunk và nhắc tự commit local rồi push remote. Kể cả sửa chú thích source cũng thuộc quy tắc; bài chỉ làm trên giấy/chat chưa thay file không có mốc commit code.
- ownership: Người dùng tự viết chú thích/code và tự stage/commit/push. Assistant chỉ hướng dẫn/nhắc, không thực hiện Git write/publish khi chưa có yêu cầu rõ.
- changes: AGENTS.md, quyết định OP-007, recovery, mục chốt phần code trong lộ trình và phần gợi ý chú thích/commit của Lesson 1/2.
- limitations: Lượt này chỉ lưu quy tắc/cập nhật tài liệu, không sửa C và không stage/commit/push.
- next action: Áp dụng quy tắc khi kiểm tra một phần code do người dùng hoàn thành hoặc soạn lesson có phần tự viết.
- should promote to current state?: yes

## L012 - 15/10/2026 - Tạo Lesson 2 về đồng bộ, CPU và ngắt

- type: documentation / observation
- summary: Người dùng yêu cầu tạo bài tiếp theo sau Lesson 1; giữ cách soạn Markdown đầy đủ và hướng dẫn tự viết từng phần.
- result: Tạo docs/lessons/02-race-spinlock-va-cpu.md với race/lost update, critical section/invariant, atomic/spinlock/acquire-release, ordering/volatile, ngắt/CSRRC, noff/intena có mức lồng, cpuid/mycpu/migration, deadlock/lock order, định hướng per-CPU và helper quan sát tự viết. Có bảng, sơ đồ, bài phân tích A–F, source map và nguồn nghiên cứu.
- sources: spinlock.c/.h, proc.c/.h, riscv.h, start.c, trap.c, kalloc.c, main.c, sleeplock.c và Makefile trong BTL; MIT xv6 book/lock lab, GCC atomics/volatile, WG14 N1570 và RISC-V ISA.
- source observations: push_off dùng rc_sstatus đọc/xóa SIE; diff source hiện tại ở kalloc.c/proc.h chỉ thêm chú thích. user/kalloctest.c và kernel/kcsan.c không có; không áp lệnh test lab như đã sẵn sàng tại BTL.
- changes: Lesson 2, liên kết từ Lesson 1/README/lộ trình và bộ nhớ. Không sửa implementation hoặc thư mục .vs/.
- validation: Kiểm tra 7 tài liệu, 73 liên kết local/số dòng, code fences, thứ tự mục 0–15 và 6 đoạn source atomic/push_off/holding/mycpu; git diff --check tài liệu đạt. Diff kernel/user/mkfs/Makefile sau soạn bài giống snapshot đầu lượt, giữ nguyên các thay đổi của người dùng.
- limitations: Chưa có bài làm/helper mới hoặc kết quả build/boot/test; không kiểm tra lại cài môi trường. Yêu cầu tạo bài tiếp theo không tự xác nhận đã hoàn thành runtime Lesson 1.
- next action: Người dùng đọc Lesson 2 theo chặng, gửi bài đang làm hoặc yêu cầu Lesson 3; kiểm tra/dạy theo điểm thực tế, giữ quyền tự viết C của người dùng.
- should promote to current state?: yes

## L011 - 14/10/2026 - Soạn Lesson 1 đầy đủ sau Lesson 0

- type: user-decision / documentation
- summary: Người dùng xác nhận đọc xong Lesson 0; yêu cầu nghiên cứu và tạo bài Lesson 1 Markdown đầy đủ, giải thích chi tiết từng phần. Không tiếp tục giữ bài học ở khai báo hàm Lesson 0.
- result: Viết lại/mở rộng file Lesson 1 hiện có, giữ đường dẫn 01-baseline-va-kalloc.md để các liên kết cũ tiếp tục dùng. Nội dung gồm byte/địa chỉ, kiểu/typedef/ABI, pointer/&/*, cast/sizeof/bước pointer, struct/./->, list/metadata, source map và từng hàm allocator gốc, ownership/khóa, môi trường/baseline, bài nhỏ A–E và hàm đếm theo chặng.
- sources: Source BTL và Makefile thực tế; MIT xv6 book/tools, WG14 N1570, RISC-V psABI, GCC Standards/Pointer Arithmetic, Microsoft WSL. Nguồn và phạm vi được ghi trong mục 15 của Lesson 1.
- operating preference: Cung cấp tài liệu đủ cả bài khi chuyển lesson; việc tự code vẫn chia từng bước nhỏ, assistant chỉ giải thích/kiểm tra/gợi ý.
- changes: Lesson 1, liên kết tiếp tục từ Lesson 0, README/lộ trình và bộ nhớ. Không thêm/sửa C/header/assembly/Makefile.
- validation: Kiểm tra Markdown/code fences, thứ tự 16 mục 0–15, 44 liên kết local và giới hạn số dòng source trong các tài liệu liên quan; git diff --check phần tài liệu đạt. Diff kernel/user/mkfs/Makefile sau soạn bài giống snapshot trước, giữ nguyên thay đổi chú thích của người dùng.
- limitations: Đã đọc xong Bài 0 là lời xác nhận của người dùng, không phải kiểm tra thành thạo; chưa có bài làm Lesson 1, chưa build/boot/test và chưa kiểm tra lại môi trường cài đặt.
- next action: Người dùng đọc Lesson 1 theo chặng, gửi bài nhỏ đang làm; assistant đọc và hướng dẫn tiếp theo điểm thực tế.
- should promote to current state?: yes

## L010 - 14/10/2026 - Tiếp tục hợp đồng hàm và khai báo C

- type: learning-note / observation
- summary: Người dùng yêu cầu tiếp tục kiến thức sau phần phân biệt mã xv6 và phần đề tài.
- covered topics: Trách nhiệm, tham số, kết quả và trạng thái phụ thuộc của kalloc; khai báo ở defs.h, định nghĩa ở kalloc.c; phân biệt void, void* và (void).
- exercise: Tự viết một dòng khai báo cho hàm dem_trang_trong, không nhận tham số, trả int; gửi trong chat. Đây là bài tập cú pháp, không phải API đề tài đã chốt, không thêm vào source.
- source checked: git diff kernel/defs.h và kernel/kalloc.c chỉ có chú thích/khoảng trắng; phần allocator chưa đổi. Không sửa các thay đổi đó thay người dùng.
- changes: Bổ sung hợp đồng hàm và bài tập vào Bài 0, cập nhật điểm tiếp tục/bộ nhớ.
- limitations: Chưa có bài làm để đánh giá mức độ hiểu; không kiểm tra lại môi trường hoặc chạy build/boot/test trong lượt này.
- next action: Kiểm tra một dòng khai báo do người dùng tự viết, rồi giải thích header/include và compile/link.
- should promote to current state?: yes

## L009 - 13/10/2026 - Phân biệt mã xv6 nền và phần tự viết của đề tài

- type: learning-note / observation
- summary: Người dùng hỏi vì sao kernel/ có nhiều code sẵn và có phải mã quản lý đề tài hay không.
- explanation: kernel/, user/, mkfs/ và Makefile đến từ xv6 nền; docs/ và agent/ phục vụ tài liệu/bộ nhớ. Đề tài mở rộng quản lý bộ nhớ trong nhân xv6; người dùng tự viết phần mở rộng và tích hợp, sử dụng hệ điều hành nền có sẵn.
- evidence: docs/Nguon-xv6.md, kernel/kalloc.c và git diff ở kernel/user/mkfs/Makefile không có thay đổi so với baseline local tại lần kiểm tra này.
- changes: Bổ sung mục 0 vào Bài 0, cập nhật điểm tiếp tục. Không sửa implementation.
- limitations: Mới giải thích ranh giới mã nền/phần đề tài; chưa kiểm tra người dùng đã nắm. Slab/per-CPU free lists chưa được triển khai; chưa build/boot.
- next action: Đọc mục 0 trước khi học khai báo/định nghĩa hàm bằng source tham chiếu.
- should promote to current state?: yes

## L008 - 13/10/2026 - Làm rõ cách dạy và tự viết C

- type: user-decision
- summary: Người dùng yêu cầu tự viết toàn bộ phần C và hiểu tất cả reference/phụ thuộc, liên kết từng thành phần từ nhỏ đến cần thiết.
- correction: Đưa một hàm hoàn chỉnh để gõ theo quá sớm không đáp ứng mục tiêu tự thiết kế/hiểu. Assistant nhận lỗi cách hướng dẫn và chuyển sang giao từng bước nhỏ.
- decision: Assistant giải thích khái niệm, hợp đồng, kiểu dữ liệu, khai báo/định nghĩa, header/include và compile/link; người dùng tự viết và tự sửa. Assistant chỉ đọc/kiểm tra/gợi ý, không sửa implementation hay đưa lời giải toàn bộ nếu chưa được yêu cầu rõ.
- changes: Cập nhật AGENTS.md, lộ trình và bài tập trong Bài 1; bỏ lời giải hàm đếm và các dòng tích hợp hoàn chỉnh. Chỉ sửa tài liệu/bộ nhớ, không thay mã C.
- next action: Bắt đầu từ vai trò khai báo trong defs.h, định nghĩa trong kalloc.c và reference từ main.c; học con trỏ/struct rồi mới tự viết từng phần của hàm nhỏ.
- should promote to current state?: yes

## L007 - 13/10/2026 - Bắt đầu học và code theo từng bài

- type: user-decision / implementation / learning-note
- summary: Người dùng yêu cầu hướng dẫn chi tiết A-Z, vừa học vừa hiểu vừa code; chọn tự gõ và để assistant hướng dẫn/kiểm tra.
- preparation: Nhập 81 file source/cấu hình được Git theo dõi từ xv6 Exercise vào root BTL, chuẩn hóa LF và giữ sửa đổi Makefile. Khởi tạo Git local và lưu baseline bằng tag xv6-baseline.
- evidence: docs/xv6-baseline.json, Git baseline, docs/Lo-trinh-hoc.md và Bài 1.
- observed environment: Kiểm tra lại WSL cho thấy chưa có distro; chưa xác nhận toolchain hoặc build/boot.
- lesson prepared: Đọc kinit/freerange/kfree/kalloc; thực hành tự gõ kfreepage_count, prototype trong defs.h và printk sau kinit.
- limitations: Code bài tập nằm trong hướng dẫn, chưa được thêm vào kernel. Chưa có kết quả runtime hoặc usertests.
- next action: Người dùng thiết lập Ubuntu/toolchain, chạy baseline gốc; sau đó tự gõ và gửi kết quả để đọc diff/kiểm tra.
- should promote to current state?: yes, chỉ các thao tác chuẩn bị và lựa chọn đã xác nhận.

## L006 - 13/10/2026 - Tích hợp bộ nhớ agent

- type: decision / implementation
- summary: Người dùng yêu cầu thêm một thư mục agent từ GitHub để quản lý lịch sử, nội dung và quy tắc.
- context: BTL nhỏ, đang học C/xv6, đã yêu cầu cấu trúc vừa đủ.
- result: Chọn project-memory-skill cấu hình Light theo đánh giá phù hợp của assistant; tích hợp tài liệu ở `agent/` và router ở root `AGENTS.md`. Đây là lựa chọn triển khai trong phạm vi yêu cầu, không phải tuyên bố repo được cộng đồng khuyên dùng nhiều nhất.
- evidence: `UPSTREAM.md`, `README.md`, `history/INDEX.md`.
- limitations: Bộ nhớ được agent duy trì bằng file; chưa cài runtime hooks/MCP hoặc đồng bộ chat liên tục. Chưa có Git repo/commit tại root BTL.
- next action: Dùng recovery/current state trong các chat tiếp theo; cập nhật theo trách nhiệm.
- should promote to current state?: yes

## L005 - 13/10/2026 - Kiểm tra source xv6 được cung cấp

- type: observation
- summary: Người dùng cung cấp bản xv6 ở Exercise/4/xv6-riscv và hỏi có dùng được không.
- method: Đọc source, Makefile, Git status/remote/log/diff; xác nhận commit trên repo MIT.
- result: Phù hợp làm nền đề tài 19; commit `06aad25c735fd3159bdfae5680be4eab7b1668b2`; allocator trang chung, giới hạn 8 CPU, cấu trúc file/inode/buffer tĩnh.
- confidence: cao về source đã kiểm tra.
- limitations: Chưa build/boot mới; chưa import vào BTL. File build có sẵn không chứng minh runtime hiện tại.
- source: `docs/Nguon-xv6.md`; snapshot chat hiện tại trong history.
- next action: Khi bắt đầu implementation, tạo baseline riêng và xác nhận build/boot.
- should promote to current state?: yes

## L004 - 13/10/2026 - Dựng workspace và định hướng cấu trúc C

- type: decision / observation
- summary: Người dùng yêu cầu cấu trúc hợp lý, không thừa thãi, hỏi về Clean Architecture; xác nhận dựng khung trước vì chưa có starter tại BTL.
- result: Dựng workspace, task build/run/clean, README và hướng dẫn. Thiết kế đề xuất giữ module C theo cấu trúc xv6: kalloc.c cho trang, kmalloc.c/.h cho object.
- method: Kiểm tra cấu hình JSON, đường dẫn và tài liệu; đọc Makefile xv6 chính thức.
- observation: Lúc kiểm tra có Git, VS Code, wsl.exe; chưa có distro WSL. Không tìm thấy toolchain trong Windows PATH.
- limitations: Không build kernel; sơ đồ file C là cấu trúc đích.
- source: `docs/Kien-truc-du-an.md`, `.vscode/tasks.json`, `BTL.code-workspace`.
- should promote to current state?: yes, với ngày kiểm tra môi trường.

## L003 - 13/10/2026 - Nghiên cứu hướng triển khai và đánh giá

- type: research
- summary: Đối chiếu catalogue, mã VstHelper và tài liệu slab/magazines/Hoard/TCMalloc/MIT.
- result: Phân biệt per-CPU page lists với per-CPU object cache. Adaptive cache sizing đã có prior work; chưa thể coi “thêm thích ứng” là tính mới.
- proposed direction: Đo batch stealing 1/4/16/64 trên 1/2/4/8 CPU; tải lệch/đổi pha; khảo sát reclaim slab rỗng; buddy ở mốc stretch.
- hypothesis: Adaptive batch có thể cải thiện trade-off ở một số tải. Chưa thử nghiệm, chưa biết có tốt hơn fixed baseline hay không.
- source: `docs/Nghien-cuu-de-tai-19.md`.
- limitations: Không có implementation hoặc benchmark; chưa xác lập khoảng trống nghiên cứu qua systematic review.
- should promote to current state?: Chỉ promote kết quả khảo sát có nguồn, không promote giả thuyết hiệu năng.

## L002 - 13/10/2026 - Kiến thức đã trao đổi

- type: learning-note
- summary: Người dùng muốn học lý thuyết trước, rồi yêu cầu tiếp tục kiến thức.
- covered topics: Kernel, allocator, allocation/free, trang 4 KB, object, slot, slab cache, free list, lock, per-CPU lists và stealing.
- covered mechanics: Metadata, con trỏ giữ địa chỉ, bitmap có quy ước bit trống/đã cấp, tái sử dụng slot, phân biệt cấp phát với khởi tạo.
- covered lifecycle: Slab empty/partial/full; free object khác free trang; empty slab có thể giữ lại hoặc reclaim.
- covered errors: Memory leak, use-after-free, double free; pointer còn địa chỉ sau free không giữ quyền dùng vùng nhớ.
- source: Snapshot chat “Tiếp tục đề tài NCKH 19” trong `history/INDEX.md`.
- limitations: Đã giải thích không đồng nghĩa đã đánh giá mức độ nắm kiến thức của người dùng.
- next suggested lesson: Đọc kalloc.c gốc, hoặc giải thích lock/race condition khi hai CPU cùng cấp phát; theo yêu cầu tiếp theo của người dùng.
- should promote to current state?: yes, chỉ ghi tiến độ bài đã trao đổi.

## L001 - Trước 13/10/2026 - Chốt đề tài 19

- type: user-decision
- summary: Người dùng đã chọn Slab allocator and per-CPU free lists, muốn kế thừa kiến thức BaoKhoaHoc/VstHelper.
- context: Chat “Chọn đề tài NCKH kế thừa BaoKhoaHoc” và yêu cầu mở đầu chat hiện tại.
- result: Tiếp tục đề tài 19; kế thừa thuật toán/thiết kế từ C# unsafe sang C/xv6, không giả định dùng lại nguyên mã .NET.
- source: Snapshot chat được tham chiếu trong `history/INDEX.md`.
- limitations: Đổi nhãn 2 sao thành 3 sao chưa được giảng viên xác nhận. Mở rộng có trọng tâm vẫn là hướng đề xuất.
- should promote to current state?: yes

## Mẫu ghi thêm

Mỗi mục cần ID, ngày, loại, kết quả, nguồn, giới hạn và bước tiếp theo nếu thay đổi. Loại có thể là `user-decision`, `decision`, `observation`, `research`, `hypothesis`, `learning-note` hoặc `blocker`. Giả thuyết được giữ dưới nhãn riêng cho đến khi có bằng chứng.
