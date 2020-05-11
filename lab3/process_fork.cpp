#include<iostream>
#include<sys/types.h>
#include<unistd.h>
using namespace std;

int main()
{
    int check=fork();
    if(check==0)//child
    {
        cout<<"I am child process";
    }
    else if(check>0)
    {
        cout<<"\nI am the parent process";
    }
    else
    {
        cout<<"Child cannot be created";
    }
    cout<<"\ndone";
}
