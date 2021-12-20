/*
以下函数都是对自定义的 信号集 进行操作的
#include <signal.h>
int sigemptyset(sigset_t *set);
    功能：清空信号集中的数据，将信号集中所有的标志位 置为0（不阻塞）
    参数：set 传出参数，需要操作的信号集
    返回值：成功返回0，失败返回-1

int sigfillset(sigset_t *set);
    功能：将信号集中所有的标志位 置为1 （阻塞所有信号）
    参数：set 传出参数，需要操作的信号集
    返回值：成功返回0，失败返回-1

int sigaddset(sigset_t *set, int signum);
    功能：设置信号集中的某一个对应的标志位 为1，表示阻塞这个信号
    参数：
        set 传出参数，需要操作的信号集
        signum: 需要设置阻塞的那个信号
    返回值：成功返回0，失败返回-1

int sigdelset(sigset_t *set, int signum);
    功能：设置信号集中的某一个信号对应的标志位为0，表示不阻断这个信号
    参数：
        set 传出参数，需要操作的信号集
        signum: 需要设置的不阻塞的那个信号
    返回值：成功返回0，失败返回-1

int sigismember(const sigset_t *set, int signum);
    功能：判断某个信号 是否阻塞
    参数： 
        set：需要操作的信号集
        signum：需要判断的那个信号
    返回值：
        1：signum 被阻塞
        0：signum 不阻塞
        -1：调用函数失败

*/

#include <signal.h>
#include <stdio.h>

int main()
{
    //创建一个信号集
    sigset_t set;

    //由于新创建的信号集可能会有各种随机值，所以要先执行清空操作
    sigemptyset(&set);

    //判断 SIGINT 是否在该信号集中
    int ret = sigismember(&set,SIGINT);
    //不阻塞
    if(ret==0)
    {
        printf("SIGINT  不阻塞\n");
    }
    else if(ret==1)
    {
        printf("SIGINT  阻塞\n");
    }
    else
    {
        perror("siginmember调用失败，出现错误!\n");
    }
    printf("=================================\n");

    //添加几个信号阻塞到信号集中
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);

    //再次判断 SIGINT 是否在信号集中
    ret=sigismember(&set,SIGINT);
    //阻塞
    if(ret==0)
    {
        printf("SIGINT  不阻塞\n");
    }
    else if(ret==1)
    {
        printf("SIGINT  阻塞\n");
    }
    else
    {
        perror("siginmember调用失败，出现错误!\n");
    }
    printf("=================================\n");

    //从信号集中删除一个信号阻塞
    sigdelset(&set,SIGQUIT);
    ret=sigismember(&set,SIGQUIT);
    //不阻塞
    if(ret==0)
    {
        printf("SIGQUIT  不阻塞\n");
    }
    else if(ret==1)
    {
        printf("SIGQUIT  阻塞\n");
    }
    else
    {
        perror("siginmember调用失败，出现错误!\n");
    }
    printf("=================================\n");

    return 0 ;
}