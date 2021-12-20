/*
#include <signal.h>
int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
    功能：检查或改变信号的处理，信号捕捉
    参数：
        signum: 需要捕捉的信号的编号或是宏值（最好是使用宏值，因为宏值在不同系统中必然相同，但编号就不一定了）
        act: 捕捉到信号之后响应的处理动作
        oldact: 上一次对信号捕捉后的相关设置，一般不使用，传递NULL
    返回值：
        成功返回0，失败发货-1
    
    struct sigaction 
    {
        //函数指针，指向的函数就是信号捕捉到后的处理函数（类似回调函数）
        void     (*sa_handler)(int);
        //不常用
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        //设置的临时阻塞信号集，在信号捕捉函数执行过程中，临时阻塞某些信号
        sigset_t   sa_mask;
        //使用哪一个信号处理对捕捉到的信号进行处理
        //如果是0，表示使用 sa_handler 处理，如果是 SA_SIGINFO 表示使用 sa_sigaction 进行处理
        int        sa_flags;
        //已废弃，一般指定NULL
        void     (*sa_restorer)(void);
    };
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
    //捕捉到信号之后响应的处理动作
    struct sigaction act;
    //使用sa_handler (回调函数)进行处理
    act.sa_flags=0;
    act.sa_handler=myalarm; 
    //清空临时信号值（可能出现的其他信号值）
    sigemptyset(&act.sa_mask);

    //注册信号捕捉
    sigaction(SIGALRM,&act,NULL);

    //setitimer函数需要的结构体
    struct itimerval new_value;
    //设置结构体中的值
    new_value.it_interval.tv_sec=2; //每隔两秒 定时一次
    new_value.it_interval.tv_usec=0;    //微秒如果不设置就会出现随机值

    //设置延迟的时间
    new_value.it_value.tv_sec=3;    //延迟三秒后执行
    new_value.it_value.tv_usec=0;   //微秒最好也设置好
    
    //以真实时间计算时间
   int ret = setitimer(ITIMER_REAL,&new_value,NULL);    //非阻塞执行
   printf("定时器开始。。\n");
   if(ret==-1)
   {
       perror("setitimer");
       exit(0);
   }

   //防止程序进程直接结束，等待 定时器结束函数
   //getchar();
   while (1);

    return 0;
}