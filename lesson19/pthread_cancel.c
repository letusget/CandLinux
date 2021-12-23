/*
#include <pthread.h>
int pthread_cancel(pthread_t thread);
    功能：取消线程(让线程终止)，例如清理程序正在扫描系统垃圾，可以临时结束，取消该线程
        取消某个线程，可以终止某个线程的运行，但是并不是立即终止，而是当线程 运行到一个取消点时，线程才会终止
        取消点：可以理解为系统规定好的一些系统调用，我们可以粗略的理解为从用户区到内核取的切换，这个位置就称为取消点
    参数：需要分离的线程ID
    返回值：
        成功：返回0
        失败：返回错误号
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void * callback(void * arg)
{
    printf("child thread id: %ld\n",pthread_self());
    for(int i=5;i<10;i++)
    {
        printf(" child %d\n",i);
    }
    printf("\n");
    return NULL;
}
int main()
{
    //创建一个线程
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,callback,NULL);
    if(ret!=0)
    {
        char * errstr=strerror(ret);
        printf("pthread_create error: %s\n",errstr);
    }

    //取消线程
    pthread_cancel(tid);
    
   for (int i = 0; i < 5; i++)
   {
       printf("%d\t",i);
   }
   printf("\n");
   
   //输出主线程 和 子线程 ID
    printf("tid: %d,main thread id : %ld\n",tid,pthread_self());
    
    //主进程资源回收，运行子进程
    pthread_exit(NULL);
    return 0;
}