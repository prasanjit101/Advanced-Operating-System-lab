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
    int pserver_socket;
    pserver_socket = socket(AF_INET, SOCK_STREAM, 0);

    //define server adress
    struct sockaddr_in pserver_address;
    pserver_address.sin_family = AF_INET;
    pserver_address.sin_port = htons(9001);
    pserver_address.sin_addr.s_addr = INADDR_ANY;

    // bind the sockt to IP and port
    int b = bind(pserver_socket, (struct sockaddr *)&pserver_address, sizeof(pserver_address));
    if (b != 0)
        printf("error in binding\n");
    // listen for connections
    int l = listen(pserver_socket, 5);
    if (l != 0)
        printf("error in listening\n");
    while (1)
    {
        char filename[256];
        int client_socket;
        client_socket = accept(pserver_socket, NULL, NULL);
        printf("[+]connection accepted\n");
        int childpid;
        if ((childpid = fork()) == 0)
        {
            while (strcmp(filename, "bye") != 0)
            {

                // get file name and search for it
                recv(client_socket, filename, sizeof(filename), 0);
                printf("[+]filename requested : %s\n", filename);
                FILE *file;
                file = fopen(filename, "r");
                if (file == NULL)
                {
                    printf("[-]file not found. requesting server\n");
                    int network_socket;
                    network_socket = socket(AF_INET, SOCK_STREAM, 0);

                    // specify adress for socket
                    struct sockaddr_in server_address;
                    server_address.sin_family = AF_INET;
                    server_address.sin_port = htons(9002);
                    server_address.sin_addr.s_addr = INADDR_ANY;

                    // connect to a server
                    int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));
                    if (connection_status == -1)
                        printf("[-]there was a error making the connection\n");
                    else if (connection_status == 0)
                        printf("[+]connection made succesfully\n");
                    send(network_socket, &filename, sizeof(filename), 0);

                    //recieve data
                    char server_response[256];
                    recv(network_socket, &server_response, sizeof(server_response), 0);

                    //print data recieved
                    printf("[+]data recieved: %s\n", server_response);
                    if (strcmp(server_response, "file found") == 0)
                    {
                        printf("[+]data transmission started\n");
                        char filedata[256];
                        FILE *file = fopen(filename, "a");
                        while (strcmp(filedata, "endoffile") != 0)
                        {
                            //char filedata[256];
                            recv(network_socket, &filedata, sizeof(filedata), 0);
                            if (strcmp(filedata, "endoffile") != 0)
                                fputs(filedata, file);
                            //fputs("---------", file);
                            send(network_socket, &filedata, sizeof(filedata), 0);
                            memset(filedata, 0, sizeof(filedata));
                            fflush(file);
                        }
                        fclose(file);
                    }
                }
                file = fopen(filename, "r");
                if (file != NULL)
                    strcpy(server_message, "file found");
                else
                {

                    strcpy(server_message, "file not found");
                }
                send(client_socket, server_message, sizeof(server_message), 0);
                char filedata[256];
                if (file != NULL)
                {
                    int data = 1;
                    while (data != 0)
                    {
                        data = fread(filedata, sizeof(filedata), 256, file);
                        send(client_socket, filedata, sizeof(filedata), 0);
                    }
                    strcpy(filedata, "endoffile");
                    send(client_socket, filedata, sizeof(filedata), 0);
                    fclose(file);
                    printf("[+]file written\n\n");
                }
            }
        }
    }
    close(pserver_socket);

    return 0;
}