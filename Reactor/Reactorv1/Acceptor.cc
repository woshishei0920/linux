
#include"Acceptor.h"

Acceptor::Acceptor(const string & ip,unsigned short port)
:_sock(),_addr(ip,port)
{

}
Acceptor::~Acceptor()
{

}
void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

void Acceptor::setReusePort()
{
    int opt=1;
    int ret=setsockopt(_sock.get_fd(),SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
    if(ret == -1)
    {
        perror("setsockopt port");
        return;
    }
}
void Acceptor::setReuseAddr()
{
    int opt=1;
    int ret=setsockopt(_sock.get_fd(),SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if(ret == -1)
    {
        perror("setsockopt addr");
        return;
    }
}
void Acceptor::bind()
{
    int ret= ::bind(_sock.get_fd(),(struct sockaddr *) _addr.getInetAddress(),sizeof(struct sockaddr_in));
    //::bind 指明是C的bind函数 与成员函数bind 区分开
    if(ret==-1)
    {
        perror("bind");
        return;
    }
}
void Acceptor::listen()
{
    int ret= ::listen(_sock.get_fd(),10);
    if(ret==-1)
    {
        perror("listen");
        return;
    }
}

int Acceptor::accept()
{
    int connfd =::accept(_sock.get_fd(),nullptr,nullptr);
    if(connfd == -1)
    {
        perror("accept");
        return -1;
    }
    return connfd;
}