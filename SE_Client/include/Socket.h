#pragma once

#include"./NonCopyable.h"

class Socket 
: NonCopyable
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int fd() const;
    void shutdownWrite();
private:
    int _fd;
};