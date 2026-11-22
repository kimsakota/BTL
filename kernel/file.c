//
// Support functions for system calls that involve file descriptors.
//

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "stat.h"
#include "proc.h"

struct devsw devsw[NDEV];
struct {
  struct spinlock lock;
  int open_count; // Giữ giới hạn tối đa NFILE file sống toàn hệ thống
} ftable;

void
fileinit(void)
{
  initlock(&ftable.lock, "ftable");
  ftable.open_count = 0;
}

// Allocate a file structure dynamically from Slab (class64).
struct file *
filealloc(void)
{
  struct file *f;

  // 1. Giữ suất tài nguyên dưới ftable.lock (chặn vượt NFILE)
  acquire(&ftable.lock);
  if (ftable.open_count >= NFILE) {
    release(&ftable.lock);
    return 0;
  }
  ftable.open_count++;
  release(&ftable.lock);

  // 2. Gọi kmalloc NGOÀI ftable.lock để không lồng khóa (ftable.lock vs slab.lock)
  f = (struct file *)kmalloc(sizeof(struct file));
  if (f == 0) {
    // 3. Rollback an toàn khi Slab OOM
    acquire(&ftable.lock);
    ftable.open_count--;
    release(&ftable.lock);
    return 0;
  }

  // 4. Thiết lập trạng thái ban đầu (slot đã được kmalloc zero-fill)
  f->ref = 1;
  f->type = FD_NONE;
  return f;
}

// Increment ref count for file f (dùng cho dup và fork).
struct file *
filedup(struct file *f)
{
  acquire(&ftable.lock);
  if (f->ref < 1)
    panic("filedup");
  f->ref++;
  release(&ftable.lock);
  return f;
}

// Close file f. (Decrement ref count, close and kmfree when reaches 0.)
void
fileclose(struct file *f)
{
  struct file ff;

  acquire(&ftable.lock);
  if (f->ref < 1)
    panic("fileclose");
  if (--f->ref > 0) {
    release(&ftable.lock);
    return;
  }

  // Lần đóng cuối cùng (ref == 0): sao chép ra stack và trả suất NFILE
  ff = *f;
  f->ref = 0;
  f->type = FD_NONE;
  if (ftable.open_count < 1)
    panic("fileclose: open_count");
  ftable.open_count--;
  release(&ftable.lock);

  // Trả struct file về lại Slab ngoài ftable.lock
  kmfree(f);

  // Đóng tài nguyên liên kết (pipe / inode) ngoài ftable.lock
  if (ff.type == FD_PIPE) {
    pipeclose(ff.pipe, ff.writable);
  } else if (ff.type == FD_INODE || ff.type == FD_DEVICE) {
    begin_op();
    iput(ff.ip);
    end_op();
  }
}

// Get metadata about file f.
// addr is a user virtual address, pointing to a struct stat.
int
filestat(struct file *f, uint64 addr)
{
  struct proc *p = myproc();
  struct stat st;

  if (f->type == FD_INODE || f->type == FD_DEVICE) {
    ilock(f->ip);
    stati(f->ip, &st);
    iunlock(f->ip);
    if (copyout(p->pagetable, p->sz, addr, (char *)&st, sizeof(st)) < 0)
      return -1;
    return 0;
  }
  return -1;
}

// Read from file f.
// addr is a user virtual address.
int
fileread(struct file *f, uint64 addr, int n)
{
  int r = 0;

  if (f->readable == 0 || n < 0)
    return -1;

  if (f->type == FD_PIPE) {
    r = piperead(f->pipe, addr, n);
  } else if (f->type == FD_DEVICE) {
    if (f->major < 0 || f->major >= NDEV || !devsw[f->major].read)
      return -1;
    r = devsw[f->major].read(1, addr, n);
  } else if (f->type == FD_INODE) {
    ilock(f->ip);
    if ((r = readi(f->ip, 1, addr, f->off, n)) > 0)
      f->off += r;
    iunlock(f->ip);
  } else {
    panic("fileread");
  }

  return r;
}

// Write to file f.
// addr is a user virtual address.
int
filewrite(struct file *f, uint64 addr, int n)
{
  int r, ret = 0;

  if (f->writable == 0 || n < 0)
    return -1;

  if (f->type == FD_PIPE) {
    ret = pipewrite(f->pipe, addr, n);
  } else if (f->type == FD_DEVICE) {
    if (f->major < 0 || f->major >= NDEV || !devsw[f->major].write)
      return -1;
    ret = devsw[f->major].write(1, addr, n);
  } else if (f->type == FD_INODE) {
    // write a few blocks at a time to avoid exceeding
    // the maximum log transaction size, including
    // i-node, indirect block, allocation blocks,
    // and 2 blocks of slop for non-aligned writes.
    int max = ((MAXOPBLOCKS - 1 - 1 - 2) / 2) * BSIZE;
    int i = 0;
    while (i < n) {
      int n1 = n - i;
      if (n1 > max)
        n1 = max;

      begin_op();
      ilock(f->ip);
      if ((r = writei(f->ip, 1, addr + i, f->off, n1)) > 0)
        f->off += r;
      iunlock(f->ip);
      end_op();

      if (r != n1) {
        // error from writei
        break;
      }
      i += r;
    }
    ret = (i == n ? n : -1);
  } else {
    panic("filewrite");
  }

  return ret;
}
#if defined(FILE_TEST) && FILE_TEST
struct kpage_node {
  struct kpage_node *next;
};

