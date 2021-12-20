/*
#include <sys/time.h>
int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value);
    功能：设置定时器，可以替代 alarm 函数，精度比alarm高，精度到 微秒，可以实现周期性调用定时器
    参数：
        which: 定时器以什么时间计时
            ITIMER_REAL：以真实时间计时，就是不管程序怎样执行，程序员感觉过了一秒钟，就是过了一秒钟，发送 SIGALARM信号，比较常用
            ITIMER_VIRTUAL： 用户时间，计算在应用层递减的时间不计算在内核层的递减的时间，如果时间到达，发送 SIGVTLRM
            ITIMER_PFOF：以该进程在用户态和内核态下所有消耗的时间来计算， 递减时间更为精确 会减去在系统中阻塞的时间，视角到达，发送 SIGPROF
        new_value: 设置定时器的属性
        //定时器结构体
        struct itimerval {
               struct timeval it_interval; //每个阶段的时间，就是定时器之间的间隔时间
               struct timeval it_value;    //延迟多长时间执行定时器
           };
        //时间结构体
        struct timeval {
            time_t      tv_sec;         //秒数
            suseconds_t tv_usec;        //微秒
        };
        old_value: 上一次的定时器的时间参数，一般用不着的话就是 NULL
    返回值：
        成功 返回0
        失败 返回-1，并设置错误号
*/
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
//目的：三秒钟后，每隔两秒 输出一个定时
int main()
{
    //该函数需要的结构体
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