#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
void sig_usr(int signum)
{
    if(signum == SIGINT)
    printf("Signal caught!");
    return;
}

int main(void)
{
    pid_t pid, ppid;
    ppid = getpid();
    printf("%d ", ppid);
    pid = fork();
    if(fork() == 0)
    { //Child
        kill(ppid, SIGINT);
    }
    else
    {
        printf("%d %d ",ppid, pid);
        struct sigaction sig;
        sigemptyset(&sig.sa_mask);
        sig.sa_flags = 0;
        sig.sa_handler = sig_usr;
        if(sigaction(SIGINT,&sig,NULL) == 0)
        printf("Signal processed OKay ");
        sleep(10);
    }
}
