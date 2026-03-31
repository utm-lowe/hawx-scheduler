#include "types.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "mem.h"
#include "string.h"
#include "console.h"
#include "trap.h"
#include "proc.h"
#include "disk.h"

void swtch(struct context *old, struct context *new);

// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run.
//  - swtch to start running that process.
//  - eventually that process transfers control
//    via swtch back to the scheduler.
void
scheduler(void)
{
  // This function is the scheduler. You have two basic options for
  // implementing the scheduler. One possibility is you could simply
  // adapt the code from xv6 to produce a round robin scheduler.
  // Afte that, try out a few more schemes!
  //
  // If you do want the extra credit, be sure to document how your 
  // scheduler works when you write the function.
  // YOUR CODE HERE
}


// Give up the CPU for one scheduling round.
void
yield(void)
{
  struct proc *p = cpu.proc;
  if(p->state == RUNNING) {
      p->state = RUNNABLE;
  }
  uartstart();
  virtio_disk_start();
  swtch(&p->context, &cpu.context);
}
