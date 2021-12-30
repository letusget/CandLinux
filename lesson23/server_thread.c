#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct sockInfo
{
    //通信文件描述符
    int fd;

    //客户端信息
    struct sockaddr_in addr;

    //线程号
    pthread_t tid;
};

//同时连接的客户端的数目
struct sockInfo sockinfos[128];

void *working(void *arg)
{
    //子线程 和 客户端通信

    //类型转换，将 arg 转换为 struct类型
    struct sockInfo *pinfo = (struct sockInfo *)arg;

    //获取客户端信息
    char cliIp[16];
    //网络字节序 转换为 点分十进制 主机字节序
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, cliIp, sizeof(cliIp));
    //端口号转换
    unsigned short cliPort = ntohs(pinfo->addr.sin_port);
    printf("client ip is: %s, port is %d\n", cliIp, cliPort);


    //接受客户端 发送的数据
    char recvBuf[1024];
    //char recvBuf[1024]={0};
    while (1)
    {
        int lens = read(pinfo->fd, &recvBuf, sizeof(recvBuf));
        if (lens == -1)
        {
            perror("read");
            exit(-1);
        }
        else if (lens > 0)
        {
            printf("recv client data: %s\n", recvBuf);
        }
        else if(lens == 0)
        {
            printf("client closed...\n");
            //客户端关闭了就结束这次循环
            break;
        }

        //服务器在像客户端写入数据，这里就将 客户端发送的数据再次写入
        //如果recvBuf没有初始化，就需要将发送的数据多加一个，带上最后的\0 strlen(recvBuf)+1
        //即使初始化了，也要长度+1，就是为了防止发送的长度不一样时最后的\0 没有被覆盖
        write(pinfo->fd, recvBuf, strlen(recvBuf) + 1);
    }

    //通信完成后，关闭文件描述符
    close(pinfo->fd);

    return NULL;
}
int main()
{
    //创建socket 套接字通信
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(0);
    }
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY; //接受任何请求

    //绑定
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        perror("bind");
        exit(-1);
    }

    //监听,这里设置最大连接数为128
    ret = listen(lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        exit(-1);
    }

    //初始化 连接的参数数据
    int max = sizeof(sockinfos) / sizeof(sockinfos[0]); // 获取长度
    for (int i = 0; i < max; i++)
    {
        //对每个结构体都初始
        bzero(&sockinfos[i], sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;  //-1 表示无效的文件描述符
        sockinfos[i].tid = -1; //无效的线程号
    }

    //循环等待 客户端连接，一旦有客户端连接，就创建一个子线程 进行通信
    while (1)
    {
        //接受的客户端数据
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        //接受连接
        int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

        //使用结构体传参  这里创建结构体指针
        struct sockInfo *pinfo;
        //由于cliaddr 为结构体，所以这里要分别将结构体中的成员赋值，或者可以使用 memor copy 也可以
        for (int i = 0; i < max; i++)
        {
            //从数组中 找到一个可用的 sockInfo 元素
            if (sockinfos[i].fd == -1)
            {
                //赋值这个结构体
                pinfo = &sockinfos[i];
                break;
            }
            if (i == max - 1)
            {
                //如果恰好到使用最后一个连接，这时就让其睡眠，直到有空余的
                sleep(1);
                i--;
            }
        }

        pinfo->fd = cfd;
        //使用内存拷贝实现
        memcpy(&pinfo->addr, &cliaddr, len);

        //创建子线程
        //pthread_t tid;

        //要使用 working 函数 来实现多线程通信，但是第四个参数只有一个指针，所以这里使用一个结构体来实现传参
        //这里在创建子线程的时候，直接就传参到 pinfo.tid 中
        pthread_create(&pinfo->tid, NULL, working, pinfo);

        //设置线程分离回收线程资源
        pthread_detach(pinfo->tid);
    }

    //关闭文件描述符
    close(lfd);

    return 0;
}