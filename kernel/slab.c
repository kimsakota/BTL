#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

extern char end[]; // first address after kernel, defined by kernel.ld

#define SLAB_MAGIC      0x534C4142U // Mã nhận diện "SLAB"
#define SLAB_CLASS_SIZE 64U         // Size class cố định của W02: 64 bytes
#define ALIGN_UP(sz, a) (((sz) + (a) - 1) & ~((a) - 1))

// Nút liên kết nhúng trực tiếp trong 8 byte đầu của mỗi Slot trống
struct slab_slot {
  struct slab_slot *next;
};

// Metadata Header 32 bytes đặt tại đầu mỗi trang Slab (offset 0x000)
struct slab_header {
  uint32 magic;      // 4 bytes: SLAB_MAGIC
  uint16 obj_size;   // 2 bytes: Kích thước mỗi slot sau căn lề 8B (stride = 64)
  uint16 capacity;   // 2 bytes: Tổng số slot trong trang (63)
  uint16 free_count; // 2 bytes: Số lượng slot hiện đang trống (0..63)
  uint16 _pad;       // 2 bytes: Đệm căn lề
  uint32 _reserved;  // 4 bytes: Dự phòng mở rộng (đủ 16B đầu)
  struct slab_slot *freelist; // 8 bytes: Con trỏ tới slot trống đầu tiên
  struct slab_header *next;   // 8 bytes: Nối tới trang Slab tiếp theo
};

// Quản lý chung cho 1 Class kích thước cố định (64B)
struct {
  struct spinlock lock;
  uint16 obj_size;
  uint16 capacity;
  struct slab_header *slabs; // Danh sách các trang Slab đang quản lý
} slab_cache;

// Khởi tạo một trang vật lý 4KB mới thành một trang Slab
static struct slab_header *
slab_page_create(void)
{
  void *page = kalloc();
  if (page == 0)
    return 0; // OOM: Cạn kiệt RAM vật lý

  struct slab_header *hdr = (struct slab_header *)page;
  hdr->magic = SLAB_MAGIC;
  hdr->obj_size = slab_cache.obj_size;
  hdr->capacity = slab_cache.capacity;
  hdr->free_count = slab_cache.capacity;
  hdr->_pad = 0;
  hdr->_reserved = 0;
  hdr->freelist = 0;
  hdr->next = 0;

  char *slot_base = (char *)page + sizeof(struct slab_header);

  for (int i = hdr->capacity - 1; i >= 0; i--) {
    struct slab_slot *s =
      (struct slab_slot *)(slot_base + (uint64)i * hdr->obj_size);
    s->next = hdr->freelist;
    hdr->freelist = s;
  }

  return hdr;
}

// 1. Hàm khởi tạo bộ cấp phát Object 1 Class (Init Path)
void
kmalloc_init(void)
{
  initlock(&slab_cache.lock, "slab_cache");

  uint32 s_min = SLAB_CLASS_SIZE < sizeof(struct slab_slot)
                   ? sizeof(struct slab_slot)
                   : SLAB_CLASS_SIZE;
  uint32 s_aligned = ALIGN_UP(s_min, 8);

  if (sizeof(struct slab_header) != 32 ||
      sizeof(struct slab_header) + s_aligned > PGSIZE)
    panic("kmalloc_init: invalid header or object size");

  slab_cache.obj_size = (uint16)s_aligned;
  slab_cache.capacity =
    (uint16)((PGSIZE - sizeof(struct slab_header)) / s_aligned);
  slab_cache.slabs = 0;
}

// 2. Hàm cấp phát 1 đối tượng từ Slab (Allocate Path)
void *
kmalloc(uint64 size)
{
  // Kiểm tra size theo hợp đồng: chỉ nhận 1..64, sai trả về 0
  if (size == 0 || size > slab_cache.obj_size)
    return 0;

  acquire(&slab_cache.lock);

  struct slab_header *curr = slab_cache.slabs;
  while (curr != 0 && curr->free_count == 0) {
    curr = curr->next;
  }

  if (curr == 0) {
    curr = slab_page_create();
    if (curr == 0) {
      release(&slab_cache.lock);
      return 0;
    }
    curr->next = slab_cache.slabs;
    slab_cache.slabs = curr;
  }

  struct slab_slot *slot = curr->freelist;
  curr->freelist = slot->next;
  curr->free_count--;

  release(&slab_cache.lock);

  memset((void *)slot, 0, slab_cache.obj_size);
  return (void *)slot;
}

