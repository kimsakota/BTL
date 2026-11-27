// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct page_pool {
  struct spinlock lock; // Khoa bao ve freelist khi nhieu CPU truy cap cung luc
  struct run *freelist;
};

#if defined(ALLOC_BENCH) && ALLOC_BENCH
// Counter theo tung CPU (chi bat khi chay benchmark, dem tron 64B tranh false sharing)
struct kalloc_cpu_counters {
  uint64 alloc_ok;
  uint64 alloc_fail;
  uint64 local_hit;
  uint64 local_miss;
  uint64 steal_attempts;
  uint64 steal_success;
  uint64 _pad[2];
};

static struct kalloc_cpu_counters kstats[NCPU];

#define KSTAT_INC(cpu, field)                                                  \
  __atomic_fetch_add(&kstats[(cpu)].field, 1, __ATOMIC_RELAXED)
#else
#define KSTAT_INC(cpu, field)                                                  \
  do {                                                                         \
  } while (0)
#endif

#if defined(KALLOC_BASELINE) && KALLOC_BASELINE

// Backend goc (single lock) dung de do so sanh baseline tren cung source
static struct page_pool kmem;

void
kinit(void)
{
  initlock(&kmem.lock, "kmem");
  kmem.freelist = 0;
  freerange(end, (void *)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char *)PGROUNDUP((uint64)pa_start);
  for (; p + PGSIZE <= (char *)pa_end; p += PGSIZE)
    kfree(p);
}

void
kfree(void *pa)
{
  struct run *r;

  if (((uint64)pa % PGSIZE) != 0 || (char *)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run *)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if (r)
    kmem.freelist = r->next;
  release(&kmem.lock);

#if defined(ALLOC_BENCH) && ALLOC_BENCH
  push_off();
  int id = cpuid();
  if (r)
    KSTAT_INC(id, alloc_ok);
  else
    KSTAT_INC(id, alloc_fail);
  pop_off();
#endif

  if (r)
    memset((char *)r, 5, PGSIZE); // fill with junk

  return (void *)r;
}

uint64
kfreepage_count(void)
{
  struct run *r;
  uint64 count = 0;

  acquire(&kmem.lock);
  r = kmem.freelist;
  while (r) {
    count++;
    r = r->next;
  }
  release(&kmem.lock);

  return count;
}

#else

static struct page_pool
  kmem[NCPU]; // Mang cac page_pool, moi CPU co mot page_pool rieng

void
kinit(void)
{
  for (int i = 0; i < NCPU; i++) {
    initlock(&kmem[i].lock, "kmem");
    kmem[i].freelist = 0;
  }
  freerange(end, (void *)PHYSTOP); // Khoi tao cac page free
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char *)PGROUNDUP((uint64)pa_start);
  for (; p + PGSIZE <= (char *)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;
  int id; // CPU id

  if (((uint64)pa % PGSIZE) != 0 || (char *)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run *)pa;
  push_off();   // Tat ngat de tranh bi preempted
  id = cpuid(); // Lay CPU id hien tai

  acquire(&kmem[id].lock);
  r->next = kmem[id].freelist;
  kmem[id].freelist = r;
  release(&kmem[id].lock);

  pop_off(); // Bat lai ngat
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;
  int id; // CPU id

  push_off();   // Tat ngat de tranh bi preempted
  id = cpuid(); // Lay CPU id hien tai

  acquire(&kmem[id].lock);
  r = kmem[id].freelist;
  if (r)
    kmem[id].freelist = r->next;
  release(&kmem[id].lock);

  if (r) {
    KSTAT_INC(id, local_hit);
  } else {
    KSTAT_INC(id, local_miss);
    // Neu freelist CPU hien tai rong, thu muon 1 trang tu cac CPU donor khac
    for (int donor = 0; donor < NCPU; donor++) {
      if (donor == id)
        continue;

      KSTAT_INC(id, steal_attempts);
      acquire(&kmem[donor].lock);
      r = kmem[donor].freelist;
      if (r)
        kmem[donor].freelist = r->next;
      release(&kmem[donor].lock);

      if (r) {
        KSTAT_INC(id, steal_success);
        break;
      }
    }
  }

#if defined(ALLOC_BENCH) && ALLOC_BENCH
  if (r)
    KSTAT_INC(id, alloc_ok);
  else
    KSTAT_INC(id, alloc_fail);
#endif

  pop_off(); // Bat lai ngat

  if (r)
    memset((char *)r, 5, PGSIZE); // fill with junk

  return (void *)r;
}

uint64
kfreepage_count(void)
{
  struct run *r;
  uint64 count = 0;

  for (int i = 0; i < NCPU; i++) {
    acquire(&kmem[i].lock);

    r = kmem[i].freelist;
    while (r) {
      count++;
      r = r->next;
    }

    release(&kmem[i].lock);
  }

  return count;
}

#endif

// Cong don counter cac CPU (chi goi ngoai fast path)
void
kalloc_get_stats(struct kalloc_stats *out)
{
  if (out == 0)
    return;
  memset(out, 0, sizeof(*out));
#if defined(ALLOC_BENCH) && ALLOC_BENCH
  for (int i = 0; i < NCPU; i++) {
    out->alloc_ok += __atomic_load_n(&kstats[i].alloc_ok, __ATOMIC_RELAXED);
    out->alloc_fail += __atomic_load_n(&kstats[i].alloc_fail, __ATOMIC_RELAXED);
    out->local_hit += __atomic_load_n(&kstats[i].local_hit, __ATOMIC_RELAXED);
    out->local_miss += __atomic_load_n(&kstats[i].local_miss, __ATOMIC_RELAXED);
    out->steal_attempts +=
      __atomic_load_n(&kstats[i].steal_attempts, __ATOMIC_RELAXED);
    out->steal_success +=
      __atomic_load_n(&kstats[i].steal_success, __ATOMIC_RELAXED);
  }
#endif
}

// Dat lai counter ve 0 truoc moi luot do
void
kalloc_reset_stats(void)
{
#if defined(ALLOC_BENCH) && ALLOC_BENCH
  for (int i = 0; i < NCPU; i++) {
    __atomic_store_n(&kstats[i].alloc_ok, 0, __ATOMIC_RELAXED);
    __atomic_store_n(&kstats[i].alloc_fail, 0, __ATOMIC_RELAXED);
    __atomic_store_n(&kstats[i].local_hit, 0, __ATOMIC_RELAXED);
    __atomic_store_n(&kstats[i].local_miss, 0, __ATOMIC_RELAXED);
    __atomic_store_n(&kstats[i].steal_attempts, 0, __ATOMIC_RELAXED);
    __atomic_store_n(&kstats[i].steal_success, 0, __ATOMIC_RELAXED);
  }
#endif
}