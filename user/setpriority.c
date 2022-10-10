#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(2, "setpriority usage: setpriority new_priority pid \n");
        return 1;
    }

    int new_priority = atoi(argv[1]);
    int pid = atoi(argv[2]);

    set_priority(new_priority, pid);

    exit(0);
}