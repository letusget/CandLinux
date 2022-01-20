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

    //服务器 地址信息
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;    //使用ipv4
    saddr.sin_port=htons(9999);  //绑定9999 端口
    inet_pton(AF_INET, "172.19.242.113", &saddr.sin_addr.s_addr);    //绑定所有服务器地址

    int num=0;
    //通信
    while (1)
    {
        char sendBuf[128];
        sprintf(sendBuf,"Hello, I am client %d\n",num++);

        //发送数据
        sendto(fd,sendBuf,strlen(sendBuf)+1,0,(struct sockaddr *)&saddr,sizeof(saddr));

        //接收数据,不需要保留服务器端地址了
        int num = recvfrom(fd,sendBuf,sizeof(sendBuf),0,NULL,NULL);
        if(num==-1)
        {
            perror("recvfrom");
            exit(-1);
        }
        //打印客户端信息 要注意 将网络号 转换为字节号
        printf("server say : %s\n",sendBuf);

        sleep(1);
    }

    //关闭文件描述符
    close(fd);    
    
    return 0;
}
