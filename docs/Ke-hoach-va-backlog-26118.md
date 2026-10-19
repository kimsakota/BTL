# Kế hoạch và backlog dự án 26118

Ngày: 19/10/2026. **Phân công/lịch mốc dự kiến, chờ nhóm thống nhất và bổ sung hạn cuối kỳ.** Linh xác nhận tuần này mới khởi động nhóm: W1 là 19–25/10/2026. Phần Linh đang học là chuẩn bị cá nhân trước dự án. Phụ trách đề xuất: Linh tầng trang/tích hợp, Minh slab/file, Khánh test/inode. Chi tiết hợp đồng nằm trong [quy trình nhóm](Quy-trinh-nhom-26118.md).

Thứ tự khởi động thực tế: **mời/accept cả hai bạn → mỗi người clone và đọc tài liệu → nhận K01–K04 trên board → thử một PR/review → phát triển theo mốc**. Xem [sổ tay GitHub](So-tay-GitHub-nhom.md) để thao tác bằng Desktop/web. Đây là việc cộng tác phải hoàn tất, không tự coi là xong vì đã bật bảo vệ main.

## Các mốc có đầu ra kiểm tra được

| Mốc | Điều kiện nghiệm thu | Trạng thái đã đối chiếu 19/10 |
|---|---|---|
| M0 Baseline | Cả ba có môi trường, cùng revision; build/boot và test nền có log | CI main53872b8 đã đạt; môi trường/log local của cả ba chưa đủ xác nhận |
| M1 Trang | Per-CPU + stealing; page invariants và test nhiều CPU | Chưa triển khai |
| M2 Object | Slab nhiều class, OOM/alignment/reuse và statistics/dump | Chưa triển khai |
| M3 Client | Ba cấu trúc file/inode/buf cấp động với lifetime đúng; regression | Chưa triển khai |
| M4 Đánh giá | Throughput 1/2/4/8 CPU, fragmentation/leak, raw data và demo | Chưa đo |
| M5 Bàn giao | Báo cáo, source, hướng dẫn reproduce và deliverables theo Overview | Chưa đủ Overview/deadline |

Không dùng số bài học đã soạn để đánh dấu mốc code hoặc ghi là kết quả nhóm mới làm trong tuần 1. Chuẩn bị cá nhân có giá trị làm đầu vào; tiến độ nhóm được ghi từ tuần khởi động và theo đầu ra đã review. Chưa có rubric/trọng số nên không gán phần trăm định lượng.

## Backlog để chuyển thành issue nhỏ

Mỗi dòng là gói việc; khi bắt đầu tách code thành chặng 1–2 buổi nếu cần. Tất cả dòng dưới là **đề xuất chưa giao nhận/chưa nghiệm thu**, kể cả phần có tài liệu hỗ trợ sẵn. Evidence là đầu ra cần có, không phải file đã tồn tại.

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

## Lịch tham khảo tám tuần

Đây là **khung co giãn**, không phải cam kết tám tuần là đủ với năng lực/quỹ giờ hiện chưa biết. Dời mốc nếu chưa qua tiêu chí; mỗi tuần vẫn báo cáo tiến độ thật. Nếu ít tuần hơn, ưu tiên mốc bắt buộc, giảm biến thể chính sách và bỏ stretch; không cắt ba cấu trúc hoặc phép đo 1–8 CPU nếu rubric giữ yêu cầu đó.

| Tuần | Linh | Minh | Khánh | Đầu ra chung |
|---|---|---|---|---|
| W1 | Chốt phạm vi/API, baseline và hàm đếm | Slab layout một class, ownership/init | Test matrix, môi trường/log schema, board | M0 hoặc ghi blocker thật; báo cáo đủ 3 người |
| W2 | P02/P03 rồi P04 từng bước | S02 trên page gốc | T02, thử protocol page baseline | Bản nhỏ chạy được, chưa hứa multiCPU khi chưa stealing |
| W3 | Hoàn thiện P05, cùng ghép init | S03/S04 | T03 và kiểm tra I00 | M1/M2 khi test đạt |
| W4 | Pipe thử và thiết kế buffer | File động | Thiết kế inode và test lifetime | Một client tích hợp qua API |
| W5 | Buffer động | Hoàn thiện file, review inode | Inode động với Linh review | M3 khi cả ba client và regression đạt |
| W6 | Sửa lỗi tích hợp, xác nhận fairness | Fragmentation/retention | B02/B03 và thu raw data | M4 bản đầu |
| W7 | Phân tích page, hỗ trợ reproduce | Leak/fragmentation và slab dump | Chạy lại độc lập, tổng hợp biểu đồ | Số liệu kiểm tra lại được; dự phòng lỗi |
| W8 | Báo cáo/demo và giải thích toàn luồng | Phần slab/client, kiểm tra API cuối | Bộ log/test/demo checklist | M5; chỉ làm stretch nếu còn thời gian |

