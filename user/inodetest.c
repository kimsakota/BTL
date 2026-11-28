// Inode user tests (W06, I03): N03, N04, N05, N06.
// Built only with INODETEST=1; needs the inodestat() test syscall.
// st[0] = inode of fd, st[1] = inode found by key, st[2] = entries with that
// key, st[3] = its ref, st[4] = itable.live, st[5] = list length,
// st[6] = inodes freed with kmfree so far, st[7] = ifree calls so far.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#ifndef IT_REV
#define IT_REV "unknown"
#endif
#ifndef IT_CPUS
#define IT_CPUS 0
#endif

#define K      4  // N03 workers
#define ROUNDS 20 // N03 rounds
#define NKID   3  // N04 children
#define P6     5  // N06 processes
#define F6     12 // N06 files per process (16 fds - 4 kept)

static int pass, fail;

static void
report(char *id, char *expected, uint64 want, uint64 actual, int ok)
{
  if (ok)
    pass++;
  else
    fail++;
  printf("KT|case=%s|expected=%s|want=%lu|actual=%lu|result=%s\n", id, expected,
         want, actual, ok ? "PASS" : "FAIL");
}

static uint64
live(void)
{
  uint64 st[8];

  if (inodestat(-1, 1, st) < 0) {
    printf("KT|error|inodestat failed (kernel built without INODETEST?)\n");
    exit(1);
  }
  return st[4];
}

static uint
inum_of(char *path)
{
  struct stat s;

  if (stat(path, &s) < 0)
    return 0;
  return s.ino;
}

// N03: K workers open the same file at once; at the barrier all must see
// one inode with ref = K.
static void
n03(void)
{
  int ready[2], go[2], r, i, same = 0, one = 0, refok = 0;
  uint64 live0 = live(), p0, p, st[8];
  char c = 'g';

  close(open("it_n03", O_CREATE | O_RDWR));
  uint inum = inum_of("it_n03");
  for (r = 0; r < ROUNDS; r++) {
    pipe(ready);
    pipe(go);
    for (i = 0; i < K; i++) {
      if (fork() == 0) {
        int fd = open("it_n03", O_RDONLY);
        uint64 s[8];
        p = 0;
        if (fd >= 0 && inodestat(fd, 0, s) == 0)
          p = s[0];
        write(ready[1], &p, sizeof(p));
        read(go[0], &c, 1);
        close(fd);
        exit(0);
      }
    }
    int eq = 1;
    p0 = 0;
    for (i = 0; i < K; i++) {
      read(ready[0], &p, sizeof(p));
      if (i == 0)
        p0 = p;
      if (p == 0 || p != p0)
        eq = 0;
    }
    inodestat(-1, inum, st);
    if (eq && st[1] == p0)
      same++;
    if (st[2] == 1)
      one++;
    if (st[3] == K)
      refok++;
    for (i = 0; i < K; i++)
      write(go[1], &c, 1);
    for (i = 0; i < K; i++)
      wait(0);
    close(ready[0]);
    close(ready[1]);
    close(go[0]);
    close(go[1]);
  }
  unlink("it_n03");
  report("N03-same-pointer", "rounds", ROUNDS, same, same == ROUNDS);
  report("N03-one-entry", "rounds", ROUNDS, one, one == ROUNDS);
  report("N03-ref-K", "rounds", ROUNDS, refok, refok == ROUNDS);
  report("N03-live-back", "live0", live0, live(), live() == live0);
}

// N04: each forked child holds cwd through idup.
static void
n04(void)
{
  int go[2], i;
  uint64 st[8], ref0, live0 = live();
  char c = 'g';

  int fd = open(".", O_RDONLY);
  inodestat(fd, 0, st);
  ref0 = st[3];
  pipe(go);
  for (i = 0; i < NKID; i++) {
    if (fork() == 0) {
      read(go[0], &c, 1);
      exit(0);
    }
  }
  inodestat(fd, 0, st);
  report("N04-ref-up", "ref0+3", ref0 + NKID, st[3], st[3] == ref0 + NKID);
  for (i = 0; i < NKID; i++)
    write(go[1], &c, 1);
  for (i = 0; i < NKID; i++)
    wait(0);
  inodestat(fd, 0, st);
  report("N04-ref-back", "ref0", ref0, st[3], st[3] == ref0);
  close(fd);
  close(go[0]);
  close(go[1]);
  report("N04-live-back", "live0", live0, live(), live() == live0);
}

