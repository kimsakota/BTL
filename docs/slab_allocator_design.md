# Bản Thiết Kế Kỹ Thuật: Cấu Trúc Quản Lý Bộ Nhóm (Slab Allocator - Single Class)

Bản thiết kế này quy định chi tiết cấu trúc dữ liệu, sơ đồ bố trí bộ nhớ (metadata/slots), cơ chế tính toán sức chứa (capacity), căn lề (alignment), quy trình khởi tạo (`init`) và các nhánh xử lý lỗi (`failure path`) cho một bộ cấp phát bộ nhớ dạng Slab (tương thích mô hình NoGC / Memory Pool hiệu năng cao).

---

## 1. Tổng Quan Kiến Trúc (Architecture Overview)

Slab Allocator là một chiến lược cấp phát bộ nhớ dựa trên việc chia nhỏ một khối bộ nhớ lớn (gọi là một *Page* hoặc *Slab Buffer*) thành nhiều ô nhỏ có kích thước cố định bằng nhau gọi là *Slot* (hoặc *Object*). Thiết kế này hướng tới việc loại bỏ hoàn toàn phân mảnh ngoại vi, đạt độ phức tạp thời gian cấp phát/thu hồi là $\mathcal{O}(1)$ và tối ưu hóa bộ nhớ đệm (Cache-friendly).

### Các Thuộc Tính Cốt Lõi:
* **Object Size ($S$):** Kích thước của một đối tượng dữ liệu do người dùng yêu cầu.
* **Alignment ($A$):** Yêu cầu căn lề bộ nhớ của đối tượng (thường là $8$-byte hoặc $16$-byte).
* **Page Size ($P$):** Kích thước tổng cộng của một trang bộ nhớ (ví dụ: chuẩn phần cứng $4096\text{ bytes}$).

---

## 2. Sơ Đồ Bố Trí Bộ Nhớ (Metadata & Slots Layout)

Bộ nhớ của một Slab được chia thành hai phần rõ rệt: **Metadata (Vùng quản lý)** nằm ở phần đầu tiên của trang và **Slots (Vùng chứa dữ liệu)** chiếm phần không gian còn lại.

```text
[ Slab Buffer (Tổng kích thước P = 4096 bytes) ]
+-------------------------------------------------------------------------+
| METADATA (Cố định ở đầu Slab)                                          |
| - magic (4 bytes)       - object_size (2 bytes)  - total_slots (2 bytes)|
| - free_count (2 bytes)  - first_free_index (2 bytes)                    |
| - padding / alignment padding to reach alignment boundary               |
+-------------------------------------------------------------------------+
| PADDING / ALIGNMENT GAP (Đảm bảo Slots bắt đầu tại địa chỉ chia hết cho A)|
+-------------------------------------------------------------------------+
| SLOTS AREA (Vùng chứa dữ liệu / danh sách liên kết các ô trống)        |
| ┌─────────────────────────────────────────────────────────────────────┐ |
| | Slot 0: [ Dữ liệu đối tượng HOẶC con trỏ chỉ tới slot trống kế tiếp ] | |
| ├─────────────────────────────────────────────────────────────────────┤ |
| | Slot 1: [ Dữ liệu đối tượng / Con trỏ next ]                        | |
| ├─────────────────────────────────────────────────────────────────────┤ |
| | Slot 2: [ Dữ liệu đối tượng / Con trỏ next ]                        | |
| ├─────────────────────────────────────────────────────────────────────┤ |
| | ...                                                                 | |
| └─────────────────────────────────────────────────────────────────────┘ |
+-------------------------------------------------------------------------+
```

### Chi Tiết Cấu Trúc Metadata (`struct SlabHeader`):
1. `uint32_t magic`: Mã nhận diện tính hợp lệ của Slab (ví dụ: `0x514C4142` tương ứng chữ "SLAB").
2. `uint16_t object_size`: Kích thước thực tế của mỗi slot.
3. `uint16_t total_slots`: Tổng số lượng slot có trong Slab này.
4. `uint16_t free_count`: Số lượng slot hiện đang trống (chưa được cấp phát).
5. `int16_t first_free_index`: Chỉ số (index) của slot trống đầu tiên trong danh sách liên kết nội bộ (Free List). Nếu bằng `-1`, Slab đã đầy.

---

## 3. Tính Toán Sức Chứa (Capacity) và Căn Lề (Alignment)

