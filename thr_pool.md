

pthread_t pthread_self(void);//返回当前线程的线程ID

线程创建
#include<pthread.h>
int pthread_create(pthread_t *threadid,const pthread_attr_t *attr,void *(*start_routine),void *arg);  //创建线程 ，执行成功返回0                    线程ID(线程执行成功，会把线程ID写入threadid指针指向的内存中)       线程属性（一般NULL）    线程执行函数       传入函数的上下文 

编译时 -l pthread 链接动态库 libpthread.s （编译时掐头去尾 lib  .s）

线程退出（让线程退出，但不会导致虚拟地址空间释放（针对主线程））

#include<pthread.h>
void  pthread_exit(void *retval)
//参数：线程退出的时候携带的数据，当前子线程的主线程会得到该数据，不使用时，指定为NULL

线程回收
#include<pthread.h>
int pthread_join(pthread_t threadid,void **retval)
// threadid 要回收的线程ID  retval：二级指针 指向 pthread_exit()传递出的数据，不需要时 NULL;  回收成功返回0
//如果有子线程在运行，调用该函数会一直阻塞 ，子线程退出函数解除阻塞并进行支援的回收，调用一次只能回收一次子线程

线程分离 
#include<pthread.h>
int pthread_detach(pthread_t thread);

线程取消
#include<pthread.h>
int pthread_cancel(pthread_t thread); //在子线程进行系统调用时，被杀死

线程比较 
int pthread_equal(pthread_t t1,pthread_t t2);// 相等返回非0 不想等返回0