#include "Task.h"
#include"ThreadPool.h"
#include<iostream>
#include <stdlib.h>
#include <time.h>

class MyTask
:public Task
{
public:
    void process () override
    {
        ::srand(::clock());//种随机种子
        int number = ::rand() % 100;//产生随机数
        cout << ">>MyTask number = " << number << endl;
    }    
};

void test(){
    unique_ptr<Task> ptask(new MyTask());
    ThreadPool pool(4,10);
    pool.start();
    int cnt=20;
    while(cnt--)
    {   
         pool.addTask(ptask.get());
    }
    pool.stop();
}
int main(){
    test();
    return 0;
}