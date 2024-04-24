#include <signal.h>
#include <stdio.h>
#include <unistd.h>
sigset_t sSet;

int main() {
    int iRet;
    sigemptyset(&sSet);
    sigaddset(&sSet, SIGINT);
    iRet = sigprocmask(SIG_BLOCK, &sSet, NULL);
    assert(iRet==0);
    if(iFlag == 0) {
        printf("HI!\n");
    }
}