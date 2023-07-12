# xv6-riscv

This is the submission for M22-OSN assignment 4. Team members are:

1. Arnav Negi, 2021101112
2. Ashmit Chamoli, 2021101114

## Running xv6

Running the xv6-riscv operating system requires the qemu emulator and the riscv64 gnu toolchain to be installed.

Use

```make qemu SCHEDULER=sched```'

to compile and run xv6 using `sched` algorithm for scheduling, `sched`must take one of the following values - `RR`, `FCFS`, `LBS`, `PBS`, `MLFQ`. By default, it is RR.
See Specification 2 for more details. Use `CPUS=` option to specify number of cpus to be used.

# Specification 1: System Calls

### `trace`

Traces a process's system calls, specified by the bit mask.
Strace user program runs the given command and traces it.

### `sigalarm`

Calls specified function on regular intervals.

### `sigreturn`

Restores the process state right after sigalarm handler exits.

# Specification 2: Scheduling

## FCFS (First come first serve)

1. Added field `ctime` to `proc.h > struct proc {}` to keep track of the creation time of the process. Initialise `ctime` with the global variable `ticks` in `proc.c > allocproc()`

2. Loop through the process list in `proc.c > scheduler()` and find the runnable process with the lowest `ctime` and context switch to it.

3. Make it so that the process does not give up the CPU when a timer interrupt occurs. This is done to ensure that the currently running process is not preempted. To do this, disable `yeild()` in `trap.c > usertrap()` and `trap.c > kerneltrap()`.

## LBS (Lottery based scheduling)

1. Added field `tickets` to `proc.h > struct proc {}` to keep track of the number of tickets owned by a process. Initialise `tickets` to 1 in `proc.c > allocproc()`.

2. Loop through the process list in `proc.c > scheduler()` to find the list of runnable processes (`runnable_procs[]`) and the sum of tickets (total_tickets) of all these processes.

3. While looping in `2`, acquire the lock for all the runnable processes as it is unknown which process will get selected.

4. Randomly select a ticket from `[1, total_tickets]` and select the process corresponding to that ticket. Release the lock for all other processes and schedule the selected process.

5. Do not disable `yeild()` in `trap.c > usertrap()` and `trap.c > kerneltrap()`, as we recquire preemption.

## PBS (Priority based scheduling)

The non preemptive PBS scheduler sets static priority of each process and uses their "niceness" to schedule processes with minimum dynamic priority.

1. Set `s_priority` of each process as 60, `niceness` as 0 by default.
2. In the scheduler, use the given formula to calculate `niceness` and then get dynamic priority of the process.
3. Schedule the processes with numerically least (highest priority) dynamic priority.
4. The processes can set their own priority using the system call `set_priority`.

## MLFQ (Multi level feedback queue)

IMPORTANT: Must be run with

```make qemu SCHEDULER=MLFQ CPUS=1```

1. Added the following fields to `proc.h > struct proc {}`

- `inqueue` : Is the process in a queue? then 1 else 0

- `q_num` : Which queue is the process in?

- `total_rtime` : "time spent running - ticks that a process has run for

- `wtime` : "time spent waiting in queue"
  
1. Implement a function `handle_spes()` which handles aging, dequeueing of sleeping processes and preemption.
2. Call the `handle_specs` function in usertrap upon timer interrupts and in the beginning of the scheduler funciton `MLFQ_scheduler`.
3. schedule the first process in the highest priority queue.

# Specification 3: Copy-on-write fork

Upon fork, the child's pagetable is mapped to the parent's pages. The PTEs pointing to these pages are marked as non writable and `COW` by clearing `PTE_W` and setting `PTE_COW` (defined as 8th bit of PTE).

In usertrap, check if the fault code is `15` and faulted address is a `COW` address. If so, allocate a new page to the PTE and map it to the process' PTE.

Edit copyout which is called when a child exits, to act the same as COW handler in usertrap.

# Performance Comparison

|            | RR | FCFS | LBS | PBS | MLFQ |
|     --     | -- |  --  |  -  |  -  |  -  |
| Avg. rtime |  11  |  15   |   13  |  16   |  14  |
| Avg. wtime |  35  |   30   |  34   |  30   |  59  |

As we can see, RR gives minimum runtime while FCFS and PBS give minimum wait time. Sinze LBS isn't given any setticket calls, it performs similar to RR. MLFQ performs well, considering it only utilises 1 CPU, giving low runtime with slightly higher wait time.

## Plot of MLFQ analysis

-1 refers to process having ended.

![ plot of queues of processes](40.png "queue numbers of processes")

The processes start at queue 0 but as they use more CPU, they are moved to lower priority queues. Aging time is 40 here.

![ plot of queues of processes](20.png "queue numbers of processes")

In this plot, aging time is 20, hence many processes age and move to higher priority queues.

![ plot of queues of processes](10.png "queue numbers of processes")

The aging time here is 10 ticks, so aging happens often.