void
file_test(void)
{
  static struct file *files[NFILE];
  static void *drain64[512];

  printk("file_test: starting file lifecycle, NFILE & OOM verification...\n");

  // [1/3] Kiem tra filealloc -> filedup -> fileclose (chi kmfree o lan dong cuoi)
  struct file *f = filealloc();
  if (f == 0 || ((uint64)f % 8) != 0 || f->ref != 1 || f->type != FD_NONE)
    panic("file_test: initial filealloc failed");
  if (ftable.open_count != 1)
    panic("file_test: open_count != 1 after filealloc");

  if (filedup(f) != f || f->ref != 2 || ftable.open_count != 1)
    panic("file_test: filedup failed");

  fileclose(f); // Giam ref: 2 -> 1 (chua duoc kmfree)
  if (f->ref != 1 || ftable.open_count != 1)
    panic("file_test: intermediate fileclose state mismatch");

  fileclose(f); // Lan dong cuoi: ref 1 -> 0 -> open_count=0 va kmfree(f)
  if (ftable.open_count != 0)
    panic("file_test: open_count != 0 after final fileclose");

  // Cap lai ngay: xac nhan slot da ve class64 (LIFO reuse + zero-fill)
  struct file *f_re = filealloc();
  if (f_re != f || f_re->ref != 1 || f_re->type != FD_NONE)
    panic("file_test: slab LIFO reuse after final fileclose failed");
  fileclose(f_re);
  if (ftable.open_count != 0)
    panic("file_test: open_count != 0 after reuse close");
  printk("file_test: [1/3] alloc/dup/final-close & slab reuse OK\n");

  // [2/3] Kiem tra gioi han tai nguyen NFILE
  for (int i = 0; i < NFILE; i++) {
    files[i] = filealloc();
    if (files[i] == 0)
      panic("file_test: failed before reaching NFILE");
  }
  if (ftable.open_count != NFILE)
    panic("file_test: open_count != NFILE at limit");
  if (filealloc() != 0)
    panic("file_test: exceeded NFILE limit");
  if (ftable.open_count != NFILE)
    panic("file_test: open_count changed after NFILE rejection");

  for (int i = 0; i < NFILE; i++)
    fileclose(files[i]);
  if (ftable.open_count != 0)
    panic("file_test: open_count != 0 after closing NFILE");
  printk("file_test: [2/3] NFILE limit (%d) & cleanup OK\n", NFILE);

  // [3/3] Kiem tra OOM thuc te & rollback open_count + retry
  struct kpage_node *oom_pages = 0;
  void *pg;
  while ((pg = kalloc()) != 0) {
    struct kpage_node *node = (struct kpage_node *)pg;
    node->next = oom_pages;
    oom_pages = node;
  }

  // Hut het cac slot ranh dang con trong class64 den khi kmalloc(64) tra 0
  int drain_cnt = 0;
  void *s;
  while ((s = kmalloc(64)) != 0) {
    if (drain_cnt >= 512)
      panic("file_test: drain64 overflow");
    drain64[drain_cnt++] = s;
  }

  // Luc nay class64 het slot va kalloc() can -> filealloc() phai tra 0 va rollback
  if (filealloc() != 0 || filealloc() != 0)
    panic("file_test: expected 0 on slab OOM");
  if (ftable.open_count != 0)
    panic("file_test: open_count leaked after OOM rollback");

  // Hoan tra RAM va slot tam
  while (oom_pages != 0) {
    struct kpage_node *next = oom_pages->next;
    kfree((void *)oom_pages);
    oom_pages = next;
  }
  for (int i = 0; i < drain_cnt; i++)
    kmfree(drain64[i]);

  // Retry sau OOM: cap du NFILE de chung minh khong hut suat nao
  for (int i = 0; i < NFILE; i++) {
    files[i] = filealloc();
    if (files[i] == 0)
      panic("file_test: retry alloc after OOM failed");
  }
  for (int i = 0; i < NFILE; i++)
    fileclose(files[i]);
  if (ftable.open_count != 0)
    panic("file_test: open_count != 0 after OOM retry cleanup");

  printk("file_test: [3/3] OOM (drained %d c64 slots), rollback & retry OK\n",
         drain_cnt);
  printk("file_test: ALL PASSED\n");
}
#endif
