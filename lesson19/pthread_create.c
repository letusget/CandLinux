/*
一般将 main函数所在的线程称之为 主线程（main线程），其余创建的线程 称为 子线程
程序中默认只有一个进程，由fork() 函数调用，产生子进程，这样就有了两个进程
程序中默认只有一个线程，由pthread_create() 函数调用产生子进程，这样就有了两个线程
#include <pthread.h>
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
    功能：创建一个子线程
    参数：
        thread: 传出参数，线程创建成功后，子线程的线程ID被写到该变量中
        attr: 需要设置的线程的属性，一般使用默认值，即设为NULL
        start_routine: 函数指针类型，子线程需要处理的逻辑代码
        arg: 给第三个参数使用，传参使用
    返回值：
        如果成功返回0
        如果失败则返回错误号（与errorno含义不一样），并且 thread 也会被设置为未定义
        获取错误号信息：char* strerror(int errnum)
        #include <string.h>
        char *strerror(int errnum);
*/
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void * callback(void* arg)
{
    printf("child thread...\n");
    //注意强制类型转换
    printf("arg value: %d\n", *(int *)arg);
    return NULL;
}
int main()
{
    pthread_t tid;
    int num=10;
    //创建一个子线程
    //int ret = pthread_create(&tid,NULL,callback,NULL);
    //要注意强制类型转换
    int ret = pthread_create(&tid, NULL, callback, (void *)&num);

    if(ret!=0)
    {
        char * errstr=strerror(ret);
        printf("error : %s\n",errstr);
    }

    for (int i = 0; i < 5; i++)
    {
        printf("%d\t",i);
    }
    printf("\n");
    //防止主线程直接执行完毕，释放资源，给子线程执行的时间
    sleep(1);

    return 0;
}
