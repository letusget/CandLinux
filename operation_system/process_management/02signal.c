#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void myAlarm(int num)
{
    printf("捕捉到信号：%d\n",num);

    //杀死子进程
    //kill();

    while (1)
    {
        //回收所有子进程 以非阻塞运行
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret>0)
        {
            //有一个子进程被回收了
            printf("child die, pid = %d\n",ret);
        }
        else if(ret==0)
        {
            //仍然有子进程
            break;
        }
        else if(ret==-1)
        {
            //没有子进程,结束回调函数
            break;
        }
    }
}
int main()
{
    //signal(SIGALRM,myAlarm);

    //提前设置好阻塞信号集，阻塞 SIGCHLD
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,NULL);

    pid_t pid;
    for(int i=0;i<2;i++)
    {
        pid=fork();
        if(pid==0)
        {
            break;
        }
        printf("I am child process, pid: %d\n",getpid());
    }

    //父进程
    if(pid>0)
    {
        //捕捉子进程死亡时发送的 SIGCHLD 信号
        //最好是使用 sigaction()函数，而不是 signal() 函数
        struct sigaction act;
        act.sa_flags=0;
        act.sa_handler=myAlarm;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD,&act,NULL);

        //注册完信号捕捉后，就解除阻塞
        sigprocmask(SIG_UNBLOCK,&set,NULL);



    }
    else if(pid==0)
    {
        //子进程
        
    }


    return 0;
}