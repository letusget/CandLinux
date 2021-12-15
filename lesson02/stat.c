/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    int stat(const char *pathname, struct stat *statbuf);
        作用：获取一个文件相关的一些信息
        参数:
            - pathname：操作的文件的路径
            - statbuf：结构体变量，传出参数，用于保存获取到的文件的信息
        返回值：
            成功：返回0
            失败：返回-1 设置errno

    int lstat(const char *pathname, struct stat *statbuf);
        参数:
            - pathname：操作的文件的路径
            - statbuf：结构体变量，传出参数，用于保存获取到的文件的信息
        返回值：
            成功：返回0
            失败：返回-1 设置errno

*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    //创建文件
    struct stat statbuf;

    //传地址，保存该文件信息
    int ret = stat("a.txt",&statbuf);

    //如果打开失败，就打印错误信息
    if(ret==-1)
    {
        perror("stat");
        return -1;
    }

    //输出文件大小
    printf("size: %ld\n",statbuf.st_size);

    return 0;
}