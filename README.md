# Introduction

In this assignment, we are going to explore schedulers. While it is relatively
easy to make a simple scheduler that works, it will be a lot more fun to
explore several different schedulers. This assignment, therefore, has two
parts:

1. Make a simple pre-emptive round-robin scheduler.
2. Try out several other schedulers and write about their performance.

In order to test the schedulers, we will be using an `init` program which
spawns 30 other processes:

- 10 "busy counters" which will count to 1,000,000 without making any other
  system calls.
- 10 "noisy counters" which print once per 1,000 numbers while counting to
  1,000,000.
- 10 "less noisy counters" which print once per 10,000 numbers while counting to
  1,000,000.

Clearly, these have different load qualities. Some will always consume their
entire alotment of CPU time, some will yield quite a lot, and some will yield
in less time. We will time 4 events, one for the completion of each class
of counters, and one for the overall completion. To assist with this timing,
there is an instrumented runner which will watch for the appropriate message.
You can access this instrument by running `make test`. This will build and run
the kernel, and report the times:

```
$ make test

...

9:      999000
10:     997000
9: Noisy Counter Done
10:     998000
10:     999000
10: Noisy Counter Done
All noisy counters are done
All counters done.
QEMU: Terminated
=============================================
Noisy counters done in 11.6015 seconds.
Busy counters done in 2.58635 seconds.
Less noisy counters done in 4.56407 seconds.
All counters done in 11.6021 seconds.
```

Note that your exact output may vary as there is a stochastic element to how
these will count! Your numbers will vary from run to run as well.

Notice something interesting in the example output: the busy counters finish
*before* the less noisy counters, even though the busy counters do comparable
amounts of total work. This is because the noisy and less noisy counters yield
the CPU whenever they make a system call to print, which gives the busy
counters more opportunities to run uninterrupted. Keep this in mind as you
analyze your results — completion order is not the same as amount of work done.

# Round Robin Scheduling
Your first task is to implement a preemptive round robin scheduler. To do this
follow the example found in xv6. You will need to edit `scheduler.c`, adding
code to the scheduler function.

Once you have implemented your round robin mechanism, run `make test` and
make sure that your program runs to completion. Recall that preemption happens
when a process is interrupted by a timer event. You will also re-enter the
scheduler after each system call as system calls yield control to the scheduler
in this lab.

Run your round robin scheduler 10 times and record the average time for
Noisy, Busy, Less Noisy, and All counter completion for a total of four
data points. The time elapsed is the time from when the kernel starts up until
each of these events occur. You can therefore put them in order by sorting their
completion times.

# Other Schemes
Now that we have a nice working scheduler, let's play! Try out the scheduling
schemes discussed in OSTEP Chapters 7 (*Scheduling: Introduction*), 8
(*Scheduling: The Multi-Level Feedback Queue*), and 9 (*Scheduling: Proportional
Share*). You need to implement at least one new scheduler idea from each
chapter, for a total of 3 new scheduling schemes. Each chapter covers several
algorithms, so you have real choices to make — pick the ones that interest you
most. If you implement more than the minimum, you will earn extra credit points
toward your lab grade (10 points per additional scheduler). As an additional
requirement, be sure to implement at least one non-preemptive scheduler.

## Turning off Preemption
Preemption is created by the timer. You can disable the timer by editing
`start.c`. Locate the line containing `timerinit()` and comment this out. This
will disable timer interrupts and therefore the only way back into the scheduler
will be via system calls. You will now be running on a cooperative multitasking
system! (Be sure to uncomment that line when you are ready to run some more
pre-emptive schemes.)

## Adding Information to Processes
The `proc.c` and `timer.c` files are precompiled in this lab. You cannot
alter them, and therefore you should also leave the struct in `proc.h` as it
is. Some of these schemes will require you to store additional information about
the processes. To do this, add your own structs and parallel arrays to the
`scheduler.c` file. This way you can have something like `proc[i]` and `extra[i]`
containing details about process `i`.

There is one additional piece of information that has been added to `proc.h`
for you to get you started. The `interrupted` field will be set if the process
was interrupted during its most recent run. A process with a non-zero `interrupted`
field is a process which used all of its CPU time during the most recent
scheduling cycle.

## Information to Record
For each of your runs, compile the following information:

1. A listing of your scheduler source code.
2. Average times across ten runs of your scheduler.
3. A brief explanation about what sort of balance and throughput is achieved
   by using this scheduling scheme.

# Some Final Thoughts
Be sure to record a narrative summary of your activities. Which scheduler works
best? There probably is not a clear winner, though some schedulers do optimize
for different things. For each scheduler you implemented, describe a real system
or workload where you would actually choose to deploy it — a web server, an
interactive desktop, an embedded controller, a batch processing cluster. What
does the diversity of good answers tell us about our job as operating system
authors?
# hawx-scheduler
