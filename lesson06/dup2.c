/*
    #include <unistd.h>
    int dup2(int oldfd, int newfd);
        作用：重定向文件描述符
        oldfd 指向 a.txt, newfd 指向 b.txt
        调用函数成功后：newfd 和 b.txt 做close, newfd 指向了 a.txt
        oldfd 必须是一个有效的文件描述符
        oldfd和newfd值相同，相当于什么都没有做
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    int fd=open("1.txt",O_RDWR|O_CREAT,0664);
    if(fd==-1)
    {
        perror("open");
        return -1;
    }
    //打开两个文件
    int fd1 = open("2.txt", O_RDWR | O_CREAT, 0664);
    if(fd1 == -1) {
        perror("open");
        return -1;
    }
    printf("fd : %d, fd1 : %d\n", fd, fd1);

    //重定向 文件描述符,fd1 指向与fd相同的文件
    //fd2 指向的和 fd1 一致
    int fd2 = dup2(fd, fd1);
    if(fd2 == -1) {
        perror("dup2");
        return -1;
    }

    //通过 fd1 去写数据，实际操作的是 1.txt 而不是2.txt,因为其已经被重定向了
    char * str="hello dup2";
    //返回写入的长度
    int len=write(fd1,str,strlen(str));

    if(len==-1)
    {
        perror("write");
        return -1;
    }

    printf("fd : %d, fd1 : %d, fd2 : %d\n",fd,fd1,fd2);

    //fd2 不用关闭，因为其和fd1 是一样的
    close(fd);
    close(fd1);

    return 0;
}