#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
    //创建子进程1
    pid_t pid1=fork();
    
    //父进程
    if(pid1>0)
    {
        //printf("parent1 process! pid: %d, ppid: %d\n",getpid(),getppid());

        //创建子进程2
        pid_t pid2=fork();
/*        
        if(pid2>0)
        {
            printf("parent2 process! pid: %d, ppid: %d\n",getpid(),getppid());
        }
        else if(pid2==0)
        {
            printf("child2 process! pid: %d, ppid: %d\n",getppid(),getppid());
        }
*/         
    }
/*
    //子进程
    else if(pid1==0)
    {
        printf("child1 process! pid: %d, ppid: %d\n",getpid(),getppid());
    }
*/
    //父子进程都要执行的代码,这里打印出 进程号，方便区别进程
    //printf("a, pid: %d\n",getpid());
    printf("I am a process, pid: %d\n",getpid());


    
    printf("\n");

    return 0;
}