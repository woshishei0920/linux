#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"
#include <queue>

using std::queue;

class Task;
using Elemtype =Task * ;
class TaskQueue
{

public:
    TaskQueue(size_t queSize);
    ~TaskQueue();
    //添加任务
    void  push( Elemtype ptask);
    //获取任务
    Elemtype pop();

    //任务队列是空与满
    bool full() const;
    bool empty() const;
    void wakeup() ;

private:
    size_t _queSize;//任务队列的大小
    queue<Elemtype> _que;//存放任务的数据结构
    MutexLock _mutex;//互斥锁
    Condition _notFull;//非满条件变量（生产者）
    Condition _notEmpty;//非空条件变量（消费者）
    bool _flag;
};

#endif
