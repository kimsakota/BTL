# Nguyễn Kim Linh - Đóng góp W01

- MSSV: 20233495.
- Tài khoản GitHub: [kimsakota](https://github.com/kimsakota).
- Ngày cập nhật nội dung: 23/10/2026.
- Kỳ: W01, tuần khởi động 19–25/10/2026; nội dung ghi nhận đến ngày 23/10/2026.
- Trạng thái: đang bổ sung hồ sơ đóng góp và minh chứng trong [PR #8](https://github.com/kimsakota/BTL/pull/8), chưa chốt báo cáo tuần của cả nhóm.

## Việc đã làm và kết quả

Trong tuần khởi động, tôi tập trung tổ chức dự án và thực hành quy trình cộng tác bằng GitHub. Các việc đã ghi nhận:

| Công việc | Đã làm gì, kết quả cụ thể | Minh chứng/đầu ra |
|---|---|---|
| Chuẩn bị repo và cách cộng tác | Tổ chức repo chung, thêm Minh và Khánh, chuẩn bị hướng dẫn dùng nhánh/PR và chia sẻ tài liệu học | [Repo BTL](https://github.com/kimsakota/BTL), [sổ tay nhóm](../../../So-tay-GitHub-nhom.md), [PR #2](https://github.com/kimsakota/BTL/pull/2) |
| Tổ chức hồ sơ báo cáo theo tuần | Đưa cấu trúc W01 lên main, có phần riêng cho từng thành viên và minh chứng chung | [Mục lục W01](../README.md), [PR #4](https://github.com/kimsakota/BTL/pull/4) |
| Thực hành GitHub Desktop và PR | Cập nhật main, xem lịch sử/diff; tạo nhánh, commit/push ghi chú onboarding và mở PR vào main | Ảnh 1, ảnh 2 và [PR #8](https://github.com/kimsakota/BTL/pull/8) |
| Ghi lại phần học trước | Ghi nhận đã đọc Lesson 0–3 và các file trong kernel; dùng làm đầu vào học tập, chưa nhận là đã triển khai slab/per-CPU | [Ghi chú onboarding của Linh](../../../onboarding/linh.md) |

## Ảnh và log minh chứng

### Ảnh 1 - Cập nhật main và xem lịch sử thay đổi

Ảnh chụp ngày 23/10/2026 hiển thị repo BTL trên GitHub Desktop, nhánh main và tab History tại commit `94f56a2` của PR #6. Tôi đã tải được thay đổi chung và xem diff ghi chú onboarding của Minh.

![GitHub Desktop hiển thị main, lịch sử và diff ghi chú onboarding của Minh](01-linh-chung.png)

Nội dung build/test xuất hiện trong diff là phần ghi chú của Minh; ảnh này minh chứng thao tác GitHub của Linh, không thay cho log chạy kiểm thử trên máy Linh.

### Ảnh 2 - Mở PR đóng góp của Linh

Ảnh chụp ngày 23/10/2026 hiển thị [PR #8 - Create onboarding](https://github.com/kimsakota/BTL/pull/8), từ nhánh `codex/linh-onboarding` vào main. Tại thời điểm chụp, tab Files changed có ghi chú onboarding của tôi và ảnh minh chứng thứ nhất. Ảnh này ghi lại bước đưa đóng góp lên GitHub để nhóm review.

![PR số 8 từ nhánh codex/linh-onboarding vào main và các file đóng góp của Linh](02-linh-chung.png)

Hai ảnh ghi lại trạng thái tại thời điểm chụp. Phạm vi hồ sơ này là tổ chức dự án/GitHub; chưa đính kèm log build, boot hoặc benchmark do Linh chạy trong kỳ này. Minh chứng chung của nhóm xem [tại đây](../chung/README.md).

## Việc còn lại để chốt W01

- Bổ sung README này và ảnh thứ hai vào cùng PR #8; nhờ một thành viên review, kiểm tra CI rồi merge khi đạt.
- Tổng hợp đóng góp và bằng chứng thực tế của cả ba người; thống nhất ngày chốt, TODO và bản báo cáo cuối trước khi nộp.

## Trở ngại và TODO kỳ sau

- Phần GitHub vẫn đang thực hành; cần kiểm tra đúng nhánh và phân biệt cập nhật main với cập nhật nhánh công việc.
- TODO đề xuất, cần nhóm xác nhận: bổ sung log môi trường/baseline trên máy Linh và thống nhất một bước nhỏ tiếp theo của tầng trang, với đầu ra và cách kiểm tra rõ ràng.

[Về mục lục W01](../README.md).
