/*
模拟实现 ps aux | grep xxx  练习 父子进程间通信
    子进程： ps aux 子进程结束进程后，将数据发给父进程
    父进程：获取到数据，（过滤），然后输出

    execlp() 函数实现 子进程将标准输出 stdout_fileno 重定向到管道写端

*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

int main()
{
    //创建一个管道
    int fd[2];
    int ret=pipe(fd);
    if(ret==-1)
    {
        perror("pipe");
        exit(0);
    }

    //一定要先创建 管道进程，在子进程直线就要创建 管道进程
    pid_t pid=fork();

    if(pid>0)
    {
        //父进程

        //关闭写端
        close(fd[1]);
        //从管道中读取数据
        char buf[1024]={0};

        //长度减一 是为了减掉最后的结尾符 \0
        //这里需要循环读入，因为进程是数据很多，一个1023 字节肯定是不够的 
        int len = -1;
        while(read(fd[0],buf,sizeof(buf)-1)>0)
        {
            //（过滤数据输出）
            printf("%s",buf);

            //每次输出完 都要情况输出缓冲区的内容，防止输出重复信息
            memset(buf,0,1024);
        }

        //回收子进程资源
        wait(NULL);
        
    }
    else if(pid==0)
    {
        //子进程
        
        //关闭读端
        close(fd[0]);
        
        //重定向 文件描述符，不将内容放到 终端，而是放到缓冲区，使父进程读到
        //文件描述符重定向stdout_fileno -> fd[1]
        dup2(fd[1],STDOUT_FILENO);
        //执行 ps aux
        execlp("ps","ps","aux",NULL);

        //如果失败
        perror("execlp");
        exit(0);

    }
    else
    {
        //创建子进程失败
        perror("fork");
        exit(0);
    }

    return 0;
}