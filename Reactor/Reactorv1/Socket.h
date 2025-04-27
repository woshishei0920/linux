#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <arpa/inet.h>
#include <string>
#include "NonCopyable.h"
class Socket
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int get_fd() const;
private:
    int _fd;
};
#endif 