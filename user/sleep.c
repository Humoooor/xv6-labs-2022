#include "kernel/types.h"
#include "user/user.h"

int isDigitStr(char *str) {
    for(int i = 0; i < strlen(str); i++) {
        if(str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    
    return 1;
}

int main(int argc, char *argv[]) {
    int status = 0;
    if(argc == 1) {
        printf("sleep: missing operand\n");
        status = -1;
    }

    for(int i = 1; i < argc && !status; i++) {
        if(!isDigitStr(argv[i])) {
            printf("sleep: invalid time interval\n");
            status = -1;
        }
    }

    for(int i = 1; i < argc && !status; i++) {
        sleep(atoi(argv[i]));
    }

    exit(status);
}
