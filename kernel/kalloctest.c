// T02 / N03: harness kiem thu tang trang (kalloc/kfree).
// Chay tren CPU 0 ngay sau kinit(), truoc khi cac CPU khac khoi dong,
// nen khong co ai khac cap phat: so trang dem duoc la chinh xac.
// Bat bang: make qemu KALLOCTEST=1   (them KALLOCTEST_INJECT=1 de tu kiem tra)
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
static int kt_inject; // 1 = co tinh gay loi de chung minh harness bat duoc
static char *kt_pages[KT_N];

// In ket qua 1 ca: so actual voi want, dem PASS/FAIL
static void
kt_report(char *id, char *expected, uint64 actual, uint64 want, int ok)
{
  if (ok)
    kt_pass++;
  else
    kt_fail++;
  printk("KT|case=%s|expected=%s|want=%lu|actual=%lu|result=%s\n", id, expected,
         want, actual, ok ? "PASS" : "FAIL");
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

// P02: kalloc phai lap day trang bang byte 5 (junk) truoc khi tra ra
static void
kt_junk_fill(void)
{
  int good = 0;
  for (int i = 0; i < KT_N; i++) {
    char *p = kt_pages[i];
    int ok = (p != 0);
    for (int j = 0; ok && j < PGSIZE; j++)
      if (p[j] != 5)
        ok = 0;
    good += ok;
  }
  kt_report("P02-junk-fill-5", "all_bytes_5", good, KT_N, good == KT_N);
}

// P03: KT_N trang dang song phai doi mot khac nhau (khong cap trung)
static void
kt_unique(void)
{
  char *copy[KT_N];
  uint64 dup = 0;
  for (int i = 0; i < KT_N; i++)
    copy[i] = kt_pages[i];
  if (kt_inject)
    copy[1] = copy[0]; // gia lap cap trung, chi tren ban sao
  for (int i = 0; i < KT_N; i++)
    for (int j = i + 1; j < KT_N; j++)
      if (copy[i] == copy[j])
        dup++;
  kt_report("P03-unique-live", "dup_pairs", dup, 0, dup == 0);
}

// P04: ghi mau rieng cho tung trang, doc lai sau khi ghi het
static void
kt_pattern(void)
{
  int good = 0;
  for (int i = 0; i < KT_N; i++) {
    uint64 *w = (uint64 *)kt_pages[i];
    for (int j = 0; j < PGSIZE / 8; j++)
      w[j] = ((uint64)i << 32) | (uint64)j;
  }
  if (kt_inject)
    ((uint64 *)kt_pages[3])[7] = 0; // gia lap ghi de
  for (int i = 0; i < KT_N; i++) {
    uint64 *w = (uint64 *)kt_pages[i];
    int ok = 1;
    for (int j = 0; ok && j < PGSIZE / 8; j++)
      if (w[j] != (((uint64)i << 32) | (uint64)j))
        ok = 0;
    good += ok;
  }
  kt_report("P04-pattern-rw", "pages_intact", good, KT_N, good == KT_N);
}

// P05: dem trang trong: cap N thi giam N, tra het thi ve ban dau
static void
kt_conserve(uint64 before)
{
  uint64 mid = kfreepage_count();
  kt_report("P05a-count-after-alloc", "before-N", mid, before - KT_N,
            mid == before - KT_N);

  int skip = kt_inject ? 5 : -1; // gia lap leak: tam giu lai 1 trang
  for (int i = 0; i < KT_N; i++)
    if (i != skip)
      kfree(kt_pages[i]);
  uint64 after = kfreepage_count();
  kt_report("P05b-count-after-free", "before", after, before, after == before);
  if (skip >= 0)
    kfree(kt_pages[skip]); // tra lai de he thong van dung
}

// P06: kfree lap day trang bang byte 1 (tru 8 byte dau la con tro next)
static void
kt_free_fill(void)
{
  char *p = kalloc();
  int ok = 0;
  if (p) {
    kfree(p);
    ok = 1;
    for (int j = 8; j < PGSIZE; j++)
      if (p[j] != 1)
        ok = 0;
  }
  kt_report("P06-free-fill-1", "bytes8_end_are_1", ok, 1, ok);
}

// P07: cap den khi het (OOM): so trang cap duoc = so trang trong,
// lan cap tiep theo tra ve 0, tra het thi so trang ve lai ban dau.
// Cac trang cap ra duoc noi thanh danh sach qua chinh 8 byte dau cua no.
static void
kt_exhaust(uint64 before)
{
  char *head = 0;
  uint64 got = 0, bad = 0;
  char *p;
  while ((p = kalloc()) != 0) {
    if (((uint64)p % PGSIZE) != 0 || (uint64)p >= PHYSTOP)
      bad++;
    *(char **)p = head;
    head = p;
    got++;
  }
  kt_report("P07a-oom-count", "before", got, before, got == before);
  kt_report("P07b-oom-bad-addr", "0", bad, 0, bad == 0);
  p = kalloc();
  kt_report("P07c-oom-returns-0", "0", (uint64)p, 0, p == 0);
  if (p)
    kfree(p);
  while (head) {
    char *next = *(char **)head;
    kfree(head);
    head = next;
  }
  uint64 after = kfreepage_count();
  kt_report("P07d-count-after-oom", "before", after, before, after == before);
}

void
kalloctest(int inject)
{
  kt_pass = kt_fail = 0;
  kt_inject = inject;
  uint64 before = kfreepage_count();
  printk("KT|begin|rev=%s|cpus=%d|inject=%d|n=%d|free_before=%lu\n", KT_REV,
         KT_CPUS, inject, KT_N, before);
  kt_alloc_align();
  kt_junk_fill();
  kt_unique();
  kt_pattern();
  kt_conserve(before);
  kt_free_fill();
  kt_exhaust(before);
  printk("KT|end|pass=%d|fail=%d|free_after=%lu|summary=%s\n", kt_pass, kt_fail,
         kfreepage_count(), kt_fail == 0 ? "ALL_PASS" : "HAS_FAIL");
}
