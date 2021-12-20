#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//回调函数
void myFun(int num)
{
    //SIGCHLD 信号为17 号信号
    printf("捕捉到的信号：%d\n",num);
    //回收子进程的PCB
    // while (1)
    // {
    //     wait(NULL);
    // }
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
    //提前设置好阻塞信号集，阻塞 SIGCHLD
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,NULL);

    //创建子进程
    pid_t pid;
    //生成20 个子进程
    for (int i = 0; i < 20; i++)
    {
        pid=fork();
        //防止子进程继续生成子进程
        if(pid==0)
        {
            break;
        }
    }
    if(pid>0)
    {
        //父进程

        //捕捉子进程死亡时发送的 SIGCHLD 信号
        //最好是使用 sigaction()函数，而不是 signal() 函数
        struct sigaction act;
        act.sa_flags=0;
        act.sa_handler=myFun;
        sigemptyset(&act.sa_mask);
        sigaction(SIGCHLD,&act,NULL);

        //注册完信号捕捉后，就解除阻塞
        sigprocmask(SIG_UNBLOCK,&set,NULL);

        while (1)
        {
            printf("parent process pid: %d\n",getpid());
            sleep(2);
        }
        
    }
    else if(pid==0)
    {
        //子进程
        printf("child process pid: %d\n",getpid());
    }

    return 0;
}