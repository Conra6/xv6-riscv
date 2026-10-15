#include "kernel/types.h"
#include "user/user.h"

// Usage: agetest [n]   (n scales the work per child, default 30)
// One low-priority child (5) competes with four high-priority ones (40).
// Reports response time and turnaround time for each child, measured from
// the moment all of them are released at once.

#define NHIGH 4

struct rec { int prio; int resp; int turn; };

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
  int n = 30;
  int go[2], rdy[2], rep[2];
  int i, sumresp = 0, sumturn = 0;
  uint start;
  struct rec r;
  char c;

  if(argc > 1)
    n = atoi(argv[1]);
  pipe(go);
  pipe(rdy);
  pipe(rep);
  setpriority(49);               // parent outranks everyone

  for(i = 0; i <= NHIGH; i++){
    if(fork() == 0){
      uint s;
      r.prio = (i == 0) ? 5 : 40;
      setpriority(r.prio);
      write(rdy[1], "r", 1);     // tell parent we are ready
      read(go[0], &s, sizeof(s)); // wait for the start signal
      r.resp = uptime() - s;     // response time: first run after release
      burn(n);
      r.turn = uptime() - s;     // turnaround time
      write(rep[1], &r, sizeof(r));
      exit(0);
    }
  }

  for(i = 0; i <= NHIGH; i++)
    read(rdy[0], &c, 1);         // all children are set up
  start = uptime();
  for(i = 0; i <= NHIGH; i++)
    write(go[1], &start, sizeof(start));

  for(i = 0; i <= NHIGH; i++){
    read(rep[0], &r, sizeof(r));
    printf("priority %d: response %d, turnaround %d\n", r.prio, r.resp, r.turn);
    sumresp += r.resp;
    sumturn += r.turn;
  }
  for(i = 0; i <= NHIGH; i++)
    wait(0);

  printf("average response %d, average turnaround %d\n",
         sumresp / (NHIGH + 1), sumturn / (NHIGH + 1));
  exit(0);
}
