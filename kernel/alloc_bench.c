// W06 (#57): Harness do thu tang trang song song (baseline vs per-CPU),
// dump 4 class slab khi co client va workload object 5 vong sau warm-up.
// Bat bang: make qemu CPUS=<1|2> ALLOC_BENCH=1 [KALLOC_BASELINE=0|1]

#if defined(ALLOC_BENCH) && ALLOC_BENCH

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

#ifndef BENCH_CPUS
#define BENCH_CPUS 1
#endif

#if defined(KALLOC_BASELINE) && KALLOC_BASELINE
#define BENCH_BACKEND "baseline"
#else
#define BENCH_BACKEND "percpu"
#endif

#define AB_PAGE_WARMUP 1
#define AB_PAGE_RUNS   3
#define AB_PAGE_BATCH  32
#define AB_PAGE_ITERS  250

#define AB_OBJ_WARMUP 1
#define AB_OBJ_ROUNDS 5
#define AB_NCLASSES   4

#define AB_NPAGES   ((PHYSTOP - KERNBASE) / PGSIZE)
#define AB_BM_WORDS ((AB_NPAGES + 31) / 32)

extern char end[];

struct ab_barrier {
  int count;
  int sense;
};

static struct ab_barrier ab_bar;
static uint32 ab_observed_mask = 0;
static int ab_page_err = 0;

// Mang con tro va bitmap khai bao static (BSS) de khong tran stack0 (4KB)
static void *ab_worker_pages[NCPU][AB_PAGE_BATCH];
static uint32 ab_page_bm[AB_BM_WORDS];

static void *ab_obj_c0[70]; // req=40B   -> class64  (cap=63, >1 slab)
static void *ab_obj_c1[35]; // req=100B  -> class128 (cap=31, >1 slab)
static void *ab_obj_c2[18]; // req=136B  -> class256 (cap=15, >1 slab)
static void *ab_obj_c3[5];  // req=1112B -> class1280 (cap=3, >1 slab)

static void
ab_barrier_wait(struct ab_barrier *b, int nworkers, int *local_sense)
{
  *local_sense = !(*local_sense);
  if (__atomic_fetch_add(&b->count, 1, __ATOMIC_ACQ_REL) == nworkers - 1) {
    __atomic_store_n(&b->count, 0, __ATOMIC_RELAXED);
    __atomic_store_n(&b->sense, *local_sense, __ATOMIC_RELEASE);
  } else {
    while (__atomic_load_n(&b->sense, __ATOMIC_ACQUIRE) != *local_sense)
      ;
  }
}

static int
ab_page_valid(void *p)
{
  uint64 addr = (uint64)p;
  return p != 0 && (addr % PGSIZE) == 0 && addr >= PGROUNDUP((uint64)end) &&
         addr < PHYSTOP;
}

static int
ab_bm_claim(void *p)
{
  uint64 idx = ((uint64)p - KERNBASE) / PGSIZE;
  uint32 bit = 1U << (idx & 31);
  uint32 old = __atomic_fetch_or(&ab_page_bm[idx >> 5], bit, __ATOMIC_ACQ_REL);
  return (old & bit) == 0;
}

static void
ab_bm_release(void *p)
{
  uint64 idx = ((uint64)p - KERNBASE) / PGSIZE;
  uint32 bit = 1U << (idx & 31);
  __atomic_fetch_and(&ab_page_bm[idx >> 5], ~bit, __ATOMIC_ACQ_REL);
}

static int
ab_count_bits(uint32 mask)
{
  int cnt = 0;
  while (mask) {
    cnt += (int)(mask & 1U);
    mask >>= 1;
  }
  return cnt;
}

