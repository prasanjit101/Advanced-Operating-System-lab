//  server will create pool of worker threads (say 5) to provide services to pool of clients (say 5).Server should be behaving as a concurrent server. Now send request (say send a request string hello) from clients. Servershould respond with string Hi, responding with Thread1, Thread2 and so forth, which is the response from server thread1, thread2, etc that you have created already
// run command on server: ./server 5000 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *thread_function(void *arg)
{
    int newsockfd = *(int *)arg;
    int n;
    char buffer[256];
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("Here is the message: %s \n", buffer);
    // respond with the thread id
    char *thread_id = (char *)malloc(256);
    n = write(newsockfd, thread_id, strlen(thread_id));
    if (n < 0)
        error("ERROR writing to socket");
    close(newsockfd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    pthread_t thread[5];
    int i = 0;
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided \n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    printf("Server is listening on port %d \n", portno);
    clilen = sizeof(cli_addr);
    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        if (pthread_create(&thread[i], NULL, thread_function, &newsockfd) != 0)
        {
            printf("Failed to create thread \n");
            return 1;
        }
        if (i >= 4)
        {
            i = 0;
            while (i < 5)
            {
                pthread_join(thread[i++], NULL);
            }
            i = 0;
        }
    }
    close(sockfd);
    return 0;
}