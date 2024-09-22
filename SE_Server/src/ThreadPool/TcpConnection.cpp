#include<iostream>
#include<sstream>
#include"../../include/ThreadPool/TcpConnection.h"
#include"../../include/ThreadPool/EventLoop.h"

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
:_loop(loop),
_sock(fd),
_sockIO(fd),
_localAddr(getLocalAddr()),
_peerAddr(getPeerAddr())
{

}

TcpConnection::~TcpConnection(){

}

void TcpConnection::send(const string &msg){
    Data dMsg;
    dMsg.length = msg.size();
    dMsg.data = msg;
    jsonTransform jTrans(dMsg);

    json jMsg;
    jTrans.to_json(jMsg);
    _sockIO.writen(jMsg.dump().c_str(),jMsg.dump().size());
}

void TcpConnection::sendInLoop(const string &msg){
    if(_loop){
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}

string TcpConnection::receive(){
    char buf[65535] = {0};
    _sockIO.recvs(buf);
    return string(buf);
}

string TcpConnection::toString(){
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();
    
    return oss.str();
}

bool TcpConnection::isClose() const{
    char buf[10] = {};
    int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);

    return (ret == 0);
}

//注册三个事件的回调函数
void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb){
    _onConnectionCb = cb;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb){
    _onMessageCb = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb){
    _onCloseCb = cb;
}

//三个事件回调函数的执行
void TcpConnection::handleConnectionCallback(){
    if(_onConnectionCb){
        _onConnectionCb(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback(){
    if(_onMessageCb){
        _onMessageCb(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback(){
    if(_onCloseCb){
        _onCloseCb(shared_from_this());
    }
}

InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(ret == -1){
        perror("getLocalAddr");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(ret == -1){
        perror("getPeerAddr");
    }
    return InetAddress(addr);
}