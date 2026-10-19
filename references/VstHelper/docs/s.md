📍 BƯỚC 1: Tầng Cấu trúc Dữ liệu Bộ nhớ Đệm (Buffer & Data Pages)
Nhiệm vụ: Xem cách bộ nhớ vừa xin ở Memory.cs được đóng gói thành các Buffer và Trang chứa dữ liệu Record.

Memory/Buffer.cs
: Xem cách gói con trỏ byte* + Length thành một Struct làm việc an toàn.
Memory/DataSlot.cs
 & 
Memory/DataPage.cs
: Xem cách các bản ghi (Records) thực tế của DB được lưu trữ vào DataPage.
Memory/StringWriter.cs
 & 
Memory/StringReader.cs
: Kỹ thuật đọc/ghi chuỗi siêu tốc không gây rác GC.
📍 BƯỚC 2: Tầng Chỉ mục Key-Value & Thuật toán Sắp xếp (⭐ TRỌNG TÂM DỰ ÁN)
Nhiệm vụ: Đây chính là TRÁI TIM của bài báo khoa học VstHelper.

Keys/KeyEntry.cs
: Xem cấu trúc KeyEntry 32-byte được thiết kế nằm vừa vặn trong 2 đường cache L1 của CPU.
Keys/KeyIndexer.cs
: Xem thuật toán Hybrid Parallel QuickSort (nằm ở trung tâm bài báo: Median-of-three, Threshold 128, sắp xếp 100K-1M records trong sub-10ms).
📍 BƯỚC 3: Tầng Toán học & Thuật toán Hash Siêu Tốc
Nhiệm vụ: Xem các thuật toán băm (Hash) dữ liệu không tốn CPU.

Math.cs
: Đọc thuật toán băm XXHash64 (Hash 64-bit siêu nhanh) và FastMD5 (mã hóa MD5 bằng con trỏ stackalloc).
📍 BƯỚC 4: Tầng Danh sách Unmanaged & Định danh
Nhiệm vụ: Xem các cấu trúc dữ liệu thay thế cho C# chuẩn để né GC.

Lists/RawList.cs
: Xem mảng động Unmanaged thay thế cho List<T> của C# để tránh GC.
ObjectId.cs
: Cơ chế tạo ID duy nhất 12-byte (tương tự MongoDB ObjectId).
📍 BƯỚC 5: Tầng Lưu trữ Đĩa & Đồng bộ Đa luồng (File I/O & Threading)
Nhiệm vụ: Xem cách DB đọc/ghi xuống ổ đĩa và khóa đa luồng.

IO/MappedFile.cs
 & 
IO/File.cs
: Đọc/ghi file bằng cơ chế Memory-Mapped File (mmap) trực tiếp RAM-to-Disk.
Thread/LockManager.cs
: Hệ thống khóa Reader-Writer Lock tối ưu đa luồng.