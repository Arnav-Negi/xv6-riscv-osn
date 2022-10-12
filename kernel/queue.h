#ifndef __QUE_H_
#define __QUE_H_

#include "param.h"

struct queue {
    int head;
    int back;
    struct proc* que[64];
};

extern struct queue queue[5];

#endif