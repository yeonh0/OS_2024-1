#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *func1(void *args) {
    printf("Func1 : %ld\n", pthread_self());
    pthread_exit(0);
}

void *func2(void *args) {
    printf("Func2 : %ld\n", pthread_self());
    pthread_exit(0);
}

void *func3(void *args) {
    printf("Func3 : %ld\n", pthread_self());
    pthread_exit(0);
}

int main() {
    void *func[3];
    int i;

    pthread_t id[3];
    func[0] = func1;
    func[1] = func2;
    func[2] = func3;

    for(i=0; i<3; i++) {
        if(pthread_create(&id[i], NULL, func[i], NULL)) printf("pthread_create: ERROR!\n");
    }
    return 0;
}