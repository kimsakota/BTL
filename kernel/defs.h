// clang-format off
struct buf;
struct context;
struct file;
struct inode;
struct pipe;
struct proc;
struct spinlock;
struct sleeplock;
struct stat;
struct superblock;

// bio.c
void            binit(void);
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
void            bwrite(struct buf*);
void            bpin(struct buf*);
void            bunpin(struct buf*);

// console.c
void            consoleinit(void);
void            consoleintr(int);
void            consputc(int);

// exec.c
int             kexec(char*, char**);

// file.c
struct file*    filealloc(void);
void            fileclose(struct file*);
struct file*    filedup(struct file*);
void            fileinit(void);
int             fileread(struct file*, uint64, int n);
int             filestat(struct file*, uint64 addr);
int             filewrite(struct file*, uint64, int n);

#if defined(FILE_TEST) && FILE_TEST
void            file_test(void);
#endif

// fs.c
void            fsinit(int);
int             dirlink(struct inode*, char*, uint);
struct inode*   dirlookup(struct inode*, char*, uint*);
struct inode*   ialloc(uint, short);
struct inode*   idup(struct inode*);
void            iinit();
void            ilock(struct inode*);
void            iput(struct inode*);
void            iunlock(struct inode*);
void            iunlockput(struct inode*);
void            iupdate(struct inode*);
int             namecmp(const char*, const char*);
struct inode*   namei(char*);
struct inode*   nameiparent(char*, char*);
int             readi(struct inode*, int, uint64, uint, uint);
void            stati(struct inode*, struct stat*);
int             writei(struct inode*, int, uint64, uint, uint);
void            itrunc(struct inode*);
void            ireclaim(int);

// kalloc.c
// Don vi counter trang (W06):
//   alloc_ok, alloc_fail : so lan goi kalloc() thanh cong / that bai (calls)
//   local_hit, local_miss: so lan lay duoc / khong lay duoc ngay tren freelist CPU hien tai (calls)
//   steal_attempts       : so luot khoa va thu muon tu 1 CPU donor khac (attempts)
//   steal_success        : so lan muon thanh cong = so trang muon duoc tu donor (pages/successes)
// Ghi chu: o backend baseline (single lock), khong co freelist cuc bo nen
//   local_hit = local_miss = steal_attempts = steal_success = 0.
struct kalloc_stats {
  uint64 alloc_ok;
  uint64 alloc_fail;
  uint64 local_hit;
  uint64 local_miss;
  uint64 steal_attempts;
  uint64 steal_success;
};

void*           kalloc(void); // Allocates one 4096-byte page of physical memory.
void            kfree(void *);
void            kinit(void);
uint64          kfreepage_count(void); // Returns the number of free pages in the physical memory.
void            kalloc_get_stats(struct kalloc_stats *out);
void            kalloc_reset_stats(void);

// kalloctest.c
void            kalloctest(int);

// alloc_bench.c
void            alloc_bench(void);

// pagestress.c
#ifdef PAGESTRESS
void            pagestress(void);
#endif

// log.c
void            initlog(int, struct superblock*);
void            log_write(struct buf*);
void            begin_op(void);
void            end_op(void);

// pipe.c
int             pipealloc(struct file**, struct file**);
void            pipeclose(struct pipe*, int);
int             piperead(struct pipe*, uint64, int);
int             pipewrite(struct pipe*, uint64, int);

// printk.c
int             printk(char*, ...) __attribute__ ((format (printf, 1, 2)));
void            panic(char*) __attribute__((noreturn));
void            printkinit(void);

// proc.c
int             cpuid(void);
void            kexit(int);
int             kfork(void);
int             growproc(int);
void            proc_mapstacks(pagetable_t);
pagetable_t     proc_pagetable(struct proc *);
void            proc_freepagetable(pagetable_t, uint64);
int             kkill(int);
int             killed(struct proc*);
void            setkilled(struct proc*);
struct cpu*     mycpu(void);
struct proc*    myproc();
void            procinit(void);
void            scheduler(void) __attribute__((noreturn));
void            sched(void);
void            sleep_prepare(void*);
void            sleep(void);
void            userinit(void);
int             kwait(uint64);
void            wakeup(void*);
void            yield(void);
int             either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
int             either_copyin(void *dst, int user_src, uint64 src, uint64 len);
void            procdump(void);

