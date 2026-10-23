## 1. Xác Nhận Môi Trường (Environment Verification)

Môi trường phát triển và kiểm thử hệ điều hành `xv6-riscv` đã được thiết lập và xác minh hoạt động ổn định trên WSL2 (Ubuntu Linux):

### 1.1. Công cụ biên dịch & Giả lập
* **Trình biên dịch chéo (Cross-compiler):** `riscv64-linux-gnu-gcc`
* **Trình liên kết (Linker):** `riscv64-linux-gnu-ld`
* **Trình gỡ lỗi (Debugger):** `gdb-multiarch` (kết nối qua `.gdbinit.tmpl-riscv`)
* **Trình giả lập phần cứng:** `qemu-system-riscv64` (chạy đa nhân với cấu hình `CPUS := 3` hoặc tối đa `NCPU = 8`)

### 1.2. Quy trình kiểm tra Baseline
Đã thực hiện biên dịch sạch và khởi chạy thành công nhân `xv6` nguyên bản:
```bash
make clean
make qemu
```
* **Kết quả khởi động:** Hệ thống nạp thành công `kernel/kernel`, đóng gói ổ đĩa ảo `fs.img` qua `mkfs/mkfs.c`, khởi chạy các lõi CPU (`hart 0`, `hart 1`, `hart 2`) và vào dấu nhắc lệnh `$` của chương trình `init` $\rightarrow$ `sh`.
* **Kiểm thử Baseline:** Chạy `forktest` và `usertests` trên bản gốc đạt trạng thái ổn định (`ALL TESTS PASSED`), sẵn sàng làm mốc cơ sở đối sánh (`docs/xv6-baseline.json`).

---

## 2. Phân Tích API Quản Lý Trang Vật Lý (`kernel/kalloc.c`)

Bộ cấp phát trang vật lý của `xv6-riscv` quản lý toàn bộ vùng RAM trống từ ngay sau nhân hệ điều hành (`end`) đến đỉnh RAM vật lý (`PHYSTOP`) theo đơn vị từng trang $4096\text{ bytes}$ ($4\text{ KB}$).


## 3. Thiết Kế Slab Allocator Một Class (Single-Class Slab)

Bộ cấp phát trang `kalloc()` chỉ cấp phát theo bội số nguyên trang $4096\text{ bytes}$. Nếu nhân hệ điều hành cần cấp phát các đối tượng nhỏ có kích thước cố định (ví dụ: cấu trúc $32\text{ bytes}$, $64\text{ bytes}$ hoặc $128\text{ bytes}$), việc dùng nguyên 1 trang $4\text{ KB}$ gây phân mảnh nội vi (*internal fragmentation*) rất lớn.

Bản thiết kế dưới đây đặc tả một **Single-Class Slab Allocator** đặt trực tiếp trên 1 trang vật lý $4096\text{ bytes}$ lấy từ `kalloc()`.

### 3.1. Sơ Đồ Bố Trí Bộ Nhớ (Metadata & Slots Layout)
Mỗi trang $4096\text{ bytes}$ được chia thành 2 vùng: **Slab Header (Metadata)** đặt ngay tại địa chỉ đầu trang (căn lề $8\text{ bytes}$) và **Mảng các Slot (Objects)** nối tiếp ngay phía sau. Khi một Slot đang trống, $8\text{ bytes}$ đầu tiên của Slot đó lưu con trỏ nhúng (`struct slab_slot *next`) trỏ tới Slot trống tiếp theo trong cùng trang.

```text
ĐỊA CHỈ TRANG VẬT LÝ (Lấy từ kalloc(), PSIZE = 4096 bytes, chia hết cho 4096)
0x80200000 +-------------------------------------------------------------------+
           | SLAB METADATA HEADER (struct slab_header - Kích thước M = 32 B)   |
           |  - magic          (uint32 : 4B) : 0x534C4142 ("SLAB")             |
           |  - obj_size       (uint16 : 2B) : Kích thước đã căn lề S_aligned  |
           |  - capacity       (uint16 : 2B) : Tổng số slot tối đa trong trang |
           |  - free_count     (uint16 : 2B) : Số slot hiện đang trống         |
           |  - reserved_pad   (uint16 : 6B) : Đệm căn lề chẵn 8 bytes         |
           |  - freelist       (struct slab_slot* : 8B) : Đầu danh sách slot   |
           |  - next_slab      (struct slab_header*: 8B): Nối sang trang Slab  |
0x80200020 +-------------------------------------------------------------------+
           | vùng SLOTS DỮ LIỆU (Bắt đầu tại offset M = 32B, chia hết cho 8)   |
           | ┌───────────────────────────────────────────────────────────────┐ |
           | │ Slot 0 (Kích thước S_aligned bytes)                           │ |
           | │  [ Khi trống: 8B đầu là con trỏ next ──► trỏ xuống Slot 1 ]   │ |
           | ├───────────────────────────────────────────────────────────────┤ |
           | │ Slot 1 (Kích thước S_aligned bytes)                           │ |
           | │  [ Khi trống: 8B đầu là con trỏ next ──► trỏ xuống Slot 2 ]   │ |
           | ├───────────────────────────────────────────────────────────────┤ |
           | │ ...                                                           │ |
           | ├───────────────────────────────────────────────────────────────┤ |
           | │ Slot (capacity - 1)                                           │ |
           | │  [ Khi trống: 8B đầu là con trỏ next = 0 (NULL) ]             │ |
           | └───────────────────────────────────────────────────────────────┘ |
           +-------------------------------------------------------------------+
           | TAIL PADDING (Phần dư cuối trang < S_aligned, không sử dụng)      |
0x80201000 +-------------------------------------------------------------------+
```

