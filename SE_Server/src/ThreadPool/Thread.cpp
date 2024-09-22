#include<stdio.h>
#include"../../include/ThreadPool/Thread.h"

Thread::Thread(ThreadCallback &&cb)
:_pthid(0),
_isRunning(false),
_cb(std::move(cb))
{

}

Thread::~Thread(){
    if(_isRunning){
        pthread_detach(_pthid);
        // _isRunning = false;
    }
}

void Thread::start(){
    int ret = pthread_create(&_pthid, nullptr, threadFunc, this);
    if(ret){
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}

void Thread::join(){
    if(_isRunning){
        pthread_join(_pthid, nullptr);
        _isRunning = false;
    }
}

//线程入口函数
void *Thread::threadFunc(void *arg){
    Thread *pth = static_cast<Thread *>(arg);
    if(pth){
        pth->_cb();
    }
    pthread_exit(nullptr);
}