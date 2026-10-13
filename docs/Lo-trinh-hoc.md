# Lộ trình vừa học vừa code đề tài 19

Phương thức đã chọn: người dùng tự thiết kế và tự viết toàn bộ phần C của bài học/đề tài. Assistant dạy, giao yêu cầu nhỏ, đọc diff, kiểm tra và gợi ý sửa; không viết implementation hoặc đưa cả lời giải để người dùng chép. Mỗi lượt tập trung một phần nhỏ.

Mỗi phần có: mục tiêu → khái niệm → trách nhiệm và dữ liệu vào/ra → reference/phụ thuộc → yêu cầu tự viết → kiểm tra → giải thích kết quả. Hướng dẫn tăng dần từ câu hỏi hoặc gợi ý, không đưa ngay toàn bộ hàm.

## Nền C phải học trước bài thực hành allocator

Bắt đầu với [Bài 0: hàm, header và liên kết](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/00-ham-header-va-lien-ket.md>). Mỗi lượt đọc một phần, mở source được chỉ dẫn và tự giải thích trước khi viết code.

1. Khai báo biến/kiểu dữ liệu và hàm: tham số, kiểu trả về, scope.
2. Khai báo khác định nghĩa; `.h` khác `.c`; `#include` có tác dụng gì.
3. Compiler tạo object file, linker giải quyết reference giữa các file; vai trò Makefile.
4. Địa chỉ, `&`, con trỏ, `*`, `void *` và phép ép kiểu.
5. Struct, dấu `.`/`->`, layout và lifetime.
6. Danh sách liên kết và cách một trang trống chứa node.

Các nội dung này được chia thành lượt nhỏ trước khi yêu cầu tự viết hàm đếm. Source xv6 đã nhập là bản tham chiếu và nền runtime; phần phát triển đề tài do người dùng viết.

| Bài | Nội dung | Sản phẩm tự viết hoặc kiểm chứng |
|---|---|---|
| 1 | Chuẩn bị baseline, build/boot; đọc allocator trang; đếm trang trống | xv6 gốc chạy được và hàm kfreepage_count |
| 2 | Race condition, spinlock, critical section, CPU ID và ngắt | Giải thích được vì sao thay freelist phải có khóa |
| 3 | Free list riêng cho từng CPU | Local allocate/free, invariant trang thuộc một nơi |
| 4 | Stealing đơn giản, lô một trang | CPU thiếu trang lấy được từ CPU khác, tránh giữ hai khóa |
| 5 | Kiểm thử tầng trang | Bảo toàn trang, OOM, nhiều CPU, lỗi pointer |
| 6 | Slab đầu tiên: một trang, một kích thước object | Metadata, layout, bitmap và cấp/trả slot |
| 7 | kmalloc nhiều size class và object free | Size boundaries, alignment và API phân biệt với page free |
| 8 | Thống kê và thu hồi slab rỗng | Dump và kiểm thử reclaim/leak |
| 9 | Tích hợp pipe để luyện, rồi file động | Lifetime, refcount và rollback khi lỗi |
| 10 | Inode và buffer động | Registry, uniqueness, pin/I/O; đáp ứng 3 cấu trúc chuyển động |
| 11 | Harness và benchmark 1/2/4/8 CPU | Dữ liệu throughput, latency, contention, memory breakdown |
| 12 | Khảo sát fixed/adaptive batch; buddy nếu làm stretch | Có đối chứng, split/merge và giải thích trade-off |
| 13 | Báo cáo và demo | Script lặp lại, biểu đồ và kết luận theo dữ liệu |

Các bài 1–11 là đường tới phần bắt buộc. Bài 12 là phần mở rộng được chọn theo thời gian và mục tiêu nghiên cứu.

## Tiến độ hiện tại

- Bản source riêng đã nhập vào BTL; metadata nguồn ở `docs/xv6-baseline.json`.
- Kernel vẫn dùng allocator gốc; code bài tập chưa được gõ.
- Bài 1 đang ở bước chuẩn bị môi trường: chưa có distro WSL và chưa xác nhận build/boot.
- Bài học chi tiết: `docs/lessons/01-baseline-va-kalloc.md`.

## Cách gửi để kiểm tra

Khi hoàn thành một bước, gửi phần output liên quan hoặc nói rõ các file đã sửa. Assistant có thể đọc trực tiếp file trong workspace và `git diff`; không cần dán lại toàn bộ source.

Nếu chạy lỗi, gửi lệnh đã chạy, terminal đang là PowerShell hay Ubuntu, và đoạn lỗi đầu tiên cùng phần cuối output. Giữ nguyên thay đổi để đối chiếu nguyên nhân.
