/*
#include <stdio.h>
void exit(int status);

#include <unistd.h>
void _exit(int status);

    statu参数：是进程退出时的一个状态信息，父进程回收子进程资源的时候可以获取到
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("hello \n");
    printf("world");

//这里退出了进程，下面的 return 0，就不再执行了
   // exit(0);
   _exit(0);

//这里调用的 return 0 与 exit(0) 是一样的，都表示进程退出
    return 0;
}