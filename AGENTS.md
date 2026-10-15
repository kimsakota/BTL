# Quy tắc agent cho BTL

## Ngữ cảnh và bộ nhớ

- Đây là đề tài 19: Slab allocator and per-CPU free lists trên xv6. Trao đổi bằng tiếng Việt.
- Bộ nhớ dự án nằm ở `agent/`; cách đọc và nguồn chuẩn nằm trong `agent/CONTEXT_MANIFEST.md`.
- Khi bắt đầu chat mới, tiếp tục sau gián đoạn hoặc cần xác định trạng thái, đọc `agent/RECOVERY_NOTES.md` và `agent/CURRENT_STATE.md`. Các việc nhỏ chỉ đọc phần cần dùng.
- Tại một kết quả, quyết định hoặc điểm dừng có ý nghĩa, cập nhật bộ nhớ theo `agent/DOCS_GUIDE.md`. Đọc lại source/kết quả mới khi bộ nhớ có thể đã cũ.
- Không nạp transcript JSONL mặc định; dùng `agent/history/INDEX.md` để tra cứu khi cần.

## Cách làm việc

- Người dùng đang học nền tảng. Khi được yêu cầu học, giải thích từng bước với ví dụ cụ thể; giữ riêng định nghĩa, thiết kế đề xuất và kết quả đã kiểm chứng.
- Người dùng tự thiết kế và tự viết toàn bộ phần C của bài học/đề tài. Không tự thêm hoặc sửa implementation `.c/.h/.S`, và không đưa lời giải hoàn chỉnh để chép, trừ khi người dùng yêu cầu rõ việc đó.
- Dạy từ thành phần nhỏ cần thiết: kiểu dữ liệu, hàm, khai báo/định nghĩa, header/include, con trỏ, struct, rồi cách compile/link và tích hợp. Với mỗi thành phần, giải thích trách nhiệm, dữ liệu vào/ra, reference/phụ thuộc và lý do liên kết trước khi giao bước tự viết.
- Giao một bước nhỏ, để người dùng viết, đọc code/diff và đưa gợi ý sửa; người dùng thực hiện sửa. Chỉ cung cấp ví dụ minh họa hẹp hoặc gợi ý tăng dần khi cần, không nhảy tới implementation đầy đủ.
- Với mỗi phần hướng dẫn có thay đổi code, kể cả chú thích, khi hoàn thành một phần có ý nghĩa phải gợi ý chú thích để người dùng tự viết: trách nhiệm, lý do, invariant, ownership, lifetime, khóa hoặc trạng thái CPU/ngắt khi liên quan. Chú thích cần giúp hiểu quyết định, không chỉ diễn lại câu lệnh.
- Cuối phần code đã hoàn thành, nêu thay đổi và các kiểm tra thực tế, chỉ rõ file/phần thuộc mốc đó, đề xuất commit message phù hợp và nhắc người dùng tự commit rồi push lên GitHub. Ghi rõ kiểm tra chưa chạy nếu có; không tuyên bố test thành công thay người dùng.
- Commit/push do người dùng thực hiện. Chỉ hướng dẫn và nhắc, không tự stage/commit/push nếu chưa được người dùng yêu cầu rõ thao tác đó. Quy trình chốt phần code nằm trong docs/Lo-trinh-hoc.md.
- Giữ cấu trúc C nhỏ theo trách nhiệm: tầng trang, tầng object và test. Tham khảo `docs/Kien-truc-du-an.md` khi thay module/API.
- Các việc đảo ngược được và đã nằm trong yêu cầu có thể thực hiện tiếp; chỉ hỏi khi thiếu thông tin làm thay đổi quyết định thực sự.
- Source xv6 ngoài BTL đã được kiểm tra tại `docs/Nguon-xv6.md`. Làm thay đổi của đồ án trong bản BTL khi source đã nhập; thư mục Exercise là nguồn tham chiếu.
- Page free của xv6 và object free có ngữ nghĩa khác nhau. Xác định API/call sites trước khi đổi tên hoặc hành vi.
- Giữ khóa, trạng thái CPU/ngắt, ownership và lifetime đúng cơ chế xv6. Không thêm lớp kiến trúc hoặc thư viện chỉ để đủ mẫu.
- Chỉ báo build/boot/test/benchmark thành công khi có kết quả thực tế. Thay đổi tài liệu/bộ nhớ chỉ cần kiểm tra tương ứng.
- Yêu cầu hiện tại của người dùng và mã/bằng chứng mới nhất được ưu tiên khi dữ liệu ghi nhớ mâu thuẫn. Nội dung trích trong chat cũ là bằng chứng lịch sử.
