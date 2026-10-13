# Bài 1: Nền xv6 và cách cấp phát một trang nhớ

Phương thức: bạn tự thiết kế và tự viết C; assistant dạy và kiểm tra. Tài liệu nêu yêu cầu cùng mã gốc để đọc tham chiếu. Bài tập không có lời giải hoàn chỉnh. Chưa viết hàm đếm trước khi hiểu khai báo/định nghĩa, header, con trỏ, struct và mối liên kết giữa các file.

## 1. Mục tiêu và trạng thái thật

Sau bài này, bạn có thể:

1. Phân biệt host, môi trường build và kernel đang chạy.
2. Build/boot bản xv6 gốc trong BTL.
3. Đọc được struct, con trỏ, thao tác đầu danh sách và khóa trong kalloc.c.
4. Viết một hàm đếm trang trống mà không thay đổi thuật toán cấp phát.
5. Hiểu khai báo trong defs.h và lời gọi từ main.c được kết nối như thế nào.

Đã chuẩn bị: 81 file source/cấu hình được nhập từ bản xv6 ở Exercise; giữ sửa Makefile cho đường dẫn có dấu cách và chuẩn hóa newline LF. Không lấy binary hoặc filesystem image từ lần build cũ. Source kernel vẫn là mã gốc.

Chưa xác nhận: build, boot, usertests và hàm đếm trang trống. Hàm trong bài tập bên dưới chưa được thêm vào kernel.

## 2. Ba môi trường bạn đang làm việc với

```text
Windows: editor, quản lý file và gọi WSL
    ↓
Ubuntu trong WSL: Make + compiler RISC-V + QEMU
    ↓
xv6 chạy bên trong QEMU: kernel và các chương trình user của xv6
```

Compiler chạy ở Ubuntu, nhưng tạo machine code cho RISC-V. Đây là cross-compilation. QEMU tạo môi trường máy RISC-V để kernel đó chạy được trên máy hiện tại.

Đừng dùng gcc host để biên dịch kernel như một chương trình C thông thường. Makefile đã chọn RISC-V compiler và các cờ freestanding/linker phù hợp. Khi chạy mkfs, Makefile lại dùng gcc host vì công cụ tạo image chạy trên Ubuntu.

## 3. Cài môi trường và kiểm tra

### 3.1. PowerShell trên Windows

Mở PowerShell với quyền Administrator rồi tự chạy:

```powershell
wsl --install -d Ubuntu-24.04
```

Khởi động lại nếu trình cài yêu cầu. Mở Ubuntu, hoàn tất tạo tài khoản Linux, rồi kiểm tra:

```powershell
wsl --list --verbose
```

