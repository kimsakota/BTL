// Operations shared by the xv6 test command and its kernel entry point.
enum {
  BUFTEST_PING,
  BUFTEST_NORMAL,
  BUFTEST_PIN,
  BUFTEST_CHECK_PAIR,
  BUFTEST_UNPIN,
  BUFTEST_RESET_PAIR,
  BUFTEST_OOM_PANIC,
};
#define BUFTEST_BLOCKS 32
