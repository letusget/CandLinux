#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

void recyleChild(int arg)
{
    //回收多个信号
    while (1)
    {
        //回收所有子进程，设置非阻塞
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret==-1)
        {
           //所有子进程回收完毕
           break;
        }
        else if(ret==0)
        {
            //还有子进程等待回收
            break;
        }
        else if(ret>0)
        {
            //被回收了
            printf("子进程 %d 被回收了\n",ret);
        }
        
    }
    
}

int main()
{
    //无法再 while循环中回收子进程，因为 wati是阻塞的，如果要回收子进程，那么就无法继续接受其他子进程创建了，即使是waitpid也不合适
    //所以这里使用信号 来解决进程回收的问题
    struct sigaction act;
    //使用第一个回调函数
    act.sa_flags=0;
    //清空临时阻塞的信号掩码
    sigemptyset(&act.sa_mask);
    //回调函数
    act.sa_handler=recyleChild;
    //注册信号捕捉
    sigaction(SIGCHLD,&act,NULL);    
    //创建socket 套接字通信
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    if(lfd==-1)
    {
        perror("socket");
        exit(0);

    }
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(9999);
    saddr.sin_addr.s_addr=INADDR_ANY;   //接受任何请求

    //绑定
    int ret = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(ret==-1)
    {
        perror("bind");
        exit(-1);
    }

    //监听,这里设置最大连接数为128
    ret=listen(lfd,128);
    if(ret==-1)
    {
        perror("listen");
        exit(-1);
    }

    //不断循环，接受客户端连接
    while (1)
    {
        //接受的客户端数据
        struct sockaddr_in cliaddr;
        int len=sizeof(cliaddr);
        //接受连接
        int cfd = accept(lfd,(struct sockaddr*)& cliaddr,&len);
        if(cfd==-1)
        {
            //如果连接断开了，没有这个if 判断就会导致直接退出该进程
            if(errno==EINTR)
            {
                //直接开始下一次循环
                continue;
            }
            perror("accept");
            exit(-1);
        }

        //没进来一个连接，就创建一个子进程 用于通信
        pid_t pid=fork();
        if(pid==0)
        {
            //子进程
            //获取客户端信息
            char cliIp[16];
            //网络字节序 转换为 点分十进制 主机字节序
            inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,cliIp,sizeof(cliIp));
            //端口号转换
            unsigned short cliPort=ntohs(cliaddr.sin_port);
            printf("client ip is: %s, port is %d\n",cliIp,cliPort);

            //接受客户端 发送的数据
            char recvBuf[1024];
            //char recvBuf[1024]={0};
            while (1)
            {
                int lens=read(cfd,&recvBuf,sizeof(recvBuf));
                if(lens==-1)
                {
                    perror("read");
                    exit(-1);
                }
                else if(len>0)
                {
                    printf("recv client data: %s\n",recvBuf);
                }
                else
                {
                    printf("client closed...\n");
                    //客户端关闭了就结束这次循环
                    break;
                }

                //服务器在像客户端写入数据，这里就将 客户端发送的数据再次写入
        //如果recvBuf没有初始化，就需要将发送的数据多加一个，带上最后的\0 strlen(recvBuf)+1
        //即使初始化了，也要长度+1，就是为了防止发送的长度不一样时最后的\0 没有被覆盖
                write(cfd,recvBuf,strlen(recvBuf)+1);

            }

            //通信完成后，关闭文件描述符
            close(cfd);
            //退出当前子进程
            exit(0);
        }
        else if(pid>0)
        {
            //父进程
            //如果使用 waitpid 的非阻塞模式来回收子进程资源就会导致父进程一直轮询回收子进程资源，就不能做自己的事情了
            //wait(NULL);
        }
        else
        {
            //创建进程失败
            perror("fork");
            exit(-1);
        }
    }
    
    //关闭监听文件
    close(lfd);

    return 0;
}