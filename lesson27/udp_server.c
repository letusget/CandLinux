#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    //创建一个通信的socket
    int fd = socket(PF_INET,SOCK_DGRAM,0);//使用 ipv4 以数据报 udp协议 创建socket

    if(fd==-1)
    {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;    //使用ipv4
    addr.sin_port=htons(9999);  //绑定9999 端口
    addr.sin_addr.s_addr=INADDR_ANY;    //绑定所有ip地址

    //绑定
    int ret = bind(fd,(struct sockaddr *)&addr,sizeof(addr));
    if(ret==-1)
    {
        perror("bind");
        exit(-1);
    }

    //通信
    while (1)
    {
        char recvbuf[128];
        char ipbuf[16]; //存储IP 字符串
        struct sockaddr_in cliaddr;
        int len=sizeof(cliaddr);
        //接收数据
        int num = recvfrom(fd,recvbuf,sizeof(recvbuf),0,(struct sockaddr *)&cliaddr,&len);
        if(num==-1)
        {
            perror("recvfrom");
            exit(-1);
        }
        //打印客户端信息 要注意 将网络号 转换为字节号
        printf("client IP : %s, Port is : %d\n",
            inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,ipbuf,sizeof(ipbuf)),
            ntohs(cliaddr.sin_port));

        //接收到的数据
        printf("client say : %s\n",recvbuf);

        //发送数据
        sendto(fd,recvbuf,strlen(recvbuf)+1,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
    }

    //关闭文件描述符
    close(fd);    
    
    return 0;
}
