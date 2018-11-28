#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int gid = getgid();
  printf(1, "Current group: (gid)%d\n", gid);
  exit();
}
