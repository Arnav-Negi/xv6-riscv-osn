#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#ifdef FCFS
#define NFORK 5
#endif
#ifndef FCFS
#define NFORK 5
#define IO 3
#endif


int main() {
    int n, pid, temp = 0;
    for (n = 0; n < NFORK; n++) {
        pid = fork();
        if (pid < 0)
            break;
        if (pid == 0) {
            if (n < IO) {
                sleep(50); // IO bound processes
            } else {
                for (uint64 i = 0; i < (uint64) 1000000000; i++) { temp++; }; // CPU bound process
            }

            printf("Process %d finished", n);
            exit(0);
        } else {
#ifdef PBS
            for (volatile int i = 0; i < 100; i++)
            {
              for (volatile int j = 0; j < 100000000; j++)
              {
              }
              sleep(100);
            } // MIXED bound process
#endif
#ifdef LBS
            if (n == 0)
            {
              settickets(100);
              printf("pid %d has 100 tickets\n", getpid());
            }
            else
            {
              settickets(1);
              printf("pid %d has 1 tickets\n", getpid());
            }
            for (volatile int i = 0; i < 100; i++)
            {
              for (volatile int j = 0; j < 100000000; j++)
              {
              }
              sleep(1);
            }
#endif
#ifdef MLFQ
            if (n < IO)
            {
              // CPU
              for (volatile int i = 0; i < 100; i++)
              {
                for (volatile int j = 0; j < 100000000; j++)
                {
                }
                sleep(20);
              }
            }
            else
            {
              // IO
              for (volatile int i = 0; i < 1000; i++)
              {
                for (volatile int j = 0; j < 5000000; j++)
                {
                }
                sleep(5);
              }
            }
#endif
            printf("Process %d finished\n", getpid());
            exit(0);
        }
    }
    for (; n > 0; n--) {
        if (wait(0) >= 0) {
        }
    }
    exit(0);
}