## Bốn đầu ra cho tuần khởi động W1

Các dòng dưới là việc **cần làm trong tuần 19–25/10**, chưa phải kết quả đã đạt. Đề xuất gửi đóng góp cá nhân ngày 23/10 và tổng hợp nội bộ ngày 24/10; đối chiếu hạn chính thức trước khi dùng.

| ID | Owner đề xuất | Đầu ra nhỏ trong W1 | Liên hệ backlog |
|---|---|---|---|
| K01 | Linh | Ghi phạm vi và API trang; build/boot/test nền hoặc log lỗi có chẩn đoán; review lại phần chuẩn bị cá nhân | G01, G02, E01, P01 |
| K02 | Minh | Xác nhận môi trường, đọc API trang và viết một trang thiết kế slab một class | E01, S01 |
| K03 | Khánh | Xác nhận môi trường, test matrix trang/object và mẫu manifest/log | E01, T01 |
| K04 | Linh điều phối, cả ba | Biên bản phân công/API/deadline, board/design link và phần đóng góp W01 của từng người | G01–G03, D01 |

Nếu cả ba bắt đầu từ ít nền tảng, bản giải thích/sơ đồ có review là đầu ra tuần đầu hợp lệ; không ép ghi đã hoàn thành thuật toán. Các việc K chưa xong phải xuất hiện trong kết quả/trở ngại W01 và tiếp tục ở TODO W02.

## Bốn task nhỏ dự kiến cho kỳ tiếp theo W2

Giữ số task báo cáo ổn định và nhỏ giúp nối TODO. Bốn task này cũng xuất hiện trong mục TODO của bản nháp W01, **chỉ nhận khi đầu vào W1 đã sẵn sàng**. Cuối tuần thay bằng scope đã chốt; không đồng thời cam kết nhiều task mới nếu còn vướng baseline.

| ID báo cáo | Owner | Đầu ra được kiểm tra | Liên hệ backlog |
|---|---|---|---|
| N01 | Linh | Mốc P02: state mảng nhưng vẫn pool 0 chung; cập nhật hàm đếm và kiểm tra phù hợp | K01, P01 |
| N02 | Minh | Bước S02 nhỏ đầu tiên theo layout đã review; cấp/trả một class với test reuse/pattern | K02, S01 |
| N03 | Khánh | Harness trang cơ bản với assertion, raw log và revision; chạy trên baseline đã sẵn sàng | K03, E01, T02 |
| N04 | Cả ba; Linh điều phối | Review chéo các đầu ra, kiểm tra hợp đồng/init và cập nhật task cùng bằng chứng | K04, G02 |

Tách task nội bộ nhỏ hơn không cần đổi ID báo cáo nếu mục tiêu/owner/đầu ra giữ nguyên. Task mới phát sinh được ghi nguồn và lý do; task cũ chưa xong vẫn có một dòng kết quả/trở ngại. Không dùng đổi tên để che việc bỏ TODO.

## Bước nhóm thực hiện ngay

Mỗi người đọc phần mình phụ trách, giải thích lại API dùng từ tầng khác và chọn đầu ra W1 trong K01–K04. Linh kiểm tra giờ nộp; từng người thực hiện rồi tự cung cấp minh chứng của tuần khởi động. Sau đó chốt TODO W2 và chuyển khung báo cáo thành bản nộp. Phần Linh học trước chỉ được ghi như nền chuẩn bị, không nhận thay đóng góp tuần đầu của cả nhóm.
