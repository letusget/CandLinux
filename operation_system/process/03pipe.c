#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    //建立管道
    int pipefd[2];
    int ret=pipe(pipefd);
    if(ret==-1)
    {
        perror("pipe");
    }

    int i=0;
    //创建两个进程
    pid_t pid;
    for ( i = 0; i < 2; i++)
    {
        pid=fork();
        //防止递归调用，子进程继续创建子进程
        if(pid==0)
        {
            break;
        }
        else if(pid<0)
        {
            //进程创建失败
            perror("fork");
            exit(1);
        }
        
         
    }

    //父进程
    if(pid>0)
    {
        //等待 子进程执行完毕
        sleep(1);
        //读入管道信息
        char buf[1024];
        int len=read(pipefd[0],buf,sizeof(buf));
        printf("parent recv: %s\n",buf);
        //printf("parent recv: %s, pid:%d\n",buf,getpid());
    }
    else if(pid==0)
    {
        //printf("child process! pid: %d\n\n",getpid());
        //子进程写入信息
        char * str="Hello I am child ";
        write(pipefd[1],str,strlen(str));
    }
    
    return 0;
}