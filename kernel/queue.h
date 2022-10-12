#ifndef __QUE_H_
#define __QUE_H_

#include "param.h"

struct queue {
    int back;
    int size;
    int time_limit;             // max number of ticks that a process can run while inside a queue
    struct proc* que[NPROC];
};

extern struct queue queue[5];

#endif