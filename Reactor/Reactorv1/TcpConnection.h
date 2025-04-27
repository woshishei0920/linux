#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"

class TcpConnection
{
public:
    explicit TcpConnection(int fd);
    ~TcpConnection();
    size_t send(const string &msg);
    string receive();
private:
    SocketIO _sockio;
};
#endif 