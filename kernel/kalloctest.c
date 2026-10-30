// T02 / N03: harness kiem thu tang trang (kalloc/kfree).
// Chay tren CPU 0 ngay sau kinit(), truoc khi cac CPU khac khoi dong,
// nen khong co ai khac cap phat: so trang dem duoc la chinh xac.
// Bat bang: make qemu KALLOCTEST=1
// Moi dong log bat dau bang "KT|" de loc bang grep.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

#ifndef KT_REV
#define KT_REV "unknown"
#endif
#ifndef KT_CPUS
#define KT_CPUS 0
#endif

#define KT_N 64 // so trang song cung luc trong cac ca P01-P05

extern char end[]; // dia chi dau tien sau kernel (kernel.ld)

static int kt_pass, kt_fail;
static char *kt_pages[KT_N];

// In ket qua 1 ca: so actual voi want, dem PASS/FAIL
static void
kt_report(char *id, char *expected, uint64 actual, uint64 want, int ok)
{
  if (ok)
    kt_pass++;
  else
    kt_fail++;
  printk("KT|case=%s|expected=%s|want=%lu|actual=%lu|result=%s\n", id,
         expected, want, actual, ok ? "PASS" : "FAIL");
}

// P01: moi trang cap ra khac 0, chia het cho PGSIZE, nam trong [end, PHYSTOP)
static void
kt_alloc_align(void)
{
  int good = 0;
  for (int i = 0; i < KT_N; i++) {
    char *p = kalloc();
    kt_pages[i] = p;
    if (p != 0 && ((uint64)p % PGSIZE) == 0 &&
        p >= (char *)PGROUNDUP((uint64)end) && (uint64)p < PHYSTOP)
      good++;
  }
  kt_report("P01-align-range", "aligned_in_range", good, KT_N, good == KT_N);
}

void
kalloctest(int inject)
{
  kt_pass = kt_fail = 0;
  uint64 before = kfreepage_count();
  printk("KT|begin|rev=%s|cpus=%d|inject=%d|n=%d|free_before=%lu\n", KT_REV,
         KT_CPUS, inject, KT_N, before);
  kt_alloc_align();
  for (int i = 0; i < KT_N; i++) // tam thoi: tra lai trang (buoc 2 se kiem tra)
    if (kt_pages[i])
      kfree(kt_pages[i]);
  printk("KT|end|pass=%d|fail=%d|free_after=%lu|summary=%s\n", kt_pass,
         kt_fail, kfreepage_count(), kt_fail == 0 ? "ALL_PASS" : "HAS_FAIL");
}
