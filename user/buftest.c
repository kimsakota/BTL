#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/buftest.h"
#include "user/user.h"

static void
worker(int fd, int start[2], int ready[2], int done[2], int slot)
{
  char state = 'F';
  char go;
  close(start[1]);
  close(ready[0]);
  close(done[1]);
  int started = read(start[0], &go, 1);
  close(start[0]);
  if (started == 1 && buftest(fd, BUFTEST_PIN, slot) == 0)
    state = 'P';
  int sent = write(ready[1], &state, 1);
  close(ready[1]);
  int received = read(done[0], &go, 1);
  close(done[0]);
  int released = state == 'P' ? buftest(fd, BUFTEST_UNPIN, slot) : -1;
  close(fd);
  exit(sent != 1 || received != 1 || released != 0);
}

static int
worker_pair(int fd)
{
  int start[2], ready[2], done[2];
  if (buftest(fd, BUFTEST_RESET_PAIR, 0) < 0 || pipe(start) < 0)
    return -1;
  if (pipe(ready) < 0) {
    close(start[0]);
    close(start[1]);
    return -1;
  }
  if (pipe(done) < 0) {
    close(start[0]);
    close(start[1]);
    close(ready[0]);
    close(ready[1]);
    return -1;
  }

  int a = fork();
  if (a == 0)
    worker(fd, start, ready, done, 0);
  int b = a < 0 ? -1 : fork();
  if (b == 0)
    worker(fd, start, ready, done, 1);
  close(start[0]);
  int started = 0;
  if (a > 0 && b > 0)
    started = write(start[1], "GG", 2);
  close(start[1]);
  close(ready[1]);
  close(done[0]);

  int rc = -1;
  char x, y;
  if (started == 2 && read(ready[0], &x, 1) == 1 &&
      read(ready[0], &y, 1) == 1 && x == 'P' && y == 'P')
    rc = buftest(fd, BUFTEST_CHECK_PAIR, 0);
  close(ready[0]);

  // One release signal for each successfully created child.
  if (a > 0 && write(done[1], "G", 1) != 1)
    rc = -1;
  if (b > 0 && write(done[1], "G", 1) != 1)
    rc = -1;
  close(done[1]);
  for (int i = 0; i < (a > 0) + (b > 0); i++) {
    int status;
    if (wait(&status) < 0 || status != 0)
      rc = -1;
  }
  return rc;
}

int
main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(2, "usage: buftest ping|normal1|normal2|oom-panic\n");
    exit(1);
  }

  if (strcmp(argv[1], "ping") == 0)
    exit(buftest(-1, BUFTEST_PING, 0) != 0);
  int dual = strcmp(argv[1], "normal2") == 0;
  int panic_mode = strcmp(argv[1], "oom-panic") == 0;
  if (!dual && !panic_mode && strcmp(argv[1], "normal1") != 0)
    exit(1);

  int fd = open("/bufdata", O_RDONLY);
  if (fd < 0) {
    fprintf(2, "buftest: cannot open /bufdata\n");
    exit(1);
  }

  if (panic_mode) {
    buftest(fd, BUFTEST_OOM_PANIC, 0);
    fprintf(2, "I04|suite=oom-panic|result=FAIL\n");
    close(fd);
    exit(1);
  }

  int ok = 1;
  if (dual) {
    int rc = 1;
    for (int i = 0; i < 12 && rc == 1; i++)
      rc = worker_pair(fd);
    ok = rc == 0;
    printf("I04|case=pair-two-harts|result=%s\n", ok ? "PASS" : "FAIL");
  } else {
    printf("I04|case=pair-two-harts|result=SKIP\n");
  }

  if (buftest(fd, BUFTEST_NORMAL, 0) < 0)
    ok = 0;
  close(fd);
  printf("I04|suite=%s|result=%s\n", argv[1], ok ? "PASS" : "FAIL");
  exit(!ok);
}
