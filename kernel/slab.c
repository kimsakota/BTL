#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"

extern char end[];
#define SLAB_MAGIC 0x534C4142U // SLAB in hex
#define SLAB_ALIGN 8UL
#define ALIGN_UP(sz, a) (((sz) + ((a)-1)) & ~((a)-1))

struct slab_slot{
    struct slab_slot *next; // Con trỏ đến slot tiếp theo trong danh sách liên kết
};

struct slab_header {
  uint32 magic;               // 4 bytes: SLAB_MAGIC
  uint16 obj_size;            // 2 bytes: Kích thước mỗi slot sau căn lề 8B
  uint16 capacity_per_page;   // 2 bytes: Tổng số slot trong trang
  uint16 free_count;          // 2 bytes: Số lượng slot hiện đang trống
  uint16 _pad[3];             // 6 bytes: Đệm căn lề
  struct slab_slot *freelist; // 8 bytes: Con trỏ tới slot trống đầu tiên
  struct slab_header *next;   // 8 bytes: Nối tới trang Slab tiếp the
};

struct {
  struct spinlock lock;        // Khóa bảo vệ truy cập vào danh sách Slab   
  uint16 obj_size;              // Kích thước của mỗi slot trong Slab
  uint16 capacity_per_page;
  struct slab_header *partial;
  struct slab_header *full;
}slab_cache;

static struct slab_header*
slab_page_create(void)
{
  // Xin 1 trang vật lý 4096B từ bộ cấp phát trang nền tảng
  void *page = kalloc();
  if(page == 0)
    return 0; // Lỗi 2 (Failure Path): OOM - Cạn kiệt RAM vật lý

  struct slab_header *hdr = (struct slab_header *)page;
  hdr->magic = SLAB_MAGIC;
  hdr->obj_size = slab_cache.obj_size;
  hdr->capacity_per_page = slab_cache.capacity_per_page;
  hdr->free_count = slab_cache.capacity_per_page    ;
  hdr->_pad[0] = 0;
  hdr->_pad[1] = 0;
  hdr->_pad[2] = 0;
  hdr->freelist = 0;
  hdr->next = 0;

  // Vùng chứa các Slot bắt đầu ngay sau Header 32 bytes
  char *slot_base = (char *)page + sizeof(struct slab_header);

  // Nối các Slot trống thành danh sách liên kết đơn (từ cuối lên đầu để cấp phát từ Slot 0)
  for(int i = hdr->capacity_per_page - 1; i >= 0; i--) {
    struct slab_slot *s = (struct slab_slot *)(slot_base + (uint64)i * hdr->obj_size);
    s->next = hdr->freelist;
    hdr->freelist = s;
  }

  return hdr;
}

// 1. Hàm khởi tạo Slab Allocator một Class (Init Path)
void
slab_init(uint32 req_size)
{
  initlock(&slab_cache.lock, "slab_cache");

  if(req_size == 0)
    panic("slab_init: invalid size 0");

  // Căn lề tối thiểu 8 bytes (bằng kích thước con trỏ 64-bit)
  uint32 s_min = req_size < sizeof(struct slab_slot) ? sizeof(struct slab_slot) : req_size;
  uint32 s_aligned = ALIGN_UP(s_min, 8);

  // Lỗi 1 (Failure Path): Kích thước vượt quá dung lượng khả dụng của trang 4096B
  if(sizeof(struct slab_header) + s_aligned > PGSIZE)
    panic("slab_init: object size too large for 4KB page");

  slab_cache.obj_size = (uint16)s_aligned;
  slab_cache.capacity_per_page = (uint16)((PGSIZE - sizeof(struct slab_header)) / s_aligned);
  slab_cache.partial = 0;
  slab_cache.full = 0;
}

