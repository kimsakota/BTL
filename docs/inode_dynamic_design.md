# Thiết kế inode động (I03) — Khánh

Issue: #46 (W05, cha #43). Bản thiết kế đầu ngày 20/11/2026, đọc trên `main` `7715b36`. Cập nhật 22/11/2026 theo review #48: chốt class 256 B cho inode (#45), đo lại trên `93f319b`, cụ thể hóa N03 và N06. Chưa có code: chờ PR class (#52) vào `main`.

## 1. Hiện trạng trong `kernel/fs.c`

- Inode trong RAM nằm trong bảng tĩnh `itable.inode[NINODE]`, `NINODE = 50`, bảo vệ bằng spinlock `itable.lock`.
- `iget(dev, inum)` duyệt bảng: có entry `ref > 0` trùng `(dev, inum)` thì `ref++` và trả về; không có thì lấy entry `ref == 0` đầu tiên, đặt `dev`, `inum`, `ref = 1`, `valid = 0`. Hết entry thì `panic("iget: no inodes")`.
- `idup(ip)` tăng `ref` dưới `itable.lock`.
- `iput(ip)` giảm `ref`. Nếu là tham chiếu cuối (`ref == 1`) của inode đã đọc (`valid`) và không còn link (`nlink == 0`): khóa `ip->lock`, nhả `itable.lock`, `itrunc`, đặt `valid = 0`, rồi `ref--`; sau cùng `ifree(dev, inum)` bằng `dev/inum` đã chụp trước. Mọi lời gọi `iput` phải nằm trong transaction (`begin_op`/`end_op`) vì có thể ghi log.
- Entry `ref == 0` không được tra cứu lại (chỉ so entry `ref > 0`), nên bảng tĩnh không giữ cache sau khi ref về 0.

Nơi tạo hoặc giữ tham chiếu:

| Nơi | Hàm | Ghi chú |
|---|---|---|
| `fs.c` | `ialloc` → `iget` | Tạo file/thư mục mới, trong transaction của `create` |
| `fs.c` | `dirlookup` → `iget` | Dùng trong `namex`, `dirlink`, `create` |
| `fs.c` | `namex` → `iget(ROOTDEV, ROOTINO)` hoặc `idup(cwd)` | Phải gọi trong transaction |
| `fs.c` | `ireclaim` → `iget` | Lúc `fsinit`, dọn inode mồ côi, có `begin_op`/`end_op` riêng |
| `proc.c` | `fork`: `idup(p->cwd)`; `exit`: `iput(p->cwd)` trong transaction | |
| `file.c` | `fileclose`: `iput(ff.ip)` trong transaction | |
| `exec.c`, `sysfile.c` | `iunlockput` | Đều trong transaction |

## 2. Mục tiêu

Mỗi inode trong RAM là một object xin bằng `kmalloc` khi lần đầu cần và trả bằng `kmfree` khi tham chiếu cuối mất đi. Bỏ giới hạn 50 entry tĩnh. Giữ nguyên API (`iget`, `idup`, `ilock`, `iunlock`, `iput`, `iunlockput`) để nơi gọi không phải đổi.

## 3. Cấu trúc

- Thêm con trỏ `struct inode *next` vào `struct inode` để nối danh sách inode đang sống. `itable` còn `lock` và con trỏ đầu danh sách `head`, thêm bộ đếm `live` (số inode đang sống) để test và thống kê.
- `iinit` chỉ khởi tạo `itable.lock`; `initsleeplock(&ip->lock, "inode")` chuyển vào lúc tạo inode mới.
- Có thể đổi danh sách thành bảng băm nhỏ theo `(dev, inum)` sau, không đổi quy tắc ở mục 4.

## 4. Quy tắc

### 4.1. Lookup duy nhất theo `(dev, inum)`

Trong RAM, mỗi `(dev, inum)` có tối đa một inode đang sống (`ref > 0`). `iget` làm tra cứu và chèn trong **cùng một đoạn giữ `itable.lock`**:

