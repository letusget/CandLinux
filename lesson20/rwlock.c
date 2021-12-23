/*
读写锁的类型 pthread_rwlock_t
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
*/

//案例：8个线程操作同一个全局变量
//3个线程 不定时写这个全局变量，5个线程不定时的读这个全局变量

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//创建共享数据
int num=1;

//定义互斥锁
//pthread_mutex_t mutex;
//定义读写锁
pthread_rwlock_t rwlock;


void * writeNum(void * arg)
{
    while (1)
    {  
        //pthread_mutex_lock(&mutex);
        //加写锁
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("++wirte, tid : %ld, num : %d\n",pthread_self(),num);
        //pthread_mutex_unlock(&mutex);
        //释放锁
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    

    return NULL;
}
void * readNum(void * arg)
{
    while (1)
    {
        //pthread_mutex_lock(&mutex);
        //加读锁
        pthread_rwlock_rdlock(&rwlock);
        printf("==read, tid : %ld, num : %d\n",pthread_self(),num);
        //pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);

        usleep(100);
        
    }
    

    return NULL;
}
int main()
{
    //初始化互斥锁
    //pthread_mutex_init(&mutex,NULL);

    //初始化读写锁
    pthread_rwlock_init(&rwlock,NULL);

    //创建八个线程
    //三个写线程，5个读线程
    pthread_t wtids[3],rtids[5];
    for (int i = 0; i < 3; i++)
    {
       pthread_create(&wtids[i],NULL,writeNum,NULL);
    }
    for (int i = 0; i < 3; i++)
    {
       pthread_create(&rtids[i],NULL,readNum,NULL);
    }

    //设置线程分离,在子线程执行结束后悔回收线程资源
    for (int i = 0; i < 3; i++)
    {
       pthread_detach(wtids[i]);
    }
    for (int i = 0; i < 3; i++)
    {
       pthread_detach(rtids[i]);
    }
    
    //退出主线程,使子线程继续运行
    pthread_exit(NULL);

    //释放互斥量资源
    // for (int i = 0; i < 3; i++)
    // {
    //     pthread_mutex_destroy(&wtids[i]);
    // }
    // for (int i = 0; i < 5; i++)
    // {
    //     pthread_mutex_destroy(&rtids[i]);
    // }
    
    //释放
    //pthread_mutex_destroy(&mutex);
    //释放读写锁
    pthread_rwlock_destroy(&rwlock);


    return 0;
}