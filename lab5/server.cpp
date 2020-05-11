#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
using namespace std;
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        int pid=fork();
        if(pid<0){cout<<"ERROR ON FORK";}
        if(pid==0)
        {
            close(server_fd);
            while(true)
            {
                char buffer[1024]={0};
                valread = read( new_socket , buffer, 1024);
                cout<<"[PORT ADDRESS:"<<address.sin_port<<" IP-ADDRESS:"<<address.sin_addr.s_addr<<"]=>";
                printf("%s\n",buffer );
                if(buffer[0]=='q' && buffer[1]=='u' && buffer[2]=='i' && buffer[3]=='t')
                {
                    send(new_socket , "SEE YOU SOON" , 12 , 0 );
                    break;
                }
                send(new_socket , "RECIEVED BY SERVER" , 18 , 0 );
            }
            exit(0);
        }
        else
        {close(new_socket);}
    }
    return 0;
}
