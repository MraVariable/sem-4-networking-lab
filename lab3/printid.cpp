#include<iostream>
#include<sys/types.h>
#include<unistd.h>
using namespace std;

int main()
{
    cout<<"Process id:"<<getpid()<<endl;
    cout<<"Creating child process"<<endl;
    if(fork()==0)
    {
        int p_pid=getppid();
        cout<<"After creating child process"<<endl;
        cout<<"Parent process id:"<<p_pid<<endl;
        cout<<"Child process created id:"<<getpid()<<endl;

    }
}
