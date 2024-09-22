#pragma once
#include<string>
#include"../ThreadPool/Socket.h"
#include"../ThreadPool/InetAddress.h"

using std::string;

class Acceptor
{
public:
    Acceptor(const string &ip,unsigned short port);
    ~Acceptor();

    void ready();
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
    int accept();
    int fd() const;

private:
    Socket _listenSock;
    InetAddress _serverAddr;
};