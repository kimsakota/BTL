# Báo cáo tiến độ nhóm 26118

Ngày chuẩn bị: 19/10/2026. Nguồn mẫu: `C:/Users/KimSakota/Downloads/Progress Report template.pdf` gồm hai trang; ví dụ `[25224] Progress Report sample week1.pdf` gồm tám trang. Hai PDF đã được trích nội dung và xem bản render. Không sao chép danh tính, tiến độ 15–20%, kết quả test hoặc công cụ đã dùng của nhóm mẫu sang nhóm 26118.

## Những trường phải giữ

Phần đầu cần Course, Project code, Project title, Design thinking page, Task management page, Members và Reported working period from/to. Mã nhóm là **26118**; tên/MSSV lấy từ ảnh Linh gửi. Mẫu ghi `[Multimedia]`, còn thông báo Linh chép ghi lớp CN: dùng tên lớp theo môn học thực tế, không tự nhận nhóm học Multimedia.

Năm phần chính theo đúng thứ tự:

1. Summary of latest progress: 200–300 từ, nói về kỳ đang báo cáo và tiến độ tích lũy.
2. Review of technical activities: task/hoạt động, phương pháp và đầu ra.
3. Personal contribution: cả Linh, Minh, Khánh, mỗi người có việc thực và đầu ra.
4. Developed code review: a Algorithmic development; b Source code enhanced and/or revised; c Testing result.
5. TODO: a Remaining issues; b Tasks for next period; c Milestone.

Ảnh/log/task board có thể thêm phụ lục; phần 6 trong ví dụ không phải mục bắt buộc được liệt kê trong template. Mẫu cho phép tiếng Việt hoặc Anh theo thông báo. Khi viết tiếng Việt, có thể kiểm tra độ dài summary theo từ tách bởi khoảng trắng; nếu giảng viên có cách đếm khác thì theo hướng dẫn đó.

## Cách nối TODO giữa các tuần

Mỗi task báo cáo có ID ổn định, owner, đầu ra và thời hạn trong kỳ. Báo cáo sau cần một bảng đối chiếu:

| TODO kỳ trước | Owner | Trạng thái thực tế | Kết quả hoặc trở ngại | Bằng chứng | Quyết định kỳ sau |
|---|---|---|---|---|---|
| ID gốc | Người phụ trách | Done / Partial / Blocked / Deferred | Cụ thể đã đạt phần nào | PR, tài liệu, log | Đóng hoặc tiếp tục với cùng ID |

Tuần đầu chưa có báo cáo trước thì ghi rõ “không áp dụng”; không tự dựng TODO lịch sử. Linh xác nhận tuần 19–25/10/2026 mới khởi động dự án, vì vậy W01 là khung báo cáo đầu tiên; phần học trước là chuẩn bị cá nhân, không phải một báo cáo nhóm trước đó.

Thông báo cho phép sai khác 25% nhưng chưa quy định công thức theo số task, độ lớn hay effort. **Quy tắc nội bộ đề xuất** là giữ ít nhất 75% đầu việc đã cam kết theo cùng mục tiêu/owner/đầu ra, mọi task cũ vẫn được giải trình và mọi thay đổi có lý do. Đây không phải cách chấm đã được giảng viên xác nhận.

Ví dụ nếu kỳ trước có 4 task, nhóm nên giữ tối thiểu 3 mục tiêu gốc và chỉ dự kiến thay tối đa 1 mục tiêu khi áp dụng cách đếm nội bộ. Task đang làm dở không được tự tính là Done, dù vẫn được giữ trong kế hoạch. Việc thêm nhiều task mới hoặc thay nội dung nhưng giữ ID vẫn là thay đổi cần giải thích; không thể kết luận tuân thủ chỉ từ số dòng. Khi scope cần đổi nhiều hơn, ghi đúng sự thật và xin giảng viên hướng dẫn, không sửa lịch sử để làm đẹp tỷ lệ.

## Nhịp nộp và bằng chứng

- Nộp ít nhất một lần mỗi tuần vào cùng thread môn học của nhóm. Không lấy mốc 14 ngày làm chu kỳ nộp.
- Nhóm thống nhất hạn nội bộ sớm hơn hạn giảng viên nếu có thể. Tuần khởi động là 19–25/10; đề xuất tổng hợp nội bộ thứ Bảy 24/10, cần kiểm tra giờ nộp chính thức.
- Mỗi người gửi phần đóng góp trước hạn tổng hợp; ghi rõ output, phần code/tài liệu/review mình làm và kết quả đã chạy.
- Bằng chứng test phải có revision, dirty diff nếu có, môi trường, lệnh, số CPU và log. Ảnh có thể minh họa, raw log giúp kiểm tra lại.
- Nêu rõ đã nghiên cứu/đã viết/đã review source/đã build/đã boot/đã test/đã đo. Không dùng các trạng thái này thay thế nhau.
- Sau nộp lưu ngày giờ Asia/Saigon, tên file, permalink reply và TODO làm gốc cho kỳ tiếp theo. Chưa nộp thì không điền trạng thái Submitted.

Tên file đề xuất: `26118-Progress-Report-WNN-YYYY-MM-DD.pdf`; ngày là ngày nộp thực. Giữ bản Markdown chỉnh sửa cùng file nộp. Không cần lập nhiều hệ thống task khác nhau: board thật + repository + thread môn học là đủ.

## Bản nháp đầu tiên hiện có

[Bản Markdown](26118-Progress-Report-W01-DRAFT.md) và PDF cùng tên là **khung chờ hoàn thiện**, chưa sẵn sàng nộp. Summary ghi đúng bối cảnh khởi động; đầu tuần chưa đủ kết quả/contributions để hoàn tất báo cáo cuối tuần. Tài liệu/code học trước chỉ là đầu vào, không tính mặc định vào kết quả nhóm W01.

Trước khi nộp, nhóm cần:

1. Điền from/to theo ngày làm và ngày chốt thực tế trong tuần khởi động; xác nhận giờ nộp chính thức.
2. Chọn nội dung đã diễn ra trong kỳ; không gán toàn bộ tài liệu chuẩn bị với assistant thành công việc chung của ba người.
3. Linh xác nhận phần thực sự tự làm; Minh/Khánh tự bổ sung ít nhất một đóng góp có output thật trong kỳ. Hiện chưa có dữ liệu để điền thay hai bạn.
4. Điền URL design page và task board truy cập được; không để đường dẫn local đóng vai trò link mà giảng viên có thể mở.
5. Cả ba xác nhận TODO/owner và mốc. Nếu có test mới thì thay “chưa chạy” bằng kết quả cùng bằng chứng, không chỉ xóa dòng.
6. Đọc lại summary 200–300 từ, đủ năm phần, đúng mã nhóm/MSSV, có đối chiếu TODO nếu áp dụng; xóa nhãn DRAFT khi mọi dữ kiện đã được xác nhận.

Không tạo báo cáo “đủ ba người” bằng cách tự gán việc dự định làm cho Minh và Khánh. Nếu hai bạn chưa làm, giao một task nghiên cứu/review/thiết kế nhỏ có đầu ra thật và thực hiện trước hạn; kết quả đó mới được đưa vào đóng góp.
