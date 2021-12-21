/*
守护进程：每隔2s，获取系统时间，将这个时间写入到磁盘文件中
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <string.h>

void work(int num)
{
    //捕捉到信号之后，获取系统时间，写入磁盘文件
    time_t tm=time(NULL);
    //将time_t 类型的时间，转化为本地时间
    struct tm* loc = localtime(&tm);

    // char buf[1024]; //存储转换好的时间
    // sprintf(buf,"%d-%d-%d %d:%d:%d\n",loc->tm_year,loc->tm_mon,
    //         loc->tm_mday,loc->tm_hour,loc->tm_min,loc->tm_sec);
    // printf("%s\n",buf);  //如果要输出到终端，需要将终端的标准输出打开你

    //写入文件
    char* str = asctime(loc);
    int fd = open("time.txt",O_RDWR|O_CREAT|O_APPEND,0664);
    write(fd,str,strlen(str));
    close(fd);
}
int main()
{
    //创建子进程，退出父进程
    pid_t pid=fork();
    if(pid>0)
    {
        exit(0);
    }

    //使用子进程 重新创建一个会话
    setsid();

    //设置掩码
    umask(022);

    //更改工作目录，这里是放到了用户目录下
    chdir("/root/");

    //关闭 重定向文件描述符
    int fd=open("dev/null",O_RDWR);
    //将标准输入 标准输出 标准错误 重定向到 打开的文件中
    dup2(fd,STDIN_FILENO);
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);

    //业务逻辑
    //捕捉定时器信号
    struct sigaction act;
    act.sa_flags=0;
    act.sa_handler=work;    //处理的回调函数
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM,&act,NULL);
    //创建定时器
    struct itimerval val;
    val.it_value.tv_sec=2;  //定时2s
    val.it_value.tv_usec=0;
    val.it_interval.tv_sec=2;   //每隔2s 执行
    val.it_interval.tv_usec=0;
    setitimer(ITIMER_REAL,&val,NULL);

    while (1)
    {
        sleep(10);
    }
    

    return 0;
}