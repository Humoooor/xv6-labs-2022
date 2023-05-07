#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid, p[2];

    pipe(p);
    pid = fork();
    if(pid == 0) {
        if(read(p[0], 0, 1)) {
            pid = getpid();
            printf("%d: received ping\n", pid);
            write(p[1], "L", 1);
            exit(0);
        }
    } else {
        write(p[1], "H", 1);
        if(read(p[0], 0, 1)) {
            pid = getpid();
            printf("%d: received pong\n", pid);
        }
        exit(0);
    }

    exit(-1);
}
