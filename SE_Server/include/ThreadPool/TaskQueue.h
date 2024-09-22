#pragma once

#include<queue>
#include<functional>
#include"./MutexLock.h"
#include"./Condition.h"

using std::queue;
using std::function;
using Task = function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t queSize);
    ~TaskQueue();

    bool empty() const;
    bool full() const;
    void push(Task &&task);
    Task pop();

    //将所有等在_notEmpty上的线程唤醒
    void wakeup();

private:
    size_t _queSize;
    queue<Task> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;
};