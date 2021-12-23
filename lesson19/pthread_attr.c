/*
#include <pthread.h>
int pthread_attr_init(pthread_attr_t *attr);
    功能：初始化线程属性变量
int pthread_attr_destroy(pthread_attr_t *attr);
    功能：释放线程属性的资源
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int* detachstate);
    功能：获取线程分离的状态属性
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
    功能：设置线程分离的状态属性
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
    //创建一个线程属性变量
    pthread_attr_t attr;
    //初始化属性变量
    pthread_attr_init(&attr);
    //设置属性 线程分离
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

    //创建一个线程
    pthread_t tid;
    int ret = pthread_create(&tid,&attr,callback,NULL);
    if(ret!=0)
    {
        char * errstr=strerror(ret);
        printf("pthread_create error: %s\n",errstr);
    }

    //获取线程的栈的大小
    ssize_t size;
    //size 为传出参数，并且传递的是地址，将获得的大小，设置到 size中
    pthread_attr_getstacksize(&attr,&size);
    printf("thread stack size: %ld\n",size);

    //输出主线程 和 子线程 ID
    printf("tid: %d,main thread id : %ld\n",tid,pthread_self());

    //释放线程属性资源
    pthread_attr_destroy(&attr);
    
    //主进程资源回收，运行子进程
    pthread_exit(NULL);
    return 0;
}