Việc tính toán dung lượng (`capacity` tức `total_slots`) phải tuân thủ nghiêm ngặt các quy tắc căn lề phần cứng để tránh lỗi truy cập bộ nhớ trên các kiến trúc như RISC-V hoặc x86_64.

### Công Thức Căn Lề (Alignment):
Đối tượng có kích thước $S$ phải được căn lề theo bội số của $A$. Kích thước hiệu chỉnh của một slot ($S_{aligned}$) được tính bằng:
$$S_{aligned} = \left( \frac{S + A - 1}{A} \right) \times A$$

### Công Thức Tính Sức Chứa ($N$ - Total Slots):
Giả sử kích thước tổng cộng của trang là $P$ (ví động $4096$ bytes) và kích thước phần metadata cố định là $M$ (ví dụ 32$ bytes):
1. Không gian khả dụng cho dữ liệu: $D = P - M$.
2. Số lượng slot tối đa tính toán lý thuyết: 
   $$N = \left\lfloor \frac{D}{S_{aligned}} \right\rfloor$$
3. Kiểm tra ràng buộc tràn bộ nhớ: Đảm bảo rằng $M + (N \times S_{aligned}) \le P$. Nếu dư thừa dung lượng cuối trang, phần dư đó được tính là khoảng đệm cuối (tail padding) và không được phân bổ cho slot nào.

---

## 4. Quy Trình Khởi Tạo (`init` Path)

Khi một Slab mới được cấp phát từ hệ thống, hàm khởi tạo (`kmalloc_init(void)') thực hiện các bước sau:

1. **Kiểm tra tham số đầu vào (Validation):**
   * Đảm bảo con trỏ bộ nhớ trang `void* raw_mem` khác `NULL`.
   * Kiểm tra $S > 0$ và yêu cầu căn lề $A$ là lũy thừa của $2$ ($1, 2, 4, 8, 16,...$).
   * Kiểm tra xem $P$ có đủ chứa ít nhất $1$ slot hay không ($P \ge \text{Header Size} + S_{aligned}$).
2. **Thiết lập Header (Metadata):**
   * Ghi giá trị `magic`, `object_size`, và `total_slots` vào vùng nhớ đầu tiên.
   * Đặt `free_count = total_slots` và `first_free_index = 0`.
3. **Xây dựng Free List nội bộ (Embedded Free List):**
   * Duyệt qua từng slot từ $i = 0$ đến $N - 1$.
   * Tại mỗi slot, tận dụng chính vùng nhớ của slot đó để lưu chỉ số của slot trống kế tiếp (`int16_t next_free`).
   * Slot cuối cùng được gán giá trị liên kết là `-1` (đánh dấu kết thúc danh sách).

---

## 5. Đường Dẫn Xử Lý Lỗi (`Failure Path`)

Trong môi trường hệ điều hành hoặc hệ thống nhúng, việc xử lý lỗi (Failure path) phải được thiết kế tường minh để tránh rò rỉ bộ nhớ hoặc gây trạng thái treo hệ thống (Kernel Panic):

* **Lỗi 1: Tham số không hợp lệ (`EINVAL` / Invalid Arguments)**
  * *Điều kiện:* `raw_mem == NULL`, $S == 0$, hoặc $A$ không phải lũy thừa của 2.
  * *Hành động:* Hàm `init` trả về mã lỗi trực tiếp, không ghi đè bất kỳ vùng nhớ nào, đảm bảo tính an toàn dữ liệu.
* **Lỗi 2: Trang quá nhỏ không chứa nổi metadata hoặc 1 slot (`ENOMEM` / Out of Memory Space)**
  * *Điều kiện:* $P < \text{sizeof(SlabHeader)} + S_{aligned}$.
  * *Hành động:* Hủy bỏ tiến trình khởi tạo Slab, trả về con trỏ trạng thái lỗi, yêu cầu gọi cấp phát trang có kích thước lớn hơn.
* **Lỗi 3: Tràn bộ nhớ khi cấp phát (`Allocation Failure` khi Slab đầy)**
  * *Điều kiện:* Khi gọi hàm cấp phát đối tượng từ Slab mà `free_count == 0` hoặc `first_free_index == -1`.
  * *Hành động:* Trả về `NULL` ngay lập tức để tầng quản lý cấp cao hơn (như Memory Pool Manager) biết đường chuyển sang cấp phát một Slab mới hoặc kích hoạt cơ chế dọn dẹp.

---
*Bản thiết kế này sẵn sàng làm cơ sở để hiện thực hóa thành mã nguồn C/C++ chuẩn xác cho Đề tài quản lý bộ nhớ.*