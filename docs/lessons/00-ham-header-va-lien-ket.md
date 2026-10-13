# Bài 0: Hàm, header và liên kết giữa các file C

Đọc mục 0 trước để biết mã đang đọc thuộc về đâu, rồi học mục 1–3. Mục tiêu là hiểu một hàm được khai báo, định nghĩa và gọi ở đâu trước khi tự viết hàm mới.

## 0. Đây là mã xv6 có sẵn, phần đề tài nằm ở đâu?

xv6 là một hệ điều hành nhỏ phục vụ học tập. Kernel nghĩa là nhân hệ điều hành: phần quản lý tiến trình, bộ nhớ, thiết bị và các chức năng cốt lõi. Thư mục kernel/ trong BTL chứa mã nhân xv6 đã có sẵn, được nhập từ bản xv6 ở Exercise mà người dùng cung cấp.

Đề tài 19 là mở rộng cơ chế quản lý bộ nhớ bên trong xv6. Cần mã hệ điều hành nền để allocator có nơi hoạt động và có những thành phần thực tế sử dụng nó.

| Vị trí trong BTL | Vai trò hiện tại |
|---|---|
| kernel/ | Mã nhân xv6 gốc; sau này cũng là nơi tích hợp phần allocator của đề tài |
| user/ | Các chương trình chạy trên xv6, như shell và các lệnh; có thể thêm chương trình kiểm thử của đề tài sau |
| mkfs/ | Công cụ tạo ảnh hệ thống file cho xv6 |
| Makefile | Quy tắc biên dịch và chạy xv6 |
| docs/ | Tài liệu thiết kế, nghiên cứu và bài học của dự án |
| agent/ | Ngữ cảnh, quyết định, lịch sử và điểm tiếp tục của dự án; không phải mã allocator |

Hiện kernel/kalloc.c là allocator trang gốc: mỗi lần kalloc lấy một trang 4096 byte từ danh sách trang trống chung. Đây là mã tham chiếu để hiểu cơ chế hiện tại. Chưa có implementation slab hoặc per-CPU free lists của đề tài trong BTL.

Khi học đến phần triển khai, người dùng sẽ tự sửa allocator trang để có danh sách theo CPU, tự viết allocator object nhỏ và tích hợp vào các thành phần kernel cần dùng. Các file mới như kernel/kmalloc.c đang là thiết kế dự kiến, chưa tồn tại như chức năng đã hoàn thành.

“Tự viết toàn bộ C của đề tài” là tự viết các thay đổi và chức năng cần cho đề tài; mã nền xv6 được giữ làm hệ điều hành để phát triển trên đó. Thông tin nhập source nằm trong [Nguồn xv6](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/Nguon-xv6.md>).

## 1. Hàm là một đơn vị công việc

Một hàm có tên, có thể nhận dữ liệu qua tham số, thực hiện công việc và có thể trả kết quả. Khi thiết kế hàm, cần trả lời: việc gì, cần thông tin gì, trả lại gì và có thay đổi dữ liệu nào không.

Hàm kalloc của xv6 có nhiệm vụ lấy một trang trống. Không cần người gọi truyền kích thước vì allocator này cấp một trang cố định. Kết quả là địa chỉ trang, hoặc 0 nếu không lấy được trang.

## 2. Khai báo: cho compiler biết cách gọi

Mở [defs.h, dòng 60](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/kernel/defs.h:60>). Đây là một khai báo đã có trong source tham chiếu:

```c
void* kalloc(void);
```

| Phần | Ý nghĩa |
|---|---|
| void* | Kiểu kết quả: con trỏ tổng quát; phần con trỏ sẽ học riêng |
| kalloc | Tên hàm |
| (void) | Không nhận tham số |
| ; | Kết thúc khai báo, chưa có phần thân |

Hai chữ void có vai trò khác nhau: void đi cùng dấu * trong kiểu trả về tạo kiểu pointer; void trong danh sách tham số thể hiện không có tham số.

Khai báo giúp compiler kiểm tra cách dùng hàm. Nó không chứa thao tác lấy trang và không tự tạo một hàm có thể chạy.

## 3. Định nghĩa: chứa phần thực hiện

Mở [kalloc.c, dòng 69](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/kernel/kalloc.c:69>). Định nghĩa có phần thân trong dấu ngoặc nhọn.

Trong lượt đầu chỉ nhận diện tên, kiểu trả về, danh sách tham số và ranh giới phần thân. Chưa cần hiểu mọi dòng xử lý bên trong.

Khai báo và định nghĩa của cùng một hàm phải tương thích về kiểu. Khi tự viết hàm, bạn sẽ thiết kế hợp đồng trước rồi tự viết phần thực hiện phù hợp.

## 4. Header và include

File .h thường công bố khai báo, kiểu dữ liệu hoặc macro cần dùng giữa các file. File .c thường chứa phần thực hiện và dữ liệu nội bộ của module.

Khi một file include defs.h, bước tiền xử lý đưa nội dung header vào đơn vị source đang biên dịch. Nhờ đó compiler thấy khai báo kalloc khi kiểm tra lời gọi.

Include không chạy hàm, không tự ghép binary và không thay thế việc biên dịch file chứa định nghĩa. Không include file .c để nối các module.

## 5. Một lời gọi thực tế

Mở [vm.c, dòng 228](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/kernel/vm.c:228>), tìm lời gọi kalloc và biến mem nhận kết quả. Đọc phần đầu vm.c để thấy file đã include defs.h.

Chỉ cần nhận diện: vm.c yêu cầu công việc, kalloc.c cung cấp phần thực hiện, defs.h cho compiler biết giao diện của hàm. Chưa học toàn bộ subsystem bộ nhớ ảo trong lượt này.

## 6. Compiler, object file và linker

```text
vm.c + các header      → compiler → vm.o
kalloc.c + các header  → compiler → kalloc.o
                                    ↓
                          linker ghép vào kernel
```

Object file chứa mã máy và thông tin symbol/reference cần cho liên kết. Khi một file gọi hàm được định nghĩa ở file khác, linker giải quyết reference tới định nghĩa tương ứng trong các object file được liên kết.

Khai báo có nhưng không đưa object chứa định nghĩa vào bước link có thể gây lỗi undefined reference. Chỉ có định nghĩa trong một file chưa đủ để mọi file khác biết cách gọi nó ở bước compile.

Mở [Makefile, dòng 10](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/Makefile:10>) để nhận diện kalloc.o, rồi [dòng 93](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/Makefile:93>) để thấy target kernel và lệnh linker. Make điều phối các bước; compiler và linker thực hiện những công việc khác nhau.

## 7. Điều cần tự giải thích ở lượt đầu

Sau mục 1–3, trả lời bằng lời của bạn:

1. Khai báo kalloc trong defs.h đã đủ để thực hiện cấp phát trang chưa?
2. File nào chứa công việc thực tế của kalloc?
3. Hai chữ void trong khai báo đang diễn tả điều gì?

Assistant sẽ đọc câu trả lời và giải thích chỗ còn vướng. Sau đó mới tiếp tục include và compile/link, rồi đến bài tự viết khai báo. Tài liệu này không chứa lời giải cho hàm mới của đề tài.
