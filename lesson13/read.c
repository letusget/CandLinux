#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

//从管道中读数据
int main()
{
    //打开管道文件,以只读方式打开，为了读写一致
    int fd=open("test",O_RDONLY);
    if(fd==-1)
    {
        perror("open");
        exit(0);
    }

    //读数据
    while (1)
    {
       char buf[1024]={0};
       int len=read(fd,buf,sizeof(buf));
       if(len==0)
       {
           printf("写端断开连接。。。\n");
           exit(0);
       }
       printf("recv buf : %s\n",buf);
    }
    
    close(fd);
    

    return 0;
}