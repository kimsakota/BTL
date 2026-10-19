# VstHelper — Tài Liệu Nghiên Cứu & Phân Tích Kiến Trúc

> **Mục tiêu**: Phân tích toàn diện thư viện `VstHelper` — một engine lưu trữ document/NoSQL tốc độ cao, được viết bằng C# với unsafe code, nhắm đến nền tảng .NET Standard 2.0.

---

## 1. Tổng Quan Dự Án

| Thuộc tính | Giá trị |
|---|---|
| Ngôn ngữ | C# (.NET Standard 2.0) |
| Unsafe Code | **Bật** (`AllowUnsafeBlocks: true`) |
| Namespace chính | `Vst`, `Vst.IO`, `Vst.Data`, `System` |
| Mô hình | Embedded NoSQL / Document Database Engine |
| Platform | Windows (Win32 API), cross-platform qua .NET Standard |

### 1.1 Thiết kế triết lý

Dự án theo đuổi hiệu năng cực đoan bằng cách:
- **Bỏ qua GC động** cho vùng nhớ hot path — dùng memory pool và slab allocation cố định.
- **Tránh boxing/unboxing** — dùng `unmanaged` struct, `MethodImpl(AggressiveInlining)`, memory copy trực tiếp.
- **P/Invoke Win32** thay vì `System.IO` managed — để kiểm soát buffer, write-through, atomic replace.
- **Không dùng `List<T>`**, `Dictionary<K,V>` của BCL — tự implement `RawList<T>`, `PointerList<T>`.

---

## 2. Cấu Trúc Module

```
VstHelper/
├── VstHelper.csproj           # Project config (netstandard2.0, AllowUnsafeBlocks)
├── Enums.cs                   # Định nghĩa enum toàn cục
├── Math.cs                    # XXHash64 + FastMD5 (hashing)
├── ObjectId.cs                # ObjectId 12-byte (MongoDB-compatible)
├── Thread/
│   ├── SystemClock.cs         # Đồng hồ tick async 10ms
│   └── LockManager.cs         # LockEngine (RWLock native) + LockingRead/LockingWrite
├── Memory/
│   ├── Memory.cs              # MemoryPool (slab allocator) + Memory static class
│   ├── Buffer.cs              # Buffer struct (byte* wrapper)
│   ├── DataSlot.cs            # DataSlot 8-byte (union field)
│   ├── DataPage.cs            # DataPage (trang 8-byte slot)
│   ├── StringWriter.cs         # Unsafe string builder (byte*)
│   └── StringReader.cs        # Unsafe string parser (byte*)
├── Lists/
│   └── RawList.cs             # RawList<T> + PointerList<T> (không GC)
├── Keys/
│   ├── KeyEntry.cs            # KeyEntry 32-byte (index entry)
│   ├── KeyIndexer.cs          # KeyIndexer<TComparer> (sort + binary search)
│   └── KeyPath.cs             # KeyEntryPath (path parsing, ví dụ "user.profile.name")
├── IO/
│   ├── File.cs                # Win32File (P/Invoke CreateFile/MapViewOfFile)
│   └── MappedFile.cs          # MappedFile static helpers (Win32 memory-mapped)
└── Helper/
    ├── StringHelper.cs         # UTF-8 encode, escape/unescape, lookup tables
    ├── JsonHelper.cs          # JSON tokenizer (JsonType lookup table)
    └── HexHelper.cs           # Hex encode/decode tables
```

---

## 3. Chi Tiết Từng Module

### 3.1 Memory — Slab Allocator (Lõi quan trọng nhất)

#### 3.1.1 Tầng 1: Memory (static class)

