/*
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
    功能：将自定义信号集中的数据设置到内核中，（设置阻塞，接触阻塞，替换）
    参数：
        how：如何对内核阻塞信号集进行处理
          SIG_BLOCK: 将用户设置的阻塞信号添加到内核中，内核中原来的数据不变
              假设内核中默认的阻塞信号集为mask,该函数进行的操作就是：mask | set（按位或），进而实现替换
          SIG_UNBLOCK: 根据用户设置的数据，对内核中的数据进行接触阻塞
              mask &= ~set(mask 赋值为 mask和 set的取反值 按位与)
          SIG_SETMASK: 覆盖内核中原来的值
        set: 已经初始化好的用户自定义的信号集
        oldset: 保存设置之前的内核中的信号集的状态，可以是NULL，使用很少
    返回值：
        成功：返回0
        失败：返回 -1，并设置错误号
          EFAULT：表示 set 或 oldset 指向了错误的地址
          EINVAL：how 参数指定非法
int sigpending(sigset_t *set);
    功能：获取系统内核中的未决信号
    参数：set，传出参数,保存的是内核中的未决信号集中的信息

*/

//编写一个程序 把所有的常规信号（1-31） 的未决状态打印到屏幕
//设置某些信号时阻塞的，通过键盘产生这些信号
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    //设置2 号信号 和 3号信号阻塞
    sigset_t set;
    sigemptyset(&set);

    //将2 和 3 号信号添加到信号集
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);

    //修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK,&set,NULL);

    //
    int num=0;

    //一直执行打印
    while (1)
    {
        num++;
        /* 获取当前信号的未决信号集的数据 */
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        //遍历前32位
        for(int i=1;i<32;i++)
        {
            if(sigismember(&pendingset,i)==1)
            {
                //阻塞
                printf("1");

            }
            else if(sigismember(&pendingset,i)==0)
            {
                //未阻塞
                printf("0");
            }
            else
            {
                perror("sigismember\n");
                exit(0);
            }
        }
        
        printf("\n=====================\n");
        sleep(1);

        //解除阻塞
        if(num==10)
        {
            //设置非阻塞，就可以使用 Ctrl+C  结束程序了，如果之前有阻塞的 Ctrl+C 信号，就会直接执行
            sigprocmask(SIG_UNBLOCK,&set,NULL);
        }
    }
    

    return 0;
}