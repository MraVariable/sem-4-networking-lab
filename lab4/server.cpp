#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
using namespace std;

int main(int argc, char const *argv[])
{
    bool first=true;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
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
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while(true)
    {
        while(true)
        {
            char buffer[1024]={0};
            valread = read( new_socket , buffer, 1024);
            if(buffer[0]=='+'){break;}
            cout<<"CLIENT:";
            if(buffer[0]=='q' && buffer[1]=='u' && buffer[2]=='i' && buffer[3]=='t')
            {
                send(new_socket,"SEE YOU SOON",12,0);
                cout<<buffer;
                return 0;
            }
            printf("%s\n",buffer );
        }
        while(true)
        {
            cout<<"Enter your message:";
            char *msg;
            if(first==false)
            {cin.ignore();}
            first=false;
            cin.getline(msg,1024,'\n');
            send(new_socket , msg , strlen(msg) , 0 );
            printf("\nmessage sent,enter \'+\' to send more messages:");
            char test;
            scanf("%c",&test);
            if(test!='+'){send(new_socket,"+",1,0);break;}
        }
    }
    return 0;
}