// swtch.S
void            swtch(struct context*, struct context*);

// spinlock.c
void            acquire(struct spinlock*);
int             holding(struct spinlock*);
void            initlock(struct spinlock*, char*);
void            release(struct spinlock*);
void            push_off(void);
void            pop_off(void);

// sleeplock.c
void            acquiresleep(struct sleeplock*);
void            releasesleep(struct sleeplock*);
int             holdingsleep(struct sleeplock*);
void            initsleeplock(struct sleeplock*, char*);

// string.c
int             memcmp(const void*, const void*, uint);
void*           memmove(void*, const void*, uint);
void*           memset(void*, int, uint);
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);

// syscall.c
void            argint(int, int*);
int             argstr(int, char*, int);
void            argaddr(int, uint64 *);
int             fetchstr(uint64, char*, int);
int             fetchaddr(uint64, uint64*);
void            syscall();

// trap.c
extern uint     ticks;
void            trapinit(void);
void            trapinithart(void);
extern struct spinlock tickslock;
void            prepare_return(void);

// uart.c
void            uartinit(void);
void            uartintr(void);
void            uartwrite(char [], int);
void            uartputc_sync(int);

// vm.c
void            kvminit(void);
void            kvminithart(void);
void            kvmmap(pagetable_t, uint64, uint64, uint64, int);
int             mappages(pagetable_t, uint64, uint64, uint64, int);
pagetable_t     uvmcreate(void);
uint64          uvmalloc(pagetable_t, uint64, uint64, int);
uint64          uvmdealloc(pagetable_t, uint64, uint64);
int             uvmcopy(pagetable_t, pagetable_t, uint64);
void            uvmfree(pagetable_t, uint64);
void            uvmunmap(pagetable_t, uint64, uint64, int);
void            uvmclear(pagetable_t, uint64);
pte_t *         walk(pagetable_t, uint64, int);
uint64          walkaddr(pagetable_t, uint64);
int             copyout(pagetable_t, uint64, uint64, char *, uint64);
int             copyin(pagetable_t, uint64, char *, uint64, uint64);
int             copyinstr(pagetable_t, uint64, char *, uint64, uint64);
int             ismapped(pagetable_t, uint64);
uint64          vmfault(pagetable_t, uint64, uint64, int);

// plic.c
void            plicinit(void);
void            plicinithart(void);
int             plic_claim(void);
void            plic_complete(int);

// virtio_disk.c
void            virtio_disk_init(void);
void            virtio_disk_rw(struct buf *, int);
void            virtio_disk_intr(void);

// number of elements in fixed-size array
#define NELEM(x) (sizeof(x) / sizeof((x)[0]))

// slab.c
void            kmalloc_init(void);
void*           kmalloc(uint64 size);
void            kmfree(void *ptr);
void            slab_test(void);
struct slab_class_stats {
  uint32 class_size;         // bytes: stride cua class (64, 128, 256, 1280)
  uint32 cap_per_slab;       // slots/slab: 63, 31, 15, 3
  uint32 slab_pages;         // pages: tong so trang slab cache dang giu
  uint32 empty_slabs;        // pages: so trang slab rong dang giu lai (retained)
  uint32 total_capacity;     // slots: slab_pages * cap_per_slab
  uint32 live;               // objects: so object dang song
  uint32 free_slots;         // slots: so slot dang ranh
  uint32 page_bytes;         // bytes: slab_pages * PGSIZE (4096 B)
  uint32 obj_capacity_bytes; // bytes: total_capacity * class_size
  uint32 live_obj_bytes;     // bytes: live * class_size (theo stride, chua phai payload thuc)
  uint32 free_slot_bytes;    // bytes: free_slots * class_size
  uint32 metadata_bytes;     // bytes: slab_pages * sizeof(slab_header) (32 B)
  uint32 tail_padding_bytes; // bytes: slab_pages * (PGSIZE - 32 - cap_per_slab * class_size)
  uint32 retained_bytes;     // bytes: empty_slabs * PGSIZE (khong goi la leak)
};

int             slab_get_class_stats(int idx, struct slab_class_stats *out);
void            slab_dump_stats(const char *tag);

// slabtest.c
void            slabtest(int);

#ifdef BUFTEST
int buftest_run(int, uint, uint*, int);
#endif
