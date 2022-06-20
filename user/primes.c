#include "user.h"
void produce(int backward_pipe_fd) {
  int prime;
  read(backward_pipe_fd, &prime, sizeof(int));
  printf("prime %d\n", prime);
  if (prime > 10) {
    return;
  }
  int pid = fork();
  int pp[2];
  pipe(pp);
  if (pid == 0) {
    /* Child */
    close(pp[1]);
    produce(pp[0]);
    close(pp[0]);
  } else {
    int forward;
    close(pp[0]);
    for (; read(backward_pipe_fd, &forward, sizeof(int));) {
      write(pp[1], &forward, sizeof(int));
    }
    close(pp[1]);
    wait(0);
    close(backward_pipe_fd);
  }
  exit(0);
}

int main() {
  printf("prime 2\n");
  int pp[2];
  pipe(pp);
  int pid = fork();
  if (pid == 0) {
    /* Child */
    close(pp[1]);
    produce(pp[0]);
  } else {
    close(pp[0]);
    for (int i = 3; i < 32; i++) {
      write(pp[1], &i, sizeof(int));
    }
    close(pp[1]);
    wait(0);
  }
  exit(0);
}
