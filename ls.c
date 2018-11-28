#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

// [New]
char*
fmtfilemode(int mode)
{
  static char buf[9];
  char rwx[] = {'r', 'w', 'x'};
  int i;
  for (i = 0; i < 9; ++i) {
    if (mode & 256){ // mode & bin(100 000 000)
      buf[i] = rwx[i % 3];
    } else {
      buf[i] = '-';
    }
    mode <<= 1;
  }
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    //printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    // [New] Show file uid, gid, modelsc
    printf(1, "FILENAME\tTYPE\tINO\tSIZE\tUID\tGID\tMODE\n");
    printf(1, "%s \t%d\t%d\t%d\t%d\t%d\t%s(%d)\n", fmtname(path), st.type, st.ino, st.size, st.uid, st.gid, fmtfilemode(st.mode), st.mode);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    // [New] Show file uid, gid, modelsc
      printf(1, "FILENAME\tTYPE\tINO\tSIZE\tUID\tGID\tMODE\n");
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0) {
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      //printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
      // [New] Show file uid, gid, modelsc
      printf(1, "%s \t%d\t%d\t%d\t%d\t%d\t%s(%d)\n", fmtname(buf), st.type, st.ino, st.size, st.uid, st.gid, fmtfilemode(st.mode), st.mode);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit();
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit();
}
