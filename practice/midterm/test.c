#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t child;
    if (strcmp(argv[1], "ls") == 0) {
        switch(child = fork()){
            case 0:
                execlp("/bin/ls", "ls", NULL);
                exit(1);
            default:
                wait(NULL);
                break;
        }
    } 
    return 0;
}