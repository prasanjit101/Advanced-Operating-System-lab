#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECV_BUF 1600

int main(int argc, char *argv[])
{

    int sockfd;
    struct sockaddr_in serv_addr;
    char *file_name;
    int serv_len = sizeof(serv_addr);
    if (argc < 2)
    {
        printf("Format : ./client filename_to_receive downloaded_file_name\n");
        exit(1);
    }
    file_name = argv[1];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("\nerror calling socket\n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8000);

    ssize_t recv_bytes;
    char recv_buff[MAX_RECV_BUF];
    printf("\n\nConnecting to the server...\n");

    sendto(sockfd, file_name, strlen(file_name), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    int ptr;
    char *newfile = argv[2];
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

    recv_bytes = recvfrom(sockfd, (char *)recv_buff, sizeof(recv_buff), 0, (struct sockaddr *)&serv_addr, (socklen_t *)&serv_len);
    if (recv_bytes <= 0)
    {
        printf("\n Error in receving \n");
        exit(1);
    }
    else
    {
        if ((ptr = open(newfile, O_WRONLY | O_CREAT, 0644)) < 0)
        {
            printf("Error in writing to file\n");
        }
        else if (strncmp(recv_buff, "INVALID", 7) == 0)
        {
            printf("File not found\n\n\n");
            exit(1);
        }
        else
        {
            open(newfile, O_RDWR | O_CREAT | O_EXCL | O_APPEND, mode);
            write(ptr, recv_buff, recv_bytes);
            printf("%s\n", recv_buff);
            printf("\nNew file '%s' has been created and the contents from server file have been copied to it.\n\n", newfile);
        }
    }
    close(sockfd);
    return 0;
}