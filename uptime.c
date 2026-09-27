// user/uptime.c
// PA1 Task 1: prints the number of clock ticks since system start.

#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("up %d clock ticks\n", uptime());
  exit(0);
}
