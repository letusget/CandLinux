/*
生产者 消费者模型
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

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
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next=head;
        head=newNode;
        
    }
    

    return NULL;
}

void * customer(void * arg)
{

    return NULL;
}
int main()
{
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
    
    while (1)
    {
        sleep(10);
    }

    //线程退出
    pthread_exit(NULL);
    

    return 0;
}