#include<iostream>
#include<sys/wait.h>
#include<unistd.h>
using namespace std;

int main()
{
    cout<<"Parent process creating child process\n";
    if(fork()>0)
    {
        cout<<"I am parent process and will wait till my child completes the counting\n";
        pid_t id= wait(NULL);
        cout<<"exiting parent after child whose id is:"<<id<<endl;
    }
    else
    {
        cout<<"I am child process and will count from 1 till 10 and then exit\n";
        for(int i=1;i<=10;i++)
        {
            cout<<i<<" ";
        }
        cout<<"\nexiting child with id:"<<getpid()<<endl;
    }
}
