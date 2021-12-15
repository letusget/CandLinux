

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{

    //创建子进程
    pid_t pid=fork();

    //判断是父进程 还是 子进程
    if(pid>0)
    {
       // printf("pid : %d\n",pid);
        //如果大于0 返回的的是子进程的id
        printf("I am parent process,pid : %d,ppid : %d \n",getpid(),getppid());

    }
    else if(pid==0)
    {
        sleep(1);
        //当前是子进程
        printf("I am child process,pid : %d,ppid : %d \n",getpid(),getppid());
        
    }

    //执行内容
    //既不属于父进程，也不属于子进程，父子进程交替执行
    for(int i=0;i<5;i++)
    {
        printf("i : %d , pid : %d\n",i,getpid());
       // sleep(1);
    }
    printf("\n");

    return 0;
}