```csharp
namespace Vst {
    public unsafe static partial class Memory {
        private static readonly MemoryPool _pool = new MemoryPool(16);

        // Rent một slot từ pool
        public static MemorySlot Rent(int size) => _pool.Rent(size);

        // Rent sizeof(T) bytes
        public static T* Rent<T>() where T : unmanaged => (T*)_pool.Rent(sizeof(T)).RawData;

        // Trả slot về pool
        public static void Return(void* rented) => _pool.Return((byte*)rented);

        //malloc/realloc/free thuần Win32
        public static T* Malloc<T>() where T : unmanaged => (T*)Marshal.AllocHGlobal(sizeof(T));
        public static byte* Realloc(void* mem, long newLength, long oldLength);
        public static void Free(void* mem) => Marshal.FreeHGlobal((IntPtr)mem);
    }
}
```

#### 3.1.2 MemoryPool — 16 Bin Fixed-Size Slab

```
Pool có 16 bins, mỗi bin chứa các slot cùng kích thước:
  Bin 0: 16 bytes
  Bin 1: 32 bytes
  Bin 2: 64 bytes
  ...
  Bin i: 16 << i bytes (16, 32, 64, 128, 256, 512, 1K, 2K, ...)
  ≥ 8KB  → AllocateHuge (Marshal.AllocHGlobal riêng)
```

#### 3.1.3 MemoryPage — 64 Slot trên một trang

```csharp
// FreeSlots dùng bitmask 64-bit để tracking slot trống
//  bit = 1 → slot trống, bit = 0 → slot đã thuê
//  Kỹ thuật: Interlocked.CompareExchange + Log2(current) để tìm slot nhanh
internal long FreeSlots;  // bitmask 64-bit

// Rent nhanh nhất: lấy bit 0
public MemorySlot RentDirect() {
    long mask = 1L << 0;
    FreeSlots &= ~mask;
    return new MemorySlot(DataHandle, SlotSize);
}

// Rent có tìm kiếm: tìm bit thấp nhất đang bật
public bool TryRent(out MemorySlot slot) {
    long current = Interlocked.Read(ref FreeSlots);
    int bitIdx = Binary.Log2(current);  // tìm trailing zero
    long mask = 1L << bitIdx;
    // CAS để đánh dấu slot đã lấy
}
```

#### 3.1.4 MemoryPageGroup — Quản lý nhiều Page

```
Cấu trúc 3 lớp:
  MemoryPool (1 instance)
    └── _bins[16] kiểu MemoryPageGroup*
          └── _pages[] kiểu MemoryPage*[]
                └── DataHandle[] → slot blocks
```

- `_top` = chỉ số page hiện tại đang ưu tiên rent.
- Thuật toán rent: ưu tiên page cuối → nếu full → tìm ngược → nếu không có → tạo page mới.
- `_lockHandle` (RWLock) bảo vệ việc tạo page mới trong multi-threaded context.

#### 3.1.5 MemorySlot — Metadata wrapper

```csharp
// Header đứng ngay trước vùng data thực:
// [MemorySlotHeader: 16 bytes][User Data: N bytes]
//  - SlotIndex: vị trí trong page (0-63)
//  - BinIndex:  chỉ số bin để trả về đúng pool
//  - PageHandle: con trỏ page gốc (để Return nhanh)
internal unsafe struct MemorySlotHeader {
    internal int SlotIndex;
    internal int BinIndex;
    internal MemoryPage* PageHandle;
}
```

### 3.2 Threading — RWLock và SystemClock

#### 3.2.1 LockEngine (RWLock)

Dùng `CreateEvent` Win32 thay vì `Monitor`/.NET Lock:

```
State encoding:
  ≥ 0: Số lượng reader đang giữ lock (shared count)
  -1: Writer đang giữ lock (exclusive)

EnterRead():
  while (!CAS(_state, s, s+1)) SpinWait → WaitForSingleObject(timeout=1ms)
  
EnterWrite():
  while (!CAS(_state, 0, -1)) SpinWait → WaitForSingleObject(timeout=1ms)
  → Chỉ 1 writer tại một thời điểm

ExitRead():
  if (Interlocked.Decrement == 0) SetEvent → đánh thức writer

ExitWrite():
  Interlocked.Exchange(0); SetEvent → đánh thức tất cả
```

