#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFSIZE 1024

int main() {
    char buf[BUFSIZE];
    size_t readlen, writelen, slen;
    pid_t cpid, mypid;
    pid_t pid = getpid();
    cpid = fork();

    if(cpid > 0) {
        wait(NULL);
        printf("Child Complete\n");
    } else if (cpid == 0) {
        execlp("ps", "ps", NULL);
    } else{
        printf("fork error");
        exit(1);
    }
    

    return 0;
}