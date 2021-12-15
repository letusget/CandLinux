/*
匿名映射: 不需要文件实体进行内存映射
但是不能对任意进程实现映射，仅能对 有关系的进程实现内存映射，比如父子进程之间的内存映射
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    //1. 创建匿名内存映射区
    int len=4096;
    // PROT_READ|PROT_WRITE 表示读写操作，MAP_SHARED|MAP_ANONYMOUS 表示如果对文件操作了，会有更改，且使用匿名映射
    void * ptr = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    if(ptr==MAP_FAILED)
    {
        perror("mmap");
        exit(0);
    }

    //父子进程间通信
    pid_t pid=fork();

    if(pid>0)
    {
        //父进程
        strcpy((char* )ptr,"hello, world!\n");
        //等待回收子进程资源
        wait(NULL);
    }
    else if(pid==0)
    {
        //子进程,等待父进程将数据写入内存映射区后再执行
        sleep(1);
        printf("%s\n",(char *) ptr);
    }

    //释放内存映射区资源
    int ret=munmap(ptr,len);
    if(ret==-1)
    {
        perror("munmap");
        exit(0);
    }



    return 0;
}
