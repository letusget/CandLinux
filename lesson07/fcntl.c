/*
#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd, ... );
    参数：
        fd : 表示需要操作的文件描述符
        cmd : 对文件描述符做 什么操作
            F_DUPFD ：复制文件描述符，复制的为第一个参数 fd,得到一个新的文件描述符
                    int ret = fcnt1(fd,F_DUPFD);
            F_GETFL : 获取指定的文件描述符状态 flag
                    获取的flag和通过open 函数传递的flag 为同一个
            F_SETFL ：设置文件描述符状态 flag
                必选项：O_RDONLY ,O_WRDOLY, O_RDWD
                O_APPEND 表示最佳数据，在原有文件后面继续写数据
                NONBLOK 设置为非阻塞
            阻塞和非阻塞：描述函数调用的行为，阻塞：调用某个函数，无法立刻得到其返回值，非阻塞：调用某个函数可以立刻得到其返回值

*/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main()
{
    //复制文件描述符
    //int fd1=open("1.txt",O_RDONLY);
    //使用fcntl 赋值文件描述符
   // int ret=fcntl(fd1,F_DUPFD);

    //修改 或 获取 文件状态 flag
    int fd=open("1.txt",O_RDWR);  //现在是以读写方式打开文件
    if(fd == -1) {
        perror("open");
        return -1;
    }
    //下面使用 fcntl修改文件描述符的 flag，加入 O_APPEND 标记
    //先 获取文件描述符 flag
    int flag=fcntl(fd,F_GETFL);
    //相当于 flag=flag|O_APPEND,将 O_APPEND 加入到 flag中
    flag|=O_APPEND;
    int ret = fcntl(fd,F_SETFL,flag);
    if(ret == -1) {
        perror("fcntl");
        return -1;
    }

    char * str = "nihao";
    write(fd, str, strlen(str));

    close(fd);

    return 0;
}