// ---------------------------------------------------------------------------
// Phan 1: Phep do trang song song (1 warm-up + 3 luot do)
// 1 op = 1 cap kalloc() + kfree(); thoi gian tinh bang r_time() ticks (10 MHz)
// ---------------------------------------------------------------------------
static void
ab_run_page_bench(int wid, int nworkers, int *sense)
{
  static uint64 t0, t1, free_before, free_after;
  int total_runs = AB_PAGE_WARMUP + AB_PAGE_RUNS;

  if (wid == 0) {
    memset(ab_page_bm, 0, sizeof(ab_page_bm));
    printk(
      "AB|meta|backend=%s|cpus=%d|workers_cfg=%d|time_unit=riscv_time_ticks"
      "|clock_hz=10000000|op_def=1_alloc_free_pair|batch=%d|iters=%d"
      "|warmup=%d|runs=%d|ram_mb=128\n",
      BENCH_BACKEND, BENCH_CPUS, nworkers, AB_PAGE_BATCH, AB_PAGE_ITERS,
      AB_PAGE_WARMUP, AB_PAGE_RUNS);
  }
  ab_barrier_wait(&ab_bar, nworkers, sense);

  for (int run = 0; run < total_runs; run++) {
    int is_warmup = (run < AB_PAGE_WARMUP) ? 1 : 0;
    int run_idx = is_warmup ? 0 : (run - AB_PAGE_WARMUP + 1);

    if (wid == 0) {
      __atomic_store_n(&ab_page_err, 0, __ATOMIC_RELAXED);
      free_before = kfreepage_count();
      kalloc_reset_stats();
      t0 = r_time();
    }
    ab_barrier_wait(&ab_bar, nworkers, sense);

    // Doan do thoi gian: TUYET DOI KHONG in log trong vong lap nay
    for (int it = 0; it < AB_PAGE_ITERS; it++) {
      int got = 0;
      for (int j = 0; j < AB_PAGE_BATCH; j++) {
        void *p = kalloc();
        if (!ab_page_valid(p)) {
          __atomic_store_n(&ab_page_err, 1, __ATOMIC_RELAXED);
          break;
        }
        if (!ab_bm_claim(p)) {
          // 2 worker duoc cap trung cung 1 trang vat ly -> loi nghiem trong
          __atomic_store_n(&ab_page_err, 2, __ATOMIC_RELAXED);
          break;
        }
        uint64 tag = ((uint64)(wid + 1) << 48) | ((uint64)(run + 1) << 32) |
                     ((uint64)it << 16) | (uint64)j;
        uint64 *u = (uint64 *)p;
        u[0] = tag;
        u[PGSIZE / sizeof(uint64) - 1] = ~tag;
        ab_worker_pages[wid][got++] = p;
      }

      for (int j = 0; j < got; j++) {
        void *p = ab_worker_pages[wid][j];
        uint64 tag = ((uint64)(wid + 1) << 48) | ((uint64)(run + 1) << 32) |
                     ((uint64)it << 16) | (uint64)j;
        uint64 *u = (uint64 *)p;
        if (u[0] != tag || u[PGSIZE / sizeof(uint64) - 1] != ~tag)
          __atomic_store_n(&ab_page_err, 3, __ATOMIC_RELAXED);
        ab_bm_release(p);
        kfree(p);
      }
    }

    ab_barrier_wait(&ab_bar, nworkers, sense);

    if (wid == 0) {
      t1 = r_time();
      struct kalloc_stats st;
      kalloc_get_stats(&st);
      free_after = kfreepage_count();

      uint64 dt = t1 - t0;
      uint64 total_ops = (uint64)nworkers * AB_PAGE_ITERS * AB_PAGE_BATCH;
      int err = __atomic_load_n(&ab_page_err, __ATOMIC_ACQUIRE);
      int obs_workers =
        ab_count_bits(__atomic_load_n(&ab_observed_mask, __ATOMIC_ACQUIRE));
      int ok =
        (err == 0 && free_after == free_before && obs_workers == nworkers &&
         st.alloc_ok == total_ops && st.alloc_fail == 0);

      printk("AB_PAGE|backend=%s|cpus=%d|workers=%d|obs_mask=0x%x"
             "|workload=page_batch_af|run=%d|warmup=%d|time_ticks=%lu"
             "|ops=%lu|alloc_ok=%lu|alloc_fail=%lu|local_hit=%lu"
             "|local_miss=%lu|steal_attempts=%lu|steal_success=%lu"
             "|free_before=%lu|free_after=%lu|err=%d|status=%s\n",
             BENCH_BACKEND, BENCH_CPUS, obs_workers, ab_observed_mask, run_idx,
             is_warmup, dt, total_ops, st.alloc_ok, st.alloc_fail, st.local_hit,
             st.local_miss, st.steal_attempts, st.steal_success, free_before,
             free_after, err, ok ? "PASS" : "FAIL");

      if (!ok)
        panic("alloc_bench: page benchmark verification failed");
    }
    ab_barrier_wait(&ab_bar, nworkers, sense);
  }
}

