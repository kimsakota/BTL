# VstHelper — mã nguồn tham khảo cho BTL 26118

Nguồn: https://github.com/kimsakota/ShiftSort.git

Đây là bản sao để cả nhóm đọc và đối chiếu ý tưởng C# về memory pool, buffer và dữ liệu. Bản sao không được tích hợp vào build kernel xv6.

- Commit nền của repo nguồn: `7236e72f83c2b780bf37fa42c9cf601d3d6db35c`.
- Bản sao lấy từ **working tree của Linh**, có cả thay đổi chưa commit và file mới không bị ignore; không đồng nhất với nội dung commit nền hoặc repo GitHub ở remote.
- Danh mục file, kích thước, SHA-256 của tệp nguồn lúc sao chép và trạng thái nguồn nằm ở [SNAPSHOT.json](SNAPSHOT.json). Checksum được ghi trước khi Git có thể chuẩn hóa xuống dòng theo .gitattributes.
- Giữ nguyên byte của file được chọn. Không kèm .git, .vs, bin, obj, thiết lập người dùng hoặc file phụ trợ build TeX. Tài liệu/biểu đồ trong thư mục là tài liệu nguồn tham khảo, không tự là kết quả đo của xv6 BTL.
- Repo gốc tại thư mục VstHelper ở root workspace được giữ nguyên để Linh tiếp tục làm riêng. Bản sao này là snapshot; thay đổi ở repo gốc không tự đồng bộ vào đây.

## Bắt đầu đọc

- [Memory](Memory/): memory pool, buffer, trang và slot trong bản nguồn C#.
- [Lists](Lists/): cấu trúc danh sách.
- [Tài liệu nghiên cứu](docs/).
- [Project C#](VstHelper.csproj).

Để chia sẻ bản cập nhật: chuẩn bị snapshot mới, kiểm tra nguồn/thay đổi, cập nhật manifest, commit trên nhánh tham khảo và mở PR. Tự kiểm tra các nhận định/tài liệu của repo nguồn trước khi áp dụng vào xv6.
