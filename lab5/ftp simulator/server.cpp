#include <iostream>
#include<sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    char *cd="C:/sem4 networking packets/lab5/ftp simulator";
    if(argc<2)
    {
        cout<<"Provide the port number to listen on";
        return 0;
    }
    bool first=true;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int portno=atoi(argv[1]);
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( portno );

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
        char buffer[1024]={0};
        valread = read( new_socket , buffer, 1024);
        if(buffer[0]=='q' && buffer[1]=='u' && buffer[2]=='i' && buffer[3]=='t')
        {
            send(new_socket,"SEE YOU SOON",12,0);
            return 0;
        }

        if(buffer[0]=='p' && buffer[1]=='u' && buffer[2]=='t')
        {
            char file[2000];
            valread=read(new_socket,file,2000);
            cout<<file<<endl;
        }

        if(buffer[0]=='g' && buffer[1]=='e' && buffer[2]=='t')
        {
                char file[1024];
                valread=read(new_socket,file,1024);
                ifstream ifobj;
                ifobj.open(file);
                if(ifobj.fail())
                {
                    send(new_socket,"No Such File",12,0);
                    continue;
                }
                char ch;
                int i=0;
                char bufstring[2000];
                while(ifobj>>ch)
                {
                    bufstring[i]=ch;
                    i++;
                }
                ifobj.close();
                send(new_socket,bufstring,strlen(bufstring),0);
        }

        if(buffer[0]=='c')
        {
            if(buffer[2]=='l' && buffer[3]=='s')
            {
                //nothing
            }
            else if(buffer[2]=='p' && buffer[3]=='w' && buffer[4]=='d')
            {
                //nothing
            }
            else if(buffer[2]=='c' && buffer[3]=='d')
            {
                //nothing
            }
        }
        if(buffer[0]=='s')
        {
            if(buffer[2]=='l' && buffer[3]=='s')
            {
                char*args[]={"./s_ls",cd,NULL};
                if(fork()==0)
                {execv(args[0],args);}
            }
            else if(buffer[2]=='p' && buffer[3]=='w' && buffer[4]=='d')
            {
                cout<<cd<<endl;
            }
            if(buffer[2]=='c' && buffer[3]=='d')
            {
                char dirname[1024]={0};
                valread=read(new_socket,dirname,1024);
                cd=dirname;
            }
        }
    }
    return 0;
}

