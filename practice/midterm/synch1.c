#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* withdraw(void *arg);

int balance = 100000;
int main(int argc, char *argv[]) {
    int num = atoi(argv[1]);
    int maxTry = atoi(argv[2]);
    
    pthread_t tid[num];

    int money_list[num];
    for(int i=0; i<num; i++) money_list[i] = 0;

    for(int i=0; i<maxTry; i++) {
        for(int j=0; j<num; j++){
            pthread_create(&tid[j], NULL, withdraw, (void *)(&money_list[j]));
        }

        for(int j=0; j<num; j++){
            pthread_join(tid[j], NULL);
        }

        printf("All done: balance is %d\n", balance);
    }
    

    return 0;
}

void* withdraw(void *arg) {
    int *amount_ptr = (int *)arg;
    int amount = *amount_ptr;

    if(balance > amount) {
        *amount_ptr = *amount_ptr + 20;
        balance -= 20;
        printf("ATM - 20 : user = $%d\n", amount + 20);
    }
}