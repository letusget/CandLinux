/*
#include <sys/ipc.h>
#include <sys/shm.h>
int shmget(key_t key, size_t size, int shmflg);
    功能：创建一个新的共享内存段，或者获取一个既有的内存段标识
        新创建的内存段中的数据会被初始为0
    参数：
        key: key_t 类型是一个整型，通过这个找到或创建一个共享内存
            一般是使用 16进制表示，并且为非零值
        size：共享内存的大小
        shmflg：共享内存的属性
            访问权限
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
        shmflg: 对内存的操作
*/