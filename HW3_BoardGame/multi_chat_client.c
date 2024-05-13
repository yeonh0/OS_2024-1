////////////////////////////////////////////////////////////////////////////////
// Header File
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>

////////////////////////////////////////////////////////////////////////////////
// User Define
#define BUFFSIZE 100             // Message Buffer Size
#define NAMESIZE 20              // User's Name

////////////////////////////////////////////////////////////////////////////////
// Function
void * rcv(void * arg);
void ErrorHandling(char * message);

////////////////////////////////////////////////////////////////////////////////
// Main Function
int main(int argc, char * argv[]) {
    // Local Var
    int sock;   // Create Server Sockets
    struct sockaddr_in serv_addr;    // Cread Server SockAddr
    pthread_t rcv_thread;         // Create Thread
    char chat[BUFFSIZE];        // Chat Buffer
    char msg[1000];      // Message Buffer

    // Arguments
    if(argc < 4) {
        printf("Usage : %s <IP> <PORT> <USER ID>\n", argv[0]);
        exit(1);
    }

    // Socket, Socket addr INIT
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) ErrorHandling("socket() Error!");
    memset(&serv_addr, 0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // Connect
    if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) ErrorHandling("Connect Error!");
    printf("Connected with Server - %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
    printf("HELLO, %s!\n", argv[3]);

    // Send ID to Server
    sprintf(msg, "%s", argv[3]);
    write(sock, msg, strlen(msg));

    // Create Thread : Keep Receiving Message
    pthread_create(&rcv_thread, NULL, rcv, (void *)sock);

    // while : Get Message and Write()
    while(1) {
        // Get Message
        fgets(chat, sizeof(chat), stdin);

        // Remove Enter
        for (int i = 0; chat[i] != 0; i++) {
            if (chat[i] == '\n') {
                chat[i] = '\0';
                break;
            }
        }

        // quit
        if(strcmp(chat, "quit") == 0) {
            sprintf(msg, "\033[0;35m%s has left the room\033[0m\n", argv[3]);
            printf("%s", msg);
            write(sock, chat, strlen(chat)+1);
            break;
        }

        // Write
        sprintf(msg, "[%s] : %s\n", argv[3], chat);
        printf("\033[0;32m%s\033[0m", msg);
        write(sock, msg, strlen(msg) + 1);
        sleep(0.5);
    }

    // close socket
    close(sock);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Receive Message From the Server
void * rcv(void * arg) {
    // Local Var
    int sock = (int) arg;
    char buff[500];
    int len;

    // while : Read()
    while(1) {
        len = read(sock, buff, sizeof(buff));
        if(len == -1) ErrorHandling("Receiving Error!");
        printf("%s\n", buff);
    }

    // thread exit
    pthread_exit(0);
    return;
}

////////////////////////////////////////////////////////////////////////////////
// Error Handling Function
void ErrorHandling(char * message) {
    puts(message);
    putc('\n', stderr);
    exit(1);
}