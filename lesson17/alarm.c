/*
#include <unistd.h>
unsigned int alarm(unsigned int seconds);
    功能：设置定时器（闹钟），其中的函数调用为开始倒计时，当倒计时为0 的时候，函数会给当前进程发送一个信号，SIGLARM
    参数：
        seconds: 倒计时的时长，单位是秒，如果参数小于0，定时器无效（不进行倒计时，不发送信号）
            所以可以通过定义 alarm(0) 来取消一个定时器
    返回值：无符号数，返回倒计时剩余的时间
        如果之前没有定时器，返回0，
        如果之前有定时器，返回之前的定时器剩余时间

    SIGALARM信号：如果程序收到该信号，则该程序默认终止当前进程，每个进程都有且只有唯一的一个定时器
        如果在一个程序的不同地方定义了两个定时器，则先定义的定时器失效，后定义的定时器生效

    alarm() 函数不阻塞，在执行 alarm函数的同时，也可以执行其他的函数
*/
#include <unistd.h>
#include <stdio.h>

int main()
{
    int seconds1 = alarm(5);
    printf("seconds1 = %d\n",seconds1);
    
    //定时器执行了两秒
    sleep(2);
    //上一个定时器失效，重新计时10秒，这个定时器的返回值是上个定时器失效时的剩余时间
    int seconds2=alarm(2);
    //应该返回上次定时器剩余的时间  3
    printf("seconds2 = %d\n",seconds2);

    //由于定时器是不阻塞的，为了防止执行 return 0 导致程序结束，所以这里放一个死循环
    while (1)
    {
        /* code */
    }
    

    return 0;
}