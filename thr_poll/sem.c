#include<string.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>

sem_t semp,semc;
pthread_mutex_t mutex;

struct Node{
   int data;
   struct Node *next;
};

struct Node *head=NULL; //链表头节点

void* producer(void *arg){
    while(1){
        sem_wait(&semp);
        pthread_mutex_lock(&mutex);
        struct Node *newNode= (struct Node *)malloc(sizeof(newNode));
        newNode->data=rand()%1000;
        newNode->next=head;
        head=newNode;
        printf("生产者,id:%ld.number:%d\n",pthread_self(),newNode->data);
        pthread_mutex_unlock(&mutex);
        sem_post(&semc);
        sleep(rand()%3);
        
    }
    return NULL;
}

void* consumer(void *arg){
    while(1){   
        sem_wait(&semc);
        pthread_mutex_lock(&mutex);
        struct Node *oldNode= head;
        head=head->next;
        free(oldNode);
        oldNode=NULL;
        printf("消费者,id:%ld\n",pthread_self());
        pthread_mutex_unlock(&mutex);
        sem_post(&semp);
        sleep(rand()%3);
     
    }
     return NULL;
}


int main(){
    pthread_mutex_init(&mutex,NULL);
    sem_init(&semp,0,5);
    sem_init(&semc,0,0);
    pthread_t t1[5],t2[5];  //5个生产者线程和5个消费者线程

    for(int i=0;i<5;i++){
        pthread_create(&t1[i],NULL,producer,NULL);
    }


    for(int i=0;i<5;i++){
        pthread_create(&t2[i],NULL,consumer,NULL);
    }
    
    for(int i=0;i<5;i++){
        pthread_join(t2[i],NULL);
    }
    for(int i=0;i<5;i++){
        pthread_join(t1[i],NULL);
    }
    sem_destroy(&semp);
    sem_destroy(&semc);
     pthread_mutex_destroy(&mutex);
    return 0;
}