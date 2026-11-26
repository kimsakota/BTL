// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.

#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"

#ifdef BUFTEST
#include "buftest.h"
#include "proc.h"
#endif

struct {
  struct spinlock lock;
  int nbuf;
  struct buf head;
} bcache;

// Hàm khởi tạo bộ đệm (buffer cache) và kiểm tra tính toàn vẹn của nó.
void
binit(void)
{
  initlock(&bcache.lock, "bcache");

  bcache.nbuf = 0;
  bcache.head.prev = &bcache.head;
  bcache.head.next = &bcache.head;

#ifdef BUFTEST
  printk("I04|phase=init|cached=%d\n", bcache.nbuf);
#endif // BUFTEST

  /*  struct buf *b; // Con trỏ đến một buffer trong bộ đệm.

  initlock(&bcache.lock, "bcache");

  // Create linked list of buffers
  bcache.head.prev = &bcache.head;
  bcache.head.next = &bcache.head;
  for (b = bcache.buf; b < bcache.buf + NBUF; b++) {
    b->next = bcache.head.next;
    b->prev = &bcache.head;
    initsleeplock(&b->lock, "buffer");
    bcache.head.next->prev = b;
    bcache.head.next = b;
  }*/

  /*#ifdef BUFTEST // Chỉ biên dịch khi BUFTEST đã được bật
  int seen = 0;

  for (b = bcache.head.next; b != &bcache.head; b = b->next) {
    // Kiểm tra liên kết kế tiếp và liên kết trước của buffer có bị null hay không.
    if (b == 0 || b->next == 0 || b->prev == 0)
      panic("buftest: null link");

    // Kiểm tra hai chiều của danh sách có khớp hay không
    if (b->next->prev != b || b->prev->next != b)
      panic("buftest: broken links");

    // Kiểm tra trạng thái ban đầu của buffer có đúng hay không
    if (b->refcnt != 0 || b->valid != 0 || b->disk != 0)
      panic("buftest: unexpected initial state");

    seen++;
    if (seen > NBUF)
      panic("buftest: cycle");
  }

  if (seen != NBUF)
    panic("buftest: wrong count");

  printk("I04|phase=init|buf_B=%lu|align_B=%lu|count=%d|result=PASS\n",
         (uint64)sizeof(struct buf), (uint64) _Alignof(struct buf), seen);
#endif*/
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf *
bget(uint dev, uint blockno, int fail_alloc, int *hart)
{
  struct buf *b;

  acquire(&bcache.lock);

  if (hart)
    *hart = cpuid();

  // Is the block already cached?
  for (b = bcache.head.next; b != &bcache.head; b = b->next) {
    if (b->dev == dev && b->blockno == blockno) {
      b->refcnt++;
      release(&bcache.lock);
      acquiresleep(&b->lock);
      return b;
    }
  }

  // Not cached.
  // Recycle the least recently used (LRU) unused buffer.
  if (bcache.nbuf < NBUF) {
    b = fail_alloc ? 0 : kmalloc(sizeof(*b));
    if (b != 0) {
      initsleeplock(&b->lock, "buffer");
      b->dev = dev;
      b->blockno = blockno;
      b->valid = 0;
      b->disk = 0;
      b->refcnt = 1;

      // head.next là buffer đầu tiên, head chỉ là buffer giả lập để quản lý danh sách liên kết
      b->next = bcache.head.next;
      b->prev = &bcache.head;
      bcache.head.next->prev = b;
      bcache.head.next = b;
      bcache.nbuf++;

      release(&bcache.lock);

      // Hàm này sẽ khóa buffer mới được cấp phát để đảm bảo rằng tiến trình hiện tại có quyền truy cập vào dữ liệu của buffer đó.
      acquiresleep(&b->lock);
      return b;
    }
  }

  // If all buffers are in use, wait for one to become free
  for (b = bcache.head.prev; b != &bcache.head; b = b->prev) {
    if (b->refcnt == 0) {
      b->dev = dev;
      b->blockno = blockno;
      b->valid = 0;
      b->refcnt = 1;
      release(&bcache.lock);
      acquiresleep(&b->lock);
      return b;
    }
  }
  panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
static struct buf *
bread_with_hart(uint dev, uint blockno, int *hart)
{
  struct buf *b;

  b = bget(dev, blockno, 0, hart);
  if (!b->valid) {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

struct buf *
bread(uint dev, uint blockno)
{
  return bread_with_hart(dev, blockno, 0);
}

// Write b's contents to disk.  Must be locked.
// Only the log calls bwrite.
void
bwrite(struct buf *b)
{
  if (!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if (!holdingsleep(&b->lock))
    panic("brelse");

  releasesleep(&b->lock);

  acquire(&bcache.lock);
  b->refcnt--;
  if (b->refcnt == 0) {
    // no one is waiting for it.
    b->next->prev = b->prev;
    b->prev->next = b->next;
    b->next = bcache.head.next;
    b->prev = &bcache.head;
    bcache.head.next->prev = b;
    bcache.head.next = b;
  }

  release(&bcache.lock);
}

void
bpin(struct buf *b)
{
  acquire(&bcache.lock);
  b->refcnt++;
  release(&bcache.lock);
}

void
bunpin(struct buf *b)
{
  acquire(&bcache.lock);
  b->refcnt--;
  release(&bcache.lock);
}

#ifdef BUFTEST
#define TEST_BLOCKS BUFTEST_BLOCKS

struct test_slot {
  struct buf *buffer;
  int hart;
  int pid;
};

static struct test_slot test_slots[2];
static int test_arrivals;

struct test_snapshot {
  int count;
  int refs;
  int matches;
  int listed;
  int bad;
};

static struct test_snapshot
test_snapshot(uint dev, uint blockno)
{
  struct test_snapshot s = {0};
  acquire(&bcache.lock);
  s.count = bcache.nbuf;
  for (struct buf *b = bcache.head.next; b != &bcache.head; b = b->next) {
    if (++s.listed > NBUF) {
      s.bad = 1;
      break;
    }
    if (b->prev->next != b || b->next->prev != b)
      s.bad = 1;
    s.refs += b->refcnt;
    if (b->dev == dev && b->blockno == blockno)
      s.matches++;
  }
  release(&bcache.lock);
  if (s.count != s.listed)
    s.bad = 1;
  return s;
}

static int
test_assert(char *name, int want, int actual)
{
  printk("I04|case=%s|want=%d|actual=%d|result=%s\n", name, want, actual,
         want == actual ? "PASS" : "FAIL");
  return want != actual;
}

static int
test_data(struct buf *b, int index)
{
  if (!b->valid || b->disk != 0)
    return 0;
  for (int i = 0; i < BSIZE; i++)
    if (b->data[i] != index + 1)
      return 0;
  return 1;
}

static int
test_oom(uint dev, uint *blocks, int expect_panic)
{
  struct buf *held[NBUF];
  struct buf *victim = 0;
  int pinned = 0;
  int matches = 0;
  int count;

  // Boot and exec can fill the cache before the shell runs this test.
  // Make one empty entry position so bget takes its allocation path.
  struct buf *idle = 0;
  acquire(&bcache.lock);
  if (bcache.nbuf == NBUF) {
    for (struct buf *b = bcache.head.prev; b != &bcache.head; b = b->prev) {
      if (b->refcnt == 0) {
        idle = b;
        b->prev->next = b->next;
        b->next->prev = b->prev;
        bcache.nbuf--;
        break;
      }
    }
  }
  release(&bcache.lock);
  if (idle)
    kmfree(idle);

  acquire(&bcache.lock);
  count = bcache.nbuf;
  for (struct buf *b = bcache.head.prev; b != &bcache.head; b = b->prev) {
    if (b->dev == dev && b->blockno == blocks[TEST_BLOCKS - 1])
      matches++;
    if (victim == 0 && b->refcnt == 0)
      victim = b;
  }

  if (count == 0 || count >= NBUF || matches != 0 || victim == 0) {
    release(&bcache.lock);
    printk("I04|case=oom-setup|cached=%d|target_entries=%d|result=FAIL\n",
           count, matches);
    return -1;
  }

  if (expect_panic) {
    // Hold every current entry without holding any buffer sleeplock.
    for (struct buf *b = bcache.head.next; b != &bcache.head; b = b->next) {
      held[pinned++] = b;
      b->refcnt++;
    }
  }
  release(&bcache.lock);

  printk("I04|case=%s|cached=%d|limit=%d|evicted_for_setup=%d|forced=1\n",
         expect_panic ? "oom-panic" : "oom-fallback", count, NBUF, idle != 0);
  struct buf *b = bget(dev, blocks[TEST_BLOCKS - 1], 1, 0);
  if (expect_panic) {
    brelse(b);
    for (int i = 0; i < pinned; i++)
      bunpin(held[i]);
    test_assert("oom-expected-panic", 1, 0);
    return -1;
  }

  int fail = test_assert("oom-reused-lru", 1, b == victim);
  fail |= test_assert("oom-reset-valid", 0, b->valid);
  brelse(b);

  b = bread(dev, blocks[TEST_BLOCKS - 1]);
  fail |= test_assert("oom-read-data", 1, test_data(b, TEST_BLOCKS - 1));
  brelse(b);

  struct test_snapshot s = test_snapshot(dev, blocks[TEST_BLOCKS - 1]);
  fail |= test_assert("oom-kept-cache-size", count, s.count);
  fail |= test_assert("oom-one-entry", 1, s.matches);
  return fail ? -1 : 0;
}

static int
test_normal(uint dev, uint *blocks)
{
  if (test_oom(dev, blocks, 0) < 0)
    return -1;

  struct test_snapshot initial = test_snapshot(dev, blocks[0]);
  struct buf *a = bread(dev, blocks[0]);
  int fail = test_assert("pin-initial-data", 1, test_data(a, 0));
  bpin(a);
  brelse(a);

  // More keys than slots, while block zero remains pinned.
  for (int i = 1; i < TEST_BLOCKS; i++) {
    struct buf *b = bread(dev, blocks[i]);
    if (!test_data(b, i) || b == a)
      fail = 1;
    brelse(b);
  }

  acquire(&bcache.lock);
  int pinned_intact =
    a->dev == dev && a->blockno == blocks[0] && a->refcnt == 1;
  release(&bcache.lock);
  fail |= test_assert("pin-survived-pressure", 1, pinned_intact);
  bunpin(a);

  struct test_snapshot warm = test_snapshot(dev, blocks[0]);
  fail |= test_assert("fill-count", NBUF, warm.count);
  fail |= test_assert("pin-balanced", initial.refs, warm.refs);

  // All blocks were allocated in the fixture. Reading key order repeatedly
  // forces cache misses and verifies data after reuse.
  uint64 pages = kfreepage_count();
  int stable = 1;
  int good_data = 1;
  for (int round = 0; round < 5; round++) {
    for (int i = 0; i < TEST_BLOCKS; i++) {
      struct buf *b = bread(dev, blocks[i]);
      if (!test_data(b, i))
        good_data = 0;
      brelse(b);
    }
    struct test_snapshot s = test_snapshot(dev, blocks[0]);
    if (s.bad || s.count != warm.count || s.refs != warm.refs ||
        kfreepage_count() != pages)
      stable = 0;
  }
  fail |= test_assert("reuse-data", 1, good_data);
  fail |= test_assert("warmup-five-rounds", 1, stable);

  // After unpin and sustained replacement, the old key may have been read
  // again. Verify its former slot has appeared under a different key.
  // Scan the current list under its lock instead of trusting the old key.
  acquire(&bcache.lock);
  int recycled = 0;
  for (struct buf *b = bcache.head.next; b != &bcache.head; b = b->next)
    if (b == a && b->refcnt == 0 && b->dev == dev && b->blockno != blocks[0])
      recycled = 1;
  release(&bcache.lock);
  fail |= test_assert("unpinned-slot-reused", 1, recycled);
  return fail ? -1 : 0;
}

int
buftest_run(int mode, uint dev, uint *blocks, int slot)
{
  if (mode == BUFTEST_PING) {
    test_assert("ping", 1, 1);
    return 0;
  }
  if (mode == BUFTEST_NORMAL)
    return test_normal(dev, blocks);
  if (mode == BUFTEST_OOM_PANIC)
    return test_oom(dev, blocks, 1);
  if (mode == BUFTEST_RESET_PAIR) {
    __atomic_store_n(&test_arrivals, 0, __ATOMIC_RELEASE);
    return 0;
  }

  if (mode == BUFTEST_PIN) {
    if (slot < 0 || slot > 1)
      return -1;
    // Short, bounded rendezvous before lookup. No cache or inode lock held.
    push_off();
    __atomic_fetch_or(&test_arrivals, 1 << slot, __ATOMIC_ACQ_REL);
    for (int i = 0; i < 1000000; i++)
      if (__atomic_load_n(&test_arrivals, __ATOMIC_ACQUIRE) == 3)
        break;
    pop_off();
    int hart = -1;
    struct buf *b = bread_with_hart(dev, blocks[0], &hart);
    if (!test_data(b, 0)) {
      brelse(b);
      return -1;
    }
    bpin(b);
    brelse(b);

    acquire(&bcache.lock);
    if (test_slots[slot].buffer) {
      release(&bcache.lock);
      bunpin(b);
      return -1;
    }
    test_slots[slot] = (struct test_slot){b, hart, myproc()->pid};
    release(&bcache.lock);
    return 0;
  }

  if (mode == BUFTEST_UNPIN) {
    if (slot < 0 || slot > 1)
      return -1;
    acquire(&bcache.lock);
    struct buf *b = test_slots[slot].buffer;
    if (!b || test_slots[slot].pid != myproc()->pid) {
      release(&bcache.lock);
      return -1;
    }
    test_slots[slot].buffer = 0;
    release(&bcache.lock);
    bunpin(b);
    return 0;
  }

  if (mode == BUFTEST_CHECK_PAIR) {
    int entries = 0, refs = 0;
    acquire(&bcache.lock);
    struct test_slot a = test_slots[0];
    struct test_slot b = test_slots[1];
    for (struct buf *cur = bcache.head.next; cur != &bcache.head;
         cur = cur->next) {
      if (cur->dev == dev && cur->blockno == blocks[0]) {
        entries++;
        refs += cur->refcnt;
      }
    }
    release(&bcache.lock);
    printk("I04|case=pair-harts|first=%d|second=%d\n", a.hart, b.hart);
    int fail =
      test_assert("pair-workers", 1, a.buffer && b.buffer && a.pid != b.pid);
    fail |=
      test_assert("pair-same-buffer", 1, a.buffer && a.buffer == b.buffer);
    fail |= test_assert("pair-one-entry", 1, entries);
    fail |= test_assert("pair-two-pins", 2, refs);
    if (fail)
      return -1;
    return a.hart == b.hart ? 1 : 0;
  }
  return -1;
}
#endif
