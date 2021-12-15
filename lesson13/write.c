#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


//向管道中写数据
int main()
{
    int ret = access("test",F_OK);
    
    if(ret==-1)
    {
        printf("管道不存在，创建管道\n");

        //创建管道文件
        ret = mkfifo("test",0664);
        if(ret==-1)
        {
            perror("mkfifo");
            exit(0);
        }
    }

    //以只写方式 打开管道，为了遵循在写时关闭读，读时关闭写，最好不要选择 以读写方式打开管道文件
    int fd=open("test",O_WRONLY);
    if(fd==-1)
    {
        //打开 失败
        perror("open");
        exit(0);
    }

    //写数据
    for(int i=0;i<100;i++)
    {
        char buf[1024];
        //向 buf 中写入数据
        sprintf(buf,"hello %d\n",i);
        printf("write data : %s\n",buf);

        write(fd,buf,strlen(buf));
        sleep(1);
    }
    //fd.close();
    close(fd);

    return 0;
}