`LockingRead` và `LockingWrite` là **struct implements `IDisposable`** — pattern RAII cho C#.

#### 3.2.2 SystemClock

```
Task.Run(async () => {
    while (_isRunning) {
        await Task.Delay(10);  // Nhịp 10ms
        OnTick?.Invoke();
        if (++_ten >= 100) { _ten=0; _sec++; OnSecond?.Invoke(); ... }
    }
})
// Events: OnTick(10ms), OnSecond, OnMinute, OnHour, OnDay
```

### 3.3 Lists — RawList và PointerList

#### 3.3.1 RawList<T> (unmanaged)

```csharp
public unsafe struct RawList<T> : IList<T> where T : unmanaged {
    private T* _items;
    private int _count;
    private int _capacity;
}
// KHÔNG có GC.Alloc — dùng Memory.RentHandle()
// GrowUp: tăng 1.5x capacity, copy bộ nhớ, trả slot cũ
```

Key methods:
- `Prepare(int n)` — chuẩn bị `n` phần tử liên tiếp (không zero-initialized).
- `Insert(int, T)` — chèn giữa, dùng `Buffer.MemCopy`.
- `Swap(int, int)` — hoán đổi nhanh.
- `Mapping(T* handle, int count)` — map trực tiếp vào một vùng nhớ có sẵn.

#### 3.3.2 PointerList<T>

Wrapper quản lý `IntPtr` array, nội bộ dùng `RawList<IntPtr>`.

### 3.4 Keys — KeyEntry, KeyIndexer, KeyPath

#### 3.4.1 KeyEntry (32 bytes)

```
[Key (8 bytes)][SecondaryKey (8 bytes)][Offset (8 bytes)][RecordLength/IndexPosition (4 bytes)]
[State (1 byte)][Type (1 byte)][KeyState (1 byte)][Calculated (1 byte)]
[Comparer.Handle (8 bytes, overlay with Offset)]
```

Primary modes:
| Mode | Xử lý Key |
|---|---|
| `Default` | Parse long từ string |
| `Fixed` | Parse long từ string |
| `ObjectId` | 12-byte raw copy |
| `String` | ≤16 bytes: copy trực tiếp vào 16-byte key; >16 bytes: XXHash64 dual hash |
| `LongString` | Kết quả dual hash 128-bit từ String |

`EnsureToCompare()` — lazy hash computation, chỉ tính khi cần.

#### 3.4.2 KeyIndexer<TComparer>

Binary search + QuickSort song song:

```csharp
// Find với binary search
public int Find(KeyEntry* e)  // trả về index hoặc -1

// Sort: QuickSort iterative (stack-based) với 3 tối ưu:
//  1. Median-of-three pivot selection
//  2. Insertion Sort cho đoạn < 128 phần tử
//  3. Parallel.Invoke khi đoạn > 128 phần tử

public unsafe class KeyIndexer<TComparer> where TComparer : struct, IIndexComparer {
    SimpleIndexComparer   // so sánh Key (8 bytes)
    ComplexIndexComparer  // so sánh Key + SecondaryKey (16 bytes)
}
```

### 3.5 IO — Win32 Native File & Memory-Mapped

#### 3.5.1 Win32File

P/Invoke trực tiếp Windows Kernel:
```
CreateFile     → mở file (READ|WRITE, write-through flag)
WriteFile      → ghi trực tiếp OS buffer
ReadFile       → đọc trực tiếp OS buffer  
SetFilePointerEx → seek 64-bit
SetEndOfFile   → resize
CreateFileMapping + MapViewOfFile → memory-mapped I/O
FlushFileBuffers → force disk write
ReplaceFile    → atomic replace (journaling)
```

#### 3.5.2 MappedFile (static helpers)

Wrapper cấp thấp cho Win32 Memory-Mapped File API:
- `SetLength(IntPtr, long)` — nở file vật lý.
- `GetLength(IntPtr)` — lấy kích thước 64-bit.