// 2. Hàm cấp phát 1 đối tượng từ Slab (Allocate Path)
void*
slab_alloc(void)
{
  acquire(&slab_cache.lock);

  // Tìm một trang Slab hiện có còn slot trống
  struct slab_header *curr = slab_cache.partial;
  while(curr != 0 && curr->free_count == 0) {
    curr = curr->next;
  }

  // Nếu chưa có trang nào hoặc các trang đều đã đầy, xin trang mới từ kalloc()
  if(curr == 0) {
    curr = slab_page_create();
    if(curr == 0) {
      // Lỗi 2 (OOM): Nhả khóa an toàn và trả về 0, không làm sập hệ thống
      release(&slab_cache.lock);
      return 0;
    }
    curr->next = slab_cache.partial;
    slab_cache.partial = curr;
  }

  // Rút 1 slot trống ở đầu freelist của trang Slab
  struct slab_slot *slot = curr->freelist;
  curr->freelist = slot->next;
  curr->free_count--;

  release(&slab_cache.lock);

  // Xóa sạch hoặc ghi mẫu để sử dụng an toàn
  memset((void*)slot, 0, slab_cache.obj_size);
  return (void*)slot;
}

// 3. Hàm thu hồi đối tượng về lại Slab (Free & Failure Paths)
void
slab_free(void *ptr)
{
  if(ptr == 0)
    panic("slab_free: null ptr");

  // Lỗi 3 (Failure Path): Kiểm tra con trỏ nằm ngoài vùng RAM vật lý hợp lệ
  if((char*)ptr < end || (uint64)ptr >= PHYSTOP)
    panic("slab_free: pointer out of physical memory range");

  // Tìm địa chỉ đầu trang 4KB chứa con trỏ ptr bằng PGROUNDDOWN
  struct slab_header *hdr = (struct slab_header *)PGROUNDDOWN((uint64)ptr);

  acquire(&slab_cache.lock);

  // Lỗi 4A (Failure Path): Kiểm tra chữ ký SLAB_MAGIC ở đầu trang
  if(hdr->magic != SLAB_MAGIC || hdr->obj_size != slab_cache.obj_size)
    panic("slab_free: invalid slab magic or corrupted header");

  // Lỗi 4B (Failure Path): Kiểm tra con trỏ có nằm đúng biên bắt đầu của 1 Slot không
  uint64 offset = (uint64)ptr - ((uint64)hdr + sizeof(struct slab_header));
  if((uint64)ptr < (uint64)hdr + sizeof(struct slab_header) ||
     (offset % hdr->obj_size) != 0 ||
     (offset / hdr->obj_size) >= hdr->capacity_per_page) {
    panic("slab_free: unaligned slot pointer");
  }

  // Lỗi 5 (Failure Path): Kiểm tra giải phóng kép (Double Free / Vượt quá sức chứa)
  if(hdr->free_count >= hdr->capacity_per_page)
    panic("slab_free: double free or corrupt free_count");

  // Ghi đè dữ liệu rác (số 1) để bẫy lỗi Dangling Pointer giống kfree()
  memset(ptr, 1, hdr->obj_size);

  // Đẩy slot vừa trả vào lại đầu freelist của trang Slab
  struct slab_slot *slot = (struct slab_slot *)ptr;
  slot->next = hdr->freelist;
  hdr->freelist = slot;
  hdr->free_count++;

  release(&slab_cache.lock);
}

// 4. Hàm tự kiểm thử (Unit Test) gọi lúc khởi động để lấy minh chứng
void
slab_test(void)
{
  printf("slab_test: starting (obj_size=64)...\n");
  slab_init(64); // Khởi tạo Slab cho vật thể 64B -> capacity = (4096-32)/64 = 63 slots

  void *p1 = slab_alloc();
  void *p2 = slab_alloc();
  if(p1 == 0 || p2 == 0 || p1 == p2)
    panic("slab_test: basic alloc failed");

  slab_free(p1);
  void *p3 = slab_alloc();
  if(p3 != p1) // LIFO: Vừa trả p1 vào đầu freelist thì xin lại phải nhận đúng p1
    panic("slab_test: LIFO reuse failed");

  slab_free(p2);
  slab_free(p3);
  printf("slab_test: ALL PASSED (capacity_per_page=%d, obj_size=%d)\n",
         slab_cache.capacity_per_page, slab_cache.obj_size);
}