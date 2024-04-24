#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SIZE 5;

int idata = 111;

int main(int argc, char *argv[]) {
    int numDead;
    pid_t childpid;
    int j;

    for(j=1; j<argc; j++) {
        switch (fork())
        {
        case -1:
            printf("fork problem\n");
            exit(1);
            break;
        case 0:
            printf("Child %d started with PID %ld and sleeping %s seconds\n", j, (long)getpid(), argv[j]);
            sleep(atoi(argv[j]));
            break;
        
        default:
            break;
        }
    }

    numDead = 0;
    for(;;) {
        childpid = wait(NULL);
        if(childpid == -1) exit(1);
        numDead ++;
        printf("wait() returned child PID %ld (numDead = %d)\n", (long)childpid, numDead);
    }
    return 1;
}