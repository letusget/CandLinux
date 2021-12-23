/*
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);
    功能：和一个已经终止的线程进行连接 (与进程不同，子进程资源需要父进程回收，任何线程都可以回收其他线程的资源)，如果多个线程结束了而不回收其资源，也会产生僵尸线程
        回收子线程资源，是 阻塞函数，调用一次只能回收一个子线程，一般在主线程中使用
    参数：
        thread: 需要回收的子线程ID
        retval: 接受子线程退出时的返回值
    参数：
        成功：0
        失败：返回非0，错误号
*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//定义为去全局变量
int value=10;

void * callback(void* arg)
{
    printf("child thread id : %ld\n",pthread_self());
    //sleep (3);
    //return NULL;
    //这个是局部变量，由于栈空间不共享，所以不能传递出去
    // int value=10;
    //效果和 return (void *)&value 是一样的
    pthread_exit((void *)&value);    //注意类型转换
}
int main()
{
    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,callback,NULL);
    
    if(ret!=0)
    {
        char* errstr=strerror(ret);
        printf("error: %s\n",errstr);
    }

    //主线程
    for (int i = 0; i < 5; i++)
    {
       printf("%d\t",i);
    }
    printf("\n");
    
    printf("tid : %ld, main thread id : %ld\n",tid,pthread_self());

    //主线程 调用pthread_join() 回收子线程资源
    int * thread_retval;
    ret = pthread_join(tid,(void **)&thread_retval);    //注意类型转换
    if(ret!=0)
    {
        char* errstr=strerror(ret);
        printf("error: %s\n",errstr);
    }
    //获取回调函数中退出的返回值
    printf("exit data : %d\n",*thread_retval);
    printf("回收子线程资源成功\n");

    //让主线程退出，使子线程执行
    //当主线程退出，不会影响运行其他子线程
    pthread_exit(NULL);

    return 0;
}