/*
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);   
    函数作用：创建子进程
    返回值：
        fork() 函数的返回值会返回两次，一次是在父进程汇总，一次是在子进程中
        在父进程中返回创建的 子进程的id
        在子进程中返回0 （表示成功）
        通过 fork 的返回值来区分父进程 和 子进程
        在父进程中返回 -1 表示创建子进程失败，并设置 errno
        创建失败主要原因：
            当前系统的进程数已经达到了系统规定的上限，这时 errno 的值被设置为 EAGAIN
            系统内存不足，这时 errno 的值被设置为 ENOMEM

*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int number=10;

    //创建子进程
    pid_t pid=fork();

    //判断是父进程 还是 子进程
    if(pid>0)
    {
        printf("pid : %d\n",pid);
        //如果大于0 返回的的是子进程的id
        printf("I am parent process,pid : %d,ppid : %d \n",getpid(),getppid());

        printf("parent num : %d\n",number);
        number+=10;
        printf("parent num : %d\n",number);
    }
    else if(pid==0)
    {
        //当前是子进程
        printf("I am child process,pid : %d,ppid : %d \n",getpid(),getppid());

        printf("child num : %d\n",number);
        number+=100;
        printf("child num : %d\n",number);
    }

    //执行内容
    //既不属于父进程，也不属于子进程，父子进程交替执行
    for(int i=0;i<5;i++)
    {
        printf("i : %d , pid : %d\n",i,getpid());
        sleep(1);
    }
    printf("\n");

    return 0;
}