//TCP 通信的客户端
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    //创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1)
    {
        perror("socket");
        exit(-1);
    }

    //连接服务器端
    struct sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    //inet_pton(AF_INET," 172.19.242.113",&serveraddr.sin_addr.s_addr);
    inet_pton(AF_INET,"47.102.139.38",&serveraddr.sin_addr.s_addr); //IP地址
    serveraddr.sin_port=htons(9999);    //端口号
    int ret = connect(fd,(struct sockaddr* )&serveraddr,sizeof(serveraddr));   
    if(ret==-1)
    {
        perror("connect");
        exit(-1);
    }

    //通信
    char recvBuf[1024]={0};
    while (1)
    {
        char * data="Hello I am client";
        //给客户端 发送数据
        write(fd,data,strlen(data));
        sleep(1);

        //读取服务端发送的数据
        //从 cfd 中读取数据，放入 recvBuf 中
        int len = read(fd,recvBuf,sizeof(recvBuf));
        if(len==-1)
        {
            perror("read");
            exit(-1);
        }
        else if(len>0)
        {
            //输出读到的数据
            printf("recv server data: %s\n",recvBuf);
        }
        else if(len==0) //表示服务端 断开连接
        {
            printf("server closed...\n");
            break;
        }
    }
    
    //关闭连接
    close(fd);

    return 0;
}