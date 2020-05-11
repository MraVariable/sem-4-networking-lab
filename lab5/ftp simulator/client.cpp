#include<sys/wait.h>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[])
{
    char *cd="C:/sem4 networking packets/lab5/ftp simulator";
    if(argc<3)
    {
        cout<<"Provide ip address and port";
        return 0;
    }
    int portno=atoi(argv[2]);
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    cout<<"CONNECTION ESTABLSHED\n";
    while(true)
    {
        char *cmd;
        cout<<"Enter the command:";
        cin>>cmd;
        send(sock , cmd , strlen(cmd) , 0 );

        if(cmd[0]=='g' && cmd[1]=='e' && cmd[2]=='t')
        {
            cout<<"Enter the file name:";
            char filename[1024];
            cin>>filename;
            send(sock,filename,strlen(filename),0);
            char result[2000];
            valread = read( sock , result, 2000);
            cout<<result<<endl;
        }

        if(cmd[0]=='p' && cmd[1]=='u' && cmd[2]=='t')
        {
            cout<<"Enter the file name:";
            char file[1024];
            cin>>file;
            ifstream ifobj;
            ifobj.open(file);
            if(ifobj.fail())
            {
                send(sock,"No Such File",12,0);
                continue;
            }
            char *content=new char[2000];
            char ch;
            int *zx=new int;
            *zx=0;
            while(ifobj>>ch)
            {
                content[*zx]=ch;
                (*zx)++;
            }
            cout<<content<<endl;
            send(sock,content,strlen(content),0);
            ifobj.close();
        }


        if(cmd[0]=='c')
        {
            if(cmd[2]=='p' && cmd[3]=='w' && cmd[4]=='d')
            {
                cout<<cd<<endl;
            }
            else if(cmd[2]=='l' && cmd[3]=='s')
            {
                if(fork()==0)
                {
                    char *args[]={"./s_ls",cd,NULL};
                    execv(args[0],args);
                }
                else{wait(NULL);}
            }
            else if(cmd[2]=='c' && cmd[3]=='d')
            {
                char dirname[1024];
                cout<<"Enter the directory name:";
                cin>>dirname;
                cd=dirname;
            }
        }
        if(cmd[0]=='s')
        {
            if(cmd[2]=='c' && cmd[3]=='d')
            {
                char dirname[1024];
                cout<<"\nEnter the directory name:";
                cin>>dirname;
                send(sock,dirname,strlen(dirname),0);
            }
        }

        if(cmd[0]=='q' && cmd[1]=='u' && cmd[2]=='i' && cmd[3]=='t')
        {
            char buffer[1024]={0};
            valread = read( sock , buffer, 1024);
            cout<<buffer<<endl<<"CLOSING CONNECTION";
            return 0;
        }
    }
    return 0;
}
