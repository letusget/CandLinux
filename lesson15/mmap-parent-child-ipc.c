/*
#include <sys/mman.h>
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
    功能：将一个文件或设备数据映射到内存中
    参数：
        void* addr: 映射内存的首地址，由内核 自动指定，一般传递 NULL
        length : 要映射的数据的长度，这个长度不可为0，建议使用要映射的文件的长度, 如果该文件的长度
                不够 系统分页的长度，则系统会自动填充，最终所有数据的长度均为 **系统分页长度** 的整数倍
            获取文件长度：stat() 或 lseek() 
        prot ：对申请的内存映射区的操作权限，
            PROT_EXEC : 可执行权限
            PROT_READ ：读权限，必须要有
            PROT_WRITE ：写权限
            PROT_NONE ：没有权限
            要操作映射内存区，就必须需要 读权限，如果只有写权限，没有读权限，也没法进行任何操作
            一般是用读权限 或 读写权限：PROT_READ|PROT_WRITE 
        flag : 权限标识位，必须要设置这个选项
            MAP_SHARED : 映射区的数据会自动和磁盘文件进行同步，进程间通信，
            MAP_PRIVATE : 不同步，内存映射区的数据改变了，对原来的文件不会修改，底层会重新创建一个新的文件(coyp on write)
        int fd : 需要映射的文件描述符
            通过 open 函数获得，open 是一个磁盘文件
            文件大小不能为0（内存不能指定大小为0），open()  指定的权限，不能和 prot 标记的权限有冲突。
            若 open() 有只读/读写 ,则 prot --> PROT_READ
            若 open() 有读写， 则 prot --> PROT_READ|PROT_WRITE 
        offset : 映射偏移量，一般不适用，必须指定的是 4K 的整数倍，0 表示不偏移
    返回值 ：
        成功返回映射内存的首地址
        失败返回 MAP_FAILED (宏定义),就是将 -1 类型转换为了指针，即： (void* ) -1
int munmap(void *addr, size_t length);
    功能：释放内存映射
    参数:
        addr : 要释放的内存首地址
        length : 要释放的内存大小，要 和 mmap()函数中的 length参数的值一样
*/

/*
使用内存映射实现 进程间通信
    有关系的进程（父子进程）
        还没有子进程的时候：通过唯一的父进程，先创建内存映射区
        有了内存映射区以后，创建子进程
        父子进程共享创建的内存映射区
    没有关系的进程间通信
        准备一个大小不是0 的磁盘空间
        进程1 通过磁盘文件创建内存映射区，得到一个 操作这块内存的指针
        进程2 通过磁盘文件创建内存映射区，得到一个 操作这块内存的指针
        使用内存映射区通信，属于 非阻塞状态        
*/
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    //打开文件,以 读写方式打开
    int fd = open("test.txt",O_RDWR);
    if(fd==-1)
    {
        perror("open");
        exit(0);
    }
    //获取文件大小
    int size = lseek(fd,0,SEEK_END);

    //创建内存映射区
    int *ptr = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr==MAP_FAILED)
    {
        //如果创建内存映射区失败
        perror("mmap");
        exit(0);
    }

    //创建子进程
    pid_t pid = fork();
    if(pid>0)
    {
        //回收子进程
        wait(NULL);

        //父进程,要将内存映射区的返回值，进行强制类型转换，转换成 char* ，方便存放 字符串
        char buf[64];
        strcpy(buf,(char* )ptr);
        printf("read data : %s\n",buf);
    }
    else if(pid==0)
    {
        //子进程
        strcpy((char* )ptr,"Hello! My Parent\n");
    }

    //关闭内存映射区
    munmap(ptr,size);

    

    return 0;
}