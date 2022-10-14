#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


#define NFORK 10
#define IO 5

int main() {
  int n, pid, temp = 0;
  int wtime, rtime;
  int twtime=0, trtime=0;
  for (n=0; n < NFORK;n++) {
      pid = fork();
      if (pid < 0)
          break;
      if (pid == 0) {
          if (n < IO) {
            sleep(50); // IO bound processes
          } else {
            for (uint64 i = 0; i < (uint64)2000000000; i++) {temp++;}; // CPU bound process
          }

          // printf("Process %d finished", n);
          exit(0);
      } else {
#ifdef PBS
        set_priority(60-IO*5+n*+5, pid); // Will only matter for PBS, set lower priority for IO bound processes 
#endif
      }
  }
  for(;n > 0; n--) {
      if((pid = waitx(0,&wtime,&rtime)) >= 0) {
          // printf("\n%d: %d  %d\n", pid, rtime, wtime);
          trtime += rtime;
          twtime += wtime;
      } 
  }
  printf("Average rtime %d,  wtime %d\n", trtime / NFORK, twtime / NFORK);
  exit(0);
}
