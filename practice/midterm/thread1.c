#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *howdy();

int main() {
    pthread_t tid;
    printf("HI! I'm main thread, %ld\n", pthread_self());
    pthread_create(&tid, NULL, howdy, NULL);
    printf("%ld is been created!\n", tid);
    pthread_join(tid, NULL);
    exit(0);

    return 1;
}

void *howdy() {
    printf("HI! I'm %ld\n", pthread_self());
    return NULL;
}