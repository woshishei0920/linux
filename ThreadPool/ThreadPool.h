#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.h"
#include<vector>
#include<memory>

using namespace std;

class Thread;
class Task;


class ThreadPool
{
    friend class WorkThread;
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();
    void start();
    void stop();

    void addTask(Task *ptask);

private:
    Task *getTask();
    void doTask();   

private:
    size_t _threadNum;//子线程的数量
    size_t _queSize;//任务队列的数量
    vector<unique_ptr<Thread> > _threads;
    TaskQueue _taskQue;
    bool _isExit;
};
#endif