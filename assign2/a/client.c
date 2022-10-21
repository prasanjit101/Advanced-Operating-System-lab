//  client will send request to server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main(int argc, char const *argv[])
{
    int sockD = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(argv[1]); // use some unused port number
    servAddr.sin_addr.s_addr = INADDR_ANY;
    int connectStatus = connect(sockD, (struct sockaddr *)&servAddr,
                                sizeof(servAddr));
    if (connectStatus == -1)
    {
        printf("Error...\n");
    }
    else
    {
        char strData[255];
        recv(sockD, strData, sizeof(strData), 0);
        printf("Message: %s\n", strData);
    }
    return 0;
}
