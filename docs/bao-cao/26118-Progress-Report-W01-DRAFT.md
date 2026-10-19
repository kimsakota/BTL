# PROGRESS REPORT

**DRAFT - CHƯA ĐỦ THÔNG TIN ĐỂ NỘP**

**Course:** Hệ điều hành - CN - 20261

**Project code:** 26118

**Project title:** Slab allocator and per-CPU free lists

**Design thinking page:** [CẦN ĐIỀN URL tài liệu thiết kế mà giảng viên mở được]

**Task management page:** [CẦN ĐIỀN URL board nhóm đang sử dụng]

**Members:**

1. Nguyễn Kim Linh - 20233495
2. Lê Trần Hiếu Minh - 20233528
3. Nguyễn An Khánh - 20224019

**Reported working period:** from [CẦN ĐIỀN ngày/tháng/2026] to [CẦN ĐIỀN ngày/tháng/2026]

**Kỳ báo cáo:** W01 - tuần khởi động 19–25/10/2026; điền ngày làm và ngày chốt thực tế ở trên.

## 1. Summary of latest progress

Tuần này là tuần khởi động dự án nhóm về slab allocator và danh sách trang theo CPU trên xv6. Phần Linh đang học là chuẩn bị cá nhân trước dự án. Các bài học, ghi chú và hàm đếm trang có sẵn được dùng làm đầu vào tham khảo, không mặc định tính là kết quả nhóm mới hoàn thành trong kỳ này. Bản báo cáo cuối tuần cần bổ sung đầu ra thực tế và đóng góp của từng thành viên.

Phương án làm việc được chuẩn bị theo ba trách nhiệm: tầng trang, tầng object và kiểm thử. Linh dự kiến phụ trách per-CPU lists, stealing và buffer; Minh phụ trách slab và file; Khánh phụ trách test, đo đạc và inode. Các thành viên cần thống nhất API, ownership và tiêu chí bàn giao trước khi code. Slab có thể phát triển trên tầng trang gốc, còn test matrix có thể viết trước implementation để các phần tiến triển song song.

Mục tiêu tuần đầu là xác nhận môi trường, baseline, thiết kế slab một class và cách kiểm tra. Hiện chưa đủ bằng chứng để nghiệm thu các mốc per-CPU, slab, ba client động hoặc benchmark; chưa gán phần trăm hoàn thành. Cuối tuần, nhóm sẽ đối chiếu kết quả với bốn đầu ra khởi động, ghi trở ngại thật và chốt TODO kỳ sau. Phần chưa xong được tiếp tục với cùng ID; công việc học trước vẫn được phân biệt với tiến độ nhóm.

<!-- pagebreak -->

## 2. Review of technical activities

**Activity 1 - Xác định điểm xuất phát.** Linh xác nhận tuần này mới bắt đầu dự án nhóm; bài học và code thử trước đó là chuẩn bị cá nhân. Ảnh đề tài yêu cầu slab, per-CPU có stealing, ba cấu trúc động và đánh giá 1–8 CPU. Các tài liệu/source có sẵn là đầu vào, chưa phải thành tích chung của nhóm W01.

**Activity 2 - Chuẩn bị phương án phát triển theo module.** Tài liệu quy trình đề xuất API nhỏ, một repository chung, nhánh theo task, PR và review chéo. Slab gọi tầng trang; client quản lý lifetime; test gọi API. Đầu ra: `docs/Quy-trinh-nhom-26118.md` và `docs/Ke-hoach-va-backlog-26118.md`. Đây là tài liệu chuẩn bị với trợ giúp của assistant, chưa xác nhận cả nhóm đã review/chốt.

**Activity 3 - Phân rã đầu ra tuần khởi động.** K01: Linh xác nhận baseline/API; K02: Minh thiết kế slab một class; K03: Khánh chuẩn bị test matrix/log; K04: cả ba chốt phân công và đóng góp. Đây là việc được đề xuất cho W01, chưa ghi Done. Cuối tuần thay đoạn này bằng kết quả/trở ngại và evidence thật của từng task.

## 3. Personal contribution