### 3.6 Helper Classes

#### 3.6.1 StringHelper

Static lookup tables (khởi tạo một lần):
```
LowerCaseTable[256]   — A-Z → a-z
EscapeTable[256]     — \" \\ \b \f \n \r \t
UnescapeTable[256]    — reverse escape
DecDigitTable[100]    — pack 2 digit vào ushort (00→"00", 99→"99")
```

Key methods:
- `EncodeUTF8(byte*, string)` — mã hóa string → UTF-8 byte*.
- `ToLowerCase(byte*, int)` — unrolled loop 4 bytes/lần.
- `Unescape(byte**, byte*, int)` — JSON unescape với surrogate pair support.
- `WriteSurrogate(ref byte*, char, char)` — UTF-8 4-byte từ surrogate pair.

#### 3.6.2 JsonHelper

Lookup table 128 bytes cho tokenizer:
```
'0'-'9' → Number
'T','t','F','f' → Boolean
'"' → String
'{','}' → Object/OpenObject
'[',']' → Array/OpenArray
'N','n' → Null
':' → Colon, ',' → Comma
```

#### 3.6.3 HexHelper

```
ByteToHexTable[256]  — uint: mỗi entry chứa 2 hex digit (4 bytes uint)
HexToByteTable[256]  — hex char → byte value (case-insensitive)
```

#### 3.6.4 Buffer

```csharp
[StructLayout(LayoutKind.Explicit, Size = 16)]
public struct Buffer {
    [FieldOffset(0)] public byte* Handle;
    [FieldOffset(8)] public int Length;
    [FieldOffset(12)] public int Iterator;

    public bool RawEquals(byte* handle, int length);  // memcmp unrolled
    public void Shift(int offset);                    // advance pointer
    public void Slice(int offset);                    // advance pointer + decrease length
    public string ToHex();                            // debug output
}
```

### 3.7 Data Types — DataSlot, DataPage, ObjectId

#### 3.7.1 DataSlot (8 bytes — union field)

```csharp
[StructLayout(LayoutKind.Explicit, Size = 8)]
public struct DataSlot {
    [FieldOffset(0)] public byte* PointerValue;
    [FieldOffset(0)] public long LongValue;
    [FieldOffset(0)] public double DoubleValue;
    [FieldOffset(0)] public bool BooleanValue;
    [FieldOffset(0)] public int FirstChildIndex;  // cho JSON tree
    [FieldOffset(4)] public int LastChildIndex;
}
```

#### 3.7.2 ObjectId (12 bytes — MongoDB-like)

```
[4 bytes: Unix timestamp (seconds)]
[3 bytes: Machine hash | 2 bytes: PID]
[3 bytes: Increment counter]
```

```csharp
// Hash: XXHash64(12-byte raw)
// Factory: ObjectId.NewId()
// Parse: FromString, FromHandle, FromObjectId, FromFixedValue
```

### 3.8 Math — XXHash64 & FastMD5

#### 3.8.1 XXHash64

Triển khai đầy đủ XXHash64 (xxh64):
- `ComputeHash(byte*, int, ulong seed)` — hash 64-bit.
- `ComputeDualHashShort(byte*, int, long*)` — tính **2 hash 64-bit** với 2 seed khác nhau trong 1 pass.
  - Seed 1: `0`
  - Seed 2: `length ^ 0xBF58476D1CE4E5B9L` (golden ratio)
- Avalanche step cuối cùng để bit diffusion.

#### 3.8.2 FastMD5

Triển khai **inline MD5** (4 rounds × 16 steps = 64 steps, hardcoded constants):
- `EncodingMD5(byte* src, int length, long* dst)` — output 128-bit (2 × `long*`).
- Padding 0x80 → length in bits ở 8 byte cuối.
- Mỗi block 64 bytes.

---

## 4. Các Enum Quan Trọng

