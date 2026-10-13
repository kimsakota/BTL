# Nguồn tích hợp project-memory

Ngày kiểm tra và tích hợp: 13/10/2026.

- Repository: [tasuku-9/project-memory-skill](https://github.com/tasuku-9/project-memory-skill).
- Branch đã kiểm tra: `master`.
- Commit pin: `de8a47d555105eccfafa1e2f050f43c8de2d48ce`.
- Cấu hình tham chiếu: [profiles/light.txt](https://github.com/tasuku-9/project-memory-skill/blob/de8a47d555105eccfafa1e2f050f43c8de2d48ce/profiles/light.txt).
- Giấy phép: MIT, Copyright (c) 2026 tasuku-9. Bản LICENSE nguyên gốc được giữ ở `agent/LICENSE`.

## Lựa chọn cho BTL

Chọn theo mức phù hợp với dự án hiện tại: Markdown lưu tại repo, có profile nhỏ, phân biệt trạng thái với lịch sử/giả thuyết, có nội dung về Codex CLI và giấy phép rõ. Đây là đánh giá của assistant cho BTL, không phải xếp hạng “được khuyên dùng nhất” toàn GitHub.

Đã đối chiếu thêm các hướng memory-bank: [rtoma/agent-markdown-memory-bank-protocol](https://github.com/rtoma/agent-markdown-memory-bank-protocol), [dapi/memory-bank](https://github.com/dapi/memory-bank) và [vanzan01/cursor-memory-bank](https://github.com/vanzan01/cursor-memory-bank). Phạm vi mong muốn ở đây là bộ nhớ tài liệu nhỏ cho C/xv6, nên dùng profile Light thay cho bộ workflow nhiều tầng hoặc cấu hình dành riêng Cursor.

## Các phần dùng và thay đổi

Dùng vai trò và cấu trúc tài liệu Light: README, CURRENT_STATE, LOGBOOK, RECOVERY_NOTES, DOCS_GUIDE và CONTEXT_MANIFEST. Đã đọc mẫu upstream, điều chỉnh nội dung sang tiếng Việt và điền dữ liệu dự án. Nội dung khởi tạo chưa phải template trống.

Các bổ sung local:

- `AGENTS.md` ở root để dẫn agent vào bộ nhớ và nêu quy tắc C/xv6.
- `history/` để lưu snapshot user/assistant từ hai chat liên quan; JSONL bỏ qua bởi Git.
- Quy tắc coi mã và bằng chứng mới nhất là căn cứ thực tế khi ghi nhớ cũ mâu thuẫn.
- Tiến độ học lý thuyết, baseline xv6 đã kiểm tra, giới hạn build/benchmark và các link tài liệu BTL.
- Capture theo kết quả hoặc thay đổi chủ đề có ý nghĩa; chỉ cập nhật file cần thiết.

Chưa copy hoặc chạy script khởi tạo/audit/handoff upstream. Chưa cài skill toàn cục, MCP hoặc hooks. Vì vậy đây là tích hợp phần tài liệu đã điều chỉnh, không phải cài toàn bộ package/runtime của repo.

## Kiểm tra nguồn tải về

Các mẫu gốc đã được tải từ raw GitHub theo commit pin và hash bằng SHA-256 trước khi điều chỉnh. Hash dưới đây là hash của mẫu gốc; không phải hash của file local đã tùy biến.

| File nguồn | SHA-256 |
|---|---|
| `LICENSE` | `93ee071d3644ce5303a0f118d15b35cc4c560d775fdbc061735000bc3ebe2b06` |
| `templates/README.md` | `12bf9c7df5bef6f74ab4455fd8f0b580c38e8c7e826510e228b3c165d1981ef6` |
| `templates/light/CURRENT_STATE.md` | `1d8055c8d6d553808c1011b7fceba51d5742c3b5b732554a986bcbcc7a2fe730` |
| `templates/RECOVERY_NOTES.md` | `b2472b588fbcb80f095b46060580bddb6e726057af5bf425e1abc8407ffbb4bb` |
| `templates/light/LOGBOOK.md` | `ebf67405e0a456122849ec691c72fa7e58642481f16c20f49d6c108069f11665` |
| `templates/light/DOCS_GUIDE.md` | `00a9b6828ea3559df7592751e97137af52142aa4e6f338351ab107b8abdb0812` |
| `templates/CONTEXT_MANIFEST.md` | `59c18bfd04ed4d2b29ded2e6672f844c94a57f3e17ba73d5a0edb4669067be62` |
| `templates/.contextignore` | `44ee88dbdeecd0c304b58284c3b5fe976313ef878f19a9aaafd898b1bacb16b4` |

## Cơ chế Codex

`AGENTS.md` là điểm nối chỉ dẫn ở root; nó yêu cầu đọc các file memory theo nhu cầu. Tài liệu OpenAI mô tả việc Codex CLI khám phá và nạp AGENTS.md theo thư mục. Không suy ra mọi file trong agent/ tự động được nạp hoặc cập nhật: agent vẫn phải thực hiện quy tắc đọc/ghi. [Codex Prompting Guide, Using agents.md](https://developers.openai.com/cookbook/examples/gpt-5/codex_prompting_guide).

Để giữ lượng ngữ cảnh vừa đủ, router ngắn chỉ dẫn tới tài liệu cần dùng cho nhiệm vụ; không yêu cầu đọc toàn bộ archive trước mỗi sửa đổi. [Hướng dẫn OpenAI về AGENTS.md](https://developers.openai.com/blog/rethinking-skills-and-prompts-for-gpt-6-astra).
