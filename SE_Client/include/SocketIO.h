#pragma once

#include<string>
#include"./JsonTrans.h"

using std::string;

class SocketIO
{
public:
    explicit SocketIO(int fd);
    ~SocketIO();
    int readn(char* buf,int len);
    int readline(char* buf,int len);
    int recvs(char* buf);//网络良好情况使用
    int sends(const char *buf,int len);
    int writen(const char* buf,int len);
    json recvMessage();
    void sendMessage(jsonTrans &jTrans);
private:
    int _fd;
};