1. `acquire(&itable.lock)`.
2. Duyệt danh sách. Gặp inode trùng `(dev, inum)` thì `ref++`, `release`, trả về.
3. Không gặp: `kmalloc(sizeof(struct inode))` khi vẫn giữ `itable.lock`; khởi tạo `dev`, `inum`, `ref = 1`, `valid = 0`, sleeplock; chèn vào đầu danh sách; `live++`; `release`; trả về.

Hai `iget` cùng `(dev, inum)` trên hai CPU bị `itable.lock` xếp hàng: lần đầu tạo và chèn, lần sau thấy và `ref++`. Không thể có hai bản vì không có khoảng hở giữa "tìm không thấy" và "chèn".

Phương án thay thế: cấp phát ngoài khóa rồi tra lại (recheck) trước khi chèn, nếu đã có thì `kmfree` bản mới. Chọn phương án giữ khóa vì đơn giản hơn và `kmalloc` không ngủ; chuyển sang recheck nếu Linh/Minh muốn giữ `itable.lock` ngắn.

### 4.2. Giữ reference

- `ref` chỉ đổi khi giữ `itable.lock` (`iget`, `idup`, `iput`).
- Ai có con trỏ inode thì đang giữ một `ref`; con trỏ chỉ dùng được tới lúc gọi `iput` tương ứng.
- `ilock`/`iunlock` không đổi `ref`; vẫn `panic` nếu `ref < 1`.

### 4.3. Đường cuối vòng đời

`iput` giữ thứ tự của bản hiện tại, thêm bước gỡ khỏi danh sách và trả bộ nhớ:

1. `acquire(&itable.lock)`; chụp `last = (ref == 1 && valid && nlink == 0)`, `dev`, `inum`.
2. Nếu `last`: `acquiresleep(&ip->lock)` (không chờ vì `ref == 1`), `release`, `itrunc`, `valid = 0`, `releasesleep`, `acquire` lại.
3. `ref--`. Nếu `ref == 0`: gỡ `ip` khỏi danh sách, `live--`, đánh dấu cần trả.
4. `release(&itable.lock)`.
5. Nếu cần trả: `kmfree(ip)`. Sau bước này không đọc `ip`.
6. Nếu `last`: `ifree(dev, inum)` bằng giá trị đã chụp ở bước 1.

Inode chỉ bị `kmfree` khi `ref == 0` và đã gỡ khỏi danh sách dưới `itable.lock`, nên không `iget` nào còn thấy nó. Với inode `nlink == 0`, không còn đường tên nào dẫn tới, nên trong lúc `itable.lock` bị nhả ở bước 2 không `iget` nào khác tạo tham chiếu mới tới nó.

### 4.4. Transaction

Giữ quy ước: mọi `iput` nằm trong `begin_op`/`end_op` vì bước 2 và 6 ghi log. `kmalloc`/`kmfree` không ghi đĩa nên không đổi ranh giới transaction.

### 4.5. Thứ tự khóa

`itable.lock` (spin) → khóa class slab (spin) → khóa pool trang `kmem` (spin, khi slab cần trang mới). Slab và `kalloc` không gọi ngược vào inode, nên không có vòng. `ip->lock` (sleep) chỉ được lấy khi đang giữ `itable.lock` ở bước 2 của `iput`, giống bản hiện tại. Cần Linh xác nhận thứ tự này khi ghép với buffer động.

### 4.6. Hết bộ nhớ

Bước đầu giữ hành vi cũ: `kmalloc` trả 0 thì `panic("iget: out of memory")`, thay cho `"iget: no inodes"`. Bước sau mới đổi `iget` trả 0 và sửa các nơi gọi ở mục 1 xử lý lỗi; việc này đụng `namex`, `dirlookup`, `create` nên tách PR riêng.

## 5. Ca kiểm thử

Chạy trong harness kernel sau cờ riêng (dạng `INODETEST`), tạo tiến trình người dùng khi cần, hoặc qua `usertests`. Log theo schema `KT|` của test matrix. Các ca cần nhìn vào bảng inode (N03, N06) dùng một hàm đếm chỉ có khi bật `INODETEST`: giữ `itable.lock`, trả số inode có khóa `(dev, inum)` cho trước, `ref` của inode đó và `live`.

