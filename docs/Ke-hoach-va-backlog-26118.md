# Kế hoạch và backlog dự án 26118

Cập nhật: 24/10/2026. Mục tiêu: hoàn thành phần bắt buộc trong **8 tuần tổng cộng**, tính W01 từ 19/10; kết thúc W08 ngày **13/12/2026**. Đây là kế hoạch đề xuất theo yêu cầu Linh, chưa là xác nhận Minh/Khánh đã nhận mọi việc hoặc hạn nộp chính thức của môn.

Linh đã xác nhận ngày 24/10: khoảng **10–12 giờ/người/tuần**, đã gồm học, code, review và báo cáo; cả nhóm có 30–36 giờ công/tuần, 7 tuần còn lại tương ứng 210–252 giờ công nếu duy trì mức này. Linh phụ trách tầng trang/buffer và tích hợp; Minh phụ trách slab/file; Khánh phụ trách test/inode. Thời gian đọc/review module của Minh và Khánh nằm trong quỹ giờ của Linh để hỗ trợ mục tiêu hiểu toàn bộ. Phân công kỹ thuật vẫn cần từng người nhận việc.

Phân bổ tham khảo mỗi người mỗi tuần: **6–7 giờ code và kiểm thử, 2 giờ học đúng phần đang làm, 1–2 giờ review/ghép, 1 giờ ghi kết quả và báo cáo**. Tổng là 10–12 giờ; điều chỉnh giữa các nhóm giờ theo trở ngại thực tế, tính buổi giải thích code chung vào phần review.

**Điểm xuất phát đã đọc ngày 24/10:** Linh xác nhận cả ba hoàn tất phần Git; source local vẫn allocator một list/lock. Remote main439e89b có thiết kế slab và hồ sơ Minh; hồ sơ Khánh được đọc từ codex/khanh-onboarding@7ddc0e2. Khánh còn thiếu toolchain; chưa có log baseline chung, implementation per-CPU/slab hoặc benchmark. Bản Word W01 local chốt kỳ 19–24/10. Đề xuất/gói việc dưới đây không phải kết quả đã thực hiện.

Dùng cùng một repository và hợp đồng API nhỏ để ba phần làm song song. Chi tiết trách nhiệm ở [quy trình nhóm](Quy-trinh-nhom-26118.md); thao tác Desktop ở [sổ tay GitHub](So-tay-GitHub-nhom.md). Không tạo lại bước mời thành viên đã hoàn tất.

## Bảng công việc đang dùng

