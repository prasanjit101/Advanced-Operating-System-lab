#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define PORT_NO 8888
#define BUFFER_SIZE 1024
#define CONNECTION_NUMBER 10

int thread_count = 0;
sem_t mutex;

void *connection_handler(void *socket_desc)
{
    int request;
    char client_reply[BUFFER_SIZE];

    int sock = *((int *)socket_desc);
    request = recv(sock, client_reply, BUFFER_SIZE, 0);

    sem_wait(&mutex);
    thread_count++;

    sem_post(&mutex);

    if (request < 0)
    {
        puts("Recv failed");
    }
    else if (request = 0)
        puts("Client disconnected unexpectedly.");

    else
    {
        // message received
        sem_wait(&mutex);
        if (client_reply == "hello")
        {
            strcpy(client_reply, "Hi");
            send(sock, client_reply, strlen(client_reply), 0);
        }
        sem_post(&mutex);
    }

    free(socket_desc);
    close(sock);
    sem_wait(&mutex);
    thread_count--;
    sem_post(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    sem_init(&mutex, 0, 1);
    int socket_desc, new_socket, c, *new_sock;
    struct sockaddr_in server, client;
    char *ip = "127.0.0.1";

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        puts("couldnot create socket.");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(PORT_NO);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Binding failed");
        return 1;
    }

    listen(socket_desc, 20);
    puts("waiting for incoming connection..");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        puts("Connection established\n");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;

        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
        {
            puts("could not create thread");
            return 1;
        }
    }

    return 0;
}

// command to run
// gcc server.c -o server -lpthread