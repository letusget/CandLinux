/*
生产者 消费者模型
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

//创建互斥量
pthread_mutex_t mutex;

//创建条件变量
pthread_cond_t cond;

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
        pthread_mutex_lock(&mutex);

        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        //头插法
        newNode->next=head;
        head=newNode;
        newNode->num=rand()%1000;
        printf("add node, num: %d, tid: %ld\n",newNode->num,pthread_self());

        //生产了一个，就通知 消费者消费
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);

        usleep(100);
    }
    

    return NULL;
}

void * customer(void * arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);

        //保存头结点的指针
        struct Node* tmp=head;
        //判断是否有数据
        if(head!=NULL)
        {
            //有数据
            head=head->next;
            printf("del node, num: %d, tid: %ld\n",tmp->num,pthread_self());
            free(tmp);

            //释放锁
            pthread_mutex_unlock(&mutex);

            usleep(100);
        }
        else
        {
            //没有数据，就阻塞等待,直到生产者 生产出内容
            //当这个函数调用阻塞的时候，会对互斥锁 进行解锁，当不阻塞（执行）的时候，会继续向下执行，并重新加锁
            pthread_cond_wait(&cond,&mutex);
            pthread_mutex_unlock(&mutex);
        }
        
        /* code */
    }
    

    return NULL;
}
int main()
{
    //初始化 互斥量
    pthread_mutex_init(&mutex,NULL);
    //条件变量 初始化
    pthread_cond_init(&cond,NULL);

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
    //释放条件变量
    pthread_cond_destroy(&cond);
    
    //线程退出
    pthread_exit(NULL);
    
   

    return 0;
}