### 3.2. Tính Toán Căn Lề (Alignment) & Sức Chứa (Capacity)

#### A. Quy tắc Căn lề (Alignment = $8\text{ bytes}$ trên RISC-V 64-bit)
Trên kiến trúc `riscv64`, con trỏ và thanh ghi đa dụng có độ rộng $64\text{-bit}$ ($8\text{ bytes}$). Do mỗi Slot khi trống phải chứa được ít nhất 1 con trỏ nhúng `struct slab_slot *next` ($8\text{ bytes}$) và tránh lỗi truy cập lệch biên (*unaligned memory access*), kích thước thực tế của mỗi Slot ($S_{\text{aligned}}$) bắt buộc phải thỏa mãn:
1. Tối thiểu bằng $8\text{ bytes}$: $S_{\min} = \max(S_{\text{req}}, 8)$.
2. Làm tròn lên bội số của $A = 8\text{ bytes}$:
$$S_{\text{aligned}} = \left\lfloor \frac{\max(S_{\text{req}}, 8) + A - 1}{A} \right\rfloor \times A \quad (\text{với } A = 8)$$

Trong code C, công thức này được viết bằng phép toán bit:
```c
#define SLAB_ALIGN 8UL
#define ALIGN_UP(sz) (((sz) + SLAB_ALIGN - 1) & ~(SLAB_ALIGN - 1))
```

#### B. Công thức Tính Sức chứa (Capacity) trên 1 Trang $4096\text{ bytes}$
Gọi:
* $P = \text{PGSIZE} = 4096\text{ bytes}$ là kích thước 1 trang vật lý từ `kalloc()`.
* $M = \text{ALIGN\_UP}(\text{sizeof(struct slab\_header)}) = 32\text{ bytes}$ là kích thước vùng Metadata đầu trang.
* Không gian còn lại dành cho vùng Slots là $D = P - M = 4096 - 32 = 4064\text{ bytes}$.

Số lượng đối tượng tối đa (**Capacity** — $N$) mà một trang Slab chứa được là:
$$N = \left\lfloor \frac{P - M}{S_{\text{aligned}}} \right\rfloor = \left\lfloor \frac{4064}{S_{\text{aligned}}} \right\rfloor$$

Phần bộ nhớ dư ở cuối trang (Tail Padding / Internal Fragmentation của trang) là:
$$R_{\text{waste}} = (P - M) - (N \times S_{\text{aligned}}) = (4064 \bmod S_{\text{aligned}})\text{ bytes}$$

#### C. Bảng Tính Mẫu cho Các Kích Thước Đối Tượng Phổ Biến
| Kích thước yêu cầu ($S_{\text{req}}$) | Kích thước sau căn lề ($S_{\text{aligned}}$) | Header ($M$) | Dung lượng khả dụng ($P - M$) | Sức chứa $N$ (`capacity`) | Phần dư cuối trang ($R_{\text{waste}}$) | Hiệu suất sử dụng trang |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| $12\text{ B}$ | $16\text{ B}$ | $32\text{ B}$ | $4064\text{ B}$ | $254\text{ slots}$ | $0\text{ B}$ | $99.22\%$ |
| $32\text{ B}$ | $32\text{ B}$ | $32\text{ B}$ | $4064\text{ B}$ | $127\text{ slots}$ | $0\text{ B}$ | $99.22\%$ |
| $64\text{ B}$ | $64\text{ B}$ | $32\text{ B}$ | $4064\text{ B}$ | $63\text{ slots}$ | $32\text{ B}$ | $98.44\%$ |
| $128\text{ B}$ | $128\text{ B}$ | $32\text{ B}$ | $4064\text{ B}$ | $31\text{ slots}$ | $96\text{ B}$ | $96.88\%$ |
| $256\text{ B}$ | $256\text{ B}$ | $32\text{ B}$ | $4064\text{ B}$ | $15\text{ slots}$ | $224\text{ B}$ | $93.75\%$ |

---

### 3.3. Quy Trình Khởi Tạo (`init` Path)

Quy trình khởi tạo gồm 2 tầng: khởi tạo bộ quản lý Class (`slab_cache_init`) lúc boot nhân và khởi tạo một trang Slab mới (`slab_page_init`) khi cần mở rộng:

