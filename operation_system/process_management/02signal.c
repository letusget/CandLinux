#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

pid_t pid=1,pid1=1;  //测试pid数值传递

void fatherfun(int arg)
{
    //printf("pid1=%d pid2=%d\n",pid,pid1);
    kill(pid, SIGHUP);
    kill(pid1, SIGHUP);
}
void child1(int arg)
{
	printf("child1 process was killed by father process\n");
    exit(0);
}
void child2(int arg)
{
	printf("child2 process was killed by father process\n");
	exit(0);
}

int main()
{
    alarm(3);
    //1，注册捕捉信号SIGALRM
    signal(SIGALRM,fatherfun);
    
	pid = fork();
    if(pid < 0){
        perror("fork failed\n");
        return -1;
    }	
    if(pid > 0){
        

        printf("test father pid=%d\n",getpid());
        pid1 = fork();
        if(pid1 < 0){
            perror("fork failed\n");
            return -1;
        }	
        //父进程等待接收捕捉信号
        if(pid1 > 0){
            wait(NULL);
            wait(NULL);
            printf("father process exit\n");
            exit(0);
        }
        //子进程2等待接收父进程发送的信号
        if(pid1 == 0){
            pid1 = getpid();
            signal(SIGHUP, child2);
            while(1);
        }
    }
    //子进程1等待接收父进程发送的信号
    if(pid == 0){
        pid = getpid();
        signal(SIGHUP, child1);
        while(1);
    }
	return 0;
}
