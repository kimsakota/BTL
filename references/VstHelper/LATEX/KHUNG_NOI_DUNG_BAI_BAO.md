# 📋 KHUNG NỘI DUNG CHI TIẾT — BÀI BÁO KHOA HỌC IEEE
# Dự án: VstHelper — Zero-GC Embedded Database Engine

> **Mục đích file này:** Đây là bản đồ nội dung (Content Map) chi tiết từ A đến Z.
> Bạn sẽ tự tay soạn nội dung cho từng mục trong file này, sau đó tổng hợp lại thành bài báo LaTeX hoàn chỉnh.
>
> **Quy ước ký hiệu:**
> - 📝 = Nội dung cần tự viết (văn bản giải thích, lập luận)
> - 📐 = Hình vẽ / Sơ đồ cần tạo (diagram, figure)
> - 📊 = Bảng số liệu / Benchmark cần chạy hoặc tính toán
> - 🔢 = Công thức Toán học cần chứng minh hoặc trình bày
> - 📚 = Tài liệu tham khảo cần trích dẫn (reference)
> - ✅ = Đã có sẵn trong paper.tex hiện tại
> - ❌ = Chưa có, cần bổ sung

---

## MỤC LỤC TỔNG QUAN (IEEE Paper Structure)

| # | Section | Trạng thái | Ghi chú |
|---|---------|-----------|---------|
| 0 | Abstract | ✅ Có rồi | Cần cập nhật nếu mở rộng phạm vi |
| 1 | Introduction | ✅ Có rồi | Cần bổ sung thêm contribution mới |
| 2 | Background & Related Work | ✅ Có rồi | Cần bổ sung thêm mục Hashing, Memory Allocator |
| 3 | Research Questions & Hypotheses | ✅ Có rồi | Cần thêm RQ cho các tầng mới |
| 4 | Proposed System Design | ✅ Một phần | **TRỌNG TÂM MỞ RỘNG** — Cần bổ sung 5 tầng kiến trúc |
| 5 | Methodology & Benchmark Settings | ✅ Có rồi | Cần bổ sung test case cho tầng mới |
| 6 | Results & Evaluation | ✅ Có rồi | Cần bổ sung kết quả cho tầng mới |
| 7 | Complexity Analysis | ✅ Có rồi | Cần bổ sung phân tích cho từng tầng |
| 8 | Discussion & Future Work | ❌ Chưa đủ | Cần viết thảo luận tổng hợp |
| 9 | Conclusion | ✅ Có rồi | Cần cập nhật |
| A | Appendix | ✅ Có rồi | Cần bổ sung source code các tầng |

---

# ═══════════════════════════════════════════════════════════
# SECTION 0: ABSTRACT (150 – 250 từ)
# ═══════════════════════════════════════════════════════════

### Trạng thái: ✅ Đã có — Cần cập nhật nếu mở rộng phạm vi

### Cấu trúc chuẩn IEEE Abstract:

| Phần | Nội dung cần viết | Số câu |
|------|-------------------|--------|
| **Context** | Bối cảnh: embedded DB, IoT, real-time systems | 1-2 |
| **Problem** | Vấn đề: GC pauses, cache inefficiency, memory fragmentation | 1 |
| **Solution** | Đề xuất: VstHelper — kiến trúc Zero-GC hoàn chỉnh 6 tầng | 1 |
| **Method** | Kỹ thuật: Slab Allocator, KeyEntry 32B, Hybrid QuickSort, XXHash64 Dual-Hash, ObjectId, RawList Unmanaged | 2 |
| **Results** | Kết quả Benchmark: speedup, cache miss, latency, zero allocation | 2 |
| **Impact** | Ý nghĩa: real-time deterministic latency, IoT applications | 1 |

---

# ═══════════════════════════════════════════════════════════
# SECTION 1: INTRODUCTION (4 – 5 đoạn văn)
# ═══════════════════════════════════════════════════════════

### Trạng thái: ✅ Đã có — Cần bổ sung thêm contribution

### Đoạn 1: BỐI CẢNH (Context)
📝 **Nội dung cần viết:**
- Embedded systems & IoT devices ngày càng cần database engines xử lý hàng nghìn key-value operations với deterministic latency.
- Xu hướng: sensor data logging, time-series, real-time telemetry.

📚 **Tham khảo:**
- [Maeda 2012] — GC impact on real-time systems
- [LaMarca 1999] — Cache-conscious algorithm design

---

### Đoạn 2: VẤN ĐỀ (Problem Statement)
📝 **Nội dung cần viết:**
- **Vấn đề 1:** GC pauses 1ms – 100ms phá vỡ real-time guarantees.
- **Vấn đề 2:** Cache inefficiency do variable-size records, pointer chasing trong B-Tree.
- **Vấn đề 3:** Memory fragmentation do Managed Heap, không kiểm soát được vị trí bộ nhớ.
- **Vấn đề 4:** Thiếu cơ chế sinh ID duy nhất nhẹ (lightweight unique ID generation) cho embedded.

🔢 **Số liệu dẫn chứng cần chuẩn bị:**
- GC Gen0 pauses: 1 – 10 ms
- GC Gen2 pauses: > 100 ms
- B-Tree cache miss rates: 40 – 60%
- B-Tree node size: 64 – 256 Bytes (1 – 4 cache lines mỗi node)

---

### Đoạn 3: GIẢI PHÁP ĐỀ XUẤT (Approach — VstHelper)
📝 **Nội dung cần viết — Giới thiệu 6 thành phần chính:**

