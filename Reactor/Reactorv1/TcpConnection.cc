#include "TcpConnection.h"

TcpConnection::TcpConnection(int fd):_sockio(fd)
{

}
TcpConnection::~TcpConnection()
{
    
}
size_t TcpConnection::send(const string &msg)
{
    return _sockio.writen(msg.c_str() ,msg.size());
}
string TcpConnection::receive()
{
    char buf[65535]={0};
    _sockio.readLine(buf,sizeof(buf));
    return string(buf);
}
