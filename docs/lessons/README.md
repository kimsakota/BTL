# Bài học chung của nhóm 26118

Đây là mục lục để Linh, Minh và Khánh đọc trên GitHub hoặc từ clone của mình. Lesson là tài liệu nền đã soạn; có file không đồng nghĩa người đọc đã thực hành xong, và không tự tính là đóng góp trong tuần.

| Bài | Nội dung | Ai nên đọc |
|---|---|---|
| [0. Hàm, header và liên kết](00-ham-header-va-lien-ket.md) | Nền C, khai báo/định nghĩa, compiler/linker | Cả ba; đọc kỹ phần chưa biết |
| [1. Baseline và allocator trang](01-baseline-va-kalloc.md) | Trang, con trỏ, struct, free list, ownership và hàm đếm | Cả ba, vì slab và test dùng API trang |
| [2. Race, spinlock và CPU](02-race-spinlock-va-cpu.md) | Khóa, ngắt, CPU ID, lifetime của vùng bảo vệ | Cả ba trước phần code có concurrency |
| [3. Free list theo CPU](03-per-cpu-free-lists.md) | Mảng pool, init, local path, đếm và giới hạn chưa stealing | Linh thực hành; Minh/Khánh đọc phần hợp đồng để review |

[Lộ trình học các phần tiếp](../Lo-trinh-hoc.md) gồm stealing, test trang, slab, client, benchmark và bàn giao. Hiện chỉ Bài 0–3 đã được soạn; các bài sau là kế hoạch.

## Cách dùng

1. Đọc mục đang cần cho task của mình; ghi câu hỏi có tên bài/mục và đoạn source liên quan.
2. Thực hành trên nhánh theo task của bản clone cá nhân; ghi output thực, không chép output ví dụ làm kết quả.
3. Nếu tìm lỗi/chỗ khó hiểu trong lesson, mở issue hoặc PR sửa tài liệu để cả nhóm được hưởng bản sửa.

Linh muốn thêm lesson: tạo file Markdown trong thư mục này, cập nhật bảng, commit/push trên nhánh docs và mở PR. Sau merge, gửi link file trên main để mọi người đọc chung; người đọc dùng bản local thì main → Fetch origin → Pull origin. Xem [sổ tay thao tác GitHub](../So-tay-GitHub-nhom.md#4-ví-dụ-linh-đăng-một-lesson-để-cả-nhóm-đọc).

Các lệnh build trong bài phải chạy tại thư mục clone của người thực hiện. Đường dẫn WSL minh họa của Linh cần đổi theo vị trí clone trên máy mình. Nguồn Exercise/VstHelper bên ngoài repo nếu được nhắc tới là nguồn local tham khảo, không mặc định có trên máy hai bạn.

Quay lại [sổ tay nhóm](../So-tay-GitHub-nhom.md), [quy trình kỹ thuật](../Quy-trinh-nhom-26118.md) và [backlog](../Ke-hoach-va-backlog-26118.md).
