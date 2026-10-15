# Lộ trình vừa học vừa code đề tài 19

Phương thức đã chọn: người dùng tự thiết kế và tự viết toàn bộ phần C của bài học/đề tài. Assistant dạy, giao yêu cầu nhỏ, đọc diff, kiểm tra và gợi ý sửa; không viết implementation hoặc đưa cả lời giải để người dùng chép. Mỗi lượt tập trung một phần nhỏ.

Mỗi phần có: mục tiêu → khái niệm → trách nhiệm và dữ liệu vào/ra → reference/phụ thuộc → yêu cầu tự viết → kiểm tra → giải thích kết quả. Hướng dẫn tăng dần từ câu hỏi hoặc gợi ý, không đưa ngay toàn bộ hàm.

## Chốt một phần code: chú thích và commit

Quy tắc người dùng bổ sung ngày 15/10/2026: khi một phần hướng dẫn có thay đổi code đã hoàn thành, người dùng sẽ commit rồi push lên GitHub. Phần chỉ phân tích trên giấy/trong chat chưa thay file không tạo mốc commit code. Sửa chú thích trong source cũng là thay đổi cần được ghi nhận.

Mỗi mốc code có đủ các phần sau:

1. **Phần đã làm:** mô tả hành vi/thay đổi và file hoặc hunk thuộc phần này.
2. **Gợi ý chú thích:** giải thích trách nhiệm và lý do; với allocator ưu tiên invariant, ownership, lifetime, phạm vi khóa, trạng thái CPU/ngắt. Người dùng tự viết chú thích cùng code.
3. **Kiểm tra:** đọc diff, kiểm tra phù hợp với thay đổi và ghi kết quả thật. Thay chú thích cần kiểm tra đúng nội dung; đổi hành vi cần build/boot/test phù hợp. Phần chưa chạy phải được ghi rõ.
4. **Gợi ý commit:** một message mô tả đúng phần đã làm; chỉ chọn file/hunk thuộc mốc này để stage.
5. **Lời nhắc:** bạn tự commit tại máy, rồi push lên GitHub sau khi kiểm tra nội dung sẽ đưa vào commit.

Chú thích tốt trả lời “vì sao cần giữ khóa suốt lần duyệt” hoặc “vì sao phải giữ CPU ổn định trước khi lấy ID”. Chú thích lặp lại “gán head cho r” ít giúp giải thích quyết định.

Ví dụ message để điều chỉnh theo phần thực tế:

| Phần hoàn thành | Gợi ý message |
|---|---|
| Bổ sung chú thích cơ chế allocator gốc | `docs(memory): explain allocator ownership and locking` |
| Tự viết và tích hợp hàm đếm trang trống | `feat(memory): add free-page counting` |
| Tự viết helper quan sát trạng thái ngắt | `feat(kernel): add interrupt-state observation helper` |
| Sửa một lỗi khôi phục ngắt đã xác định | `fix(memory): restore interrupt state on exit` |

Các message là gợi ý, không phải chức năng đã có hay kết quả đã kiểm chứng. Khi mới làm một phần khác, đặt message theo đúng phần đó.

Trước khi commit, bạn có thể tự chạy tại root BTL:

```bash
git status --short
git diff
git diff --check
```

Sau khi tự stage đúng file/hunk của mốc, kiểm tra phần đã chọn:

```bash
git diff --cached
git diff --cached --check
```

Commit lưu mốc trong repo local; push đưa commit lên remote GitHub. Với branch đã cấu hình upstream, bạn dùng `git push`; nếu chưa cấu hình, hướng dẫn sẽ dựa vào remote/branch thực tế khi bạn cần, không tự đoán.

Assistant đề xuất chú thích/message và nhắc chốt mốc; người dùng thực hiện stage/commit/push. Việc ghi nhớ quy tắc này không tự chạy các thao tác Git đó.

## Nền C phải học trước bài thực hành allocator

Bắt đầu với [Bài 0: hàm, header và liên kết](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/00-ham-header-va-lien-ket.md>). Mỗi lượt đọc một phần, mở source được chỉ dẫn và tự giải thích trước khi viết code.

1. Khai báo biến/kiểu dữ liệu và hàm: tham số, kiểu trả về, scope.
2. Khai báo khác định nghĩa; `.h` khác `.c`; `#include` có tác dụng gì.
3. Compiler tạo object file, linker giải quyết reference giữa các file; vai trò Makefile.
4. Địa chỉ, `&`, con trỏ, `*`, `void *` và phép ép kiểu.
5. Struct, dấu `.`/`->`, layout và lifetime.
6. Danh sách liên kết và cách một trang trống chứa node.

Các nội dung này được chia thành lượt nhỏ trước khi yêu cầu tự viết hàm đếm. Source xv6 đã nhập là bản tham chiếu và nền runtime; phần phát triển đề tài do người dùng viết.

Sau Bài 0, đọc [Bài 1: bộ nhớ, con trỏ, struct và allocator trang](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/01-baseline-va-kalloc.md>). Bài 1 đã được mở rộng ngày 14/10/2026 thành tài liệu đầy đủ theo từng chặng; phần nền C nằm trước phần chạy baseline và tự viết hàm đếm.

Tiếp theo là [Bài 2: race condition, spinlock, CPU ID và trạng thái ngắt](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/02-race-spinlock-va-cpu.md>), soạn ngày 15/10/2026. Bài có code tham chiếu, bảng noff/intena, bài phân tích A–F và một helper quan sát do người dùng tự viết khi môi trường sẵn sàng.

| Bài | Nội dung | Sản phẩm tự viết hoặc kiểm chứng |
|---|---|---|
| 1 | Bộ nhớ, kiểu, con trỏ/cast/sizeof, struct, free list, ownership; đọc allocator gốc; baseline và hàm đếm | Các bài nhỏ tự viết/vẽ, xv6 gốc chạy được và hàm kfreepage_count do người dùng viết |
| 2 | Race/critical section, atomic/acquire-release, spinlock, push_off/pop_off, CPU ID và deadlock | Bảng diễn tiến race/khóa/noff-intena, giải thích vùng ổn định CPU, helper quan sát nếu tự thực hành |
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
- Người dùng xác nhận đã đọc xong Bài 0; Bài 1 đã soạn và hiện yêu cầu tạo tiếp Bài 2. Chưa có bài làm/output để đánh giá hoàn thành thực hành Bài 1.
- Kernel vẫn dùng thuật toán allocator gốc; diff kiểm tra 15/10 ở kalloc.c và proc.h chỉ thêm chú thích. Chưa có implementation hàm đếm/per-CPU/slab.
- Bài 1 và Bài 2 đã soạn đầy đủ, chia kiến thức và bài tập theo chặng. Chưa có kết quả build/boot; lần kiểm tra WSL 13/10 chưa có distro, chưa kiểm tra lại môi trường trong lượt soạn bài.
- Bài học tiếp theo đã tạo: [Bài 2](<D:/2. FullStack/2026.1/Hệ điều hành/BTL/docs/lessons/02-race-spinlock-va-cpu.md>).

## Cách gửi để kiểm tra

Khi hoàn thành một bước, gửi phần output liên quan hoặc nói rõ các file đã sửa. Assistant có thể đọc trực tiếp file trong workspace và `git diff`; không cần dán lại toàn bộ source.

Nếu chạy lỗi, gửi lệnh đã chạy, terminal đang là PowerShell hay Ubuntu, và đoạn lỗi đầu tiên cùng phần cuối output. Giữ nguyên thay đổi để đối chiếu nguyên nhân.