// N05: last close of an unlinked file frees the inode and its inum.
static void
n05(void)
{
  uint64 st[8], live0 = live();
  struct stat s;
  char buf[512];

  memset(buf, 'x', sizeof(buf));
  int fd = open("it_n05", O_CREATE | O_RDWR);
  write(fd, buf, sizeof(buf)); // one data block for itrunc to free
  fstat(fd, &s);
  uint inum = s.ino;
  unlink("it_n05");
  inodestat(fd, 0, st);
  report("N05-held-after-unlink", "count", 1, st[2], st[2] == 1);
  uint64 nfree0 = st[6], nifree0 = st[7];
  close(fd);
  inodestat(-1, inum, st);
  report("N05-kmfree-once", "delta", 1, st[6] - nfree0, st[6] - nfree0 == 1);
  report("N05-ifree-once", "delta", 1, st[7] - nifree0, st[7] - nifree0 == 1);
  report("N05-freed", "count", 0, st[2], st[2] == 0);
  report("N05-live-back", "live0", live0, st[4], st[4] == live0);
  fd = open("it_n05b", O_CREATE | O_RDWR);
  fstat(fd, &s);
  close(fd);
  unlink("it_n05b");
  report("N05-inum-reused", "inum", inum, s.ino, s.ino == inum);
}

// N06: 5 processes x 12 open files = 60 inodes alive, more than 50.
static void
n06(void)
{
  int ready[2], go[2], p, i, opened = 0;
  uint64 live0 = live(), mid;
  char c = 'g', name[8];

  pipe(ready);
  pipe(go);
  for (p = 0; p < P6; p++) {
    if (fork() == 0) {
      int fds[F6];
      char n = 0;
      close(ready[0]);
      close(go[1]);
      close(0); // keep 1, 2, ready[1], go[0]: 12 fds left
      name[0] = 'n';
      name[1] = '6';
      name[2] = '_';
      name[3] = 'a' + p;
      name[5] = 0;
      for (i = 0; i < F6; i++) {
        name[4] = 'a' + i;
        fds[i] = open(name, O_CREATE | O_RDWR);
        if (fds[i] >= 0)
          n++;
      }
      write(ready[1], &n, 1);
      read(go[0], &c, 1);
      for (i = 0; i < F6; i++) {
        name[4] = 'a' + i;
        if (fds[i] >= 0)
          close(fds[i]);
        unlink(name);
      }
      exit(0);
    }
  }
  for (p = 0; p < P6; p++) {
    char n = 0;
    read(ready[0], &n, 1);
    opened += n;
  }
  mid = live();
  printf("KT|info|n06_procs=%d|files_each=%d|fds_kept=4|nofile=16|nfile=100|"
         "open_fail=%d\n",
         P6, F6, P6 * F6 - opened);
  report("N06-open-all", "files", P6 * F6, opened, opened == P6 * F6);
  report("N06-live-over-50", "live0+60", live0 + P6 * F6, mid,
         mid >= live0 + P6 * F6 && mid > 50);
  for (p = 0; p < P6; p++)
    write(go[1], &c, 1);
  for (p = 0; p < P6; p++)
    wait(0);
  close(ready[0]);
  close(ready[1]);
  close(go[0]);
  close(go[1]);
  report("N06-live-back", "live0", live0, live(), live() == live0);
}

int
main(void)
{
  uint64 st[8];
  uint64 live0 = live();

  printf("KT|begin|harness=inode-user|rev=%s|cpus=%d|live_before=%lu\n", IT_REV,
         IT_CPUS, live0);
  n03();
  n04();
  n05();
  n06();
  inodestat(-1, 1, st);
  report("L01-list-matches-live", "len", st[4], st[5], st[5] == st[4]);
  printf(
    "KT|end|harness=inode-user|pass=%d|fail=%d|live_after=%lu|summary=%s\n",
    pass, fail, st[4], fail ? "HAS_FAIL" : "ALL_PASS");
  exit(0);
}
