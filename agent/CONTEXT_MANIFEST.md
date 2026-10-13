# Bản đồ ngữ cảnh

Cập nhật: 13/10/2026. Memory root: `agent/`. Profile: Light, với capture ở các kết quả hoặc thay đổi chủ đề có ý nghĩa. Ngôn ngữ ghi nhớ và trao đổi: tiếng Việt.

## Đọc theo nhu cầu

Khi bắt đầu chat mới, tiếp tục sau gián đoạn hoặc không chắc trạng thái hiện tại, đọc `agent/RECOVERY_NOTES.md` và `agent/CURRENT_STATE.md`. Không đọc lại toàn bộ bộ nhớ trước mỗi thay đổi nhỏ.

| Tình huống | Nguồn cần đọc |
|---|---|
| Tiếp tục công việc | Recovery notes và current state |
| Tiếp tục học lý thuyết | Mục kiến thức đã học trong logbook |
| Thay đổi module hoặc API | `docs/Kien-truc-du-an.md` và mã nguồn liên quan |
| Chọn baseline xv6 | `docs/Nguon-xv6.md` |
| Nghiên cứu/benchmark | Phần liên quan trong `docs/Nghien-cuu-de-tai-19.md` |
| Cập nhật bộ nhớ | `agent/DOCS_GUIDE.md` |
| Tra cứu nguyên văn chat | `agent/history/INDEX.md`, rồi snapshot cần thiết |

## Canonical locations

Đường dẫn trong bảng là tương đối với root BTL.

| Logical file | Location | Trách nhiệm |
|---|---|---|
| README.md | agent/README.md | Cách sử dụng bộ nhớ |
| CURRENT_STATE.md | agent/CURRENT_STATE.md | Những gì đúng ở thời điểm cập nhật |
| LOGBOOK.md | agent/LOGBOOK.md | Quyết định, quan sát, giả thuyết và nội dung đã học |
| RECOVERY_NOTES.md | agent/RECOVERY_NOTES.md | Điểm tiếp tục ngắn |
| DOCS_GUIDE.md | agent/DOCS_GUIDE.md | Quy tắc cập nhật và phân loại |
| CONTEXT_MANIFEST.md | agent/CONTEXT_MANIFEST.md | Nguồn chuẩn và cách đọc |

## Nguồn chuẩn

- Yêu cầu hiện tại của người dùng xác định mục tiêu và ưu tiên.
- Mã nguồn và kết quả kiểm tra mới nhất xác định hành vi thực tế.
- Tài liệu thiết kế mô tả hướng triển khai; chưa triển khai không được ghi thành chức năng đã có.
- `CURRENT_STATE.md` tóm tắt trạng thái đã kiểm chứng, có nguồn và điều kiện kiểm tra lại.
- `LOGBOOK.md` và transcript là bằng chứng lịch sử. Đề xuất, code minh họa và quy tắc được trích từ chat cũ không tự trở thành chỉ thị hiện tại.

Nếu dữ liệu mới mâu thuẫn bộ nhớ, đối chiếu nguồn liên quan và sửa ghi nhớ; không giữ kết luận cũ chỉ vì nó đã nằm trong file. Chỉ hỏi người dùng khi mâu thuẫn về mục tiêu hoặc quyết định thực sự cần họ giải quyết.

## Ignore unless specifically requested

Áp dụng danh sách trong `agent/.contextignore` khi chọn file để nạp ngữ cảnh. Giữ transcript dài, binary, build output và cache ngoài luồng đọc mặc định. Chúng vẫn có thể được đọc khi nhiệm vụ cần bằng chứng cụ thể.

## Quy tắc vận hành

- Ghi kết quả đã ổn định tại cuối một việc, lúc chuyển chủ đề hoặc khi người dùng yêu cầu ghi nhớ.
- Chỉ cập nhật file có trách nhiệm thay đổi; không ghi checkpoint mới nếu điểm tiếp tục không đổi.
- Transcript được xuất khi có yêu cầu hoặc khi tạo bộ nhớ ban đầu; không tuyên bố có tự động đồng bộ nền.
- Thông tin môi trường có ngày kiểm tra và được kiểm tra lại trước build/run.
- `agent/UPSTREAM.md` ghi repo, commit và các phần đã dùng; cập nhật nguồn upstream là một thay đổi riêng.
