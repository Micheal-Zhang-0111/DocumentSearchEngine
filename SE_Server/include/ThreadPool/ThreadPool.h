#pragma once

#include<vector>
#include<memory>
#include"../ThreadPool/Thread.h"
#include"../ThreadPool/TaskQueue.h"

using std::vector;
using std::unique_ptr;

class ThreadPool
{
public:
    ThreadPool(size_t threadNum,size_t queSize);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task &&task);
    Task getTask();

private:
    void threadFunc();

private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit;
};