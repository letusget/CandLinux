#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {

    // 创建socket
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in seraddr;
    inet_pton(AF_INET, "172.19.242.113", &seraddr.sin_addr.s_addr);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(9999);

    // 连接服务器
    int ret = connect(fd, (struct sockaddr *)&seraddr, sizeof(seraddr));

    if(ret == -1){
        perror("connect");
        return -1;
    }

    int num=0;

    while(1) {
        char sendBuf[1024] = {0};
       
       //发送 sendBuf ，内容为 send data num
        //sprintf(sendBuf,"send data %d",num++);
        
        //通过键盘录入，发送数据
        fgets(sendBuf,sizeof(sendBuf),stdin);

        write(fd, sendBuf, strlen(sendBuf) + 1);

        // 接收
        int len = read(fd, sendBuf, sizeof(sendBuf));
        if(len == -1) {
            perror("read");
            return -1;
        }
        else if(len > 0) 
        {
            printf("read buf = %s\n", sendBuf);
        } else 
        {
            printf("服务器已经断开连接...\n");
            break;
        }

        //防止发送的过快，效果不明显
        //sleep(1); //由于是发送手动输入的数据，所以就不需要睡眠了
    }

    close(fd);

    return 0;
}
