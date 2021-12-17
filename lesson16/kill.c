/*
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid,int sig)
    功能：给任何进程或者进程组pid，发送任何信号 sig
    参数：
        pid: 需要发送给的进程 id
            pid>0: 将信号发送给指定的进程
            pid=0: 将信号发送给当前的进程组的所有进程
            pid=-1：将信号发送给每一有权限接收的进程
            pid<-1: 这个pid=某个进程组的id取反
        sig: 需要发送的信号，或者是 宏值，更建议使用 宏值，因为编号 在不同架构下，有可能会有不同，但是 宏值永远都是一样的，如果是0，就不发送任何信号
    kill(getppid(),9);  //给父进程发送 9号信号
    kill(getpid(),9);   //给当前进程发送 9号信号
    返回值：
        成功：返回0
        失败：返回非0

#include <signal.h>
int raise(int sig);
    功能：给当前进程发送信号
    参数：
        sig: 要发送的信号
    返回值：
        成功：返回0
        失败：返回非0
    相当于 kill(getpid(),sig); 函数的作用
#include <stdlib.h>
void abort(void);  
    功能：发送 SIGABRT 信号给当前的进程，杀死当前进程
*/

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    //创建子进程
    pid_t pid=fork();

    if(pid==0)
    {
        //子进程
        int i=0;
        for ( i = 0; i < 5; i++)
        {
            /* code */
            printf("child process\n");
            sleep(1);
        }
        
    }
    else if(pid>0)
    {
        //父进程
        printf("parent process\n");
        sleep(2);
        printf("kill child process now \n");
        //杀死子进程
        kill(pid,SIGINT);
    }

    return 0;
}