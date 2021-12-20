/*
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
SIGKILL 和 SIGSTOP 信号既不能被捕捉，也不能被忽略
    功能：不同的系统会有不同的功能差异，但主要的功能是捕捉信号
    参数：
        signum: 要捕捉的信号
        handler: 捕捉到的信号要如何处理
            SIG_IGN：忽略信号
            SIG_DFL：使用信号的默认行为
            回调函数：内核调用该函数，程序员只负责写该函数，捕捉到信号后该如何处理信号，函数的类型根据实际需要，主要是看函数指针的定义，函数指针是实现回调的，函数实现之后，将函数名放到函数指针的位置就可以了

    返回值：
        成功：返回上一次注册的信号处理函数的地址，第一次就返回NULL
        失败：返回SIG_ERR，设置错误号
*/

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>

//回调函数
void myalarm(int num)   //int 类型的参数表示捕捉到的信号的值
{
    printf("捕捉到的信号的编号是: %d\n",num);
    printf("------------------\n\n");
}
//目的：三秒钟后，每隔两秒 输出一个定时
int main()
{
    //注册信号捕捉
    //signal(SIGALRM,SIG_IGN);    //忽略定时器发送的SIGALRM信号
    //signal(SIGALRM,SIG_DFL);    //默认执行信号
    signal(SIGALRM,myalarm);   //回调函数

    //setitimer函数需要的结构体
    struct itimerval new_value;
    //设置结构体中的值
    new_value.it_interval.tv_sec=2; //每隔两秒 定时一次
    new_value.it_interval.tv_usec=0;    //微秒如果不设置就会出现随机值

    //设置延迟的时间
    new_value.it_value.tv_sec=3;    //延迟三秒后执行
    new_value.it_value.tv_usec=0;   //微秒最好也设置好
    
   int ret = setitimer(ITIMER_REAL,&new_value,NULL);    //非阻塞执行
   printf("定时器开始。。\n");
   if(ret==-1)
   {
       perror("setitimer");
       exit(0);
   }

   //防止程序进程直接结束，等待 定时器结束函数
   getchar();

    return 0;
}