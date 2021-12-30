#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main()
{
    //创建socket 套接字通信
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    if(lfd==-1)
    {
        perror("socket");
        exit(0);

    }
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(9999);
    saddr.sin_addr.s_addr=INADDR_ANY;   //接受任何请求

    //绑定
    int ret = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(ret==-1)
    {
        perror("bind");
        exit(-1);
    }

    //监听,这里设置最大连接数为128
    ret=listen(lfd,128);
    if(ret==-1)
    {
        perror("listen");
        exit(-1);
    }

    //循环等待 客户端连接，一旦有客户端连接，就创建一个子线程 进行通信
    while (1)
    {
        
        
    }
    


    return 0;
}