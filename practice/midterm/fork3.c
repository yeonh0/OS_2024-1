#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define CMD_SIZE 200

int main(int argc, char *argv[]) {
    char cmd[CMD_SIZE];
    pid_t childpid;

    printf("Parent PID : %d\n", getpid());

    switch (childpid = fork())
    {
    case -1:
        exit(1);
        break;
    
    default:
        sleep(3);
        strcpy(cmd, "ps ");
        system(cmd);

        if(kill(childpid, SIGKILL) == -1) printf("error in kill process\n");
        sleep(6);
        printf("after sending kill sig to zombi(PID=%d)\n", childpid);
        system(cmd);
        break;
    }

    return 1;
}