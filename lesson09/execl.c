/*
#include <unistd.h>
int execl(const char *path, const char *arg, ... );
    参数：
        path : 需要指定的 执行的文件路径或名称，使用相对路径或绝对路径都可以，建议使用绝对路径
        arg : 执行可执行文件所需的参数列表
            第一个参数一般没什么作用，为了方便，一般写的是执行的程序的名称
            从第二个参数开始往后，就是程序执行 所需的参数列表
            参数见最后需要以 NULL  结束（哨兵）
    返回值：
        正常执行 没有返回值，只有调用出错或是调用失败才有返回值，返回 -1

*/

#include <unistd.h>
#include <stdio.h>

int main()
{
    //床架一个子进程，在子进程中国 执行 exec函数族中的函数

    pid_t pid=fork();

    if(pid>0)
    {
        //父进程
        printf("I am parent process, pid : %d\n",getpid());
        //防止孤儿进程
        sleep(1);
    }
    else if(pid==0)
    {
        //子进程
        //使用绝对路径  可执行程序  NULL结尾
        execl("/root/clearn/lesson09/hello","hello",NULL);
        //下面的代码就会被替换掉，不会执行，而是执行上面的execl 中的内容
        printf("I am a child process  pid : %d \n",getpid());

    }

    for (int i=0;i<5;i++)
    {
        printf("i = %d, pid = %d\n",i,getpid());

    }

    return 0;
}