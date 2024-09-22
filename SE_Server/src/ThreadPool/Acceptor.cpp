#include<stdio.h>
#include"../../include/ThreadPool/Acceptor.h"

Acceptor::Acceptor(const string &ip,unsigned short port)
:_listenSock(),
_serverAddr(ip,port)
{

}

Acceptor::~Acceptor(){

}

void Acceptor::ready(){
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

void Acceptor::setReuseAddr(){
    int on = 1;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(ret){
        perror("setReuseAddr");
        return;
    }
}

void Acceptor::setReusePort(){
    int on = 1;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    if(ret){
        perror("setReusePort");
        return;
    }
}

void Acceptor::bind(){
    int ret = ::bind(_listenSock.fd(),
                    (struct sockaddr *)_serverAddr.getInetAddrPtr(),
                    sizeof(struct sockaddr));
    if(ret == -1){
        perror("bind");
        return;
    }
}

void Acceptor::listen(){
    int ret = ::listen(_listenSock.fd(),128);
    if(ret == -1){
        perror("listen");
        return;
    }
}

int Acceptor::accept(){
    int connfd = ::accept(_listenSock.fd(), nullptr, nullptr);
    if(connfd == -1){
        perror("accept");
        return -1;
    }
    return connfd;
}

int Acceptor::fd() const{
    return _listenSock.fd();
}