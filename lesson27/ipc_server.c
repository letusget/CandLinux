#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main()
{
    //删除通信文件
    unlink("server.sock");

    //创建套接字
    int lfd = socket(AF_LOCAL,SOCK_STREAM,0);

    if(lfd==-1)
    {
        perror("socket");
        exit(-1);
    }

    //绑定本地套接字
    struct sockaddr_un addr;
    addr.sun_family=AF_LOCAL;
    //使用内存拷贝实现
    strcpy(addr.sun_path,"server.sock");
    //绑定
    int ret = bind(lfd,(struct sockaddr* )&addr,sizeof(addr));
    if (ret ==-1)
    {
        perror("bind");
        exit(-1);
    }

    //监听
    ret=listen(lfd,100);
    if (ret==-1)
    {
        perror("listen");
        exit(-1);
        /* code */
    }

    //等待客户端连接
    struct sockaddr_un cliaddr;
    int len=sizeof(cliaddr);
    int cfd = accept(lfd,(struct sockaddr*)&cliaddr,&len);
    if(cfd==-1)
    {
        perror("accept");
        exit(-1);
    }
    
    //有客户端连接, 输出客户端信息
    printf("client socket filename: %s\n",cliaddr.sun_path);
    

    //进行通信
    while (1)
    {
        char buf[128];
        int len=recv(cfd,buf,sizeof(buf),0);

        if(len==-1)
        {
            perror("recv");
            exit(-1);
        }
        else if(len == 0)
        {
            printf("client closed...");
            break;
        }
        else if(len>0)
        {
            printf("client say: %s\n",buf);
            send(cfd,buf,len,0);
        }
        
    }
    
    close(cfd);
    close(lfd);

    return 0;
}