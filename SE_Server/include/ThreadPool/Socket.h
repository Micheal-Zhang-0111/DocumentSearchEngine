#pragma once

#include"../ThreadPool/NonCopyable.h"

class Socket
:NonCopyable
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();

    int fd() const;
    void shutDownWrite();
    
private:
    int _fd;
};