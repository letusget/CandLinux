/*
信号量的类型 sem_t
int sem_init(sem_t *sem, int pshared, unsigned int value);
初始化信号量：
    参数：
        sem: 信号量变量的地址
        pshared: 0用在线程间，非0 用在进程间
        value：信号量中的值
int sem_destroy(sem_t *sem);
释放资源
int sem_wait(sem_t *sem);
对信号量加锁，调用一次就对 信号量(value)的值-1，如果值为0，就阻塞
int sem_trywait(sem_t *sem);
尝试对信号量加锁，会检测信号，如果信号没有发过来，则返回EAGAIN=11，如果有收到信号，返回0。
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
对信号量加锁，指定时间，当递减操作不能立即执行时，sem_timedwait的abs_timeout参数指定了调用应该阻塞的时间限制

int sem_post(sem_t *sem);
对信号量解锁，调用一次就对 信号量(value)的值+1
int sem_getvalue(sem_t *sem, int *sval);
测试信号量的值
*/

/*
生产者 消费者模型
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

//创建互斥量
pthread_mutex_t mutex;
//创建两个信号量
sem_t psem; //生产者信号量
sem_t csem; //消费者信号量


struct Node{
    int num;
    struct Node* next;
};

//头结点
struct Node* head=NULL;

void * producer(void* arg)
{
    //不断创建新节点，添加到链表中
    while (1)
    {
        //对信号量加锁，初始值为8，表示可以生产8个 生产者-1
        sem_wait(&psem);
        //对互斥量加锁
        pthread_mutex_lock(&mutex);

        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        //头插法
        newNode->next=head;
        head=newNode;
        newNode->num=rand()%1000;
        printf("add node, num: %d, tid: %ld\n",newNode->num,pthread_self());
        //对互斥量解锁
        pthread_mutex_unlock(&mutex);
        //对信号量解锁，生产了8个，每次都可以让消费者消费 消费者+1
        sem_post(&csem);
        //usleep(100);
    }
    

    return NULL;
}

void * customer(void * arg)
{
    while (1)
    {
        //对信号量加锁,如果生产者生产了，就可以消费  消费者-1
        sem_wait(&csem);
        //对互斥量加锁
        pthread_mutex_lock(&mutex);
        //保存头结点的指针
        struct Node* tmp=head;
            //有数据
            head=head->next;
            printf("del node, num: %d, tid: %ld\n",tmp->num,pthread_self());
            free(tmp);
            //释放锁
            pthread_mutex_unlock(&mutex);
            //解锁，通知生产者生产 +1
            sem_post(&psem);

            //usleep(100);

    }
    

    return NULL;
}
int main()
{
    //初始化 互斥量
    pthread_mutex_init(&mutex,NULL);
    //信号量初始化
    sem_init(&psem,0,8);    //初始时生成8个
    sem_init(&csem,0,0);

    //创建5个生产者线程，创建5 个 消费者线程
    pthread_t ptids[5],ctids[5];

    //创建线程
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&ptids[i],NULL,producer,NULL);
        pthread_create(&ctids[i],NULL,customer,NULL);
    }
    
    //分离线程
    for (int i = 0; i < 5; i++)
    {
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }

   //防止线程分离之后，就执行释放互斥量，导致互斥量失灵 
    while (1)
    {
        sleep(10);
    }

     //释放互斥量
    pthread_mutex_destroy(&mutex);
    
    //线程退出
    pthread_exit(NULL);
    
    return 0;
}