/*
#include <pthread.h>
void pthread_exit(void *retval);
    功能：终止一个线程，在哪个线程中调用，就终止哪个线程
    参数：
        retval: 需要传递一个指针，作为一个返回值，可以在 pthread_join() 中获取到
    pthread_t pthread_self(void);
        功能：获取当前的线程ID
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>

void * callback(void* arg)
{
    printf("child thread id : %ld\n",pthread_self());
    return NULL;
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

    //让主线程退出，使子线程执行
    //当主线程退出，不会影响运行其他子线程
    pthread_exit(NULL);

    return 0;
}