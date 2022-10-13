struct Q {
    int size;
    int time_limit;             // max number of ticks that a process can run while inside a queue
    struct proc* que[NPROC];
};

#define MAX_Q 5