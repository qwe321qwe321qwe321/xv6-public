#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if (argc < 1) {
    printf(2, "usage: su [uid]\n[uid] is a number in range(0 ~ 127).\n");
    exit();
  }
  su(atoi(argv[1]), 0);
  int uid = getuid();
  printf(1, "Current user:(uid)%d\n", uid);
  exit();
}
