#include "TcpConnection.h"

TcpConnection::TcpConnection(int fd)
:_sockio(fd)
,_sock(fd)
,_server(getServer())
,_client(getClinet())
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

InetAddress TcpConnection::getServer()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    int ret = getsockname(_sock.get_fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname");
    }

    return InetAddress(addr);
}
InetAddress TcpConnection::getClinet()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr );
    //获取对端地址的函数getpeername
    int ret = getpeername(_sock.get_fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getpeername");
    }

    return InetAddress(addr);
}
string TcpConnection::toServerIp()
{
    return _server.ip();
}
unsigned short TcpConnection::toServerPort()
{
    return _server.port();
}
string TcpConnection::toClientIp()
{
    return _client.ip();
}
unsigned short  TcpConnection::toClientPort()
{
    return _client.port();
}

    
void TcpConnection::setNewConectionCallback(const TcpConnectionCallback &cb)
{   if(cb){
        _onNewConnectionCb=cb;
         }
    else 
        {
            cout<<"_onNewConnectionCb is nullptr"<<endl;
         }
}
void TcpConnection::handleNewConectionCallback()
{
    if(_onNewConnectionCb){
        _onNewConnectionCb(shared_from_this());
         }
    else 
        {
            cout<<"handleNewConectionCallback is nullptr"<<endl;
         }
}
void TcpConnection::handleMessageCallback()
{
    if(_onMessageCb){
        _onMessageCb(shared_from_this());
         }
    else 
        {
            cout<<"handleMessageCallback is nullptr"<<endl;
         }
}
void TcpConnection::handleCloseCallback()
{
    if(_onCloseCb){
        _onCloseCb(shared_from_this());
         }
    else 
        {
            cout<<"handleCloseCallback is nullptr"<<endl;
         }
}
void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{
    if(cb){
        _onMessageCb=cb;
         }
    else 
        {
            cout<<"_onMessageCb is nullptr"<<endl;
         }
}
void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{
    if(cb){
        _onCloseCb=cb;
         }
    else 
        {
            cout<<"_onCloseCb is nullptr"<<endl;
         }
}


bool TcpConnection::isClosed()
{   char buf[10]={0};
    int ret=recv(_sock.get_fd(),buf,sizeof(buf),MSG_PEEK);
    return (ret == 0);
}


