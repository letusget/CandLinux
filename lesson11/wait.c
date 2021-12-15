/*
#include <sys/types.h>
#include <sys/wait.h>
pid_t wait(int *wstatus);
    功能：等待任意一个子进程结束，如果子进程结束，此函数会回收子进程占用的资源
    参数： int *wstatus
        进程退出时的状态信息，传入的是一个 int 类型的地址，传出参数
    返回值：
        成功： 返回被回收的子进程的id
        失败： 返回-1，原因：所有的子进程都结束，调用函数失败
调用wait 函数的进程会被挂起（阻塞），直到它的一个子进程退出或者受到一个不能被忽略的信号时，才被唤醒
如果没有子进程，该函数会立即返回，返回 -1，如果所有子进程都已经结束了，也会立即返回，返回-1
如果没有子进程，该函数会立即返回，返回 -1，如果所有子进程都已经结束了，也会立即返回，返回-1
如果没有子进程，该函数会立即返回，返回 -1，如果所有子进程都已经结束了，也会立即返回，返回-1

*/
#include <sys/types.h>
#include <sys/wait.h>   //exit 函数需要的头文件
#include <unistd.h> //fork函数需要的头文件
#include <stdio.h>
#include <stdlib.h>


int main()
{
    //有一个父进程，创建 5个子进程，相当于兄弟进程
    pid_t pid;

    //创建5个子进程
    for(int i=0;i<5;i++)
    {
        //因为在执行的时候，父函数创建的子进程也会执行这个for循环，
        //就是说子进程也会创建子进程，就会出现 呈指数增长的进程数，但这并不是我们想要的结果
        pid=fork();
        //如果产生子进程，就结束这个循环，就可以防止这样重复的产生进程
        if(pid==0)
        {
            break;
        }

    }

    if(pid>0)
    {
        //父进程
        while (1)
        {
            printf("I am parent process, pid = %d , ppid = %d \n",getpid(),getppid());

            //int  ret=wait(NULL);
            int st=0;
            int ret=wait(&st);
            if(ret==-1)
            {
                break;
            }
            if(WIFEXITED(st))
            {
                //判断是不是正常退出
                printf("退出的状态码：%d\n",WEXITSTATUS(st));
            }
            if(WIFSIGNALED(st))
            {
                //判断是不是异常终止
                printf("被哪个型号干掉了： %d \n",WTERMSIG(st));
            }
            printf("child die,pid = %d \n",ret);

            sleep(1);

        }
           
    }
    else if(pid==0)
    {
        //子进程
        //为了方便演示，这里加入while循环，手动杀死 子进程（结束子进程），看父进程的运行情况
        while (1)
        {
            printf("I am child process, pid = %d, ppid = %d \n",getpid(),getppid());
            sleep(1);
        }
        //exit(0);
    }


    return 0;
}