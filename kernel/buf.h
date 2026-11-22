struct buf {
  int valid; // has data been read from disk?
  int disk;  // does disk "own" buf?
  uint dev;
  uint blockno;
  struct sleeplock lock;
  uint refcnt;      // biến đếm số lượng tham chiếu đến buffer
  struct buf *prev; // LRU cache list
  struct buf *next;
  uchar data[BSIZE];
};
