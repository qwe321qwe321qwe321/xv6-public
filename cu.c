#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  if (argc < 4) {
    printf(2, "usage: cu [username] [password] [gid]\nCreate a user with username and password.\n");
    exit();
  }
  char* username = argv[1];
  char* password = argv[2];
  int gid = atoi(argv[3]);
  int fd;

  if((fd = open("USERS", O_RDWR)) < 0){
      printf(2, "Cannot open USERS\n");
      exit();
  }
  char txtBuf[100];
  while(read(fd, txtBuf, sizeof(txtBuf)) > 0); // 讓fd的指標指到文件尾端
  printf(fd, "\n%s %s %d", username, password, gid); // Append一筆新資料
  close(fd);
  printf(1, "Create user %s\n", username);
  exit();
} 