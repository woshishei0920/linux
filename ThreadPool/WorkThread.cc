#include "WorkThread.h"
#include "ThreadPool.h"
WorkThread::WorkThread(ThreadPool &pool)
:_pool(pool)
{

}
WorkThread::~WorkThread()
{

}
void WorkThread::run(){
    // pool.doTask();
    _pool.doTask();
}