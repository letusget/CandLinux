//使用内存映射实现文件拷贝功能
/*
思路：
    1. 对原始文件进行内存映射
    2. 创建一个新文件(拓展该文件)
    3. 把新文件的数据映射到内存中
    4. 通过内存拷贝将第一个文件的内存数据拷贝到新的文件的内存中
    5. 释放资源
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    //1. 对原始文件进行内存映射
    int fd=open("english.txt",O_RDWR);
    if(fd==-1)
    {
        perror("open");
        exit(0);
    }
    
    //获取原始文件的大小,表示从末尾 偏移 0
    int len = lseek(fd,0,SEEK_END);

    //2. 创建一个新文件(拓展该文件)
    int fd1=open("cpy.txt",O_RDWR|O_CREAT,0664);
    if(fd1==-1)
    {
        perror("open1");
        exit(0);
    }

    //对新创建的文件 进行拓展
    truncate("cpy.txt",len);
    write(fd1," ",1);   //写入一个空格，否则不生效

    //3. 把新文件的数据映射到内存中
    void * ptr = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    void * ptr1 = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0);

    if(ptr==MAP_FAILED)
    {
        perror("mmap");
        exit(0);
    }
    if(ptr1==MAP_FAILED)
    {
        perror("mmap");
        exit(0);
    }

    //内存拷贝 参数：目的地址 源地址  拷贝大小
    memcpy(ptr1,ptr,len);

    //释放资源
    munmap(ptr1,len);   //后打开的先释放，先打开的后释放
    munmap(ptr,len);

    //关闭资源
    close(fd1);
    close(fd);

    return 0;
}