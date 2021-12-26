//TCP 通信的服务器端

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    //创建套接字socket,用于监听的套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd==-1)
    {
        perror("socket");
        exit(-1);
    }

    //绑定 端口号和IP地址
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    //将主机字节序 转换为 网络字节序
    //inet_pton(AF_INET,"47.102.139.38",saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr=0;    //表示任意地址，相当于 0.0.0.0,只有服务器端可以这样偷懒写，客户端不可以
    saddr.sin_port=htons(9999);    //端口号也要转换为 网络字节序
    int ret = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(ret==-1)
    {
        perror("bind");
        exit(-1);
    }

    //监听
    ret=listen(lfd,8);
    if(ret==-1)
    {
        perror("listen");
        exit(-1);
    }

    //接收客户端连接
    struct sockaddr_in clientaddr;
    socklen_t lens=sizeof(clientaddr);
    int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&lens);
    if(cfd==-1)
    {
        perror("accept");
        exit(-1);
    }

    //输出客户端信息
    char clientIP[16];
    //要转换为 主机字节序
    inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,clientIP,sizeof(clientIP));   //IP 信息
    unsigned short clientPort=ntohs(clientaddr.sin_port);
    printf("client ip is %s, port is %d\n",clientIP,clientPort);

    //获取客户端数据
    char recvBuf[1024]={0};
    while (1)
    {
        //从 cfd 中读取数据，放入 recvBuf 中
        int len = read(cfd,recvBuf,sizeof(recvBuf));
        if(len==-1)
        {
            perror("read");
            exit(-1);
        }
        else if(len>0)
        {
            //输出读到的数据
            printf("recv client data: %s\n",recvBuf);
        }
        else if(len==0) //表示客户端 断开连接
        {
            printf("client closed...\n");
            break;
        }
        //给客户端发送数据
        char * data="hello I am server";
        write(cfd,data,strlen(data));   
        
    }


    //关闭文件描述符
    close(cfd);
    close(lfd);

    return 0;
}