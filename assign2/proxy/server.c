#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

int main()
{
    char server_message[256] = "server reached!";

    //create sock
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //define server adress
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the sockt to IP and port
    int b = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (b != 0)
        printf("error in binding\n");
    // listen for connections
    int l = listen(server_socket, 5);
    if (l != 0)
        printf("error in listening\n");
    char filename[256];
    while (strcmp(filename, "bye") != 0)
    {
        int client_socket;
        client_socket = accept(server_socket, NULL, NULL);

        // get file name and search for it
        recv(client_socket, filename, sizeof(filename), 0);
        printf("filename requested : %s\n\n", filename);
        FILE *file;
        file = fopen(filename, "r");
        if (file != NULL)
        {
            printf("file found\n");
            strcpy(server_message, "file found");
        }
        else
            strcpy(server_message, "file not found");
        send(client_socket, server_message, sizeof(server_message), 0);
        char filedata[256];
        if (file != NULL)
        {
            int data = 1;
            char check[256];
            while (data != 0)
            {
                data = fread(filedata, sizeof(char), 255, file);
                send(client_socket, filedata, sizeof(filedata), 0);
                recv(client_socket, check, sizeof(check), 0);
                if (strcmp(filedata, check) != 0)
                {
                    printf("\n\nsomething wrong with the transmission\n\n\n");
                }
            }
            strcpy(filedata, "endoffile");
            send(client_socket, filedata, sizeof(filedata), 0);
            fclose(file);
            printf("file written\n");
        }
    }
    close(server_socket);

    return 0;
}