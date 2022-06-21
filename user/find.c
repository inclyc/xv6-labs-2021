#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void _get_name_by_path(const char *path, char *name) {
  const char *p;
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  ++p; // p 原来指向 '/'，现在指向文件名的首字母
  strcpy(name, p);
}


int get_stat(const char *path, int *fd, struct stat *st) {
  if ((*fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return -1;
  }
  if (fstat(*fd, st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(*fd); // 关闭path文件描述符
    return -2;
  }
  return 0;
}

void find(const char *path, const char *name);

int _read_dir(const int fd, const char * path, const char * name, const char * buf_name){
  struct dirent de;
  int n;
  while ((n = read(fd, &de, sizeof(de))) == sizeof(de)) {
    if (de.inum == 0)
      continue;
    // printf("%s\n", de.name);
    char buf[512];
    strcpy(buf, path);
    char *p = buf + strlen(buf);
    if (buf[strlen(buf) - 1] != '/')
      *p++ = '/';
    if(strlen(buf) + 1 + DIRSIZ + 1 > sizeof buf) {
      printf("find: path too long\n");
      return -1;
    }
    memmove(p, de.name, DIRSIZ);
    if (!name || strcmp(name, buf_name) == 0) {
      printf("%s\n", buf);
    }
    if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
    find(buf, name);
  }
  return 0;
}

void find(const char *path, const char *name) {
  // 递归地读取path目录下的所有文件，并打印文件名
  int fd; // path目录的文件描述符
  struct stat st;

  if(get_stat(path, &fd, &st)) goto err;

  char buf_name[512];
  _get_name_by_path(path, buf_name);
  if (st.type == T_DIR) {
    // 如果path是目录，则递归地读取path目录下的所有文件

    if(_read_dir(fd, path, name, buf_name)) goto err;
  } else if (st.type == T_FILE) {
    // 如果path是文件，则判断文件名是否符合要求
    if (!name || strcmp(name, buf_name) == 0) {
      printf("%s\n", path);
    }
  }
err:
  close(fd);
}

int main(int argc, char *argv[]) {
  if(argc < 2){
    find(".", 0);
  } else if(argc == 2){
    find(argv[1], 0);
  } else {
    find(argv[1], argv[2]);
  }
  exit(0);
}