// 3. Hàm thu hồi đối tượng về lại Slab (Free & Failure Paths)
void
kmfree(void *ptr)
{
  // Hợp đồng API: kmfree(NULL) là no-op (không làm gì cả)
  if (ptr == 0)
    return;

  // Kiểm tra con trỏ nằm ngoài vùng RAM vật lý hợp lệ
  if ((char *)ptr < end || (uint64)ptr >= PHYSTOP)
    panic("kmfree: pointer out of physical memory range");

  // Tìm địa chỉ đầu trang 4KB chứa con trỏ ptr bằng PGROUNDDOWN
  struct slab_header *hdr = (struct slab_header *)PGROUNDDOWN((uint64)ptr);

  acquire(&slab_cache.lock);

  // Kiểm tra chữ ký SLAB_MAGIC ở đầu trang
  if (hdr->magic != SLAB_MAGIC || hdr->obj_size != slab_cache.obj_size)
    panic("kmfree: invalid slab magic or corrupted header");

  // Kiểm tra con trỏ có nằm đúng biên bắt đầu của 1 Slot không
  uint64 slot_base = (uint64)hdr + sizeof(struct slab_header);
  if ((uint64)ptr < slot_base)
    panic("kmfree: pointer inside header");

  uint64 offset = (uint64)ptr - slot_base;
  if ((offset % hdr->obj_size) != 0 ||
      (offset / hdr->obj_size) >= hdr->capacity)
    panic("kmfree: unaligned slot pointer");

  // Kiểm tra giải phóng kép (Double Free) bằng cả free_count lẫn duyệt danh sách freelist
  if (hdr->free_count >= hdr->capacity)
    panic("kmfree: free_count overflow");

  for (struct slab_slot *cur = hdr->freelist; cur != 0; cur = cur->next) {
    if ((void *)cur == ptr)
      panic("kmfree: double free detected");
  }

  // Ghi đè dữ liệu rác (số 1) để bẫy lỗi Dangling Pointer
  memset(ptr, 1, hdr->obj_size);

  // Đẩy slot vừa trả vào lại đầu freelist của trang Slab
  struct slab_slot *slot = (struct slab_slot *)ptr;
  slot->next = hdr->freelist;
  hdr->freelist = slot;
  hdr->free_count++;

  release(&slab_cache.lock);
}

// 4. Hàm tự kiểm thử (Reuse + Pattern + Contract Boundary Test)
void
slab_test(void)
{
  printk("slab_test: starting S02 verification...\n");
  kmalloc_init();

  // Kiểm tra hợp đồng biên: size == 0, size > 64 và kmfree(0)
  if (kmalloc(0) != 0 || kmalloc(65) != 0)
    panic("slab_test: size validation failed");
  kmfree(0); // Phải là no-op, không được panic

  // Kiểm tra cấp phát và LIFO Reuse
  void *p1 = kmalloc(16);
  void *p2 = kmalloc(64);
  if (p1 == 0 || p2 == 0 || p1 == p2)
    panic("slab_test: basic alloc failed");

  kmfree(p1);
  void *p3 = kmalloc(32);
  if (p3 != p1)
    panic("slab_test: LIFO reuse failed");

  kmfree(p2);
  kmfree(p3);

  // Kiểm tra Pattern Test lấp đầy 1 trang (63 slots)
  void *slots[63];
  for (int i = 0; i < slab_cache.capacity; i++) {
    slots[i] = kmalloc(64);
    if (slots[i] == 0)
      panic("slab_test: full page alloc failed");
    memset(slots[i], (i + 0x20) & 0xFF, 64);
  }
  for (int i = 0; i < slab_cache.capacity; i++) {
    uchar *bytes = (uchar *)slots[i];
    uchar expected = (uchar)((i + 0x20) & 0xFF);
    for (int j = 0; j < 64; j++) {
      if (bytes[j] != expected)
        panic("slab_test: pattern corrupted");
    }
  }
  for (int i = 0; i < slab_cache.capacity; i++) {
    kmfree(slots[i]);
  }

  printk("slab_test: ALL PASSED (capacity=%d, obj_size=%d)\n",
         slab_cache.capacity, slab_cache.obj_size);
}