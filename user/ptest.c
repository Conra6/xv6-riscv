#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int pid;

  printf("initial priority: %d\n", getpriority());

  if(setpriority(10) < 0){
    printf("setpriority(10) failed\n");
    exit(1);
  }
  printf("after setpriority(10): %d\n", getpriority());
  printf("setpriority(50) returns %d (expect -1)\n", setpriority(50));
  printf("setpriority(-1) returns %d (expect -1)\n", setpriority(-1));
  printf("priority unchanged: %d\n", getpriority());

  pid = fork();
  if(pid == 0){
    printf("child inherited priority: %d\n", getpriority());
    setpriority(20);
    printf("child after setpriority(20): %d\n", getpriority());
    exit(0);
  }
  wait(0);
  printf("parent still has priority: %d\n", getpriority());
  exit(0);
}