| # | Thành phần | File nguồn | Mô tả ngắn |
|---|-----------|-----------|------------|
| 1 | Slab Allocator (Zero-GC Memory) | `Memory.cs`, `Buffer.cs`, `DataSlot.cs`, `DataPage.cs` | Lock-free O(1) rent/return, 16 Bins power-of-two |
| 2 | KeyEntry Memory Layout | `KeyEntry.cs` | Struct 32B, 2 entries / Cache Line 64B |
| 3 | KeyIndexer Hybrid Parallel Sort | `KeyIndexer.cs` | 3-tier: Parallel → QuickSort → ShiftSort (InsertionSort) |
| 4 | XXHash64 Dual-Hash & FastMD5 | `Math.cs` | Single-pass 128-bit hash, branchless bitwise |
| 5 | RawList & PointerList | `RawList.cs` | Mảng động Unmanaged, hệ số mở rộng 1.5x |
| 6 | ObjectId Generator | `ObjectId.cs` | 12-byte globally unique ID, Atomic `Interlocked` |

---

### Đoạn 4: ĐÓNG GÓP CHÍNH (Contributions — 4–6 bullets)
📝 **Nội dung cần viết (dạng itemize):**

- [ ] Contribution 1: Kiến trúc Slab Allocator lock-free với 16 Bins power-of-two, đạt O(1) rent/return và zero GC pressure.
- [ ] Contribution 2: KeyEntry 32-byte layout giảm 40–50% cache misses so với B-Tree.
- [ ] Contribution 3: Hybrid Parallel QuickSort đạt 2.1–3.6× speedup so với Array.Sort.
- [ ] Contribution 4: XXHash64 single-pass dual-hash 128-bit với chất lượng phân bố vượt trội.
- [ ] Contribution 5: RawList<T> Zero-GC dynamic array với hệ số mở rộng 1.5x tiết kiệm 33% RAM.
- [ ] Contribution 6: ObjectId 12-byte atomic ID generator an toàn đa luồng, không cần database sequence.

---

### Đoạn 5: CẤU TRÚC BÀI BÁO (Roadmap)
📝 **Template:** "The remainder of this paper is organized as follows. Section II presents... Section III defines... Section IV details... Section V describes... Section VI presents... Section VII analyzes... Section VIII concludes..."

---

# ═══════════════════════════════════════════════════════════
# SECTION 2: BACKGROUND & RELATED WORK
# ═══════════════════════════════════════════════════════════

### Trạng thái: ✅ Một phần — Cần bổ sung 3 subsection mới

---

