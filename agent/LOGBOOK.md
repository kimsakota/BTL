# Nhật ký dự án

Cập nhật: 13/10/2026. Lưu kết quả có ý nghĩa; transcript nằm riêng trong `history/`.

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
