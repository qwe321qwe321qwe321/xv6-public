#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
getInput(char *buf, int nbuf)
{
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  buf[strlen(buf) - 1] = '\0'; // 刪除換行符
  return 0;
}

typedef struct accountInfo {
  int uid;
  int gid;
  char username[32];
  char password[32];
} AccountInfo;

// 用特定字元切開字串
int split(char* buf, char atom, char** outBuf) {
  int i, j = 0, outLen = 0;
  for(i = 0; buf[i] != 0; i += 1) {
      if(buf[i] == atom) {
        j = 0;
        outLen += 1;
        continue;
      }
      outBuf[outLen][j++] = buf[i];
  }
  // 判斷最後一組是否空白, 空白就不要添加
  if(j != 0) {
    outLen += 1;
  }
  /*for(i = 0; i < outLen; i += 1) {
    printf(1, "%s\n", outBuf[i]);
  }*/
  return outLen;
}
// 將USERS檔案內容轉成AccountInfo結構陣列
int splitUserList(char* txtBuf, AccountInfo* infos) {
  int i;
  char** lines = malloc(32 * sizeof(char*)); // 開[32][128]空間給行字串
  for (i = 0; i < 32; i += 1) {
    lines[i] = malloc(128 * sizeof(char));
  }
  char** elements = malloc(3 * sizeof(char*)); // 每行字串以空白分隔成3項資料，分別是Username Password GroupID
  for (i = 0; i < 3; i += 1) {
    elements[i] = malloc(32 * sizeof(char));
  }
  int lineCount;
  lineCount = split(txtBuf, '\n', lines); // 切開資料行

  for(i = 0; i < lineCount; i += 1) {
    // 每行字串以空白分隔成3項資料，分別是Username Password GroupID
    split(lines[i], ' ', elements);
    infos[i].uid = i;
    memmove(infos[i].username, elements[0], 32);
    memmove(infos[i].password, elements[1], 32);
    infos[i].gid = atoi(elements[2]);
  }
  return lineCount;
}

char *argv[] = { "sh", 0 };

int
main(void)
{
  printf(1, "====Login System====\n");

  //static char buf[100];
  int fd;
  int i;

  char txtBuf[512];
  memset(txtBuf, 0, 512);
  if((fd = open("USERS", O_RDONLY)) < 0){
      printf(2, "Cannot open USERS\n");
      exit();
  }
  int n;
  AccountInfo* infos = malloc(32 * sizeof(AccountInfo));
  int userCount = 0;
  while((n = read(fd, txtBuf, sizeof(txtBuf))) > 0) {
    printf(1, "[DEBUG]\n%s\n", txtBuf);
    userCount = splitUserList(txtBuf, infos);
  }
  //printf(1, "Loaded USERS Datas\n");
  close(fd);
  
  while(1) {
    // 使用者輸入帳密.
    do {
      printf(1, "Username: ");
    } while(getInput(txtBuf, 32));
    do {
      printf(1, "Password: ");
    } while(getInput((txtBuf + 33), 32));

    // Check user/passwd.
    for (i = 0; i < userCount; i += 1) {
      if (strcmp(infos[i].username, txtBuf) == 0) {
        if (strcmp(infos[i].password, txtBuf + 33) == 0) {
          su(i);
          printf(1, "Login %s...\nuid: %d gid: %d\n", infos[i].username, infos[i].uid, infos[i].gid);
          goto LOGIN_SUCCESS; // 懶得再開變數，直接用GOTO省空間
        }
        printf(1, "Error Password.\n");
        goto ERROR_PASSWD; // 懶得再開變數，直接用GOTO省空間
      }
      //printf(1, "txtBuf: %s\nuserID: %s\n", txtBuf, infos[i].username);
    }
    printf(1, "User not found.\n");
    ERROR_PASSWD:
      printf(1, "Retry.\n");
  }
  // Success enter system.
  LOGIN_SUCCESS: 
    printf(1, "Login succeed.\n");
  // Execute shell.
  exec("sh", argv);
  exit();
} 