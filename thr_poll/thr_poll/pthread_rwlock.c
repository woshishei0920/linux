//8个线程操作同一个共享变量，3个线程不定时写该资源，5个线程读
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int num=0;//全局资源
#define MAX 50
pthread_rwlock_t rwlock;

void* WriteNum(void *arg){
     for(int i=0;i<MAX;i++){
        pthread_rwlock_wrlock(&rwlock);
        int cur=num;
        cur++;
        num=cur;
        printf("写操作完成，number:%d,tid:%ld\n",num,pthread_self());
        pthread_rwlock_unlock(&rwlock);
         usleep(rand()%5);
    }

}
void* ReadNum(void *arg){
     for(int i=0;i<MAX;i++){
        pthread_rwlock_rdlock(&rwlock);
        printf("读操作完成，number:%d,tid:%ld\n",num,pthread_self());
        pthread_rwlock_unlock(&rwlock);
        usleep(rand()%5);
    }

}

int main(){
  pthread_rwlock_init(&rwlock,NULL);
  pthread_t p1[5],p2[3];
  for(int i=0;i<5;i++){
     pthread_create(&p1[i],NULL,ReadNum,NULL);

  }
   for(int i=0;i<3;i++){
     pthread_create(&p2[i],NULL,WriteNum,NULL);

  }
   for(int i=0;i<5;i++){
     pthread_join(p1[i],NULL);

  }
   for(int i=0;i<3;i++){
     pthread_join(p2[i],NULL);

  }


  pthread_rwlock_destroy(&rwlock);
 

}