#include "kernel/types.h"
#include "user/user.h"
#include "kernel/pstat.h"

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "usage: time command [args...]\n");
    exit(1);
  }

  int start = uptime();

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "time: fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    exec(argv[1], argv + 1);
    fprintf(2, "time: exec %s failed\n", argv[1]);
    exit(1);
  } else {
    int status;
    struct rusage ru;
    wait2(&status, &ru);

    int end = uptime();
    int elapsed = end - start;
    int cputime = ru.cputime;
    int pct = 0;
    if (elapsed > 0)
      pct = (100 * cputime) / elapsed;

    printf("elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n",
           elapsed, cputime, pct);
  }

  exit(0);
}
