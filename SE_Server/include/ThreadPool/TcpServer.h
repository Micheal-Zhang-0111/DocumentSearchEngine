#pragma once

#include<iostream>
#include<unistd.h>
#include"../ThreadPool/Acceptor.h"
#include"../ThreadPool/EventLoop.h"
#include"../ThreadPool/TcpConnection.h"

using std::cout;
using std::endl;

class TcpServer
{
public:
    TcpServer(const string &ip,unsigned short port)
    :_acceptor(ip,port),
    _loop(_acceptor)
    {

    }

    void start(){
        _acceptor.ready();
        _loop.loop();
    }

    void stop(){
        _loop.unloop();
    }

    void setAllCallback(TcpConnectionCallback &&onConnection,
                        TcpConnectionCallback &&onMessage,
                        TcpConnectionCallback &&onClose)
    {
        _loop.setConnectionCallback(std::move(onConnection));
        _loop.setMessageCallback(std::move(onMessage));
        _loop.setCloseCallback(std::move(onClose));
    }

private:
    Acceptor _acceptor;
    EventLoop _loop;
};