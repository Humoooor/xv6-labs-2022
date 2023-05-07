#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int status, pid, new_argc;
    int idx = 0;
    char buf;
    char *new_argv[MAXARG];

    for(int i = 1; i < argc; i++) {
        new_argv[i-1] = argv[i];	
    }

    while(read(0, &buf, 1)) {
        new_argc = argc;
        idx = 0;
        new_argv[new_argc-1] = (char*)malloc(MAXARG);
        do {
            // only read one line each time
            if(buf == '\n') {
                new_argv[new_argc-1][idx] = '\0';
                new_argv[new_argc] = 0;
                break;
            } else if(buf == ' ') {
                // if meet ' ', divide into more argv
                // except for two ' '
                if(idx == 0) {
                    continue;
                }
                new_argv[new_argc-1][idx] = '\0';
                new_argc++;
                idx = 0;
                new_argv[new_argc-1] = (char*)malloc(MAXARG);
                continue;
            }
            new_argv[new_argc-1][idx++] = buf;
        } while(read(0, &buf, 1));

        pid = fork();
        if(pid == 0) {
            exec(new_argv[0], new_argv);
            printf("wrong command\n");
            exit(-1);
        } else {
            wait(&status);
        }

        for(int i = argc; i <= new_argc; i++) {
            free(new_argv[i-1]);
        }
    }

    exit(status);
}