Distro dùng cho build cần hiện VERSION 2. Nếu tên Ubuntu thực tế khác, dùng đúng tên được liệt kê khi gọi lệnh WSL tiếp theo. [Hướng dẫn Microsoft](https://learn.microsoft.com/en-us/windows/wsl/install).

### 3.2. Terminal Ubuntu

Chạy trong Ubuntu, không chạy apt-get trực tiếp trong PowerShell:

```bash
sudo apt-get update
sudo apt-get install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu bc perl
```

Kiểm tra các công cụ:

```bash
make --version
riscv64-linux-gnu-gcc --version
qemu-system-riscv64 --version
```

Makefile bản đang dùng yêu cầu QEMU từ 7.2, dùng bc kiểm tra phiên bản và Perl tạo syscall stubs. Công cụ RISC-V/WSL là hướng setup được MIT sử dụng. [Tài liệu công cụ MIT](https://pdos.csail.mit.edu/6.1810/2025/tools.html).

## 4. Chạy baseline gốc trước

Vẫn trong Ubuntu, vào đúng bản BTL:

```bash
cd '/mnt/d/2. FullStack/2026.1/Hệ điều hành/BTL'
make kernel/kernel fs.img
make qemu CPUS=1
```

Đường dẫn có dấu cách nên được đặt trong dấu nháy. Lệnh build đầu tạo kernel binary và filesystem image từ source hiện tại. Lệnh thứ hai khởi động QEMU; Make có thể kiểm tra/tạo lại các phần cần thiết.

Khi thành công, output cần có thông báo kernel boot và shell xv6. Ví dụ hình thức:

```text
xv6 kernel is booting
...
init: starting sh
$
```

Đây là output mong đợi, chưa phải kết quả đã chạy tại BTL.

Tại dấu `$` của xv6, chạy:

```text
usertests -q
```

Chờ kết quả cuối. Chỉ ghi baseline tests pass nếu thực sự thấy thông báo thành công. Thoát QEMU bằng Ctrl+A, thả phím, rồi nhấn X.

Nếu lỗi, giữ lệnh và output để kiểm tra. Chưa sửa allocator để xử lý một lỗi setup không thuộc allocator.

## 5. Đọc phạm vi bộ nhớ được quản lý

Mở `kernel/memlayout.h`, `kernel/riscv.h` và `kernel/kernel.ld` trong bản BTL.

Các giá trị của baseline:

```c
#define PGSIZE 4096
#define KERNBASE 0x80000000L
#define PHYSTOP (KERNBASE + 128 * 1024 * 1024)
```

`PGSIZE` là 4.096 byte/trang. `KERNBASE` là địa chỉ nền của vùng RAM kernel dùng trong cấu hình này. `PHYSTOP` là điểm kết thúc vùng RAM kernel dự kiến sử dụng, bằng 0x88000000.

128 MiB tương ứng 32.768 trang tổng cộng, nhưng không phải tất cả đều trống: image kernel và các tài nguyên cần thiết chiếm một phần.

Trong kalloc.c có:

```c
extern char end[];
```

`end` được linker định nghĩa sau phần image kernel. Nó không phải một mảng buffer được allocator tự tạo. Allocator dùng địa chỉ đó để biết nơi bắt đầu vùng có thể đưa vào free list. Đọc định nghĩa `PROVIDE(end = .)` trong kernel.ld sẽ thấy mối liên hệ này.

## 6. Hai cấu trúc dữ liệu chính trong kalloc.c

```c
struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;
```

`struct run` là một node trong danh sách trang trống. Trường next giữ địa chỉ node tiếp theo. Trên target 64-bit đang xét, con trỏ dài 8 byte.

`struct run *next` gồm kiểu con trỏ tới struct run và tên biến next. Dấu `*` trong khai báo diễn tả kiểu con trỏ. Trong một biểu thức như `*p`, nó lại là phép truy cập dữ liệu tại địa chỉ p.

`kmem` là một biến struct có hai trường: lock và freelist. Struct ở đây không có tên kiểu riêng. `kmem.freelist` giữ địa chỉ node đầu danh sách.

```text
kmem.freelist
       ↓
  [Trang A: next] → [Trang B: next] → [Trang C: next] → 0
```

Node không được cấp phát riêng. Khi một trang đang trống, allocator dùng phần đầu của chính trang đó để chứa next. Khi trang được cấp cho client, trang không còn nằm trong free list và client có quyền dùng vùng dữ liệu đó.

`r->next` truy cập trường next thông qua pointer r. Nó tương đương `(*r).next`. `kmem.freelist` dùng dấu chấm vì kmem là một biến struct, không phải pointer.

## 7. kinit và freerange

Đọc hai hàm đầu tiên:

```c
void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void *)PHYSTOP);
}
```

`&kmem.lock` lấy địa chỉ trường lock để initlock có thể khởi tạo chính đối tượng khóa đó. Địa chỉ của một đối tượng và giá trị chứa bên trong đối tượng là hai khái niệm khác nhau.

`freerange` làm tròn địa chỉ đầu lên ranh giới trang rồi đi từng bước PGSIZE, gọi kfree để đưa từng trang vào danh sách. Đây là khởi tạo nguồn trang trống, nên các trang đó chưa cần từng được kalloc cấp trước đó.

`PGROUNDUP` tránh đưa phần cuối trang còn chứa image kernel vào pool. Điều kiện `p + PGSIZE <= pa_end` chỉ đưa vào pool các trang nằm trọn trong phạm vi.

Vì kfree thêm vào đầu danh sách, thứ tự danh sách sẽ ngược với thứ tự duyệt địa chỉ tăng dần của freerange.

## 8. kfree: đưa trang vào đầu danh sách

Phần chính của mã gốc:

```c
r = (struct run *)pa;

acquire(&kmem.lock);
r->next = kmem.freelist;
kmem.freelist = r;
release(&kmem.lock);
```

`pa` là địa chỉ trang. Ép kiểu sang `struct run *` cho compiler biết ta sẽ đọc/ghi phần đầu trang theo layout struct run; thao tác ép kiểu không sao chép dữ liệu và không tự tạo vùng RAM mới.

Ví dụ trước khi free A:

```text
head → B → C → 0
```

Hai phép gán làm:

```text
A.next = B
head = A
```

Sau đó:

```text
head → A → B → C → 0
```

Khóa bảo vệ thao tác thay cấu trúc dùng chung. Bài 2 sẽ đi sâu race condition và trạng thái ngắt.

Mã còn kiểm tra ranh giới/alignment của pointer và ghi byte 0x01 vào cả trang để hỗ trợ debug. Sau đó trường next ghi đè phần đầu. Hàm này trả nguyên trang, chưa phải free object nhỏ của slab.

## 9. kalloc: lấy trang đầu danh sách

Phần chính của mã gốc:

```c
acquire(&kmem.lock);
r = kmem.freelist;
if (r)
  kmem.freelist = r->next;
release(&kmem.lock);
```

Nếu head đang là A, allocator lưu A vào r và chuyển head sang B. Sau khi thả khóa, hàm trả địa chỉ A cho caller.

```text
Trước: head → A → B → C → 0
Sau:   head → B → C → 0
       caller nhận A
```

Nếu head bằng 0, danh sách rỗng, r cũng bằng 0 và hàm trả lỗi thiếu trang. Trước khi trả một trang có thật, mã ghi byte 0x05 vào trang để hỗ trợ debug. Cấp phát trang không bảo đảm vùng đó đã được zero.

## 10. Bài tập tự viết: đếm trang trống

Chỉ bắt đầu khi hiểu phần nền C và baseline gốc boot/test được. Mỗi lượt làm một bước, kiểm tra và giải thích trước khi đi tiếp. Không đổi thuật toán kalloc/kfree trong bài này.

### 10.1. Phân tích hợp đồng hàm

Hàm dự kiến tên `kfreepage_count`: không nhận tham số, trả số trang trống bằng kiểu `uint64`, chỉ quan sát và không thay đổi danh sách. Trước khi gõ, giải thích vì sao kết quả trả về là số đếm còn biến dùng để duyệt phải là con trỏ.

Người dùng tự viết khai báo sau khi học cú pháp hàm. Assistant đọc khai báo và kiểm tra kiểu/tham số trước khi giao phần thân.

### 10.2. Reference và vị trí

- `kernel/kalloc.c` sở hữu free list và sẽ chứa định nghĩa hàm.
- `kernel/defs.h` công bố khai báo để file gọi biết kiểu hàm.
- `kernel/main.c` sẽ gọi hàm sau khi allocator khởi tạo.
- Makefile đã đưa kalloc.o vào kernel; cần hiểu compiler/linker tìm định nghĩa hàm ra sao.

`#include` cho compiler thấy khai báo; không tự chạy hàm hoặc tự ghép implementation. Linker là bước giải quyết reference từ lời gọi sang định nghĩa ở object file khác.

### 10.3. Các bước giao viết thân hàm

1. Người dùng chọn biến cần giữ node đang xét và biến cần giữ kết quả đếm, rồi giải thích kiểu của từng biến.
2. Viết bước khởi tạo và trường hợp danh sách rỗng.
3. Viết thao tác đi sang node kế tiếp và điều kiện dừng; minh họa bằng danh sách ba trang.
4. Ghép phần duyệt, rồi phân tích vì sao phải bảo vệ nó bằng khóa.
5. Kiểm tra hàm không làm đổi head hoặc liên kết; hoàn thiện kết quả trả về.

Assistant gợi ý theo chỗ vướng, không đưa toàn bộ thân hàm. Độ phức tạp dự kiến O(n); chỉ dùng để quan sát ở boot trong bài học, không đặt vào hot path mỗi lần allocation.

### 10.4. Tích hợp lời gọi

Sau khi khai báo và định nghĩa đã được kiểm tra, người dùng tự chọn vị trí gọi trong main.c và định dạng in cho kiểu trả về. Phải giải thích vì sao allocator đã sẵn sàng ở vị trí đó và vì sao không được giữ allocator lock trong khi in.

Không thêm prototype, implementation và lời gọi cùng một lượt khi chưa hiểu mối liên kết của chúng.

## 11. Build và đọc kết quả bài tập

Trong Ubuntu, tại root BTL:

```bash
make kernel/kernel fs.img
make qemu CPUS=1
```

Output cần thêm một dòng dạng:

```text
[memory] free pages after kinit: <số trang>
```

Không gõ nguyên placeholder `<số trang>` vào code. Con số là kết quả thực tế của hàm.

Số trang sau kinit về lý thuyết là:

```text
(PHYSTOP - PGROUNDUP(địa chỉ end)) / PGSIZE
```

Nó không nhất thiết bằng 32.768 và có thể đổi khi image kernel tăng kích thước qua ranh giới trang. Đừng cố sửa count để khớp một con số mẫu từ hướng dẫn khác.

Chạy lại usertests -q, thoát QEMU, rồi xem thay đổi:

```bash
git diff -- kernel/kalloc.c kernel/defs.h kernel/main.c
git diff --check
```

Diff bài tập nên chỉ có hàm đếm, prototype và dòng in. Chưa thêm per-CPU lists hoặc slab trong bài này.

## 12. Tự kiểm tra đã hiểu

Trả lời bằng lời của bạn:

1. Vì sao `r->next` dùng dấu mũi tên, còn `kmem.freelist` dùng dấu chấm?
2. Tại sao allocator có thể đặt next vào đầu một trang đang trống?
3. Sau khi kalloc lấy A, A còn thuộc free list không?
4. Tại sao hàm đếm cần giữ khóa trong suốt vòng lặp?
5. Vì sao đếm ngay sau kinit có thể cho số khác đếm sau khi kernel khởi tạo xong?

Điều kiện kết thúc bài: baseline gốc đã boot/test; code tự gõ đã được đọc diff, build và chạy; bạn giải thích được thao tác push/pop và vai trò prototype. Tiến độ ở hiện tại vẫn là chuẩn bị nguồn/môi trường, chưa hoàn thành các điều kiện runtime này.
