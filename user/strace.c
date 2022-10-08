#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char **argv)
{
    if (argc <= 2) {
        fprintf(2, "strace usage: strace mask command [args]\n");
        return 1;
    }

    int mask = atoi(argv[1]);
    trace(mask);

    exec(argv[2], &argv[2]);

    exit(0);
}