```c
#define SLAB_MAGIC 0x534C4142U // "SLAB"

struct slab_slot {
  struct slab_slot *next;
};

struct slab_header {
  uint32 magic;
  uint16 obj_size;
  uint16 capacity;
  uint16 free_count;
  uint16 pad[3];
  struct slab_slot *freelist;
  struct slab_header *next_slab;
};

struct slab_cache {
  struct spinlock lock;
  uint16 obj_size;
  uint16 capacity_per_page;
  struct slab_header *partial; // Danh sách trang còn slot trống
  struct slab_header *full;    // Danh sách trang đã cấp phát hết
};
```

#### Các bước thực hiện trong `init` Path:
1. **Bước 1 — Kiểm tra và chuẩn hóa tham số (`Validation & Alignment`):**
   * Tính $S_{\text{aligned}} = \text{ALIGN\_UP}(\max(\text{req\_size}, 8))$.
   * Kiểm tra ràng buộc kích thước: Nếu $S_{\text{aligned}} == 0$ hoặc $M + S_{\text{aligned}} > \text{PGSIZE}$ (không chứa nổi 1 slot trong 1 trang $4\text{ KB}$), chuyển ngay sang nhánh lỗi.
2. **Bước 2 — Khởi tạo khóa đồng bộ (`Spinlock Init`):**
   * Gọi `initlock(&cache->lock, "slab_cache")` để bảo vệ cấu trúc quản lý class.
   * Tính trước `cache->capacity_per_page` $= (4096 - 32) / S_{\text{aligned}}$.
3. **Bước 3 — Xin trang vật lý và thiết lập `slab_header` (`Page Initialization`):**
   * Gọi `void *page = kalloc();` để lấy 1 trang $4096\text{ bytes}$ căn lề chuẩn từ bộ cấp phát trang.
   * Đặt `struct slab_header *hdr = (struct slab_header*)page;`.
   * Ghi `hdr->magic = SLAB_MAGIC`, `hdr->obj_size = S_aligned`, `hdr->capacity = N`, `hdr->free_count = N`.
4. **Bước 4 — Xây dựng danh sách liên kết các Slot trống (`Embedded Free List`):**
   * Bắt đầu từ địa chỉ `char *base = (char*)page + sizeof(struct slab_header);`.
   * Chạy vòng lặp từ $i = 0$ đến $N - 1$: mỗi bước nhảy `base + i * S_aligned` được ép kiểu thành `struct slab_slot*` và nối con trỏ `next` sang slot $i + 1$ (slot cuối cùng trỏ về `0`).

---

### 3.4. Đường Dẫn Xử Lý Lỗi (`Failure Path`)

Mọi tình huống bất thường trong quá trình khởi tạo, cấp phát (`slab_alloc`) và thu hồi (`slab_free`) đều được định nghĩa tường minh để tránh hỏng vùng nhớ Kernel:

| Tình huống lỗi | Điều kiện kích hoạt | Xử lý tại `Failure Path` |
| :--- | :--- | :--- |
| **1. Kích thước đối tượng không hợp lệ lúc `init`** | `req_size == 0` hoặc `sizeof(struct slab_header) + S_aligned > PGSIZE` | Gọi `panic("slab_init: invalid size")` vì cấu hình tĩnh của class sai ngay từ lúc khởi động Kernel. |
| **2. Cạn kiệt RAM vật lý (`kalloc` trả về `0`)** | Tất cả các trang trong `cache->partial` đều đầy và lệnh gọi `kalloc()` xin trang mới trả về `0` (`NULL`). | Không `panic` hệ thống; nhả khóa `release(&cache->lock)` ngay lập tức và trả về `0` (`NULL`) để tầng gọi (ví dụ syscall) xử lý lỗi thiếu bộ nhớ một cách an toàn. |
| **3. Giải phóng con trỏ ngoài vùng hợp lệ (`slab_free`)** | `ptr == 0`, hoặc `(char*)ptr < end`, hoặc `(uint64)ptr >= PHYSTOP`. | Gọi `panic("slab_free: range")` giống quy chuẩn của `kfree()` để chặn ngay lỗi con trỏ rác. |
| **4. Giải phóng con trỏ không thuộc trang Slab hoặc lệch biên Slot** | Tìm đầu trang chứa đối tượng bằng `hdr = (struct slab_header*)PGROUNDDOWN((uint64)ptr)` nhưng `hdr->magic != SLAB_MAGIC` hoặc `((char*)ptr - slot_base) % hdr->obj_size != 0`. | Gọi `panic("slab_free: bad ptr or unaligned slot")` vì con trỏ truyền vào không phải địa chỉ đầu của một Slot hợp lệ. |
| **5. Giải phóng kép / Tràn số lượng Slot (`Double Free` / `Overflow`)** | Khi gọi `slab_free(ptr)` nhưng `hdr->free_count >= hdr->capacity`. | Gọi `panic("slab_free: double free or corrupt count")` để ngăn việc chèn trùng lặp làm tạo vòng lặp vô tận trong `freelist`. |

---

