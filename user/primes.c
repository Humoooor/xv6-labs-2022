#include "kernel/types.h"
#include "user/user.h"

void printPrime(int prime);
void primes(int start);

int main(int argc, char *argv[]) {
    primes(1);

    exit(-1);
}

void printPrime(int prime) {
    printf("prime %d\n", prime);
}

void primes(int start) {
    int prime = 2;
    int n, pid, p[2];

    if(!start && read(0, &prime, sizeof(prime)) == 0) {
        exit(0);
    }
    printPrime(prime);

    pipe(p);
    pid = fork();
    if(pid == 0) {
        // p[0] => stdin
        close(0);
        dup(p[0]);
        close(p[0]);

        // do not need p[1]
        close(p[1]);

        primes(0);
        exit(0);
    } else {
        if(start == 1) {
            for(int i = 3; i <= 35; i++) {
                if(i % prime != 0) {
                    write(p[1], &i, sizeof(i));
                }
            }
        } else {
            while(read(0, &n, sizeof(n))) {
                if(n % prime != 0) {
                    write(p[1], &n, sizeof(n));
                }
            }
        }
        close(p[1]);

        // wait for child process
        int status;
        wait(&status);
        exit(status);
    }
}
