/*
#include <sys/types.h>
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *wstatus, int options);
    功能：回收指定进程号的子进程，可以设置是否阻塞
    参数：
        pid : 
            pid > 0 表示某个子进程的pid
            pid = 0 表示回收当前进程组的所有子进程
            pid = -1 表示回收所有的子进程，相当于 wait()  (最常用)
            pid < -1 表示回收任意一个进程组的组id,  
        *wstatus :
            与 wait() 函数的参数含义一致
        options :
            设置阻塞或是非阻塞
                0 ： 阻塞，相当于 wait()  函数的作用
                WNOHANG ：宏定义，非阻塞
    返回值：
        > 0 : 返回子进程的id
        = 0 ： options=WHONANG, 表示还有子进程或者其他的
        = -1 ： 错误或者没有子进程

*/

#include <sys/types.h>
#include <sys/wait.h>   //exit 函数需要的头文件
#include <unistd.h> //fork函数需要的头文件
#include <stdio.h>
#include <stdlib.h>


int main()
{
    //有一个父进程，创建 5个子进程，相当于兄弟进程
    pid_t pid;

    //创建5个子进程
    for(int i=0;i<5;i++)
    {
        //因为在执行的时候，父函数创建的子进程也会执行这个for循环，
        //就是说子进程也会创建子进程，就会出现 呈指数增长的进程数，但这并不是我们想要的结果
        pid=fork();
        //如果产生子进程，就结束这个循环，就可以防止这样重复的产生进程
        if(pid==0)
        {
            break;
        }

    }

    if(pid>0)
    {
        //父进程
        while (1)
        {
            sleep(1);
            printf("I am parent process, pid = %d , ppid = %d \n",getpid(),getppid());

            //int  ret=wait(NULL);
            int st=0;
            //int ret=wait(&st);
            //int ret=waitpid(-1,&st,0);
            //非阻塞，父进程不必挂起，可以继续执行 父进程代码，在子进程执行完毕之后，再回收子进程资源
            int ret = waitpid(-1,&st,WNOHANG);
            if(ret==-1)
            {
                break;
            }
            else if(ret==0)
            {
                //还有子进程存在
                continue;
            }
            //回收子进程
            else if(ret>0)
            {
                if(WIFEXITED(st))
                {
                    //判断是不是正常退出
                    printf("退出的状态码：%d\n",WEXITSTATUS(st));
                }
                if(WIFSIGNALED(st))
                {
                    //判断是不是异常终止
                    printf("被哪个型号干掉了： %d \n",WTERMSIG(st));
                }
                printf("child die,pid = %d \n",ret);
            }

        }
           
    }
    else if(pid==0)
    {
        //子进程
        //为了方便演示，这里加入while循环，手动杀死 子进程（结束子进程），看父进程的运行情况
        while (1)
        {
            printf("I am child process, pid = %d, ppid = %d \n",getpid(),getppid());
            sleep(1);
        }
        exit(0);
    }


    return 0;
}