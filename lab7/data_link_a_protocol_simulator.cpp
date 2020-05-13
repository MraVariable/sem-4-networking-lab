#include <unistd.h>
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#include<time.h>
#include <fcntl.h>
using namespace std;

int main()
{
    int sendfds1[2],sendfds2[2],ackfds1[2],ackfds2[2];
    int returnstatus1, returnstatus2,returnstatus3,returnstatus4;

    returnstatus1 = pipe(sendfds1);
    if (returnstatus1 == -1)
    {
        cout<<"Unable to create pipe 1 \n";
        return 1;
    }

    returnstatus2 = pipe(sendfds2);
    if (returnstatus2 == -1)
    {
        cout<<"Unable to create pipe 2 \n";
        return 1;
    }

    returnstatus3=pipe(ackfds1);
    if(returnstatus3==-1)
    {
        cout<<"Unable to create pipe 3\n";
        return 1;
    }

    returnstatus4=pipe(ackfds2);
    if(returnstatus4==-1)
    {
        cout<<"Unable to create pipe 4\n";
        return 1;
    }

    int nonblockstatus=fcntl(ackfds2[0], F_SETFL, O_NONBLOCK);
    if(nonblockstatus<0){cout<<"Error";return 1;}

    int n1 = fork();
    int n2 = fork();

    if (n1 > 0 && n2 > 0) {
        char readmessage[20];
        close(sendfds1[0]);
        close(sendfds2[0]);close(sendfds2[1]);
        close(ackfds1[0]);close(ackfds1[1]);
        close(ackfds2[1]);
        while(true)
        {
            cout<<"Sender sending....\n";
            sleep(1);
            write(sendfds1[1],"hey receiver",sizeof("hey receiver"));
            bool res=false;
            for(int i=0;i<8;i++)
            {
                sleep(1);
                if(read(ackfds2[0],readmessage,sizeof(readmessage))!=-1){res=true;break;}
            }
            if(res==true){cout<<"Acknowledged\n";}
            else{cout<<"Resending message\n";}
            sleep(5);
        }
    }
    else if (n1 == 0 && n2 > 0)
    {
        char readmessage[20];
        close(sendfds1[1]);
        close(sendfds2[0]);
        close(ackfds1[1]);
        close(ackfds2[0]);
        srand(time(0));
        while(true)
        {
            read(sendfds1[0],readmessage,sizeof(readmessage));
            int num=rand()%100+1;
            if(num<30)
            {
                cout<<"Message dropped by channel\n";
                continue;
            }
            cout<<"Message in channel transmitting message to receiver"<<endl;
            sleep(1);
            write(sendfds2[1],readmessage,sizeof(readmessage));
            read(ackfds1[0],readmessage,sizeof(readmessage));
            num=rand()%100+1;
            if(num<30)
            {
                cout<<"Acknowledge message dropped\n";
                continue;
            }
            cout<<"Transmitting acknowledgment signal\n";
            sleep(1);
            write(ackfds2[1],readmessage,sizeof(readmessage));
        }
    }
    else if (n1 > 0 && n2 == 0)
    {
        char readmessage[20];
        close(sendfds1[0]);close(sendfds1[1]);
        close(sendfds2[1]);
        close(ackfds1[0]);
        close(ackfds2[0]);close(ackfds2[1]);
        while(true)
        {
            read(sendfds2[0],readmessage,sizeof(readmessage));
            cout<<"Receiver:"<<readmessage<<endl<<"Sending acknowledgment\n";
            sleep(1);
            write(ackfds1[1],"+",sizeof("+"));
        }
    }
    return 0;
}
