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
print_field_str(char *s, int width)
{
  int len = strlen(s);
  printf(1, "%s", s);
  for(int i = len; i < width; i++)
    printf(1, " ");
}

static void
print_field_int(int value, int width)
{
  char buf[16];
  int len = 0;

  if(value == 0){
    buf[len++] = '0';
  } else {
    int v = value;
    if(v < 0){
      buf[len++] = '-';
      v = -v;
    }

    char tmp[16];
    int tlen = 0;
    while(v > 0 && tlen < (int)sizeof(tmp)){
      tmp[tlen++] = '0' + (v % 10);
      v /= 10;
    }
    for(int i = tlen - 1; i >= 0; i--)
      buf[len++] = tmp[i];
  }

  buf[len] = '\0';
  print_field_str(buf, width);
}

static void
print_header(void)
{
    int current_ticks = uptime();
    printf(1, "Uptime: %d ticks\n", current_ticks);
    print_field_str("PID", 5);
    printf(1, " ");
    print_field_str("STATE", 10);
    printf(1, " ");
    print_field_str("MEM(bytes)", 12);
    printf(1, " ");
    print_field_str("CPU(ticks)", 11);
    printf(1, " NAME\n");
    printf(1, "-----------------------------------------------------------\n");
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
      print_field_int(p.pid, 5);
      printf(1, " ");
      print_field_str(state_name(p.state), 10);
      printf(1, " ");
      print_field_int((int)p.sz, 12);
      printf(1, " ");
      print_field_int((int)p.cpu_ticks, 11);
      printf(1, " %s\n", p.name);
    }

    sleep(100);  // adjust refresh rate as you like
  }

  exit();
}
