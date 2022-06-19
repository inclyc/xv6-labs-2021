#include "user.h"

int main(int argc, char *argv[]) {
  int pipes[2];
  int n = pipe(pipes);
  if (n < 0) {
    printf("pipe error\n");
    exit(1);
  }
  int pid = fork();
  if (pid < 0) {
    printf("fork error\n");
    exit(1);
  }
  if (pid == 0) {
    char rd;
    read(pipes[0], &rd, sizeof(char));
    printf("%d: received ping\n", getpid());
    write(pipes[1], &rd, sizeof(char));
  } else {
    // parent process
    char wr = 'p';
    write(pipes[1], &wr, sizeof(char));
    read(pipes[0], &wr, sizeof(char));
    printf("%d: received pong\n", getpid());
  }
  exit(0);
}
