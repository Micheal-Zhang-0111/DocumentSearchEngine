#include"../../include/ThreadPool/TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
:_queSize(queSize),
_que(),
_mutex(),
_notEmpty(_mutex),
_notFull(_mutex),
_flag(true)
{

}

TaskQueue::~TaskQueue(){

}

bool TaskQueue::empty() const{
    return _que.size() == 0;
}

bool TaskQueue::full() const{
    return _que.size() == _queSize;
}

void TaskQueue::push(Task &&task){
    MutexLockGuard autoLock(_mutex);//autoLock栈对象

    while(full()){//while防止虚假唤醒
        _notFull.wait();
    }

    _que.push(std::move(task));
    _notEmpty.notify();
}

Task TaskQueue::pop(){
    MutexLockGuard autoLock(_mutex);//autoLock栈对象

    while(_flag && empty()){
        _notEmpty.wait();
    }

    if(_flag){
        Task tmp = _que.front();
        _que.pop();
        _notFull.notify();
        return tmp;
    }
    else{
        return nullptr;
    }
}

//将所有等在_notEmpty上的线程唤醒
void TaskQueue::wakeup(){
    _flag = false;
    _notEmpty.notifyAll();
}