**Member 1 - Nguyễn Kim Linh - 20233495.** Đã cung cấp thông tin nhóm/đề tài và xác nhận bối cảnh khởi động; đề nghị phương án chia module và quy trình báo cáo. [CẦN LINH BỔ SUNG kết quả review tài liệu, task K01/K04 và bằng chứng trong kỳ]. Bài học/hàm đếm trước đó là chuẩn bị cá nhân, không tự tính là đầu ra W01. Tài liệu chuẩn bị cùng assistant cần Linh đọc và kiểm tra trước khi nhận là đóng góp của mình.

**Member 2 - Lê Trần Hiếu Minh - 20233528.** [CẦN MINH ĐIỀN công việc thực tế đã làm, đầu ra, thời điểm và bằng chứng trong kỳ]. Phần slab/file hiện là phân công đề xuất, chưa được ghi nhận là đóng góp đã hoàn thành.

**Member 3 - Nguyễn An Khánh - 20224019.** [CẦN KHÁNH ĐIỀN công việc thực tế đã làm, đầu ra, thời điểm và bằng chứng trong kỳ]. Phần test/inode hiện là phân công đề xuất, chưa được ghi nhận là đóng góp đã hoàn thành.

<!-- pagebreak -->

## 4. Developed code review

### a. Algorithmic development

Thiết kế dự kiến tách tầng trang và tầng object. Mỗi free list theo CPU có khóa; stealing xử lý local miss. Slab chia trang thành slot và giữ metadata nội bộ. API, alignment, giới hạn kích thước, bootstrap, lock order và failure path phải được nhóm thống nhất trước khi triển khai. Chưa có implementation thuật toán mới để kết luận đúng hoặc nhanh hơn.

### b. Source code enhanced and/or revised

Source có sẵn `kfreepage_count` trong `kernel/kalloc.c`, prototype trong `kernel/defs.h`, caller sau `kinit` trong `kernel/main.c`. Đây là phần chuẩn bị học trước, chưa nhận là thay đổi nhóm W01. Lượt chuẩn bị quy trình ngày 19/10/2026 chỉ bổ sung tài liệu và mẫu quản lý, không sửa implementation C/header/assembly. [CUỐI TUẦN BỔ SUNG phần code thực sự thay đổi trong W01 nếu có].

### c. Testing result

Đã review source và đối chiếu sự tồn tại của hàm đếm, khai báo, caller; đây là kiểm tra tĩnh. Chưa có log build/boot/usertests/benchmark được đối chiếu. Không có `kalloctest.c` hoặc `kcsan.c` của MIT lock-lab trong bản BTL hiện tại; chưa ghi các test đó là đã chạy. [BỔ SUNG kết quả thực trong kỳ nếu có, gồm revision, lệnh, cấu hình và log].

## 5. TODO

### a) List of remaining issues

Chưa xác nhận giờ nộp chính thức, hạn cuối kỳ, đóng góp/đầu ra mỗi người, URL design/board và common deliverables. Chưa có runtime baseline được đối chiếu; per-CPU, slab, ba client động và số liệu đánh giá còn phải triển khai. Cần xác nhận cách tính ba cấu trúc, tên object free cuối và quy tắc sai khác 25%.

### b) List of tasks to carry out in the next period

| ID | Owner dự kiến | Đầu ra cho kỳ kế tiếp |
|---|---|---|
| N01 | Linh | P02: state mảng nhưng vẫn pool 0; cập nhật hàm đếm và kiểm tra phù hợp |
| N02 | Minh | Bước S02 nhỏ đầu tiên: một class theo layout đã review, test reuse/pattern |
| N03 | Khánh | Harness trang cơ bản có assertion, revision và raw log trên baseline |
| N04 | Cả ba; Linh điều phối | Review chéo đầu ra, kiểm tra hợp đồng/init và cập nhật task cùng evidence |

Các task N01–N04 cho W02 chỉ nhận khi đầu vào W01 sẵn sàng; nếu chưa, tiếp tục task K01–K04 còn vướng và ghi lý do. Cuối tuần nhóm chốt lại TODO theo kết quả thực. Đối chiếu báo cáo kỳ trước: không áp dụng vì đây là báo cáo đầu tiên của dự án nhóm.

### c) Milestone

W01 hướng tới M0: baseline cùng revision, môi trường và log build/boot/test nền kiểm tra lại được, cùng hợp đồng API đã review. Nếu M0 đạt, mốc kế tiếp là các lát cắt page/object/test tối thiểu; nếu chưa, ưu tiên hoàn tất M0. Cuối kỳ báo cáo cập nhật trạng thái bằng bằng chứng thật.
