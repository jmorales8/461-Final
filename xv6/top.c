#include "types.h"
#include "stat.h"
#include "user.h"

#define MAXPROCS 64

// Must match enum procstate order in proc.h
static char *states[] = {
  "UNUSED",
  "EMBRYO",
  "SLEEPING",
  "RUNNABLE",
  "RUNNING",
  "ZOMBIE"
};

static void
print_header(void)
{
  printf(1, "PID   STATE      MEM(bytes)   CPU(ticks)   NAME\n");
  printf(1, "------------------------------------------------\n");
}

static char *
state_name(int s)
{
  if(s < 0 || s > 5)
    return "???";
  return states[s];
}

int
main(int argc, char **argv)
{
  struct uproc procs[MAXPROCS];

  while(1){
    // Clear the screen and move the cursor to the top so the table
    // is refreshed in place, similar to Linux "top".
    printf(1, "\x1b[2J\x1b[H");
    printf(1, "----- xv6 top -----\n");

    int n = getprocs(procs, MAXPROCS);

    print_header();
    for(int i = 0; i < n; i++){
      struct uproc p = procs[i];
      printf(1, "%d    %s   %d        %d          %s\n",
             p.pid,
             state_name(p.state),
             (int)p.sz,
             (int)p.cpu_ticks,
             p.name);
    }

    sleep(100);  // adjust refresh rate as you like
  }

  exit();
}
