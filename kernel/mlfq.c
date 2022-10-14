#include "mlfq.h"

struct Q network[5];

void init_queue()
{
    for (int i = 0; i < 5; i++)
    {
        network[i].size = 0;
        network[i].time_limit = 1 << i;
        for (int j = 0; j < NPROC; j++)
        {
            network[i].que[j] = 0;
        }
    }
}

int push_process(int qnum, struct proc *p)
{
    if (network[qnum].size == NPROC)
        return 1;
    network[qnum].que[network[qnum].size++] = p;
    return 0;
}

int delete_process(int qnum, int index)
{
    if (index >= NPROC)
        return 1;

    for (int i = index; i < NPROC - 1; i++)
        network[qnum].que[index] = network[qnum].que[index + 1];
    network[qnum].que[NPROC] = 0;

    return 0;
}

struct proc *MLFQ_scheduler(void)
{
    
}