#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *func(void *args);

int main() {
    pthread_t pid[3];
    int i;

    for(i = 0; i<3; i++) {
        if(pthread_create(&pid[i], NULL, func, (void *)(&i))){
            printf("Error: PTHREAD CREATE!\n");
            exit(10);
        }
    }

    // for(i = 0; i<3; i++) {
    //     if(pthread_join(pid[i], NULL)) {
    //         printf("Error: PTHREAD JOIN!\n");
    //         exit(9);
    //     }
    // }

    printf("END OF MAIN\n");
}

void *func(void *args) {
    int i = *((int *)(args));

    printf("Thread %ld: created\n", pthread_self());
    sleep(2);
    printf("Thread %ld, value %d : EXIT!\n", pthread_self(), i);
    pthread_exit(0);
}