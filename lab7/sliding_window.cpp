#include <unistd.h>
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#include<time.h>
#include <fcntl.h>
#include<fstream>
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

    nonblockstatus=fcntl(sendfds2[0], F_SETFL, O_NONBLOCK);
    if(nonblockstatus<0){cout<<"Error";return 1;}

    int n1 = fork();
    int n2 = fork();

    if (n1 > 0 && n2 > 0) {
        int seq=0;int ack=-1;int outstanding=0;
        char readmessage[20];
        close(sendfds1[0]);
        close(sendfds2[0]);close(sendfds2[1]);
        close(ackfds1[0]);close(ackfds1[1]);
        close(ackfds2[1]);
        ifstream ifobj("sw_send_file1.txt");
        ofstream ofobj("output1.txt");
        char t[10];
        while(ifobj.good())
        {
            if(outstanding<3)
            {
                char temp[10];
                ifobj>>temp;
                t[0]=48+seq;
                t[1]='_';
                if(ack==-1){t[2]='@';}
                else{t[2]='0'+ack;}
                ack=-1;
                t[3]='_';
                t[4]=temp[0];
                t[5]=temp[1];
                t[6]='\0';
                seq=(1+seq)%3;
                outstanding+=1;
                cout<<"1_Sender sending....\n";
                sleep(1);
                write(sendfds1[1],t,sizeof(t));
            }
            for(int i=0;i<5;i++)
            {
                sleep(1);
                int d=read(ackfds2[0],readmessage,sizeof(readmessage));
                if(d!=-1)
                {
                    ack=readmessage[0]-'0';
                    if(readmessage[2]!='@'){outstanding-=1;cout<<"1_Frame "<<readmessage[2]<<" acknowledged\n";}
                    cout<<"data saved"<<"_1"<<endl;
                    ofobj<<readmessage+4;ofobj<<" ";
                }
                else if(!d){break;}

            }
        }
        close(sendfds1[1]);
        ifobj.close();
        ofobj.close();
        cout<<"Transfer Complete\n";
    }
    else if (n1 == 0 && n2 > 0)
    {
        int c=1;
        char readmessagea[20],readmessageb[20];
        close(sendfds1[1]);
        close(sendfds2[0]);
        close(ackfds1[1]);
        close(ackfds2[0]);
        srand(time(0));
        while(true)
        {
            if(read(sendfds1[0],readmessagea,sizeof(readmessagea)))
            {
                cout<<"Message in channel transmitting message to B"<<endl;
                write(sendfds2[1],readmessagea,sizeof(readmessagea));
            }
            if(read(ackfds1[0],readmessageb,sizeof(readmessageb)))
            {
                cout<<"Message in channel transmitting message to A\n";
                write(ackfds2[1],readmessageb,sizeof(readmessageb));
            }
            c++;if(c==9){break;}
        }
        close(ackfds2[1]);
        close(sendfds2[1]);
    }
    else if (n1 > 0 && n2 == 0)
    {
        int seq=0;int ack=-1;int outstanding=0;
        char readmessage[20];
        close(sendfds1[0]);close(sendfds1[1]);
        close(sendfds2[1]);
        close(ackfds1[0]);
        close(ackfds2[0]);close(ackfds2[1]);
        ifstream ifobj("sw_send_file2.txt");
        ofstream ofobj("output2.txt");
        char t[10];
        while(ifobj.good())
        {
            if(outstanding<3)
            {
                char temp[10];
                ifobj>>temp;
                t[0]=48+seq;
                t[1]='_';
                if(ack==-1){t[2]='@';}
                else{t[2]='0'+ack;}
                ack=-1;
                t[3]='_';
                t[4]=temp[0];
                t[5]=temp[1];
                t[6]='\0';
                seq=(1+seq)%3;
                outstanding+=1;
                cout<<"2_Sender sending....\n";
                sleep(1);
                write(ackfds1[1],t,sizeof(t));
            }
            for(int i=0;i<5;i++)
            {
                sleep(1);
                int d=read(sendfds2[0],readmessage,sizeof(readmessage));
                if(d!=-1)
                {
                    ack=readmessage[0]-'0';
                    if(readmessage[2]!='@'){outstanding-=1;cout<<"2_Frame "<<readmessage[2]<<" acknowledged\n";}
                    cout<<"data saved"<<"_2"<<endl;
                    ofobj<<readmessage+4;ofobj<<" ";
                }
                else if(!d){break;}
            }
        }
        ofobj.close();
        ifobj.close();
    }
    return 0;
}


