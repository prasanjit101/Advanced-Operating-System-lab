#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main()
{
    // create a socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify adress for socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9001);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // connect to a server
    int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    // check error
    if (connection_status == -1)
        printf("there was a error making the connection\n\n");
    else if (connection_status == 0)
        printf("connection made succesfully\n\n");

    //get file name
    while (1)
    {
        char filename[256];
        printf("Enter file name: ");
        scanf("%s", filename);

        //send file name
        send(network_socket, &filename, sizeof(filename), 0);

        //recieve data
        char server_response[256];
        recv(network_socket, &server_response, sizeof(server_response), 0);

        //print data recieved
        printf("data recieved: %s\n", server_response);
        if (strcmp(server_response, "file found") == 0)
        {
            printf("Transmission started\n");
            char filedata[256];
            FILE *file = fopen(filename, "a");
            while (strcmp(filedata, "endoffile") != 0)
            {
                recv(network_socket, &filedata, sizeof(filedata), 0);
                if (strcmp(filedata, "endoffile") != 0)
                    fputs(filedata, file);
            }
            fclose(file);
        }
    }
    //close socket
    //close(network_socket);
    return 0;
}