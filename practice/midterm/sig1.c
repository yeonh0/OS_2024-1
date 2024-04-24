#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int stack = 0;

void ouch(int sig) {
    if (stack == 10) {
        printf("SIG_DFL!\n");
        signal(SIGINT, SIG_DFL);
    }
    stack ++;
    printf("STACK: %d\n", stack);
    printf("OUCH!-I got signal %d\n", sig);
}

int main() {
    //signal(SIGINT, ouch);

    void (*pfRet) (int);
    pfRet = signal(SIGINT, ouch);
    pfRet = signal(SIGTSTP, SIG_DFL);

    while(1) {
        printf("Hello World\n");
        sleep(1);
    }
}