// ---------------------------------------------------------------------------
// Phan 2: Dump 4 class khi co client & Workload object 5 vong sau warm-up
// ---------------------------------------------------------------------------
static void
ab_run_object_bench(void)
{
  static const uint32 exp_size[AB_NCLASSES] = {64, 128, 256, 1280};
  static const uint32 exp_cap[AB_NCLASSES] = {63, 31, 15, 3};
  static const uint32 req_size[AB_NCLASSES] = {40, 100, 136, 1112};
  static const uint32 obj_cnt[AB_NCLASSES] = {70, 35, 18, 5};
  void **ptrs[AB_NCLASSES] = {ab_obj_c0, ab_obj_c1, ab_obj_c2, ab_obj_c3};

  // 2a. Dump 4 class khi dang co client that (filealloc + cac client da init)
  struct file *client_f = filealloc();
  if (client_f == 0)
    panic("alloc_bench: filealloc client failed");
  slab_dump_stats("w06_clients_active");
  fileclose(client_f);

  // 2b. Chup moc baseline (giu nguyen tai nguyen kernel/cache hop le)
  struct slab_class_stats base[AB_NCLASSES];
  for (int c = 0; c < AB_NCLASSES; c++) {
    if (slab_get_class_stats(c, &base[c]) != 0)
      panic("alloc_bench: base stats getter failed");
    if (base[c].class_size != exp_size[c] ||
        base[c].cap_per_slab != exp_cap[c] ||
        base[c].live + base[c].free_slots != base[c].total_capacity)
      panic("alloc_bench: slab class config/invariant mismatch");
  }
  slab_dump_stats("w06_obj_baseline");

  uint32 ref_pages[AB_NCLASSES];
  uint32 ref_empty[AB_NCLASSES];
  int total_rounds = AB_OBJ_WARMUP + AB_OBJ_ROUNDS;

  for (int r = 0; r < total_rounds; r++) {
    int is_warmup = (r < AB_OBJ_WARMUP) ? 1 : 0;
    int round_idx = is_warmup ? 0 : (r - AB_OBJ_WARMUP + 1);

    // Cap phat va ghi du lieu theo kich thuoc yeu cau thuc (req_size)
    for (int c = 0; c < AB_NCLASSES; c++) {
      for (uint32 i = 0; i < obj_cnt[c]; i++) {
        void *p = kmalloc(req_size[c]);
        if (p == 0 || ((uint64)p % 8) != 0)
          panic("alloc_bench: obj kmalloc failed or unaligned");
        memset(p, (char)(0x20 + c * 16 + (i & 15)), req_size[c]);
        ptrs[c][i] = p;
      }
    }

    // Chup dinh tai (peak) de ghi rounding / metadata / tail padding thuc
    struct slab_class_stats peak[AB_NCLASSES];
    for (int c = 0; c < AB_NCLASSES; c++) {
      if (slab_get_class_stats(c, &peak[c]) != 0)
        panic("alloc_bench: peak stats getter failed");
      if (peak[c].live + peak[c].free_slots != peak[c].total_capacity ||
          peak[c].live != base[c].live + obj_cnt[c])
        panic("alloc_bench: peak live invariant mismatch");
    }

    // Kiem tra toan ven du lieu va CHI giai phong object thu nghiem
    for (int c = 0; c < AB_NCLASSES; c++) {
      for (uint32 i = 0; i < obj_cnt[c]; i++) {
        unsigned char *b = (unsigned char *)ptrs[c][i];
        unsigned char exp = (unsigned char)(0x20 + c * 16 + (i & 15));
        for (uint32 k = 0; k < req_size[c]; k++) {
          if (b[k] != exp)
            panic("alloc_bench: obj data corrupted");
        }
        kmfree(ptrs[c][i]);
        ptrs[c][i] = 0;
      }
    }

    // Kiem tra sau giai phong: live ve baseline, retained pages on dinh
    for (int c = 0; c < AB_NCLASSES; c++) {
      struct slab_class_stats after;
      if (slab_get_class_stats(c, &after) != 0)
        panic("alloc_bench: after stats getter failed");

      int ok = 1;
      if (after.live + after.free_slots != after.total_capacity)
        ok = 0;
      if (after.live != base[c].live)
        ok = 0;

      if (round_idx == 1) {
        ref_pages[c] = after.slab_pages;
        ref_empty[c] = after.empty_slabs;
      } else if (round_idx >= 2) {
        if (after.slab_pages != ref_pages[c] ||
            after.empty_slabs != ref_empty[c])
          ok = 0;
      }

      uint32 req_B = obj_cnt[c] * req_size[c];
      uint32 stride_B = obj_cnt[c] * exp_size[c];
      uint32 rounding_B = stride_B - req_B;

      printk("AB_OBJ|backend=%s|cpus=%d|workload=obj_4class_5r|round=%d"
             "|warmup=%d|class=%d|cap=%d|req_size=%d|test_objs=%d"
             "|req_payload_B=%d|stride_B=%d|rounding_B=%d|meta_B=%d"
             "|tail_pad_B=%d|base_live=%d|peak_live=%d|after_live=%d"
             "|slab_pages=%d|empty_slabs=%d|retained_B=%d|status=%s\n",
             BENCH_BACKEND, BENCH_CPUS, round_idx, is_warmup, exp_size[c],
             exp_cap[c], req_size[c], obj_cnt[c], req_B, stride_B, rounding_B,
             peak[c].metadata_bytes, peak[c].tail_padding_bytes, base[c].live,
             peak[c].live, after.live, after.slab_pages, after.empty_slabs,
             after.retained_bytes, ok ? "PASS" : "FAIL");

      if (!ok)
        panic("alloc_bench: object retention/baseline check failed");
    }
  }

  slab_dump_stats("w06_obj_final");
}

void
alloc_bench(void)
{
  int wid = cpuid();
  int nworkers = BENCH_CPUS;
  int sense = 0;

  if (nworkers < 1)
    nworkers = 1;
  if (nworkers > NCPU)
    nworkers = NCPU;

  __atomic_fetch_or(&ab_observed_mask, 1U << wid, __ATOMIC_RELAXED);
  ab_barrier_wait(&ab_bar, nworkers, &sense);

  ab_run_page_bench(wid, nworkers, &sense);

  if (wid == 0) {
    ab_run_object_bench();
    printk("AB|done|backend=%s|cpus=%d|status=PASS\n", BENCH_BACKEND,
           BENCH_CPUS);
  }

  ab_barrier_wait(&ab_bar, nworkers, &sense);
}

#endif