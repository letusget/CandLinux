#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/epoll.h>


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

    //调用 epll_create 创建一个epoll 实例
    int epfd = epoll_create(100);  //参数为大于0 的数字即可

    //需要将要监听的文件描述符信息添加到 epll 实例中
    struct epoll_event epev;
    epev.events=EPOLLIN;    //检测 EPOLLIN 事件
    epev.data.fd=lfd;   //要检测的文件描述符
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev);

    struct epoll_event epevs[1024];

    while (1)
    {
        //检测哪些有新数据了
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        //判断调用情况
        if(ret==-1)
        {
            perror("epoll_wait");
            exit(-1);
        }

        //打印检测到的信息
        printf("ret = %d\n",ret);

        for (int i = 0; i < ret; i++)
        {
            int  curfd = epevs[i].data.fd;
            if(curfd==lfd)
            {
                //监听的文件描述符有数据到达（有客户端连接）
                 //如果有新的客户端连接进来
                struct sockaddr_in cliaddr;
                int len=sizeof(cliaddr);
                int cfd = accept(lfd,(struct sockaddr* )&cliaddr,&len); //返回通信的文件描述符
                
                //检测这个新文件
                epev.events = EPOLLIN; //如果要同时检测读写事件发生
                epev.data.fd=cfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);

            }
            else
            {
                //如果是读数据，就继续读入下一个，这里暂时不做其他处理了
                //实际开发时要注意对各个事件进行不同的处理
                if(epevs[i].events & EPOLLOUT)
                {
                    continue;
                }
                
                //说明这个文件描述符对应的客户端发送了数据
                char buf[5]={0};    //一次只能读入5字节数据
                int len=read(curfd,buf,sizeof(buf));

                if(len==-1)
                {
                    perror("read");
                    exit(-1);
                }
                else if(len==0)
                {
                    //对方断开连接
                    printf("client closed...\n");

                    //在内核中清空该实例
                    epoll_ctl(epfd,EPOLL_CTL_DEL,curfd,NULL);

                    //关掉该文件
                    close(curfd);
                }
                else if(len>0)
                {
                    printf("read buf = %s\n",buf);
                    //回写内容,将 客户端发送的数据，再次返回给客户端
                    write(curfd,buf,strlen(buf)+1);
                }
            }
        }
        
    }
    //文件描述符使用完后要关闭
    close(lfd);
    close(epfd);
    return 0;
}