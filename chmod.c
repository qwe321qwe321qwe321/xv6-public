#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    printf(2, "usage: chmod [file_name] [mode]\n");
    exit();
  }
  // Check file is exist.
  int fd;
  struct stat st;
  if ((fd = open(argv[1], 0)) < 0) {
    printf(2, "chmod: cannot open %s\n", argv[1]);
    exit();
  }
  if (fstat(fd, &st) < 0) {
    printf(2, "chmod: cannot get file stat\n");
    close(fd);
    exit();
  }
  // Just need a number in 0 ~ 777.(like linux.)
  int mode = atoi(argv[2]) % 1000;
  int hundred = mode / 100;
  int ten = mode % 100 / 10;
  int one = mode % 10;
  if (hundred > 7 || ten > 7 || one > 7) {
    printf(2, "chmod: mode number must be 000 ~ 777\n");
    close(fd);
    exit();
  }
  // transform to binary.
  mode = (hundred << 6) + (ten << 3) + one;
  // Call chown system call.
  chmod(argv[1], mode);
   // Get stat again, check the change.
  fstat(fd, &st);
  printf(1, "Change %s group to (mode)%d\n", argv[1], st.mode);
  close(fd);
  exit();
}
