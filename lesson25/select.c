#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

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

    //创建一个 fd_set 的集合，
    fd_set rdset,tmp;
    FD_ZERO(&rdset);
    FD_SET(lfd,&rdset);
    int maxfd=lfd;  //要检测的文件描述符的最大值

    while (1)
    {
        //为了防止内核将 rdset 中的数据全部修改，下次使用时就是 内核修改完的数据了，这里使用tmp 来保存该数据
        //使内核修改tmp，而一直保留rdset 
        tmp=rdset;
        int ret = select(maxfd+1,&tmp,NULL,NULL,NULL);

        //如果打开失败，就直接退出
        if(ret==-1)
        {
            perror("select");
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
            if(FD_ISSET(lfd,&tmp))
            {
                //如果有新的客户端连接进来
                struct sockaddr_in cliaddr;
                int len=sizeof(cliaddr);
                int cfd = accept(lfd,(struct sockaddr* )&cliaddr,&len); //返回通信的文件描述符

                // 输出客户端的信息
                //printf("client's ip is %s, and port is %d\n", cliaddr.sin_addr,cliaddr.sin_port);

                //将新的文件描述符 放入集合中
                FD_SET(cfd,&rdset);
                //更新最大的文件描述符
                maxfd=maxfd>cfd?maxfd:cfd;

            }

            //遍历寻找，是否有其他的文件描述符变化
            for (int i = lfd+1; i <= maxfd; i++)
            {
                if(FD_ISSET(i,&tmp))
                {
                    //说明这个文件描述符对应的客户端发送了数据
                    char buf[1024]={0};
                    int len=read(i,buf,sizeof(buf));

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
                        FD_CLR(i,&rdset);
                    }
                    else if(len>0)
                    {
                        printf("read buf = %s\n",buf);
                        //回写内容,将 客户端发送的数据，再次返回给客户端
                        write(i,buf,strlen(buf)+1);
                    }
                }
               
            }
            

        }

    }

    //关闭文件描述符
    close(lfd);
    


    return 0;
}