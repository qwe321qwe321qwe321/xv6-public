#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    printf(2, "usage: chgrp [file_name] [gid]\n[gid] is a number in range(0 ~ 127).\n");
    exit();
  }
  // Check file is exist.
  int fd;
  struct stat st;
  if ((fd = open(argv[1], 0)) < 0) {
    printf(2, "chgrp: cannot open %s\n", argv[1]);
    exit();
  }
  if (fstat(fd, &st) < 0) {
    printf(2, "chgrp: cannot get file stat\n");
    close(fd);
    exit();
  }
   // gid is in 0 ~ 127.
  int gid = atoi(argv[2]);
  if (gid > 127) {
    printf(2, "chgrp: gid is in range[0 ~ 127]\n");
    close(fd);
    exit();
  }
  // Call chgrp system call.
  chgrp(argv[1], gid);
   // Get stat again, check the change.
  fstat(fd, &st);
  printf(1, "Change %s group to (gid)%d\n", argv[1], st.gid);
  close(fd);
  exit();
}
