#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
struct Test{

    int num;
    int age;
}T;

void* callback(void *arg){
  struct Test *t=(struct Test *) arg ;
  t->num=100;
  t->age=18;
  for(int i=0;i<5;i++)
    printf("子线程 i=%d\n",i);

   printf("子线程：%ld\n",pthread_self());

   pthread_exit(t);
   return NULL;
}

int main()
{   void *p;
    pthread_t tid;
    struct Test t;
    pthread_create(&tid,NULL,callback,&t);
    printf("主线程：%ld\n",pthread_self());
    pthread_detach(tid);

    pthread_exit(NULL);

#if 0
    pthread_join(tid,NULL);
   // sleep(3); 防止子线程未占领cpu时间片，主线程执行完毕后直接释放主线程， 等待子线程占领cpu
    printf("num:%d,age:%d\n",t.num,t.age);
#endif
    return 0;
}