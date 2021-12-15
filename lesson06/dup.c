/*
#include <unistd.h>
int dup(int oldfd);
    作用：复制一个新的文件描述符
    fd=3, int fd1 = dup(fd),
    fd指向的是a.txt, fd1也是指向a.txt
    从空闲的文件描述符表中找一个最小的，作为新的拷贝的文件描述符
*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
    //以读写方法打开文件
    int fd = open("a.txt",O_RDWR|O_CREAT,0664);

    //复制文件描述符
    int fd1=dup(fd);
    if(fd1==-1)
    {
        perror("dup");
        return -1;
    }
    //输出文件描述符位置
    printf("fd : %d , fd1 : %d \n",fd,fd1);
    //关闭文件
    close(fd);

    char * str="hello world";
    //通过复制的文件描述符，向文件中写入信息
    int ret=write(fd1,str,strlen(str));
    if(ret==-1)
    {
        perror("write");
        return -1;
    }
    else
    {
        printf("write success!\n");
    }


    return 0;
}