#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <poll.h>


int main()
{
    //创建 socket
    int lfd=socket(PF_INET,SOCK_STREAM,0);
    //绑定的地址
    struct sockaddr_in saddr;
    saddr.sin_port=htons(9999); //绑定的端口
    saddr.sin_family=AF_INET;   //协议族：ipv4
    saddr.sin_addr.s_addr=INADDR_ANY;   //监听所有IP地址

    //绑定
    bind(lfd,(struct sockaddr* )&saddr,sizeof(saddr));

    //监听
    listen(lfd,8);

    //判断 socket 和 bing 的返回值是否正确

    //初始化检测的文件描述符数组
    struct pollfd fds[1024];    //该大小可以根据需要修改, 可以定义为一个宏值，方便修改
    //对每一个结构体中的成员进行初始化
    for(int i=0;i<1024;i++)
    {
        fds[i].fd=-1;   //初始时 文件描述符集合为无效
        fds[i].events = POLLIN;   //初始为检测读事件
    }
    //初始时的文件描述符集合
    fds[0].fd=lfd;
   
   //最后一个有效数组元素的下标
    int nfds=0;

    while (1)
    {
        //调用 poll 系统函数，让内核检测那些文件描述符有数据
        int ret = poll(fds,nfds+1,-1);    //如果没有数据就阻塞
        

        //如果打开失败，就直接退出
        if(ret==-1)
        {
            perror("poll");
            exit(-1);
        }
        else if(ret==0)
        {
            continue;
        }
        else if(ret>0)
        {
            //检测到文件描述符的对应缓冲区的数据发生了变化
            //返回发生变换的文件个数

            //下面判断
            if(fds[0].revents & POLLIN)
            {
                //如果有新的客户端连接进来
                struct sockaddr_in cliaddr;
                int len=sizeof(cliaddr);
                int cfd = accept(lfd,(struct sockaddr* )&cliaddr,&len); //返回通信的文件描述符

                // 输出客户端的信息
                //printf("client's ip is %s, and port is %d\n", cliaddr.sin_addr,cliaddr.sin_port);

                //将新的文件描述符 放入集合中
                for (int i = 1; i < 1024; i++)  //因为0 是已经监听的文件描述符了
                {
                    //判断时候有连接
                    if (fds[i].fd==-1)
                    {
                        fds[i].fd=cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }
                
                //更新最大的文件描述符
                nfds = nfds > cfd ? nfds : cfd ;

            }

            //遍历寻找，是否有其他的文件描述符变化
            for (int i = 1; i <= nfds; i++) //从第一个索引开始遍历
            {
                if(fds[i].revents & POLLIN)
                {
                    //说明这个文件描述符对应的客户端发送了数据
                    char buf[1024]={0};
                    int len=read(fds[i].fd,buf,sizeof(buf));

                    if(len==-1)
                    {
                        perror("read");
                        exit(-1);
                    }
                    else if(len==0)
                    {
                        //对方断开连接
                        printf("client closed...\n");

                        //关掉该文件
                        close(i);

                        //移除该文件描述符
                        fds[i].fd=-1;
                    }
                    else if(len>0)
                    {
                        printf("read buf = %s\n",buf);
                        //回写内容,将 客户端发送的数据，再次返回给客户端
                        write(fds[i].fd,buf,strlen(buf)+1);
                    }
                }
               
            }
            

        }

    }

    //关闭文件描述符
    close(lfd);
    


    return 0;
}