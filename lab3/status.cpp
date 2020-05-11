#include<iostream>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
using namespace std;

int main()
{
    if(fork()==0)
    {
        cout<<"Child process id:"<<getpid()<<" performing some commands"<<endl;
        //execl("/bin/sh", "bin/sh", "-c", "./wrongpath", "NULL");///////comment this line to see the status when child process works fine
    }
    else
    {
        int child_id;
        int status;
        child_id=wait(&status);
        if(WIFEXITED(status)==true)
        {
            cout<<"child process id:"<<child_id<<" terminated with status:"<<WEXITSTATUS(status);
            if(WEXITSTATUS(status)==0)
            {
                cout<<" =>Child process commands successful";
            }
            if(WEXITSTATUS(status)==127)
            {
                cout<<" =>Invalid path";
            }
        }
    }
}
