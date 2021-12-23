/*
使用多线程实现 买票的案例
三个窗口 100 张票
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//作为全局变量，使所有线程都共享同一份资源
int tickets=1000;

//创建互斥量
pthread_mutex_t mutex;

void * sellticket(void * arg)
{
    //买票 如果还是 while(tickets>0) 就会 是同一个进程在一直执行了
    while(1)
    {
        //加锁
        pthread_mutex_lock(&mutex);

        if (tickets>0)
        {
            usleep(8000);   //防止单个进程执行太快，看不到线程切换
            printf("%ld 正在买第 %d 张门票\n",pthread_self(),tickets);
            tickets--;
        }
        else
        {
            //解锁
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        //解锁
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
//主线程只负责回收子线程资源
int main()
{
    //初始化互斥量
    pthread_mutex_init(&mutex,NULL);

    //创建3个子线程
    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,sellticket,NULL);
    pthread_create(&tid2,NULL,sellticket,NULL);
    pthread_create(&tid3,NULL,sellticket,NULL);


    //回收子线程资源  阻塞函数
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    //也可以设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);

    //退出主线程
    pthread_exit(NULL);

    //释放互斥量资源
    pthread_mutex_destroy(&mutex);

    return 0;
}