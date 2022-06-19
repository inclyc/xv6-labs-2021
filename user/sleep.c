#include "user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: sleep <seconds>\n");
    exit(-1);
  }
  int ticks = atoi(argv[1]);
  sleep(ticks);
  exit(0);
}
