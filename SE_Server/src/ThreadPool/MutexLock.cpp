#include<stdio.h>
#include"../../include/ThreadPool/MutexLock.h"

MutexLock::MutexLock(){
    int ret = pthread_mutex_init(&_mutex,nullptr);
    if(ret){
        perror("pthread_mutex_init");
    }
}

MutexLock::~MutexLock(){
    int ret = pthread_mutex_destroy(&_mutex);
    if(ret){
        perror("pthread_mutex_destory");
    }
}

void MutexLock::lock(){
    int ret = pthread_mutex_lock(&_mutex);
    if(ret){
        perror("pthread_mutex_lock");
    }
}

void MutexLock::unlock(){
    int ret = pthread_mutex_unlock(&_mutex);
    if(ret){
        perror("pthread_mutex_unlock");
    }
}

pthread_mutex_t *MutexLock::getMutexLockPtr(){
    return &_mutex;
}