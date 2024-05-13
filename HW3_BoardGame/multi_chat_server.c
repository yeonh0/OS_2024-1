////////////////////////////////////////////////////////////////////////////////
// Header File
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>

////////////////////////////////////////////////////////////////////////////////
// User Define
#define CLNT_MAX 2              // Max Client
#define BUFFSIZE 200            // Message Buffer Size

////////////////////////////////////////////////////////////////////////////////
// Global Var
int g_clnt_socks[CLNT_MAX];     // Socket List
int g_clnt_count = 0;           // Client Count

pthread_mutex_t g_mutex;    // Socket List Mutex

////////////////////////////////////////////////////////////////////////////////
// Function
void clnt_connection(void * arg);
void send_all_clnt(char * msg, int my_sock);
void ErrorHandling(char * message);

////////////////////////////////////////////////////////////////////////////////
// Main Function
int main() {
    // Local Var
    int serv_sock, clnt_sock;   // Create Client, Server Sockets
    pthread_t t_thread;         // Create Thread

    struct sockaddr_in clnt_addr, serv_addr;    // Cread Client, Server SockAddr
    int clnt_addr_size;         // Client Address Size

    pthread_mutex_init(&g_mutex, NULL);         // Init Mutex

    // Init Server Socket & Binding
    // Server IP : ANY, Port : 3500
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;             
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(3500);

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) ErrorHandling("Bind Error!");
    
    // Listening
    if(listen(serv_sock, 5) == -1) ErrorHandling("Listening Error!");

    // Message Buffer & Length
    char buff[200];
    int recv_len = 0;

    // while
    while(1) {
        // Accept & Return Client Socket
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

        // Add Client Socket to Client Socket List
        pthread_mutex_lock(&g_mutex);
        g_clnt_socks[g_clnt_count++] = clnt_sock;
        pthread_mutex_unlock(&g_mutex);

        // Create Thread : clnt_connection((void*)clnt_sock)
        pthread_create(&t_thread, NULL, clnt_connection, (void*)clnt_sock);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Thread gonna executes this Function
void clnt_connection(void * arg) {
    // Local Var
    int clnt_sock = (int)arg;
    int str_len = 0;
    char msg[BUFFSIZE];
    int i;

    // while : Keep Reading Message
    while(1) {
        // Read Message from the Client
        str_len = read(clnt_sock, msg, sizeof(msg));
        if(str_len == -1) ErrorHandling("Reading Error!");

        // Send Message to the ALL Clients & Print Message on the Server Program
        send_all_clnt(msg, clnt_sock);
        printf("%s\n", msg);
    }

    // Remove the client socket from the socket list
    pthread_mutex_lock(&g_mutex);
    for(i=0; i<g_clnt_count; i++) {
        if(clnt_sock == g_clnt_socks[i]) {
            for(; i<g_clnt_count-1; i++) {
                g_clnt_socks[i] = g_clnt_socks[i+1];
            }
            break;
        }
    }
    pthread_mutex_unlock(&g_mutex);

    // Close the Client Socket
    if(close(clnt_sock) == -1) ErrorHandling("Closing Error!");

    // Exit Thread
    pthread_exit(0);

}

////////////////////////////////////////////////////////////////////////////////
// Send Message to the ALL Clients
void send_all_clnt(char * msg, int my_sock) { 
    // Sending
    pthread_mutex_lock(&g_mutex);
    for(int i=0; i<g_clnt_count; i++) {
        if(my_sock != g_clnt_socks[i]) {
            printf("Send Message : %s\n", msg);
            write(g_clnt_socks[i], msg, strlen(msg) + 1);
        }
    }
    pthread_mutex_unlock(&g_mutex);
}

////////////////////////////////////////////////////////////////////////////////
// Error Handling Function
void ErrorHandling(char * message) {
    puts(message);
    putc('\n', stderr);
    exit(1);
}