```csharp
// Trạng thái bản ghi trong index
enum RecordState { None, Indexed, Updated, Inserted, Deleted = 255 }

// Chế độ khóa chính
enum PrimaryMode { Default, Fixed, ObjectId, String = 4, LongString = 8 }

// Trạng thái KeyEntry
enum KeyEntryState { None, Fixed, Rented, Raw, DocumentHeap }

// Loại JSON token
enum JsonType { None, Object, Array, String, Number, Boolean, Null, DateTime, ObjectId, ... }
```

---

## 5. Các Kỹ Thuật Tối Ưu Hóa Quan Trọng

| Kỹ thuật | Ví dụ | Lợi ích |
|---|---|---|
| **AggressiveInlining** | Tất cả hot-path methods | Tránh call overhead |
| **Bitmask slot allocation** | `FreeSlots &= ~mask` | O(1) rent/return |
| **Unrolled loop** | `while (rem >= 4)` trong `ToLowerCase` | Giảm branch prediction misses |
| **Lookup table** | `LowerCaseTable`, `EscapeTable`, `HexToByteTable` | O(1) char ops |
| **Dual hash in single pass** | `ComputeDualHashShort` | 2 hash cùng lúc |
| **Slab allocation** | 16 bin × 64 slot/page | Zero fragmentation, O(1) alloc |
| **Inline MD5/XXHash** | Không tạo đối tượng trung gian | Không GC pressure |
| **Memory-mapped I/O** | `MapViewOfFile` | Zero-copy read |
| **CAS (Compare-And-Swap)** | `Interlocked.CompareExchange` | Lock-free rent/return |
| **RWLock** | Đọc song song, ghi độc quyền | Multi-reader throughput |
| **Parallel Sort** | `Parallel.Invoke` cho sort >128 items | Multi-core utilization |
| **Stack-allocated buffer** | `stackalloc byte[N]` | Không heap allocation |
| **Struct overlay** | `DataSlot` (union) | Tiết kiệm memory |
| **Thread-local buffer** | `_threadBuffer` trong `StringHelper` | Không race condition |

---

## 6. Luồng Dữ Liệu Tổng Quát

```
Application Code
       │
       ▼
  VstHelper API
       │
       ├── JSON parse ──→ DataSlot[] (DataPage) ──→ StringReader
       │
       ├── Key extraction ──→ KeyEntry (32B) ──→ KeyIndexer.Sort()
       │                         │
       │                         ├── ≤16B → Buffer copy
       │                         └── >16B → XXHash64 dual hash
       │
       ├── Memory allocation ──→ MemoryPool.Rent()
       │                              │
       │                              ├── Bin lookup (Log2)
       │                              ├── Page.TryRent (CAS bitmask)
       │                              └── PageGroup (RWLock)
       │
       └── Persistence ──→ Win32File
                               │
                               ├── WriteFile (write-through)
                               ├── Memory-Mapped (MapViewOfFile)
                               └── Atomic Replace (ReplaceFile)
```

---

## 7. Quan Hệ Giữa Các Module

```
ObjectId ──(dùng)→ XXHash64 ──(dùng)→ Binary.Log2

KeyEntry ──(dùng)→ XXHash64.ComputeDualHashShort
KeyEntry ──(dùng)→ StringReader.GetLong
KeyEntry ──(dùng)→ HexHelper.WriteBytes

KeyIndexer ──(dùng)→ KeyEntry (array)
KeyIndexer ──(dùng)→ Buffer.MemCopy
KeyIndexer ──(dùng)→ LockEngine (khi sort song song)

MemoryPool ──(dùng)→ MemoryPageGroup
MemoryPageGroup ──(dùng)→ MemoryPage (CAS bitmask)
MemoryPageGroup ──(dùng)→ LockEngine (RWLock)

Win32File ──(P/Invoke)→ kernel32.dll (CreateFile, MapViewOfFile, ...)

StringWriter ──(dùng)→ StringHelper.DecDigitTable
StringWriter ──(dùng)→ StringHelper.EscapeTable
StringReader ──(dùng)→ StringHelper.UnescapeTable
```

