#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "mlfq.h"

struct Q network[MAX_Q];

void init_queue()
{
    for (int i = 0; i < MAX_Q; i++)
    {
        network[i].size = 0;
        network[i].time_limit = 1 << i;
        for (int j = 0; j<NPROC; j++) {
            network[i].que[j] = 0;
        }
    }
}

struct proc *front_proc(int q_num)
{
    return network[q_num].que[0];
}

int push_process(int qnum, struct proc *p)
{
    if (network[qnum].size == NPROC)
        return 1;
    p->inqueue = 1;
    p->q_num = qnum;
    network[qnum].que[network[qnum].size++] = p;
    return 0;
}

int delete_process(int qnum, int index)
{
    if (index >= NPROC)
        return 1;
    network[qnum].que[index]->inqueue = 0;
    for (int i = index; i < NPROC - 1; i++)
        network[qnum].que[i] = network[qnum].que[i + 1];
    network[qnum].que[NPROC-1] = 0;
    network[qnum].size--;

    return 0;
}

int find_index(struct proc *p, int q_num)
{
    for (int i = 0; i < NPROC; i++)
    {
        if (!network[q_num].que[i])
            return -1;
        if (network[q_num].que[i]->pid == p->pid)
            return i;
    }
    return -1;
}

void handle_specs()
{
    int index, preempt = 0;

    for (struct proc *p = proc; p < &proc[NPROC]; p++)
    {
        if (p->state == RUNNABLE && !p->inqueue)
        {
            // back from io, go to same queue
            push_process(p->q_num, p);
        }
        else if (p->state == RUNNABLE && p->inqueue && p->wtime >= AGETICKS && p->q_num != 0)
        {
            // aging
            index = find_index(p, p->q_num);
            if (index == -1) {
                printf("index error\n");
                return;
            }
            delete_process(p->q_num, index);
            p->q_num++;
            push_process(p->q_num, p);
        }
        else if (p->state != RUNNING && p->state != RUNNABLE && p->inqueue)
        {
            // wait for io
            index = find_index(p, p->q_num);
            if (index == -1) {
                printf("index error\n");
                return;
            }
            delete_process(p->q_num, index);
        }
    }
    struct proc *running = myproc();

    if (running && running->rtime >= network[running->q_num].time_limit) // preemption
    {
        delete_process(running->q_num, 0);
        if (running->q_num == 4)
        {
            push_process(running->q_num, running);
        }
        else
        {
            running->q_num++;
            push_process(running->q_num, running);
        }
        preempt = 1;
    }
    
    if (running)
        for (int i = 0; i < running->q_num; i++)
        {
            if (network[i].size != 0)
                preempt = 1; // after aging, higher priority.
        }

    if (preempt)
        yield();
}

struct proc *MLFQ_scheduler(void)
{
    handle_specs();

    int best = -1;
    for (int i = 0; i < MAX_Q; i++)
    {
        if (network[i].size != 0)
        {
            best = i;
            break;
        }
    }

    if (best == -1)
        return (struct proc *)0;
    front_proc(best)->num_scheduled++;
    return front_proc(best);
}