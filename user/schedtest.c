#include "kernel/types.h"
#include "user/user.h"

// Usage: schedtest [n]
// Forks three CPU-bound children with priorities 5, 20 and 40 (started
// lowest first) plus a monitor that runs ps while they compete.
// With the priority scheduler they should finish in the order 40, 20, 5.
// n scales the amount of work per child (default 5).

static void
burn(int n)
{
  volatile int x = 0;
  int i, j;

  for(i = 0; i < n; i++)
    for(j = 0; j < 10000000; j++)
      x += j;
}

int
main(int argc, char *argv[])
{
  int prios[3] = {5, 20, 40};
  int n = 5;
  int i;
  uint start;
  char *psargv[] = { "ps", 0 };

  if(argc > 1)
    n = atoi(argv[1]);

  setpriority(49);       // parent runs first so it can create everyone
  start = uptime();

  for(i = 0; i < 3; i++){
    if(fork() == 0){
      setpriority(prios[i]);
      burn(n);
      printf("priority %d finished at tick %d\n", prios[i], uptime() - start);
      exit(0);
    }
  }

  if(fork() == 0){       // monitor: show ps while the children compete
    pause(5);
    exec("ps", psargv);
    exit(1);
  }

  for(i = 0; i < 4; i++)
    wait(0);
  exit(0);
}
