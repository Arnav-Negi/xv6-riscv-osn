#ifdef MLFQ

#include "queue.h"
#include "proc.h"
#include "defs.h"

extern struct Q queue[5];

void
init_queue()
{
    for(int i = 0; i < 5; i++)
    {
        queue[i].back = -1;
        queue[i].size = 0;
        for(int j = 0; j < NPROC; j++)
            (queue[i].que)[j] = 0;
    }
}

int quesize(int qnum)
{
    return queue[qnum].size;
}

struct proc * quefront(int qnum)
{
    return queue[qnum].que[0];
}

// push
void
push_proc(struct proc* p, int q_num)
{
    if(queue[q_num].size >= NPROC - 1)
    {
        panic("MLFQ: queue out of memory");
    }
    p->inqueue = 1;
    p->q_num = q_num;

    queue[q_num].size++;
    queue[q_num].back++;
    queue[q_num].que[queue[q_num].back] = p;
}
// pop
struct proc*
pop_proc(int q_num)
{
    if(queue[q_num].size == 0)
    {
        panic("MLFQ: cannot pop empty queue");
    }
    struct proc* p = queue[q_num].que[0];
    
    for(int i = 1; i < NPROC; i++)
    {
        queue[q_num].que[i-1] = queue[q_num].que[i];
    }

    p->inqueue = 0;

    queue[q_num].size--;
    queue[q_num].back--;

    return p;
}

#endif
