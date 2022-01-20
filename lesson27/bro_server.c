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

    //设置广播属性
    int op=1;
    setsockopt(fd,SOL_SOCKET,SO_BROADCAST,&op,sizeof(op));

    //创建广播地址
    struct sockaddr_in cliaddr;
    cliaddr.sin_family=AF_INET;    //使用ipv4
    cliaddr.sin_port=htons(9999);  //绑定9999 端口
    //广播的网络号
    inet_pton(AF_INET,"172.19.255.255",&cliaddr.sin_addr.s_addr);

    //通信
    int num=0;
    while (1)
    {
        char sendBuf[128];
        sprintf(sendBuf,"Hello client...%d\n",num++);

        //发送数据,使用过广播地址实现
        sendto(fd,sendBuf, strlen(sendBuf)+1, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

        //展示广播数据
        printf("广播的数据: %s\n",sendBuf);

        //防止过快
        sleep(1);
    }

    //关闭文件描述符
    close(fd);    
    
    return 0;
}
