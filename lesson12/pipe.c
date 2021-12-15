/*
#include <unistd.h>
int pipe(int pipefd[2]);
    功能：创建一个匿名管道，用来进行进程间通信
    参数：int pipedf[2] 是传出参数
        pipefd[0] 对应管道的读端
        pipefd[1] 对应管道的写端
    返回值：
        成功：返回0
        失败：返回-1
    注意：匿名管道只能用于具有关系的进程之间通信，比如父子进程，兄弟进程
*/
//实现 子进程给父进程发送数据，父进程读取并输出
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    //在 fork 之前 创建管道
    //创建大小为2 的数组，第一个为读端，第二个为写端
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret==-1)
    {
        //标准错误输出
        perror("pipe");
        //return ;
    }

    //创建子进程
    pid_t pid=fork();
    if(pid>0)
    {
        // 父进程
        printf("I am parent process, pid : %d\n",getpid());

        //关闭写端
        close(pipefd[1]);

        //从管道读取段读取数据
        char buf[1024]={0};
        //先读再写
        while(1)
        {
            int len = read(pipefd[0],buf,sizeof(buf));
            printf("parent recv : %s, pid : %d \n",buf,getpid());

            //相互通信，相互写入数据
            //向管道中写入数据
            //char * str="Hello I am parent";
            //write(pipefd[1],str,strlen(str));
            //sleep(1);
        }
        
        
    }
    else if(pid==0)
    {
        //子进程
        printf("I am child process, pid : %d\n",getpid());

        //关闭 读端
        close(pipefd[0]);

        char buf[1024]={0};
        //先写后读，因为父进程是先读后写，所以父进程在读的时候，
        //就需要确保子进程已经写入数据了，否则就会陷入死锁状态
        while(1)
        {
            //向管道中写入数据
            char * str="Hello I am child\n";
            write(pipefd[1],str,strlen(str));
            //sleep(1);

            //int len = read(pipefd[0],buf,sizeof(buf));
            //printf("child recv : %s, pid : %d \n",buf,getpid());
            //重置缓冲区，将 buf 内前1024个字节清零
            //bzero(buf,1024);
        }
        
    }

    return 0;
}