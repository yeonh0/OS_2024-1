#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *howdy(void *arg);

int main() {
    pthread_t tid[5];
    printf("HI! I'm main thread, %ld\n", pthread_self());

    for(int i=0; i<5; i++) {
        pthread_create(&tid[i], NULL, howdy, (void *)&i);
        printf("%ld is been created!\n", tid[i]);
    }
    
    for(int i=0; i<5; i++) {
        pthread_join(tid[i], NULL);
    }

    return 1;
}

void *howdy(void *arg) {
    printf("%d: HI! I'm %ld\n", *(int *)arg, pthread_self());
    return NULL;
}