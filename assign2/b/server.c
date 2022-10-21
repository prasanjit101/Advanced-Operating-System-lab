#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define MAX_SEND_BUF 1600

char msg[1000];
long data_len;
int sockfd;
struct sockaddr_in serv_addr, client_address;
int cli_len = sizeof(client_address);
char *newfile;

void con_handler(int sockfd)
{
    int fd;
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    char send_buff[MAX_SEND_BUF];
    if ((fd = open(newfile, O_RDONLY, 0644)) < 0)
    {
        printf("Error in opening file\n");
        exit(1);
    }
    else
    {
        int bytes_read;
        while ((bytes_read = read(fd, send_buff, MAX_SEND_BUF)) > 0)
        {
            sendto(sockfd, send_buff, bytes_read, 0, (struct sockaddr *)&client_address, sizeof(client_address));
        }
        printf("File sent successfully from thread id %ld\n", pthread_self());
        close(fd);
    }
}

int main(int argc, char *argv[])
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Error calling Socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8000);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("error binding");
        exit(1);
    }

    while (1)
    {
        data_len = recvfrom(sockfd, (char *)msg, sizeof(msg), 0, (struct sockaddr *)&client_address, (socklen_t *)&cli_len);
        newfile = msg;
        if (data_len)
        {
            printf("\n\nClient connected to Multithread connectionless server\n");
            printf("File name recieved: %s\n", msg);
        }
        pthread_t child;
        if (pthread_create(&child, NULL, (void *)con_handler, (void *)(intptr_t)sockfd) < 0)
        {
            printf("Error creating thread\n");
            exit(1);
        }
    }
}
