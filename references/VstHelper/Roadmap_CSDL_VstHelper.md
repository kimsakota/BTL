# 🎓 LỘ TRÌNH KIẾN THỨC CHUYÊN SÂU LÀM CHỦ ENGINE CSDL NHÚNG VSTHELPER

Tài liệu tổng hợp các môn học và chủ đề cốt lõi thuộc Khoa học Máy tính & Kỹ thuật Phần mềm giúp bạn nắm vững trọn vẹn kiến trúc bên trong (Database Internals) của dự án **VstHelper** (từ con trỏ Unmanaged RAM, Slab Allocator, Cache Line 64B, SIMD Intrinsics đến File I/O đĩa).

---

## 🏛️ 1. KIẾN TRÚC MÁY TÍNH & VI XỬ LÝ (COMPUTER ARCHITECTURE & MICROPROCESSORS)

Môn học quan trọng số 1 giúp hiểu cách phần cứng CPU thực thi mã lệnh và tại sao dự án phải căn lề bộ nhớ 32 Bytes.

* **Bộ nhớ đệm CPU (CPU Cache Hierarchy):**
  - Khái niệm L1 Data Cache (32KB), L2 Cache (512KB), L3 Cache (32MB+).
  - Kiến trúc đường **Cache Line (64 Bytes)** và hiện tượng trượt cache (*Cache Misses*).
  - Tối ưu căn lề bộ nhớ 32 Bytes (`KeyEntry`) để nạp vừa tròn 2 bản ghi / 1 Cache Line.
* **Đường ống CPU & Dự đoán rẽ nhánh (CPU Pipeline & Branch Prediction):**
  - Khái niệm CPU Pipeline (Fetch, Decode, Execute, Writeback).
  - Hiện tượng đoán sai rẽ nhánh (*Branch Misprediction Stall*) tốn 15-20 chu kỳ CPU.
  - Thuật toán so sánh không rẽ nhánh (**Branchless Comparison**).
* **Tập lệnh Mã máy & SIMD (x86-64 Instruction Set & SIMD Intrinsics):**
  - Các thanh ghi phần cứng (`RAX`, `RBX`, `RCX`...) và lệnh di chuyển khối RAM (`MOVSX`, `REP MOVSB` / ERMSB).
  - Lệnh lập trình phần cứng đếm bit 0: **`TZCNT`** (`BitOperations.TrailingZeroCount`).
  - Tập lệnh tính toán song song SIMD: **SSE2, AVX2, AVX-512** (`Vector128`, `Vector256`).

---

## 🖥️ 2. HỆ ĐIỀU HÀNH (OPERATING SYSTEMS - OS)

Môn học giúp hiểu cách CSDL giao tiếp với RAM và Ổ đĩa SSD ở tầng Kernel.

* **Bộ nhớ Ảo & Memory Mapping (Virtual Memory & mmap):**
  - Cơ chế phân trang bộ nhớ (Paging, Page Tables, TLB).
  - Kỹ thuật **Memory-Mapped Files (`mmap` / Win32 `CreateFileMapping` / `MapViewOfFile`)** – Đọc/ghi đĩa SSD với tốc độ RAM.
* **Quản lý Tiến trình & Đa luồng (Processes, Threads & Concurrency):**
  - Đa luồng song song (`Parallel.Invoke` / Task Parallel Library).
  - Phép toán nguyên tử **`Interlocked` (Atomic Operations)** cho Lock-free allocator.
  - Khái niệm SpinLock, Mutex, Semaphore và Cấu trúc dữ liệu không dùng khóa (*Lock-free Data Structures*).
* **Quản lý Đĩa & File I/O (File Systems & Page Cache):**
  - Đọc/ghi đĩa trực tiếp, cờ `FILE_FLAG_WRITE_THROUGH`, cờ `PAGE_READWRITE`.
  - Cơ chế Page Cache của Hệ điều hành.

---

## 🧠 3. CẤU TRÚC DỮ LIỆU & THUẬT TOÁN NÂNG CAO (ADVANCED DATA STRUCTURES & ALGORITHMS)

Môn học giúp hiểu cách động cơ `KeyIndexer` sắp xếp và tìm kiếm dữ liệu lớn.

* **Thuật toán Sắp xếp Hybrid & Tối ưu Cache (Cache-Conscious Sorting):**
  - Thuật toán QuickSort, InsertionSort.
  - Kỹ thuật chọn chốt Trung vị 3 điểm (**Median-of-three**).
  - Sơ đồ phân đoạn Hoare Partition Scheme.
  - Thuật toán sắp xếp lai 3 tầng: `ParallelSort` $\to$ `Iterative QuickSort` $\to$ `ShiftSort`.
* **Toán tử Bitwise & Số học Nhị phân (Bitwise Manipulation):**
  - Thuật toán làm tròn lên lũy thừa của 2 (`RoundUpToPowerOf2`).
  - Kỹ thuật Bit-Smearing (Lấp đầy bit 1).
  - Bitmask và các phép toán `AND`, `OR`, `XOR`, `NOT`, Dịch bit (`<<`, `>>`).
