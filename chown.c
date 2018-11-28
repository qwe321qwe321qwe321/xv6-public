#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    printf(2, "usage: chown [file_name] [uid]\n[uid] is a number in range(0 ~ 127).\n");
    exit();
  }
  // Check file is exist.
  int fd;
  struct stat st;
  if ((fd = open(argv[1], 0)) < 0) {
    printf(2, "chown: cannot open %s\n", argv[1]);
    exit();
  }
  if (fstat(fd, &st) < 0) {
    printf(2, "chown: cannot get file stat\n");
    close(fd);
    exit();
  }
  // uid is in 0 ~ 127.
  int uid = atoi(argv[2]);
  if (uid > 127) {
    printf(2, "chown: uid is in range[0 ~ 127]\n");
    close(fd);
    exit();
  }
  // Call chown system call.
  chown(argv[1], uid);
  // Get stat again, check the change.
  fstat(fd, &st);
  printf(1, "Change %s owner to (uid)%d\n", argv[1], st.uid);
  close(fd);
  exit();
}
