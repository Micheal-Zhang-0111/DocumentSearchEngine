#pragma once

class SocketIO
{
public:
    explicit SocketIO(int fd);
    ~SocketIO();

    int readn(char *buf, int len);
    int recvs(char *buf);
    int readLine(char *buf,int len);
    int writen(const char *buf,int len);
    
private:
    int _fd;
};