[Project BTL 26118](https://github.com/users/kimsakota/projects/1) đã triển khai ngày 24/10, có [cây cha–con](https://github.com/users/kimsakota/projects/1/views/1) và [board trạng thái](https://github.com/users/kimsakota/projects/1/views/3). W02: [cha #16](https://github.com/kimsakota/BTL/issues/16), [N01 Linh #17](https://github.com/kimsakota/BTL/issues/17), [N02 Minh #18](https://github.com/kimsakota/BTL/issues/18), [N03 Khánh #19](https://github.com/kimsakota/BTL/issues/19), [N04 cả nhóm #20](https://github.com/kimsakota/BTL/issues/20). Tất cả đang Todo; người phụ trách xác nhận nhận việc trên Issue khi bắt đầu. Xem [cách dùng và hạn dự kiến](Thiet-lap-GitHub-26118.md).

## Các mốc có đầu ra kiểm tra được

| Mốc | Điều kiện nghiệm thu | Trạng thái đã đối chiếu 24/10 |
|---|---|---|
| M0 Baseline | Cả ba có môi trường, cùng revision; build/boot và test nền có log | CI baseline cũ đã đạt; Minh có ghi chú boot/test, Khánh thiếu toolchain; chưa đạt baseline chung |
| M1 Trang | Per-CPU + stealing; page invariants và test nhiều CPU | Chưa triển khai |
| M2 Object | Slab nhiều class, OOM/alignment/reuse và statistics/dump | Có thiết kế S01, chưa có implementation |
| M3 Client | Ba cấu trúc file/inode/buf cấp động với lifetime đúng; regression | Chưa triển khai |
| M4 Đánh giá | Throughput 1/2/4/8 CPU, fragmentation/leak, raw data và demo | Chưa đo |
| M5 Bàn giao | Báo cáo, source, hướng dẫn reproduce và deliverables theo Overview | Có bản Word W01 local; chưa chốt Overview/deadline và chưa nộp cuối kỳ |

Không dùng số bài học đã soạn để đánh dấu mốc code hoặc ghi là kết quả nhóm mới làm trong tuần 1. Chuẩn bị cá nhân có giá trị làm đầu vào; tiến độ nhóm được ghi từ tuần khởi động và theo đầu ra đã review. Chưa có rubric/trọng số nên không gán phần trăm định lượng.

## Backlog để chuyển thành issue nhỏ

Mỗi dòng là gói việc; khi bắt đầu tách code thành chặng 1–2 buổi nếu cần. Các dòng dưới là **gói việc để nhận và nghiệm thu riêng**. S01 đã có tài liệu của Minh, E01 đã có kết quả tự báo cáo một phần; không đánh Done toàn bộ chỉ vì có file. Owner/reviewer dưới đây là phân công đề xuất; người nhận phải xác nhận trong issue/PR.

| ID | Owner và reviewer | Đầu ra và tiêu chí nghiệm thu | Phụ thuộc |
|---|---|---|---|
| G01 | Linh / cả nhóm | Chốt deadline, tên/MSSV, năng lực/quỹ giờ, phân công và rubric còn thiếu; biên bản ngắn | Không |
| G02 | Linh / Minh | Hợp đồng page/object, size/alignment/OOM/free names, init và quy tắc đổi API | G01; có thể đọc trước |
| G03 | Khánh / Linh | Board task thật, mỗi task có ID/owner/reviewer; URL truy cập được | G01, tài khoản nhóm |
| E01 | Mỗi người / Khánh | Cài hoặc xác nhận toolchain; cùng baseline; version/build/boot log từng người | Không |
| E02 | Khánh / Linh | Kiểm tra CI hiện có; ghi rõ lệnh thật và log đạt/chưa đạt, không mặc định CI xanh | E01 |
| P01 | Linh / Minh | Kiểm tra/chốt hàm đếm đã có; baseline runtime, giải thích bảo vệ vòng duyệt | E01 |
| P02 | Linh / Minh | State mảng nhưng pool 0 chung như Lesson 3 M1; count đúng phạm vi | P01 |
| P03 | Linh / Minh | Local path và guard theo CPU; mốc học CPUS=1, ghi giới hạn chưa stealing | P02 |
| P04 | Linh / Khánh | Remote allocation/stealing một policy đơn giản, rõ ownership batch và khóa | P03, T01 |
| P05 | Linh / Khánh | Test bảo toàn/không cấp trùng/OOM/local-miss; log 1/2/4/8 CPU | P04, T02 |
| S01 | Minh / Linh | Layout một class; vẽ metadata/slots, tính capacity/alignment, init và failure path | G02; page gốc sẵn |
| S02 | Minh / Linh | Tự viết allocate/free một class; reuse/pattern test chạy trên page gốc | S01, E01, T01 |
| S03 | Minh / Khánh | Nhiều class đủ cho client; test class boundaries/size0/oversize/OOM | S02, T03 |
| S04 | Minh / Linh | Statistics/dump có định nghĩa đơn vị và snapshot semantics; số liệu kiểm tra được | S03 |
| T01 | Khánh / Minh | Test matrix + cách kích hoạt test, expected result, timeout và log schema | G02; bản đầu viết trước API chốt |
| T02 | Khánh / Linh | Harness trang/đa CPU và duplicate detection; chứng minh test có assertion | T01, E01 |
| T03 | Khánh / Minh | Harness object kiểm tra pattern, uniqueness, alignment và rollback | T01, S02 |
| I00 | Linh / Minh | Ghép trang/slab, init và build files; test cả stock-page+slab và perCPU+slab | P05, S03, T03 |
| I01 | Linh / Khánh | Pipe làm thử tích hợp; lỗi allocation và đóng hai đầu đúng; chưa tính chắc vào 3 | S03 |
| I02 | Minh / Linh | File động; ref/dup/fork/close và OOM rollback đúng, giữ hạn tài nguyên đầu | I00, T03 |
| I03 | Khánh / Linh | Thiết kế rồi triển khai inode động; uniqueness/recheck/ref/transaction tests | I00; thiết kế có thể sớm hơn |
| I04 | Linh / Minh | Buffer động; lookup uniqueness/cache retention/ref/pin/I/O tests | I00; thiết kế có thể sớm hơn |
| I05 | Khánh / cả nhóm | Regression sau ghép 3 client; log test hệ thống và lỗi còn lại | I02, I03, I04 |
| A01 | Linh / Minh | Chốt tên API theo rubric; nếu đổi tên page free, cập nhật mọi page call site riêng | G02, xác nhận rubric; trước M5 |
| B01 | Khánh / Linh | Protocol đối chứng trang/object, timer, warmup/repeats và CPU worker mapping | T01; trước thu số liệu |
| B02 | Khánh / Minh | Runner/parser thu log/CSV có manifest commit và fail counts; không có số giả | B01, E01 |
| B03 | Khánh / Linh | Dữ liệu throughput cho stock/perCPU và object workload 1/2/4/8 CPU | P05, S04, B02, I05 cho kết luận client |
| B04 | Minh / Khánh | Phân tích fragmentation/retention và leak qua chu kỳ; công thức có mẫu số | S04, B02 |
| D01 | Cả ba; Linh tổng hợp | Báo cáo tuần có đóng góp thực/ID nối TODO, file và permalink nộp | Lặp mỗi tuần, không chờ code |
| D02 | Linh / cả nhóm | Báo cáo cuối, README reproduce, demo/dump, đối chiếu common deliverables | M4, Overview |
| X01 | Linh / Khánh | So sánh batch stealing cố định nếu phần bắt buộc đã ổn định | M4; tùy thời gian |
| X02 | Người nhóm chọn / người khác | Buddy hoặc chính sách thích ứng có câu hỏi và test riêng | M5 sẵn sàng; stretch |

Đường triển khai dài nhất đi qua baseline → thuật toán tối thiểu → tích hợp client → regression → đo → bàn giao. Phần khảo sát/test matrix/thiết kế client có thể song song trước khi implementation phụ thuộc hoàn tất. Không cần chờ page tối ưu xong mới bắt đầu slab.

## Phân công để Linh hiểu toàn bộ

| Thành viên | Phần trực tiếp tự viết | Phần đọc và review | Đầu ra chứng minh đã hiểu |
|---|---|---|---|
| Linh | Page allocator, per-CPU/stealing; buffer động; phần ghép/init chung | Slab của Minh, harness và inode của Khánh | Tự lần theo allocate/free qua các tầng; nêu khóa, ownership, OOM; chạy được test tích hợp và tự sửa lỗi thuộc phần mình |
| Minh | Slab, class, thống kê/dump; file động | Page allocator của Linh, object tests của Khánh | Giải thích layout/capacity và refcount file; test xác nhận cấp/trả đúng |
| Khánh | Harness/assertion, runner/parser; inode động | Hợp đồng API, test của từng thay đổi, file/buffer qua các ca dùng chung | Test bắt được lỗi thật, log gắn revision, giải thích lifetime inode và đọc kết quả đo |

Mỗi tuần có một buổi 45–60 phút: tác giả trình bày một luồng code đang chạy, Linh giải thích lại và trả lời trường hợp lỗi. Dành thêm một buổi ghép 45 phút ở W03–W05 khi cần. Tác giả vẫn tự viết và đứng tên phần mình; Linh thực hiện phần tích hợp, test hoặc sửa lỗi có thực để vừa học vừa đóng góp. Không lấy số dòng đã đọc làm kết quả kỹ thuật.

Review kiến trúc/API và lifetime quan trọng có Linh tham gia; Minh review code trang của Linh, Khánh review khả năng kiểm thử. PR nhỏ thông thường có thể review chéo giữa Minh/Khánh; mục tiêu phản hồi trong một ngày làm việc để tránh chờ riêng Linh. Lịch đọc/test của Linh là một phần công việc, phải tính vào quỹ giờ.

## Lịch tám tuần và điều kiện nghiệm thu

| Tuần và ngày | Linh | Minh | Khánh | Kết quả cần có cuối tuần |
|---|---|---|---|---|
| W01 19–25/10 | Repo, cộng tác, tài liệu, hồ sơ Git | Onboarding, môi trường tự báo cáo, thiết kế S01 | Onboarding, ảnh/ghi chú, xác định thiếu toolchain | Phần khởi động đã có; báo cáo Word chốt đến 24/10. M0 chưa đủ log chung |
| W02 26/10–01/11 | E01/P01; P02 mảng pool vẫn dùng pool 0; review S01/G02 | Chốt S01; S02 prototype một class trên page allocator gốc | Hoàn tất E01; K03/T01/T02 harness trang cơ bản và test matrix object | Baseline ba máy có log; code P02, slab một class và harness chạy riêng được |
| W03 02–08/11 | P03→P04→P05: local và stealing 1 trang; ghép I00 | S03 nhiều class đủ cho client; S04 số liệu/dump; review P04 | T02/T03 assertion page/object, OOM/remote-free và chạy 1/2/4/8 CPU | M1 và lõi M2 chạy được; test chứng minh bản nhiều CPU hoàn chỉnh; API chung ổn định |
| W04 09–15/11 | Thiết kế/viết I04 buffer động; ghép init/API; đọc inode | I02 file động; S04 trả slab rỗng qua thao tác trim rõ ràng | Thiết kế/viết I03 inode động; test lookup/refcount; B01 protocol đo | File động có test; buffer/inode có bản đầu; test slab rỗng/trim tái lập được |
| W05 16–22/11 | Hoàn thiện buffer; cùng Khánh ghép inode; xử lý lock/ownership | Hoàn thiện file; review buffer/inode dùng object allocator | Hoàn thiện inode; I05 regression cho cả ba client | M3: file/inode/buf thực sự lấy object động, lifetime đúng, test hệ thống qua |
| W06 23–29/11 | Sửa lỗi nhiều CPU/tích hợp; giải thích toàn luồng | Kiểm tra occupancy, retained pages, dump và trim | B02 runner/parser; stress dài, thu dữ liệu thử và kiểm tra log | Khóa phạm vi chức năng; bộ test lặp lại được; phép đo đầu tiên chạy trọn quy trình |
| W07 30/11–06/12 | Phân tích throughput/page, kiểm tra giới hạn kết luận | Phân tích phân mảnh/leak và số liệu object | B03/B04 thu dữ liệu chính thức 1/2/4/8 CPU, biểu đồ và chạy lại độc lập | M4: raw data, manifest, hình và kết luận đối chiếu được; source ổn định |
| W08 07–13/12 | D02 báo cáo/demo; kiểm tra toàn bộ luồng; sửa lỗi còn lại | Rà source, giải thích slab/file; kiểm tra reproduce | Chạy lại từ clone sạch, đối chiếu log/demo; hoàn thiện bộ kết quả | M5: source, hướng dẫn chạy, báo cáo và demo; thời gian dự phòng sửa lỗi |

W04 chuẩn bị ba client song song vì `file`, `inode`, `buf` có refcount/cache/lifetime riêng. Không đợi W05 mới đọc source. Đối chiếu trực tiếp `filealloc/fileclose`, `iget/iput`, `bget/brelse/bpin/bunpin` trong bản BTL và [chương file system của xv6](https://mit-pdos.github.io/xv6-riscv-book/fs.html). Pipe đã lấy trang động trong source gốc, nên không dùng một lần đổi lời gọi ở pipe để thay cho ba cấu trúc mục tiêu.

## Bốn gói việc tuần 2

Giữ ID N01–N04 để nối kế hoạch cũ; các việc nhỏ dùng ID kỹ thuật đã có. Bản Word W01 liệt kê E01, K01/G02, S01→S02, K03/T01 và D01; bảng này phân rã cùng các mục đó, bổ sung chặng P02/T02 theo khả năng nghiệm thu. Trước nộp W01, cả nhóm chốt TODO; từ W02 ghi kết quả/trở ngại cho từng mục đã hẹn, không thay toàn bộ TODO bằng danh sách khác.

### N01 Linh làm tầng trang và review hợp đồng

- **Đầu vào:** baseline build/boot thực trên máy Linh; hiểu hàm đếm đang có và luồng init/free/allocate. Đây là việc thực hành code kế tiếp, không chờ học hết Lesson 4–13.
- **Ngày 26–27/10:** E01/P01, lưu revision/version/lệnh/log; cùng Minh chốt G02: đầu vào/ra của page và object, căn lề, OOM, quyền trả và tên API.
- **Ngày 28–30/10:** P02, tự định nghĩa pool nội bộ và mảng NCPU; init mọi khóa, ban đầu kalloc/kfree vẫn dùng pool 0; cập nhật hàm đếm phù hợp. Tách kiểm tra sau từng thay đổi nhỏ.
- **Tiêu chí:** build/boot; cấp-trả không mất/nhân đôi trang trong test; số trang trở về mốc cũ ở thời điểm yên; các pool không dùng chung nhầm head hoặc sao chép trạng thái lock.
- **Nếu P02 đạt sớm:** P03 local-only trên nhánh đang làm, kiểm tra CPUS=1. Bản chưa có stealing không nghiệm thu như per-CPU nhiều CPU hoàn chỉnh; P04/P05 thuộc W03.
- **Vùng sửa:** `kernel/kalloc.c`; Linh điều phối `main.c`, `defs.h`, Makefile để ghép hook/test, người khác nêu yêu cầu trong PR.
- **Reviewer:** Minh review cấu trúc/khóa; Khánh chạy harness. Linh lưu ghi chú review S01 để có đầu ra học phần slab.

### N02 Minh hiện thực slab một class

- **Đầu vào:** API trang gốc vẫn hoạt động. Slab gọi API trang, không đọc trực tiếp mảng pool của Linh, nên không phụ thuộc P04 xong mới viết.
- **Ngày 26–27/10:** review S01 cùng Linh. Hồ sơ README Minh ở remote439e89b mô tả header32 byte/con trỏ nhúng, còn `slab_allocator_design.md` đã đọc mô tả header theo các field/index; cần chốt một layout thống nhất trước code.
- **Cần chốt:** metadata/slot bắt đầu ở đâu, stride đủ cho liên kết trống và alignment, capacity theo kích thước header thực, tìm owner khi free, init, đồng bộ và xử lý cấp phát thất bại. Chọn một cơ chế quản lý slot cho bản đầu.
- **Ngày 28–30/10:** S02 prototype một class cố định trên trang gốc; tự viết struct, init, allocate/free và gọi qua API đã review. File mới `kernel/kmalloc.c`/header chỉ tạo khi bắt đầu hiện thực.
- **Tiêu chí:** object không trùng khi còn sống; pointer đúng biên/căn lề; ghi và đọc pattern; free rồi cấp lại; hết chỗ không làm hỏng metadata; giải thích được slot rỗng và trang rỗng.
- **Reviewer:** Linh review từng luồng; Khánh chạy object test khi API sẵn. Bản đầu dùng spinlock phù hợp xv6; chưa chuyển CAS của C# vào chỉ để giống nguồn.

### N03 Khánh làm môi trường và code kiểm thử

- **Ngày 26–27/10:** hoàn tất RISC-V toolchain còn thiếu, build/boot cùng revision và lưu log E01. Minh hỗ trợ chẩn đoán công cụ nếu vướng.
- **Ngày 28–30/10:** K03/T01 test matrix và mẫu log; T02 code harness trang gọi `kalloc/kfree`, kiểm tra địa chỉ theo trang, tính duy nhất của các trang còn sống, ghi pattern và bảo toàn trang sau trả tại thời điểm yên.
- **Khi S02 có API:** thêm ca object alignment, uniqueness, pattern, reuse và hết chỗ; một số ca nhiều CPU/OOM hoàn chỉnh chuyển T03/W03.
- **Vùng sửa:** test module và hướng dẫn kích hoạt; Linh ghép phần init/build dùng chung một lần. Không cần thêm syscall và user benchmark trong W02 chỉ để gọi vài test kernel đầu tiên.
- **Tiêu chí:** test phải có điều kiện phát hiện sai và báo FAIL; lưu commit, lệnh, cấu hình CPUS, expected/actual và log. Ca chưa chạy ghi chưa chạy; không suy từ `printf OK` hoặc CI baseline cũ.
- **Reviewer:** Minh review object scenarios; Linh review page scenarios và cách test không làm đổi ownership của allocator.

### N04 Cả ba review và chốt W02

- **Thứ Hai:** mỗi người xác nhận gói việc và một nhánh đang làm; ghi cùng contract/API.
- **Thứ Tư:** ghép hook/build và chạy thử đầu ra nhỏ, review lỗi sớm; không chờ dồn cuối tuần.
- **Thứ Sáu:** chốt code/phạm vi kết quả; việc chưa qua tiêu chí tiếp tục cùng ID.
- **Thứ Bảy 31/10:** mỗi người cập nhật comment trực tiếp trên Issue N01–N04 với code/PR/log; Linh tổng hợp từ các comment, cả nhóm đọc và chốt TODO W03. Không tạo file hồ sơ W02 riêng trong repo.
- **Chủ Nhật 01/11:** mốc nộp nội bộ dự kiến, phải theo hạn môn học nếu sớm hơn. Mỗi tuần vẫn có báo cáo dù chưa đạt hết mốc kỹ thuật.

## API và Git để ba người làm tách biệt

- Page allocator cung cấp trang; slab dùng trang để cấp object; test gọi API. Caller chịu trách nhiệm kết thúc lifetime trước khi free. Object free và page free là hai hợp đồng khác nhau.
- Tạm dùng tên `kmalloc/kmfree` cho tầng object khi review; page `kalloc/kfree` giữ ngữ nghĩa nguyên trang. Chốt yêu cầu tên chính thức trong G02/A01 trước W04; nếu phải dùng `kfree` cho object thì đổi API trang và tất cả caller trong một PR riêng, có regression, không đổi hành vi bằng cách dùng lại tên mơ hồ.
- Mỗi người chỉ cần một nhánh công việc đang làm: ví dụ Linh `codex/page-pools`, Minh `codex/slab-one-class`, Khánh `codex/allocator-tests`. Khi việc đã merge thì xóa nhánh đó; việc tiếp theo bắt đầu từ main đã cập nhật. Tên nhánh là đề xuất, chưa tạo nhánh tự động. Issue N01–N04 đã tạo/gán trên GitHub; thành viên xác nhận nhận việc khi bắt đầu.
- Linh điều phối các file dùng chung, còn Minh/Khánh tự phát triển module mình. Khi cần API mới, review chữ ký/hành vi trước, không sửa nội bộ module của người kia để ghép tạm.
- Main nhận các lát cắt giữ được build/boot/test. P02 dùng pool0 có thể giữ hành vi cũ; P03 local-only thiếu stealing là bước học trung gian, chưa là tính năng nhiều CPU đạt.

## VstHelper được dùng ở đâu

| Nguồn trong snapshot | W02–W03 lấy nguyên lý nào | Điều chỉnh cho xv6 |
|---|---|---|
| `MemoryPool.Rent` | W03 chọn class theo kích thước yêu cầu | Class đủ chứa sizeof các client; kiểm tra size0/oversize/OOM; không chép ngưỡng trong comment |
| `MemoryPage` và `FreeSlots` | W02 chia vùng nhớ thành slot, quản lý slot trống | Tính capacity với metadata/alignment trên PGSIZE thực; review chọn bitmap hoặc freelist, không áp cứng 64 slot |
| `MemorySlotHeader`/`PageHandle` | W02 xác định chủ sở hữu để free đúng | Chốt layout/header hoặc owner theo slab; kiểm tra pointer trước khi truy metadata |
| `TryRent/Return` | W02 kiểm tra allocate/free và reuse | Bản đầu đồng bộ bằng spinlock; CAS của C# chưa là quyết định cho kernel |
| `MemoryPageGroup` | W03 quản lý các slab cùng class | Có thể tìm/chọn slab còn chỗ và thống kê; không mang runtime .NET vào kernel |

Nguồn đọc: [Memory.cs](../references/VstHelper/Memory/Memory.cs), [phần đối chiếu kỹ thuật](Nghien-cuu-de-tai-19.md#4-kế-thừa-baokhoahoc-ở-mức-nào). Trong allocator tham khảo đã đọc chưa có per-CPU page lists/stealing; phần này dựa vào cơ chế xv6 và [MIT lock lab](https://pdos.csail.mit.edu/6.1810/2025/labs/lock.html). Những con số/tài liệu benchmark C# không phải kết quả của xv6.

## Kiểm thử và đánh giá để kịp bàn giao

- Cuối W03 phải phân biệt hai việc đã đạt: allocator trang chạy nhiều CPU có stealing; slab cấp/trả object có assertion. Chạy sớm để tránh mang lỗi vào ba client.
- W04–W05 kiểm tra `file` qua open/dup/fork/close; `inode` qua lookup cùng định danh, refcount và transaction; `buf` qua đọc/ghi, lookup duy nhất, pin/I/O/LRU. Chuyển sang allocation thật, không chỉ xin một khối lúc boot rồi giữ mô hình bảng tĩnh mà gọi là xong.
- Statistics/dump bắt đầu W03, protocol B01 ở W04, runner B02 ở W06. W07 so sánh page baseline chung với per-CPU trên cùng đơn vị trang; so slab trên page gốc/per-CPU với cùng object workload. Không trộn chênh lệch kích thước cấp phát với tác động khóa.
- Đo 1/2/4/8 vCPU, có warmup và lặp; mục tiêu ban đầu 10 lần/config, điều chỉnh theo độ biến thiên. Lưu raw data, commit/toolchain/QEMU/host/CPUS, workload và lần lỗi. Không dùng thời gian in màn hình làm throughput allocator.
- Fragmentation: tách payload, rounding, metadata/padding, slot trống; leak: xét live object và trang sau chu kỳ cấp/trả/trim tại thời điểm yên, tính cả cache hợp lệ đang giữ. Dump phải có số đối chiếu, không chỉ trang trí.
- CI và test lab chỉ báo đạt khi có log thực. BTL chưa có sẵn kalloctest/kcsan của lab; dùng harness của nhóm hoặc nhập/adapt rõ nguồn như một task riêng. Không đặt `make grade` của MIT thành lệnh đã sẵn có trong BTL.

## Dự phòng nếu chậm mốc

- Nếu E01 còn vướng sau 27/10, Minh hỗ trợ Khánh cài/chẩn đoán; Khánh vẫn tự viết matrix và test source trong khi chờ môi trường. Ghi rõ test chưa chạy.
- Nếu cuối W03 chưa đạt M1/M2, W04 ưu tiên sửa lõi và thiết kế client song song; Linh/Minh hỗ trợ bug cụ thể của nhau. Chốt lại khối lượng/hỗ trợ ngay, thay vì nhận ngầm mọi mốc vẫn đúng hạn.
- Nếu cuối W05 chưa đủ ba client, W06 ưu tiên I05 và sửa lifetime trước thu số liệu chính thức; giữ W08 làm dự phòng. Tăng thời gian hoặc đổi người phụ trách theo task có ghi nhận nếu cần.
- Phần được hoãn đầu tiên: buddy, adaptive stealing, object magazine, nhiều biến thể batch và biểu đồ phụ. Phần bắt buộc vẫn giữ slab/per-CPU/stealing, ba client, thống kê/dump, throughput/fragmentation/leak và báo cáo.
- Mỗi báo cáo ghi từng TODO đã hẹn là xong/chưa xong/đổi có lý do và kèm bằng chứng. Giữ ID xuyên tuần để kiểm tra độ ăn khớp theo yêu cầu môn; không tự đặt công thức tính 25% thay giảng viên.

## Bước bắt đầu

Linh thực hành **P02 trong kalloc.c**, sau baseline và review G02 ngắn; Minh review S01 rồi tự code S02; Khánh hoàn tất môi trường và tự code harness. Nhóm có thể bắt đầu từ ba gói việc này mà không cần học hết chuỗi lesson. Các Issue N01–N04 và Project đã triển khai ngày 24/10 theo yêu cầu Linh; dùng liên kết ở mục Bảng công việc đang dùng.
