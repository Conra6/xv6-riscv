#ifndef _PSTAT_H_
#define _PSTAT_H_

// From PA1: resource usage returned by wait2()
struct rusage {
  uint cputime;
};

enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// PA2: per-process info returned by getprocs()
struct pstat {
  int pid;                // Process ID
  enum procstate state;   // Process state
  uint64 size;            // Size of process memory (bytes)
  int ppid;               // Parent process ID
  char name[16];          // Parent command name
};

#endif
