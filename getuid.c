#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int uid = getuid();
  printf(1, "Current user: (uid)%d\n", uid);
  exit();
}