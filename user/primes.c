#include "user.h"
void produce(int backward_pipe_fd) {
  int prime;
  read(backward_pipe_fd, &prime, sizeof(int));
  printf("prime %d\n", prime);
  if (prime >= 31) {
    return;
  }
  int pp[2];
  pipe(pp);
  int pid = fork();
  if (pid == 0) {
    /* Child */
    close(pp[1]);
    produce(pp[0]);
    close(pp[0]);
  } else {
    int n;
    close(pp[0]); //  close a ----- b ----read--- c
    for (; read(backward_pipe_fd, &n, sizeof(int));) {
      if (n % prime)
        write(pp[1], &n, sizeof(int));
    }
    close(pp[1]);            // close a ---------- b ---write-- c
    close(backward_pipe_fd); // close a ---read--- b ---------- c
    wait(0);
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
    for (int i = 3; i < 32; i += 2) {
      write(pp[1], &i, sizeof(int));
    }
    close(pp[1]);
    wait(0);
  }
  exit(0);
}
