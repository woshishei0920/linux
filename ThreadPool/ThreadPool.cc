#include"ThreadPool.h"
#include"WorkThread.h"
#include"Task.h"
#include<iostream>
#include <unistd.h>

using namespace std;
ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
:_threadNum(threadNum)
,_queSize(queSize)
,_taskQue(queSize)
,_isExit(false)
{
    _threads.reserve(_threadNum);
}
ThreadPool::~ThreadPool(){

}
void ThreadPool::start(){
    for(size_t idx=0;idx!=_threadNum;idx++){
      
        unique_ptr<Thread > up(new WorkThread(*this));
        _threads.push_back(std::move(up));
    }
    for(auto &th:_threads){
        th->start();
    }
}
void ThreadPool::stop(){
    while(!_taskQue.empty())
    {
    sleep(1);
    }
    _isExit=true;
    _taskQue.wakeup();
    for(auto &th:_threads){
        th->stop();
    }
}

void ThreadPool::addTask(Task *ptask)
{
     if(ptask){
        _taskQue.push(ptask);
     }
}



Task * ThreadPool::getTask(){
    return _taskQue.pop();
}
void ThreadPool::doTask(){
    
    while(!_isExit){
        Task *ptask=getTask();
        if(ptask){
             ptask->process();
        }
        else{
            cout<<"nullptr==ptask"<<endl;
        }
    }
}
