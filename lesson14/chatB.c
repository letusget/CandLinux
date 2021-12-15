#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    //1. 判断有名管道文件是否存在
    int ret=access("fifo1",F_OK);
    if (ret==-1)
    {
        //文件不存在
        printf("管道不存在，创建对应的管道\n");
        ret = mkfifo("fifo1",0664);
        if(ret==-1)
        {
            perror("mkfifo");
            exit(0);
        }
    }

    ret=access("fifo2",F_OK);
    if (ret==-1)
    {
        //文件不存在
        printf("管道不存在，创建对应的管道\n");
        ret = mkfifo("fifo2",0664);
        if(ret==-1)
        {
            perror("mkfifo");
            exit(0);
        }
    }

    //2. 以只读方式打开 fofo1
    int fdr = open("fifo1",O_RDONLY);
    if(fdr==-1)
    {
        //打开文件失败
        perror("open");
        exit(0);
    }
    printf("打开管道fifo1 成功，等待读入...\n");
    
    //3. 以只写方式打开 fifo2
    int fdw = open("fifo2",O_WRONLY);
    if(fdw==-1)
    {
        //打开文件失败
        perror("open");
        exit(0);
    }
    printf("打开管道fifo2 成功，等待写入...\n");

    char buf[128];

    //循环读写数据
    while (1)
    {
        //先将要写入的字符数组的内容清空
        memset(buf,0,128);
        
        //4. 读管道数据
        memset(buf,0,128);
        ret = read(fdr,buf,128);
        if(ret<=0)
        {
            perror("read");
            exit(0);
        }
        printf("buf : %s \n",buf);

        //5. 写管道数据
        //先将要写入的字符数组的内容清空
        memset(buf,0,128);
        //获取标准输入的数据
        fgets(buf,128,stdin);
        ret = write(fdw,buf,strlen(buf));
        if(ret==-1)
        {
            perror("write");
            exit(0);
        }

        //6. 关闭文件描述符
        close(fdw);
        close(fdr);

    }
    
    

    return 0;
}