---

## 8. Các Lớp Chính Cần Triển Khai Tiếp

Dựa trên mã nguồn hiện tại, các thành phần sau **chưa được triển khai đầy đủ** và cần phát triển thêm để có một document database hoàn chỉnh:

1. **Document Engine** — JSON serializer/deserializer nối các `DataSlot` thành tree, hỗ trợ nested object/array.
2. **Store/Collection** — quản lý file `.dat` (data) + `.idx` (index), transaction log.
3. **Query Engine** — XPath-like path query trên `KeyEntryPath`, filter, sort, pagination.
4. **B-Tree / SkipList Index** — thay thế array-based `KeyIndexer` để hỗ trợ range query hiệu quả.
5. **WAL (Write-Ahead Log)** — journaling cho crash recovery dùng `AtomicReplace`.
6. **Replication** — cơ chế replication để sync giữa các instance.

---

## 9. Đánh Giá Kiến Trúc

### Ưu điểm
- **Kiến trúc memory layer cực kỳ bài bản**: slab allocator + bitmask + CAS lock-free là nền tảng vững.
- **Hash function tự triển khai**: XXHash64 và MD5 inline không phụ thuộc thư viện ngoài.
- **Unsafe code có kỷ luật**: rõ ràng ranh giới giữa unsafe (core) và safe API (sẽ có).
- **Thread-safety từ đầu**: LockEngine, Interlocked, ThreadStatic buffers.
- **Serialization cực nhanh**: StringWriter/StringReader không boxing.

### Hạn chế tiềm ẩn
- **Không có unit test** trong codebase hiện tại.
- **Không có error handling tập trung**: nhiều `throw new Exception(...)`.
- **StringHelper._threadBuffer** dùng `Marshal.AllocHGlobal` nhưng **chưa có Dispose** — memory leak khi thread exit (tuy nhiên vì `_threadBuffer` là thread-static nên chỉ leak khi AppDomain unload).
- **`FastMD5`** — thiếu round constant cho đầy đủ MD5 (đang dùng MD5Lite/SHA-like approach).
- **Parallel Sort** dùng `Parallel.Invoke` nhưng không có `WaitHandle` cleanup rõ ràng cho short-lived sorts.

---

## 10a. Tài Liệu Bổ Sung

Ngoài file `agents.md` này, dự án còn có tài liệu tương tác:

- **`docs/sorting-algorithm.html`** — File HTML tương tác, bao gồm:
  - Tổng quan thuật toán sắp xếp song song
  - Nền tảng kiến thức (QuickSort, Median-of-three, Insertion Sort, Parallel.Invoke, IndexRange overlay)
  - Chi tiết từng thành phần với code và minh họa
  - **Mô phỏng tương tác**: chạy step-by-step, điều chỉnh threshold, xem stack, xem log
  - So sánh với thuật toán khác (MergeSort, TimSort, HeapSort, IntroSort)
  - Phân tích sâu: Amdahl's Law, cache performance, generic monomorphization

---

## 11. Kết Luận

`VstHelper` là một dự án nghiên cứu và phát triển **embedded database engine** thể hiện kiến thức sâu về:
- **Memory management** (slab allocator, bitmask, lock-free CAS)
- **Win32 API** (file I/O, memory-mapped, atomic replace)
- **Hash algorithm** (XXHash64, MD5 inline)
- **Unsafe C# patterns** (pointer arithmetic, struct overlay, MethodImpl)
- **Locking strategy** (RWLock với spin-wait + native event)

Dự án cần phát triển thêm document engine, query layer, và persistence layer để trở thành một NoSQL database hoàn chỉnh. Phần core (memory, indexing, hashing, I/O) đã được xây dựng rất chắc chắn với hiệu năng cao.
