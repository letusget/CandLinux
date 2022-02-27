// #include <unistd.h>
// #include <sys/types.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <signal.h>

// pid_t pid;

// void myKill(int num)
// {
//     //杀死子进程
//     for(int i=0;i<2;i++)
//     {
//         kill(pid,SIGINT);
//     }
//     printf("信号编号: %d\n",num);
// }

// void myPrint(int num)
// {
//     printf("kill child OK\n");
//     //wait(NULL);
//     exit(1);
// }

// // void beKill(int num)
// // {

// // }
// int main()
// {
//     //定时3S
//     alarm(3);

    
//     for(int i=0;i<2;i++)
//     {
//         pid=fork();

//         //防止重复创建子进程
//         if(pid==0)
//         {
//             break;
//         }
//     }


//     if(pid>0)
//     {
//         printf("parent process: %d\n",getpid());
//         sleep(1);

//         //捕捉中断信号，杀死子进程
//         signal(SIGALRM,myKill);
//         //捕捉子进程被杀死
//         //signal(SIGCHLD,myPrint);
//         struct sigaction act;
//         act.sa_flags=0;
//         act.sa_handler=myPrint;
//         sigemptyset(&act.sa_mask);
//         sigaction(SIGCHLD,&act,NULL);


//         printf("\nend\n");
//     }
//     else if(pid==0)
//     {
//         printf("child process: %d\n",getpid());
//         //signal(SIGKILL,beKill);
//         printf("child be killed!\n");
//     }



//     return 0;
// }

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

