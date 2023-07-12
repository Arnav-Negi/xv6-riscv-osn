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

int main()
{
  int n, pid;
  for (n = 0; n < NFORK; n++)
  {
    pid = fork();
    if (pid < 0)
      break;
    if (pid == 0)
    {
#ifdef FCFS
      //   if (n < IO) {
      // sleep(1000); // IO bound processes
      //   } else {
      for (volatile int j = 0; j < 1000000000; j++)
      {
      }
#endif
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
    else
    {
#ifdef PBS
      if (n % 2 == 1)
      {
        set_priority(80, pid); // Will only matter for PBS, set lower priority for IO bound processes
      }
#endif
    }
  }
  for (; n > 0; n--)
  {
    if (wait(0) >= 0)
    {
    }
  }
  exit(0);
}