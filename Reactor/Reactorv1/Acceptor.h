#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "Socket.h"
#include "InetAddress.h"
#include <string>

using std::string;

class Acceptor
{
public:
    Acceptor(const string & ip,unsigned short port);
    ~Acceptor();
    void ready();
    int accept();
private:
    void setReusePort();
    void setReuseAddr();
    void bind();
    void listen();

private:
    Socket _sock;
    InetAddress _addr;
};
#endif 