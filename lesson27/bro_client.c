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

    struct in_addr in;

    //客户端绑定本地的 ip 和端口
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

    //通信,因为是广播通信，只用接受 服务器信息即可
    while (1)
    {
        char buf[128];
        //接收数据
        int num = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
        if(num==-1)
        {
            perror("recvfrom");
            exit(-1);
        }

        //接收到的数据
        printf("client say : %s\n",buf);
    }

    //关闭文件描述符
    close(fd);    
    
    return 0;
}