| ID | Kiểm tra | Cách làm | Expected |
|---|---|---|---|
| N01 | Lookup duy nhất | `iget` cùng `(dev, inum)` hai lần | Cùng con trỏ, `ref = 2`; sau hai `iput` thì `live` về mức đầu |
| N02 | Inode khác nhau | `iget` hai `inum` khác nhau | Hai con trỏ khác nhau, không chồng vùng |
| N03 | Lookup đồng thời | Trên `CPUS=2`, K worker cùng mở một file rồi dừng ở điểm đồng bộ (barrier) khi vẫn giữ file mở. Tại đó: mỗi worker báo con trỏ inode nhận được; hàm đếm giữ `itable.lock` đếm số inode có khóa `(dev, inum)` của file. Sau đó các worker đóng file; lặp nhiều vòng | Mọi worker nhận cùng một con trỏ; đúng 1 inode cho khóa đó; `ref` = K + số tham chiếu khác đang giữ; không panic; đóng hết thì `live` về mức đầu |
| N04 | Refcount qua fork | Tiến trình con dùng `cwd` qua `idup`, rồi thoát | `ref` của `cwd` tăng rồi giảm đúng; `live` về mức đầu |
| N05 | Tham chiếu cuối của file đã xóa | Tạo file, mở, `unlink`, đóng | `itrunc` + `ifree` chạy đúng một lần; inode bị `kmfree`; `inum` cấp lại được |
| N06 | Vượt 50 inode cũ | Mỗi tiến trình chỉ mở được `NOFILE = 16` file (3 fd đầu đã dùng), toàn hệ thống `NFILE = 100` `struct file`. Dùng 5 tiến trình, mỗi tiến trình giữ mở 12 file khác nhau (60 file, 60 inode khác nhau), chờ ở điểm đồng bộ rồi đọc `live` qua hàm đếm | Mọi `open` thành công; `live` tăng ít nhất 60 so với mức đầu, vượt 50 inode cũ; không panic; đóng hết thì `live` về mức đầu. `open` trả -1 do hết descriptor hoặc `struct file` thì ghi riêng, không tính là lỗi inode |
| N07 | Reuse ổn định | Lặp mở/đóng cùng workload sau warm-up | Số trang của class chứa inode không tăng mãi |
| N08 | Hết bộ nhớ | Giả lập `kmalloc` trả 0 trong `iget` | Panic có thông điệp đúng (bước đầu) |
| R01 | Hồi quy | `usertests -q` với `CPUS=2` | Toàn bộ OK |

## 6. Class cho inode

- Đo lại trên `main` `93f319b` bằng toolchain RISC-V: `sizeof(struct inode)` = 136 B, căn lề 8 B; thêm `next` thành 144 B.
- Bảng class đã chốt ở #45 là 64/128/256/1280 B (PR #52 của Minh). Inode vào class 256 B: header 32 B, vùng slot 4064 B, 15 object/trang, dư 224 B cuối trang; mỗi inode để trống 112 B trong slot.
- Còn chờ: PR #52 vào `main` thì mới viết được code inode động. I05 cần ít nhất một client đã ghép; hiện chưa có client nào trên `main`.

## 7. Kế hoạch

1. Linh đã đồng ý hướng lookup, reference và giải phóng ở mục 4 (#48).
2. Sau khi PR #52 (class 256 B) vào `main`: PR nhỏ đổi `itable` sang danh sách động theo mục 3–4.6, kèm harness N01–N08 và log `CPUS=1`/`CPUS=2`.
3. Cập nhật `slabtest.c` theo bảng 64/128/256/1280 B: `kmalloc(129)` vào class 256, ngưỡng trả 0 thành 1281; thêm biên, capacity, OOM/retry, nhiều slab, reuse cho 4 class.
4. Chạy R01/I05 trên revision có client đã ghép.
