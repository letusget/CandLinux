/*
#include <pthread.h>
int pthread_detach(pthread_t thread);
    功能：分离一个线程，被分离的线程在终止的时候，会自动释放资源返回给系统
        不能多次分离，会产生无法预料的问题
        不能连接一个已经分离的线程，如果这样做，会报错
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

    //输出主线程 和 子线程 ID
    printf("tid: %d,main thread id : %ld\n",tid,pthread_self());

    //设置子线程分离，子线程分离后，子线程结束时，对应的资源就不需要主线程释放
    ret = pthread_detach(tid);
    if(ret!=0)
    {
        char * errstr=strerror(ret);
        printf("pthread_detach error: %s\n",errstr);
    }

    //设置分离后，对分离的子线程进行连接，会报错
    ret = pthread_join(tid,NULL);
    if(ret!=0)
    {
        char * errstr=strerror(ret);
        printf("pthread_join error: %s\n",errstr);
    }
    
    //主进程资源回收，运行子进程
    pthread_exit(NULL);
    return 0;
}