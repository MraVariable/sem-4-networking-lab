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
    int sendfds[2],ackfds1[2],ackfds2[2];
    if(pipe(sendfds)==-1)
    {
        cout<<"Error creating pipe\n";
        return 1;
    }
    if(pipe(ackfds1)==-1)
    {
        cout<<"Error creating pipe\n";
        return 1;
    }
    if(pipe(ackfds2)==-1)
    {
        cout<<"Error creating pipe\n";
        return 1;
    }
    if(fcntl(ackfds1[0], F_SETFL, O_NONBLOCK)==-1)
    {
        cout<<"Error creating non-blocking process\n";
        return 1;
    }
    if(fcntl(ackfds2[0], F_SETFL, O_NONBLOCK)==-1)
    {
        cout<<"Error creating non-blocking process\n";
        return 1;
    }
    int n1=fork();
    int n2=fork();

    if(n1>0 && n2>0)
    {
        close(sendfds[0]);
        close(ackfds1[1]);
        close(ackfds2[0]);close(ackfds2[1]);
        char msg[4][15]={"1_I","1_am","1_process","1_1"};
        int cur=0;
        srand(time(0));
        char readmessage[25];
        int tries=2;
        for(int z=1;;z++)
        {
            if((z%7)==0)
            {
                cout<<"Process 1 sending\n";
                write(sendfds[1],msg[cur],sizeof(msg[cur]));
                int num=rand()%2+5;
                bool ack=false;
                for(int i=0;i<num;i++)
                {
                    sleep(1);
                    if(read(ackfds1[0],readmessage,sizeof(readmessage))!=-1)
                    {
                        cur+=1;
                        cout<<"Acknowledged\n";
                        ack=true;break;
                    }
                }
                if(ack==false)
                {
                    tries-=1;
                    if(tries==0)
                    {cout<<"Number of tries exhausted for process 1\n";cur+=1;tries=2;}
                    else
                    {cout<<"Process 1 resending\n";}
                }
                if(cur==4){break;}
                sleep(5);
            }
            else{cout<<"Process 1 didn't send in this slot\n";}
            sleep(1);
        }
        cout<<"Process 1 transfer complete\n";
        close(sendfds[1]);
    }
    else if(n1==0 && n2>0)
    {
        close(sendfds[0]);
        close(ackfds1[0]);close(ackfds1[1]);
        close(ackfds2[1]);
        char msg[4][15]={"2_I","2_am","2_process","2_2"};
        int cur=0;
        srand(time(0));
        char readmessage[25];
        int tries=2;
        for(int z=1;;z++)
        {
            if((z%5)==0)
            {
                cout<<"Process 2 sending\n";
                write(sendfds[1],msg[cur],sizeof(msg[cur]));
                int num=rand()%2+6;
                bool ack=false;
                for(int i=0;i<num;i++)
                {
                    sleep(1);
                    if(read(ackfds2[0],readmessage,sizeof(readmessage))!=-1)
                    {
                        cur+=1;
                        cout<<"Acknowledged\n";
                        ack=true;break;
                    }
                }
                if(ack==false)
                {
                    tries-=1;
                    if(tries==0)
                    {cout<<"Number of tries exhausted for process 2\n";cur+=1;tries=2;}
                    else
                    {cout<<"Process 2 resending\n";}
                }
                if(cur==4){break;}
                sleep(5);
            }
            else{cout<<"Process 2 didn't send in this slot\n";}
            sleep(1);
        }
        cout<<"Process 2 transfer complete\n";
        close(sendfds[1]);
    }
    else if(n1>0 && n2==0)
    {
        close(sendfds[1]);
        close(ackfds1[0]);
        close(ackfds2[0]);
        char readmessage[25];
        ofstream obj1("process1.txt"),obj2("process2.txt");
        while(read(sendfds[0],readmessage,sizeof(readmessage)))
        {
            if(readmessage[0]=='1')
            {
                obj1<<readmessage+2<<" ";
                cout<<"Receiver:Process 1 message->"<<readmessage+2<<" Saved in process1.txt"<<endl;
                write(ackfds1[1],"+",2);
            }
            else
            {
                obj2<<readmessage+2<<" ";
                cout<<"Receiver:Process 2 message->"<<readmessage+2<<" Saved in process2.txt"<<endl;
                write(ackfds2[1],"+",2);
            }
            sleep(2);
        }
        obj1.close();
        obj2.close();
        close(ackfds1[1]);
        close(ackfds2[1]);
    }
    return 0;
}

