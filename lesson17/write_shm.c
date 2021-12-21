/*
#include <sys/ipc.h>
#include <sys/shm.h>
int shmget(key_t key, size_t size, int shmflg);
    功能：创建一个新的共享内存段，或者获取一个既有的内存段标识
        新创建的内存段中的数据会被初始为0
    参数：
        key: key_t 类型是一个整型，通过这个找到或创建一个共享内存
            一般是使用 16进制表示，并且为非零值,即使是其他进制，在使用时也会被转换为16进制
        size：共享内存的大小
        shmflg：共享内存的属性
            访问权限：与文件访问权限一样，使用八进制表示
            附加属性：创建/判断 共享内存是不是存在等
                创建：IPC_CREAT
                判断共享内存是否存在：IPC_EXCL，需要和 IPC_CREAT 一起使用
    返回值：
        如果成功：返回 >0 的数字，即共享内存的引用ID，后面操作共享内存都是通过这个值
        如果失败：返回-1，并设置错误号

#include <sys/types.h>
#include <sys/shm.h>
void *shmat(int shmid, const void *shmaddr, int shmflg);
    功能：和当前的进程进行关联
    参数：
        shmid: 共享内存的标识（ID），由 shmget 返回值获得
        shmaddr: 申请的共享内存的起始地址，一般指定为NULL，表示由内核指定
        shmflg: 对共享内存的操作
            读：SHM_RDONLY, 必须要有读权限
            读写：0
    返回值：
        成功：返回共享内存的首地址
        失败：返回 (void*) -1

void *shmat(int shmid, const void *shmaddr, int shmflg);
    功能：解除当前进程和共享内存的关联
    参数：
        shmaddr: 共享内存的首地址
    返回值：成功返回0，失败返回-1

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
    功能：对共享内存进行操作，共享内存要删除才会消失，创建共享内存的进程被销毁了，对共享内存没有影响
    参数：
        shmid: 共享内存的ID
        cmd：要进行的操作
            IPC_STAT: 获取共享内存的当前状态
            IPC_SET: 设置共享内存的状态
            IPC_RMID: 标记共享内存需要被销毁
        buf: 结构体指针，表示各个参数的属性信息
            IPC_STAT: buf存储数据，表示一个传出参数
            IPC_SET: buf中需要初始化数据，设置到内核中
            IPC_RMID: 没有用，为NULL

#include <sys/types.h>
#include <sys/ipc.h>
key_t ftok(const char *pathname, int proj_id);
    功能：根据指定的路径名和 int 值，生成一个共享内存 key
    参数：
        pathname: 指定一个存在的路径，eg：/root
        proj_id: int  类型的值，但是系统调用只会使用其中的1个字节
            范围：0-255 


*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

int main()
{
    //创建共享内存
    int shmid = shmget(100,4096,IPC_CREAT|0664);
    printf("shmid : %d\n",shmid);

    //和当前进程进行关联
    void* ptr = shmat(shmid,NULL,0);

    char* str="hello world!";
    //写共享内存
    memcpy(ptr,str,strlen(str)+1);  //包含结束符 \0

    printf("按任意键继续\n");
    getchar();

    //解除关联
    shmdt(ptr);

    //删除内存
    shmctl(shmid,IPC_RMID,NULL);



    return 0;
}
