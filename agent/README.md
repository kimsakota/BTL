# Bộ nhớ dự án BTL

Thư mục này lưu ngữ cảnh để tiếp tục đề tài 19 qua nhiều cuộc chat: trạng thái hiện tại, quyết định, kiến thức đã trao đổi, quy tắc ghi chép và điểm tiếp tục.

Tích hợp từ cấu hình Light của [tasuku-9/project-memory-skill](https://github.com/tasuku-9/project-memory-skill), commit `de8a47d555105eccfafa1e2f050f43c8de2d48ce`, giấy phép MIT. Đã điều chỉnh sang tiếng Việt, điền ngữ cảnh xv6 và bổ sung snapshot chat. Nguồn và phạm vi thay đổi nằm trong `UPSTREAM.md`; giấy phép gốc nằm trong `LICENSE`.

## Mở đầu từ đâu?

- Tiếp tục công việc: `RECOVERY_NOTES.md`, rồi `CURRENT_STATE.md`.
- Xem lý do, lịch sử và nội dung đã học: `LOGBOOK.md`.
- Biết nguồn nào đáng tin và cần đọc: `CONTEXT_MANIFEST.md`.
- Biết khi nào và ghi vào đâu: `DOCS_GUIDE.md`.
- Đọc chat đã xuất: `history/INDEX.md` và các snapshot JSONL tại máy local.

`AGENTS.md` ở root BTL dẫn agent tới bộ nhớ này. Các file trong `agent/` là dữ liệu ghi nhớ của dự án; không phải một dịch vụ AI hoặc chương trình nền.

## Cách dùng trong chat

Có thể yêu cầu bằng ngôn ngữ tự nhiên:

```text
Tiếp tục dự án BTL. Đọc agent/RECOVERY_NOTES.md và agent/CURRENT_STATE.md rồi làm bước tiếp theo.
```

```text
Ghi lại quyết định vừa chốt và cập nhật trạng thái dự án.
```

```text
Tiếp tục học lý thuyết từ phần cuối trong agent/LOGBOOK.md.
```

```text
Xuất lại lịch sử chat của dự án và cập nhật index snapshot.
```

Agent cập nhật các file cần thiết tại một kết quả, quyết định hoặc điểm dừng có ý nghĩa. Không phải mọi câu trả lời đều làm thay đổi toàn bộ bộ nhớ. Việc cập nhật phụ thuộc agent thực hiện quy tắc; thư mục không tự chạy khi không có chat/agent hoạt động.

## Lịch sử chat và ghi nhớ có vai trò khác nhau

`LOGBOOK.md` là lịch sử được tóm tắt, phân biệt điều người dùng đã chốt, đề xuất và kết quả có bằng chứng. Nó giữ nội dung hữu ích để tiếp tục mà không cần nạp toàn bộ transcript.

`history/*.jsonl` là các bản xuất tin nhắn user và câu trả lời cuối của assistant do công cụ đọc chat trả về. Snapshot không chứa system/developer prompts, reasoning hoặc tool outputs. Nó không tự đồng bộ liên tục. Thời điểm và phạm vi từng snapshot được ghi trong index.

Các snapshot JSONL được lưu local và bỏ qua khi Git theo dõi; ghi nhớ tóm tắt và index có thể theo dõi cùng source. `.contextignore` hướng dẫn agent bỏ qua dữ liệu dài khi đọc ngữ cảnh; nó không phải cơ chế chặn truy cập của Codex hoặc Git.

## Phạm vi tích hợp

Đã tích hợp mô hình tài liệu Light và quy tắc định tuyến ghi nhớ. Chưa cài skill toàn cục, hooks, MCP server hoặc các script runtime của repo upstream. Chưa có Git repository tại root BTL; “có thể theo dõi bằng Git” không có nghĩa các file hiện đã được commit.