* **Cấu trúc Chỉ mục CSDL (Database Indexing Structures):**
  - Tìm kiếm nhị phân $O(\log N)$ trên mảng Unmanaged.
  - Các mô hình chỉ mục: B-Tree, B+Tree, LSM-Tree, In-Memory Key-Value Index.

---

## ⚡ 4. LẬP TRÌNH HỆ THỐNG & QUẢN LÝ BỘ NHỚ (SYSTEM PROGRAMMING & MEMORY MANAGEMENT)

Môn học giúp làm chủ các kỹ thuật Unmanaged Pointer trong C/C++/C#.

* **Con trỏ RAM & Bộ nhớ Unmanaged (Unmanaged Memory & Pointers):**
  - Thao tác trực tiếp với con trỏ RAM: `byte*`, `long*`, `void*`, `int*`.
  - Từ khóa C# Unmanaged: `unsafe`, `fixed`, `stackalloc`, `Marshal`.
* **Cấu trúc Cấp phát Bộ nhớ (Memory Allocators):**
  - Kiến trúc **Slab Allocator** trong `Memory.cs` (phân nhóm Bins từ 16B đến 2KB).
  - Kỹ thuật Memory Pools, Free Lists, Page Groups.
  - Căn lề bộ nhớ (Memory Alignment 16-byte/32-byte).
* **Quản lý Rác & Kiến trúc Zero-GC (Garbage Collection & Zero-GC):**
  - Cơ chế Generational GC của .NET (Gen 0, Gen 1, Gen 2, LOH).
  - Kỹ thuật triệt tiêu 100% rác bộ nhớ bằng struct, unmanaged heap và stackalloc.

---

## 🗄️ 5. KIẾN TRÚC BÊN TRONG CƠ SỞ DỮ LIỆU (DATABASE INTERNALS & STORAGE ENGINES)

Môn học giúp hiểu mô hình thiết kế file dữ liệu và vòng đời bản ghi.

* **Thiết kế Động cơ Lưu trữ (Storage Engine Architecture):**
  - Định dạng file dữ liệu đĩa (`.dat`) và file mảng chỉ mục (`.idx`).
  - Quản lý trang dữ liệu (`DataPage`) và ô dữ liệu đè đĩa (`DataSlot`).
  - Kỹ thuật đè bộ nhớ C/C++ Union Overlay (`[StructLayout(LayoutKind.Explicit)]`).
* **Vòng đời Bản ghi & Dọn rác đĩa (Record Lifecycle & Compaction):**
  - Các trạng thái bản ghi: `Inserted`, `Updated`, `Indexed`, `Deleted`.
  - Kỹ thuật **Soft Delete / Tombstone** (Xóa mềm trong 0.1ns).
  - Tiến trình ngầm Nén đĩa (**Compaction / Vacuum**) thu hồi dung lượng SSD.

---

## 🌐 6. MỞ RỘNG: MẠNG MÁY TÍNH & HỆ THỐNG PHÂN TÁN (COMPUTER NETWORKS & DISTRIBUTED SYSTEMS)

*(Phân mục mở rộng nếu muốn nâng cấp VstHelper từ CSDL Nhúng thành CSDL Client-Server như Redis / MongoDB)*

* **Lập trình Mạng Socket (Socket Programming):** Lập trình TCP/IP Sockets hiệu năng cao bằng Unmanaged I/O Ring / epoll / IOCP.
* **Giao thức Truyền tải Dữ liệu (Binary Protocols):** Thiết kế nhị phân mã hóa gói tin request/response.
* **Kiến trúc Client-Server:** Xử lý bất đồng bộ, Event Loop, I/O Multiplexing.

---

## 📚 SÁCH & TÀI LIỆU KINH ĐIỂN KHUYÊN ĐỌC

1. 📖 **Computer Systems: A Programmer's Perspective (CS:APP)** – *Randal E. Bryant & David R. O'Hallaron*  
   *(Cuốn sách "gối đầu giường" hay nhất thế giới về CPU Cache, Assembly, Memory Hierarchy và Pipeline).*
2. 📖 **Database Internals: A Deep Dive into How Distributed Data Systems Work** – *Alex Petrov*  
   *(Cuốn sách chi tiết nhất về cách viết Storage Engines, B-Tree, LSM-Tree và Indexing).*
3. 📖 **Designing Data-Intensive Applications (DDIA)** – *Martin Kleppmann*  
   *(Cuốn sách kinh điển về kiến trúc CSDL và hệ thống dữ liệu lớn).*
4. 📖 **Operating System Concepts** – *Silberschatz, Galvin, Gagne*  
   *(Cuốn sách giáo trình chuẩn quốc tế về Hệ điều hành, Virtual Memory và Paging).*