### 2.1 Sorting Algorithms ✅ (Đã có)
📝 Nội dung hiện có: QuickSort (Hoare), Parallel QuickSort (Amdahl's Law), Hybrid Optimization.

---

### 2.2 Cache-Conscious Data Structures ✅ (Đã có)
📝 Nội dung hiện có: L1/L2/L3 Cache hierarchy, Cache Line 64B, LaMarca & Ladner.

---

### 2.3 Index Structures for Embedded Databases ✅ (Đã có)
📝 Nội dung hiện có: B-Tree vs Array-based indexing, bảng so sánh.

---

### 2.4 .NET Runtime and Garbage Collection ✅ (Đã có)
📝 Nội dung hiện có: Generational GC, Zero-GC techniques.

---

### 2.5 Hash Functions for Database Indexing ❌ (CẦN BỔ SUNG)
📝 **Nội dung cần viết:**
- Vai trò của Hash Function trong CSDL: chuyển đổi key bất kỳ (string, binary) thành số nguyên cố định.
- Yêu cầu: Tốc độ cao (throughput), phân bố đều (low collision), deterministic.
- Các Hash Function phổ biến: MurmurHash3, CityHash, FarmHash, XXHash.
- **XXHash** (Yann Collet, 2012): Nhanh nhất trong nhóm non-crypto hash, đạt gần tốc độ RAM bandwidth.
- Khái niệm **Dual-Hash / Double Hashing**: Dùng 2 hash function độc lập để tạo 128-bit key, giảm xác suất va chạm xuống $2^{-128}$.

📚 **Tham khảo cần tìm:**
- Yann Collet — XXHash: Extremely Fast Non-Cryptographic Hash Algorithm
- Knuth — The Art of Computer Programming, Vol. 3 (Hashing)
- Dietzfelbinger et al. — Universal Hashing

🔢 **Công thức cần trình bày:**
- Xác suất va chạm Birthday Paradox: $P(\text{collision}) \approx 1 - e^{-n^2 / 2m}$ với $m = 2^{64}$ hoặc $2^{128}$.
- Throughput hash: bytes/second, cycles/byte.

---

### 2.6 Memory Allocators in Systems Programming ❌ (CẦN BỔ SUNG)
📝 **Nội dung cần viết:**
- Vấn đề của `malloc/free` truyền thống: fragmentation, thread contention.
- Các kiến trúc allocator hiện đại: Slab Allocator (Bonwick 1994), jemalloc (Facebook), tcmalloc (Google), mimalloc (Microsoft).
- **Slab Allocator** (Jeff Bonwick, SunOS 1994): Chia RAM thành các Bin kích thước cố định, O(1) cấp phát, zero fragmentation nội bộ.
- Lock-free allocation: CAS (Compare-And-Swap), bitmask free-slot tracking.

📚 **Tham khảo cần tìm:**
- Bonwick 1994 — The Slab Allocator: An Object-Caching Kernel Memory Allocator
- jemalloc — A Scalable Concurrent malloc Implementation
- Microsoft mimalloc

---

### 2.7 Unique Identifier Generation ❌ (CẦN BỔ SUNG)
📝 **Nội dung cần viết:**
- Vấn đề: Trong hệ thống phân tán / embedded, không thể dùng AUTO_INCREMENT vì không có CSDL trung tâm.
- Các giải pháp: UUID v4 (16 bytes, random), MongoDB ObjectId (12 bytes, timestamp-based), Twitter Snowflake (8 bytes).
- So sánh: UUID vs ObjectId vs Snowflake (kích thước, tính sắp xếp theo thời gian, tốc độ sinh).
- Ưu điểm ObjectId: Nhỏ gọn (12B), sắp xếp được theo thời gian (timestamp ở đầu), an toàn đa luồng.

📚 **Tham khảo cần tìm:**
- MongoDB ObjectId Specification
- RFC 9562 — UUID Version 7
- Twitter Snowflake ID

---

# ═══════════════════════════════════════════════════════════
# SECTION 3: RESEARCH QUESTIONS & HYPOTHESES
# ═══════════════════════════════════════════════════════════

### Trạng thái: ✅ Có 4 RQ — Cần thêm 2 RQ mới

---

### RQ1: Cache-Efficient Memory Layout ✅ (Đã có)
> H1: KeyEntry 32-byte đạt 40–50% fewer cache misses so với B-Tree nodes.

### RQ2: Parallel Sort Performance ✅ (Đã có)
> H2: KeyIndexer đạt 2–3× speedup so với Array.Sort cho N > 100K.

### RQ3: Multi-core Scalability ✅ (Đã có)
> H3: KeyIndexer đạt 2.5–3.5× parallel speedup với 8 threads.

### RQ4: Real-time Latency Guarantees ✅ (Đã có)
> H4: Maximum latency < 5ms cho 100K keys, zero heap allocation.

---

### RQ5: Hash Quality & Throughput ❌ (CẦN BỔ SUNG)
📝 **Cần viết:**
> **H5:** XXHash64 Dual-Hash đạt throughput ≥ 10 GB/s trên dữ liệu ≥ 32 bytes, với tỉ lệ va chạm ≤ $2^{-64}$ trên 10 triệu keys ngẫu nhiên.

- **Motivation:** Hash function là nút thắt cổ chai (bottleneck) khi chuyển đổi string key sang integer key cho KeyEntry. Throughput thấp sẽ làm chậm toàn bộ pipeline indexing.
- **Metrics:** Throughput (GB/s), Cycles/byte, Collision rate, Chi-squared uniformity test.

---

### RQ6: Memory Allocator Efficiency ❌ (CẦN BỔ SUNG)
📝 **Cần viết:**
> **H6:** Slab Allocator đạt thời gian cấp phát O(1) amortized với zero fragmentation nội bộ, và throughput rent/return ≥ 50 triệu ops/giây trên single thread.

- **Motivation:** Mọi thao tác CSDL (Insert, Update, Delete, Query) đều bắt đầu bằng việc thuê bộ nhớ từ allocator. Nếu allocator chậm hoặc bị lock contention, toàn bộ hệ thống bị nghẽn.
- **Metrics:** Rent latency (ns), Return latency (ns), Throughput (ops/s), Fragmentation ratio.

---

# ═══════════════════════════════════════════════════════════
# SECTION 4: PROPOSED SYSTEM DESIGN ⭐ TRỌNG TÂM MỞ RỘNG
# ═══════════════════════════════════════════════════════════

### Trạng thái: ✅ Một phần — Cần bổ sung 5 subsection mới

---

## 📐 HÌNH VẼ BẮT BUỘC: Kiến trúc Tổng quan 6 Tầng
> **Đây là hình quan trọng nhất của toàn bài báo!**

📐 **Cần tạo Figure: "VstHelper 6-Layer Architecture Overview"**

```
┌─────────────────────────────────────────────────────────────────┐
│                    APPLICATION LAYER                            │
│              (Insert / Query / Update / Delete)                 │
├─────────────────────────────────────────────────────────────────┤
│  TẦNG 6: ObjectId Generator         │  TẦNG 5: RawList<T>      │
│  (12-byte Atomic ID, Interlocked)    │  (Zero-GC Dynamic Array) │
├──────────────────────────────────────┴──────────────────────────┤
│  TẦNG 4: Hashing & Bitwise Layer (Math.cs)                     │
│  XXHash64 Dual-Hash 128-bit  │  FastMD5  │  Binary (Log2, Pow2)│
├─────────────────────────────────────────────────────────────────┤
│  TẦNG 3: Key Indexing & Sorting Layer                          │
│  KeyEntry 32B  │  KeyIndexer (Parallel QuickSort + ShiftSort)   │
├─────────────────────────────────────────────────────────────────┤
│  TẦNG 2: Memory Read/Write Layer                               │
│  StringWriter (Zero-GC)  │  StringReader (Bitwise Parse)       │
├─────────────────────────────────────────────────────────────────┤
│  TẦNG 1: Memory & Buffer Foundation                            │
│  Slab Allocator (Memory.cs)  │  Buffer.MemCopy  │  DataSlot    │
├─────────────────────────────────────────────────────────────────┤
│                    HARDWARE / OS LAYER                          │
│  CPU Cache (L1/L2/L3)  │  Virtual Memory  │  SSD File I/O      │
└─────────────────────────────────────────────────────────────────┘
```

---

### 4.1 Design Goals ✅ (Đã có — cần mở rộng)
📝 **Bổ sung thêm 2 nguyên tắc thiết kế:**
- [ ] (Đã có) Zero GC Pressure
- [ ] (Đã có) Cache Efficiency
- [ ] (Đã có) Multi-core Scalability
- [ ] **(Mới)** Deterministic Memory Layout — Mọi struct đều có kích thước cố định, vị trí xác định.
- [ ] **(Mới)** Minimal Abstraction — Dùng struct thay class, pointer thay reference, inline thay virtual.

> ⚠️ **Lưu ý:** Dự án hiện tại chạy trên `netstandard2.0`. Các kỹ thuật Hardware Intrinsics (SIMD `Vector128`, `BitOperations.TrailingZeroCount`) chưa khả dụng và chỉ được đề cập ở **Section 8.2 Future Work** như hướng phát triển khi nâng cấp lên .NET 6+.

---

### 4.2 KeyEntry Memory Layout (32-byte) ✅ (Đã có)

---

### 4.3 KeyIndexer Algorithm ✅ (Đã có)

---

### 4.4 Memory Architecture: Slab Allocator ✅ (Đã có — cần mở rộng chi tiết)

📝 **Nội dung cần bổ sung chi tiết:**

#### 4.4.1 Cấu trúc Slab Allocator
📐 **Hình cần tạo:** "Slab Allocator Internal Architecture"
```
Memory.cs Slab Allocator:
┌──────────────────────────────────────────────────────────────┐
│ Bin Array (16 Bins, kích thước power-of-two):                │
│ ┌────────┬────────┬────────┬─── ─ ─ ─ ─┬──────────────────┐ │
│ │ Bin[0] │ Bin[1] │ Bin[2] │    ...     │ Bin[15]          │ │
│ │  16B   │  32B   │  64B   │           │ 524,288B (512KB) │ │
│ └───┬────┴───┬────┴───┬────┴─── ─ ─ ─ ─┴────┬─────────────┘ │
│     ▼        ▼        ▼                      ▼               │
│  PageGroup PageGroup PageGroup          PageGroup            │
│  ┌──────┐  ┌──────┐                                         │
│  │Page 0│→ │Page 1│→ ...  (Linked List các Pages)            │
│  │64slot│  │64slot│                                          │
│  └──────┘  └──────┘                                          │
└──────────────────────────────────────────────────────────────┘
```

📝 **Cần giải thích:**
- [ ] Tại sao chọn 16 Bins power-of-two? → Tính Bin Index bằng `Binary.Log2` (1 lệnh CPU).
- [ ] Mỗi Page chứa 64 slot → Quản lý bằng bitmask `ulong` (64-bit), mỗi bit = 1 slot.
- [ ] `Interlocked.CompareExchange` trên bitmask → Lock-free O(1) rent/return.
- [ ] `Marshal.AllocHGlobal` để cấp phát RAM ngoài Managed Heap → Zero GC.

🔢 **Công thức cần trình bày:**
- Bin Index: $\text{bin} = \lceil \log_2(\text{size}) \rceil - 4$
- Slot allocation bitmask: $\text{freeSlot} = \text{TrailingZeroCount}(\text{bitmask})$
- Page capacity: $\text{pageSize} = 64 \times 2^{(\text{bin} + 4)}$ bytes

📊 **Bảng cần tạo:** "Slab Allocator Bin Configuration"

| Bin | Slot Size | Slots/Page | Page Size | Ví dụ sử dụng |
|-----|-----------|-----------|-----------|---------------|
| 0   | 16 B      | 64        | 1,024 B   | Small keys    |
| 1   | 32 B      | 64        | 2,048 B   | KeyEntry      |
| 2   | 64 B      | 64        | 4,096 B   | Small buffers |
| ... | ...       | 64        | ...       | ...           |
| 15  | 512 KB    | 64        | 32 MB     | Large docs    |

---

#### 4.4.2 Lock-free Rent/Return bằng CAS (Compare-And-Swap)
📝 **Cần giải thích kỹ thuật cốt lõi:**
- [ ] Cơ chế CAS: `Interlocked.CompareExchange(ref bitmask, newValue, expectedValue)`
- [ ] Nếu thread khác đã thay đổi bitmask → CAS thất bại → retry spin loop.
- [ ] So sánh với Mutex/Lock: CAS không cần context switch vào kernel → nhanh hơn 10-100×.

📐 **Hình cần tạo:** "Lock-free CAS Rent Flow Diagram"

🔢 **Phân tích cần viết:**
- Amortized complexity: O(1) trong trường hợp trung bình, O(k) worst-case với k = số thread tranh chấp.
- Throughput lý thuyết: 1 CAS ≈ 10-20 ns trên x86 → 50-100 triệu ops/giây.

---

#### 4.4.3 Buffer.MemCopy & Lệnh phần cứng REP MOVSB
📝 **Cần giải thích:**
- [ ] `Buffer.MemCopy` được JIT compile thành lệnh x86 `REP MOVSB` hoặc `MOVSX`.
- [ ] Intel ERMSB (Enhanced REP MOVSB): vi mã tích hợp bên trong chip, copy khối RAM ở tốc độ bus phần cứng tối đa.
- [ ] Ứng dụng: ShiftSort dịch mảng KeyEntry, RawList.RemoveAt dịch phần tử, GrowUp copy mảng cũ sang mảng mới.

📐 **Hình cần tạo:** "MemCopy Hardware Path: C# → JIT → x86 REP MOVSB"

---

### 4.5 Hashing & Bitwise Layer (Math.cs) ❌ (CẦN BỔ SUNG)

#### 4.5.1 Binary Utilities (RoundUpToPowerOf2, Log2)
📝 **Cần giải thích:**
- [ ] `RoundUpToPowerOf2`: Kỹ thuật Bit-Smearing — lấp đầy tất cả bit 1 bên phải bit cao nhất, rồi +1.
- [ ] `Log2` (Trailing Zero Count): Thuật toán De Bruijn Sequence hoặc bitwise divide-and-conquer.
- [ ] Ứng dụng: Tính Bin Index cho Slab Allocator trong O(1).

🔢 **Công thức cần chứng minh:**
- Bit-Smearing sequence:
  ```
  size |= size >> 1    →  Lấp 2 bit cao nhất
  size |= size >> 2    →  Lấp 4 bit
  size |= size >> 4    →  Lấp 8 bit
  size |= size >> 8    →  Lấp 16 bit
  size |= size >> 16   →  Lấp 32 bit
  size += 1            →  Làm tròn lên lũy thừa 2
  ```
- Chứng minh: Sau 5 bước dịch, mọi bit từ vị trí MSB trở xuống đều = 1, nên `size + 1` = lũy thừa 2 tiếp theo.

📐 **Hình cần tạo:** "Bit-Smearing Visualization — RoundUpToPowerOf2 step-by-step"

---

#### 4.5.2 XXHash64 Dual-Hash (Single-Pass 128-bit)
📝 **Cần giải thích chi tiết:**
- [ ] Cơ chế Single-Pass Dual-Seed: 2 trạng thái hash (`state1` seed=0, `state2` seed=length⊕MAGIC) chạy song song trong cùng 1 vòng lặp `while`.
- [ ] Mỗi lần đọc 8 bytes từ RAM, cả 2 state đều được cập nhật → Tiết kiệm 50% bandwidth RAM so với chạy 2 lần riêng.
- [ ] Hàm Avalanche: trộn bit cuối cùng để đảm bảo phân bố đều.

🔢 **Công thức cần trình bày:**
- XXHash64 core mixing:
  $$\text{state} = \text{RotateLeft}(\text{state} + \text{input} \times P2, 31) \times P1$$
- Avalanche finalization:
  $$h = h \oplus (h \gg 33); \quad h = h \times P2; \quad h = h \oplus (h \gg 29); \quad h = h \times P3; \quad h = h \oplus (h \gg 32)$$
- Xác suất va chạm 128-bit:
  $$P(\text{collision among } n \text{ keys}) \approx \frac{n^2}{2^{129}} \approx 0 \text{ cho } n < 10^{18}$$

📊 **Bảng Benchmark cần chạy:** "XXHash64 Throughput vs Other Hash Functions"

| Hash Function | Key Size | Throughput (GB/s) | Cycles/Byte | Collision Rate (10M keys) |
|--------------|----------|-------------------|-------------|--------------------------|
| XXHash64     | 16 B     | ? GB/s            | ? c/B       | 0 / 10M                  |
| XXHash64     | 64 B     | ? GB/s            | ? c/B       | 0 / 10M                  |
| MurmurHash3  | 16 B     | ? GB/s            | ? c/B       | ? / 10M                  |
| CityHash64   | 16 B     | ? GB/s            | ? c/B       | ? / 10M                  |
| MD5          | 16 B     | ? GB/s            | ? c/B       | 0 / 10M                  |

---

#### 4.5.3 FastMD5 (Unrolled Stackalloc MD5)
📝 **Cần giải thích:**
- [ ] RFC 1321 MD5: 64 bước (4 rounds × 16 steps), mỗi bước gồm: F/G/H/I function + bitwise rotation + addition.
- [ ] Tại sao "Fast": Unrolled hoàn toàn 64 bước (không dùng vòng lặp) + `stackalloc` (zero heap allocation).
- [ ] Ứng dụng trong VstHelper: Sinh key cho `PrimaryMode.LongString` (chuỗi dài > 16 bytes).

📚 **Tham khảo:** RFC 1321 — The MD5 Message-Digest Algorithm (Rivest 1992)

---

### 4.6 RawList<T> — Zero-GC Dynamic Array ❌ (CẦN BỔ SUNG)

📝 **Cần giải thích:**
- [ ] Cấu trúc: struct 16 bytes (`T* _items`, `int _count`, `int _capacity`).
- [ ] Bộ nhớ mảng cấp phát hoàn toàn từ Slab Allocator (`Memory.Rent/Return`) → Zero GC.
- [ ] Hệ số mở rộng 1.5x (`_capacity + (_capacity >> 1)`) thay vì 2x → Tiết kiệm 33% RAM.
- [ ] `Prepare(n)` — đặt chỗ n ô trống 1 lần, trả về pointer để ghi trực tiếp → giảm n-1 lần bounds check.
- [ ] `RemoveAt(index)` — dịch RAM bằng `Buffer.MemCopy` (REP MOVSB) thay vì vòng lặp.

📐 **Hình cần tạo:** "RawList<T> Memory Layout & GrowUp 1.5x Process"

🔢 **Phân tích cần viết:**
- So sánh hệ số mở rộng:
  - 2x: Tổng RAM lãng phí = $\sum_{i=0}^{k} 2^i = 2^{k+1} - 1 \approx 2N$ (lãng phí ~50%)
  - 1.5x: Tổng RAM lãng phí ≈ $1.33N$ (lãng phí ~33%)
- Amortized Push cost: O(1) cho cả 2 hệ số.

📊 **Bảng so sánh cần tạo:** "RawList<T> vs List<T> (.NET)"

| Tiêu chí | `RawList<T>` (VstHelper) | `List<T>` (.NET) |
|----------|--------------------------|------------------|
| Heap Allocation | Zero (Unmanaged) | Managed Heap (GC tracked) |
| Growth Factor | 1.5x | 2x |
| Bounds Check | Không (unsafe pointer) | Có (mỗi lần truy cập) |
| GC Pressure | 0 bytes | Mỗi GrowUp tạo mảng mới trên Managed Heap |
| Push Latency | ~1 ns (1 lệnh MOV) | ~5-10 ns (bounds check + GC barrier) |

---

### 4.7 ObjectId — 12-byte Unique Identifier ❌ (CẦN BỔ SUNG)

📝 **Cần giải thích:**
- [ ] Cấu trúc 12 bytes: Timestamp (4B) + Machine (3B) + PID (2B) + Counter (3B).
- [ ] `Interlocked.Increment` (lệnh CPU `LOCK XADD`) → Atomic, an toàn đa luồng, không cần Mutex.
- [ ] Bitmask `& 0xffffff` cắt giữ 24-bit Counter ($2^{24} \approx 16.7$ triệu ID/giây).
- [ ] Bitwise OR `_info | increment` ghép các trường vào đúng vị trí bit.
- [ ] `GetStoreHash()` nén 12B → 8B bằng XXHash64 để lưu vào KeyEntry.Key.
- [ ] So sánh với UUID v4 (16B) và MongoDB ObjectId (12B).

📐 **Hình cần tạo:** "ObjectId 12-byte Bit Layout Diagram"

```
Byte:  0    1    2    3    4    5    6    7    8    9   10   11
     ┌─────────────────┬──────────────────┬─────────┬──────────────┐
     │   Timestamp     │    Machine Hash  │   PID   │   Counter    │
     │   (4 Bytes)     │    (3 Bytes)     │ (2 Bytes)│  (3 Bytes)  │
     │   Unix UTC      │   & 0xffffff     │ & 0xffff│ Interlocked  │
     └─────────────────┴──────────────────┴─────────┴──────────────┘
```

📊 **Bảng so sánh cần tạo:** "ObjectId vs UUID vs Snowflake"

| Tiêu chí | VstHelper ObjectId | UUID v4 | MongoDB ObjectId | Twitter Snowflake |
|----------|-------------------|---------|-----------------|------------------|
| Kích thước | 12 Bytes | 16 Bytes | 12 Bytes | 8 Bytes |
| Sắp xếp theo thời gian | ✅ Có | ❌ Không | ✅ Có | ✅ Có |
| Thread-safe | ✅ `Interlocked` | ✅ Random | ✅ `Interlocked` | ✅ |
| Unique/giây | 16.7M / process | Vô hạn (random) | 16.7M / process | 4096 / ms |
| Cần server trung tâm | ❌ Không | ❌ Không | ❌ Không | ✅ Cần |

---

### 4.8 StringWriter & StringReader (Zero-GC Serialization) ❌ (CẦN BỔ SUNG)

📝 **Cần giải thích:**
- [ ] **StringWriter:** Struct 24B (`byte* Start, Current, End`), ghi trực tiếp vào Unmanaged buffer.
  - `Write2Digit` lookup table: Ghi 2 chữ số ASCII trong 1 lệnh `ushort` write (thay vì 2 lệnh).
  - `Write(long)`: Chia ngược backward by 100 (`value / 100`, `value % 100`) + `Write2Digit`.
  - JSON auto-escaping: Xử lý `\"`, `\\`, `\n`, `\t` trực tiếp trên buffer.
- [ ] **StringReader:** Struct 24B (`byte* Handle, Current, End`), parse trực tiếp từ Unmanaged buffer.
  - Bitwise `b & 15`: Chuyển ASCII char '0'-'9' thành số 0-9 trong 1 lệnh AND.
  - Nhân 10 bằng bit shift: `(x << 1) + (x << 3)` = `2x + 8x = 10x` (tránh lệnh MUL chậm).
  - `stackalloc` cho JSON unescape buffer → zero heap allocation.

🔢 **Công thức cần chứng minh:**
- `b & 15` = `b & 0x0F`: ASCII '0' = 0x30, '1' = 0x31, ..., '9' = 0x39 → `0x3X & 0x0F = X`.
- `(x << 1) + (x << 3)` = $2x + 8x = 10x$.

---

# ═══════════════════════════════════════════════════════════
# SECTION 5: METHODOLOGY & BENCHMARK SETTINGS
# ═══════════════════════════════════════════════════════════

### Trạng thái: ✅ Đã có — Cần bổ sung test case cho tầng mới

---

### 5.1 Experimental Environment ✅ (Đã có)

### 5.2 Baseline Algorithms ✅ (Đã có — cần bổ sung)
📝 **Bổ sung baseline cho tầng mới:**
- [ ] **Hash baseline:** So sánh XXHash64 vs MurmurHash3 vs CityHash vs System.HashCode.
- [ ] **Allocator baseline:** So sánh Slab Allocator vs `Marshal.AllocHGlobal` vs `new byte[]` vs ArrayPool.
- [ ] **List baseline:** So sánh RawList<T> vs List<T> vs ArrayPool-backed list.
- [ ] **ID baseline:** So sánh ObjectId.NewId() vs Guid.NewGuid() vs custom Snowflake.

### 5.3 Metrics ✅ (Đã có — cần bổ sung)
📝 **Bổ sung metrics:**
- [ ] Hash throughput (GB/s), Cycles/byte.
- [ ] Allocator rent/return latency (ns), throughput (ops/s).
- [ ] RawList Push/Pop throughput (ops/s), GrowUp latency (ns).
- [ ] ObjectId generation throughput (ops/s), uniqueness verification.

### 5.4 Test Configurations ❌ (CẦN BỔ SUNG)
📊 **Bảng cần tạo:** "Test Matrix"

| Test | Input Size | Iterations | Metric |
|------|-----------|-----------|--------|
| Sort Performance | 10K, 100K, 1M, 10M | 30 | Time (ms), Speedup |
| Hash Throughput | 8B, 16B, 32B, 64B, 256B, 1KB | 30 | GB/s, Cycles/byte |
| Allocator Stress | 1M rent+return | 30 | Latency (ns), Throughput |
| RawList Push | 1M pushes | 30 | Throughput (ops/s) |
| ObjectId Gen | 10M IDs | 30 | Throughput (ops/s), Uniqueness |

---

# ═══════════════════════════════════════════════════════════
# SECTION 6: RESULTS & EVALUATION
# ═══════════════════════════════════════════════════════════

### Trạng thái: ✅ Có 4 RQ results — Cần thêm 2 RQ results mới

---

### 6.1 RQ1: Cache-Efficient Memory Layout ✅ (Đã có)
### 6.2 RQ2: Parallel Sort Performance ✅ (Đã có)
### 6.3 RQ3: Multi-core Scalability ✅ (Đã có)
### 6.4 RQ4: Real-time Latency Guarantees ✅ (Đã có)
### 6.5 Threshold Analysis ✅ (Đã có)

---

### 6.6 RQ5: Hash Quality & Throughput Results ❌ (CẦN BỔ SUNG)
📊 **Bảng kết quả cần chạy Benchmark:**
- XXHash64 throughput tại các key sizes.
- Chi-squared uniformity test trên 10M keys.
- Collision count / 10M keys.

📐 **Hình cần tạo:**
- Figure: "Hash Throughput Comparison (GB/s)" — Bar chart.
- Figure: "Hash Distribution Uniformity" — Histogram 256 bins.

---

### 6.7 RQ6: Memory Allocator Efficiency Results ❌ (CẦN BỔ SUNG)
📊 **Bảng kết quả cần chạy Benchmark:**
- Slab Allocator rent/return latency (ns) so với malloc/free, new/GC.
- Throughput (ops/s) tại single-thread và multi-thread.
- Memory fragmentation ratio.

📐 **Hình cần tạo:**
- Figure: "Allocator Latency Distribution" — Box plot.
- Figure: "Allocator Throughput vs Thread Count" — Line chart.

---

### 6.8 Threats to Validity ✅ (Đã có — cần bổ sung)

---

# ═══════════════════════════════════════════════════════════
# SECTION 7: COMPLEXITY ANALYSIS
# ═══════════════════════════════════════════════════════════

### Trạng thái: ✅ Đã có Sort complexity — Cần bổ sung cho các tầng khác

📊 **Bảng tổng hợp cần tạo:** "Time & Space Complexity Summary"

| Component | Time (Average) | Time (Worst) | Space | Ghi chú |
|-----------|---------------|-------------|-------|---------|
| KeyIndexer.Sort | $O(n \log n)$ | $O(n^2)$ | $O(\log n)$ stack | Median-of-three giảm worst-case probability |
| KeyIndexer.Find | $O(\log n)$ | $O(\log n)$ | $O(1)$ | Binary Search |
| Slab Allocator Rent | $O(1)$ amortized | $O(k)$ contention | $O(1)$ | k = competing threads |
| Slab Allocator Return | $O(1)$ | $O(k)$ contention | $O(1)$ | Lock-free CAS |
| XXHash64 | $O(n)$ | $O(n)$ | $O(1)$ | n = key length in bytes |
| RawList.Push | $O(1)$ amortized | $O(n)$ GrowUp | $O(n)$ | 1.5x growth factor |
| RawList.RemoveAt | $O(n)$ | $O(n)$ | $O(1)$ | MemCopy shift |
| ObjectId.NewId | $O(1)$ | $O(1)$ | $O(1)$ | Atomic Interlocked |
| Binary.Log2 | $O(1)$ | $O(1)$ | $O(1)$ | Branchless bitwise |
| Binary.RoundUpToPowerOf2 | $O(1)$ | $O(1)$ | $O(1)$ | 5 shift-OR ops |

---

# ═══════════════════════════════════════════════════════════
# SECTION 8: DISCUSSION & FUTURE WORK
# ═══════════════════════════════════════════════════════════

### Trạng thái: ❌ Cần viết mới hoàn toàn

---

### 8.1 Discussion — Phân tích sâu kết quả
📝 **Cần viết:**
- [ ] Tại sao Zero-GC quan trọng cho Real-time: So sánh biến thiên latency (StdDev) giữa KeyIndexer (0.08ms) vs Array.Sort (4.97ms).
- [ ] Tại sao InsertionSort threshold 128 không hiệu quả trên CPU hiện đại: Out-of-order execution, lớn L1 cache (48KB Tiger Lake).
- [ ] Trade-off giữa kích thước KeyEntry 32B và thông tin lưu trữ: 32B là tối thiểu cần thiết cho 128-bit key + disk offset + metadata.
- [ ] Hạn chế của kiến trúc hiện tại: Chưa có Compaction/Vacuum cho dữ liệu Deleted, chưa có Transaction/ACID.

---

### 8.2 Future Work — Hướng phát triển
📝 **Cần viết (5 hướng):**
- [ ] **Nâng cấp .NET 6+:** Tận dụng `BitOperations.TrailingZeroCount` (TZCNT), `Vector128` SIMD, Tiered Compilation JIT.
- [ ] **Compaction / Vacuum:** Dọn dẹp bản ghi `Deleted` (Tombstone), thu hồi dung lượng đĩa SSD.
- [ ] **SIMD Sorting:** AVX2/AVX-512 cho partition step trong QuickSort.
- [ ] **Adaptive Threshold:** Tự động điều chỉnh InsertionSort threshold dựa trên kích thước L1 Cache thực tế.
- [ ] **Cross-platform ARM validation:** Kiểm chứng trên Apple M1/M2, Raspberry Pi (ARM Cortex).

---

# ═══════════════════════════════════════════════════════════
# SECTION 9: CONCLUSION
# ═══════════════════════════════════════════════════════════

### Trạng thái: ✅ Đã có — Cần cập nhật nếu mở rộng phạm vi

📝 **Cấu trúc Conclusion chuẩn IEEE (3-4 câu):**
1. Tóm tắt lại vấn đề và giải pháp (1 câu).
2. Liệt kê các kết quả chính (2-3 bullets).
3. Ý nghĩa thực tiễn và hướng phát triển (1 câu).

---

# ═══════════════════════════════════════════════════════════
# APPENDIX: PHỤ LỤC
# ═══════════════════════════════════════════════════════════

### A. Source Code Listings ✅ (Đã có — cần bổ sung)
- [ ] KeyEntry struct (đã có)
- [ ] KeyIndexer.Sort pseudo-code (đã có)
- [ ] **(Mới)** Memory.Rent / Memory.Return pseudo-code
- [ ] **(Mới)** XXHash64.ComputeDualHashShort pseudo-code
- [ ] **(Mới)** ObjectId.NewId() pseudo-code
- [ ] **(Mới)** RawList<T>.GrowUp pseudo-code

### B. Benchmark Configuration ✅ (Đã có)

### C. Raw Benchmark Data ❌ (CẦN BỔ SUNG)
📊 Bảng raw data cho mỗi experiment (30 iterations × n configurations).

---

# ═══════════════════════════════════════════════════════════
# CHECKLIST TỔNG HỢP — DANH SÁCH HÌNH VẼ CẦN TẠO
# ═══════════════════════════════════════════════════════════

| # | Tên Figure | Section | Trạng thái |
|---|-----------|---------|-----------|
| 1 | VstHelper 6-Layer Architecture Overview | 4.0 | ❌ Chưa tạo |
| 2 | Slab Allocator Internal Architecture | 4.4.1 | ❌ Chưa tạo |
| 3 | Lock-free CAS Rent Flow Diagram | 4.4.2 | ❌ Chưa tạo |
| 4 | MemCopy Hardware Path (C# → JIT → REP MOVSB) | 4.4.3 | ❌ Chưa tạo |
| 5 | Bit-Smearing Visualization — RoundUpToPowerOf2 | 4.5.1 | ❌ Chưa tạo |
| 6 | XXHash64 Dual-Hash Single-Pass Pipeline | 4.5.2 | ❌ Chưa tạo |
| 7 | RawList<T> Memory Layout & GrowUp 1.5x | 4.6 | ❌ Chưa tạo |
| 8 | ObjectId 12-byte Bit Layout | 4.7 | ❌ Chưa tạo |
| 9 | Hash Throughput Comparison (Bar Chart) | 6.6 | ❌ Chưa tạo |
| 10 | Hash Distribution Uniformity (Histogram) | 6.6 | ❌ Chưa tạo |
| 11 | Allocator Latency Distribution (Box Plot) | 6.7 | ❌ Chưa tạo |
| 12 | Allocator Throughput vs Thread Count | 6.7 | ❌ Chưa tạo |
| 13 | Sort Time Comparison | 6.2 | ✅ Đã có |
| 14 | Scalability Speedup | 6.3 | ✅ Đã có |
| 15 | Latency Distribution | 6.4 | ✅ Đã có |
| 16 | Heap Allocation Comparison | 6.4 | ✅ Đã có |

---

# ═══════════════════════════════════════════════════════════
# CHECKLIST TỔNG HỢP — DANH SÁCH BẢNG CẦN TẠO
# ═══════════════════════════════════════════════════════════

| # | Tên Table | Section | Trạng thái |
|---|----------|---------|-----------|
| 1 | Comparison of Indexing Approaches | 2.3 | ✅ Đã có |
| 2 | KeyEntry 32-byte Memory Layout | 4.2 | ✅ Đã có |
| 3 | Experimental Environment | 5.1 | ✅ Đã có |
| 4 | Sort Performance (Speedup) | 6.2 | ✅ Đã có |
| 5 | Parallel Scalability | 6.3 | ✅ Đã có |
| 6 | Latency Statistics | 6.4 | ✅ Đã có |
| 7 | Threshold Analysis | 6.5 | ✅ Đã có |
| 8 | Slab Allocator Bin Configuration | 4.4.1 | ❌ Chưa tạo |
| 9 | RawList vs List (.NET) | 4.6 | ❌ Chưa tạo |
| 10 | ObjectId vs UUID vs Snowflake | 4.7 | ❌ Chưa tạo |
| 11 | XXHash64 Throughput Comparison | 6.6 | ❌ Chưa tạo |
| 12 | Allocator Benchmark Results | 6.7 | ❌ Chưa tạo |
| 13 | Time & Space Complexity Summary | 7 | ❌ Chưa tạo |
| 14 | Test Matrix | 5.4 | ❌ Chưa tạo |

---

# ═══════════════════════════════════════════════════════════
# CHECKLIST TỔNG HỢP — TÀI LIỆU THAM KHẢO CẦN TÌM
# ═══════════════════════════════════════════════════════════

| # | Tham khảo | Dùng ở Section | Trạng thái |
|---|----------|---------------|-----------|
| 1 | Hoare 1962 — QuickSort | 2.1 | ✅ Đã có |
| 2 | LaMarca 1999 — Cache-conscious sorting | 2.2 | ✅ Đã có |
| 3 | Rao 2000 — Making B+-Trees cache conscious | 2.3 | ✅ Đã có |
| 4 | Maeda 2012 — GC impact on real-time | 2.4 | ✅ Đã có |
| 5 | Sanders 2009 — Parallel sorting | 2.1 | ✅ Đã có |
| 6 | Aumüller 2013 — Randomized sorting | 2.1 | ✅ Đã có |
| 7 | BenchmarkDotNet | 5.1 | ✅ Đã có |
| 8 | Yann Collet — XXHash | 2.5 | ❌ Cần tìm |
| 9 | Bonwick 1994 — Slab Allocator | 2.6 | ❌ Cần tìm |
| 10 | Knuth — Art of Programming Vol.3 (Hashing) | 2.5 | ❌ Cần tìm |
| 11 | RFC 1321 — MD5 (Rivest 1992) | 4.5.3 | ❌ Cần tìm |
| 12 | MongoDB ObjectId Specification | 2.7 | ❌ Cần tìm |
| 13 | RFC 9562 — UUID v7 | 2.7 | ❌ Cần tìm |
| 14 | jemalloc / tcmalloc / mimalloc papers | 2.6 | ❌ Cần tìm |
| 15 | Intel ERMSB (Enhanced REP MOVSB) | 4.4.3 | ❌ Cần tìm |
