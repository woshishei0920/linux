#include "Acceptor.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

void ConnectionEvent(const TcpConnectionPtr &con)
{
    cout<<"------Tcp连接成功--------"<<endl;
    cout<<"server ip :"<<con->toServerIp()<<"server port :"<<con->toServerPort()<<endl;
    cout<<"client ip :"<<con->toClientIp()<<"client port :"<<con->toClientPort()<<endl;
}
void CloseEvent(const TcpConnectionPtr &con)
{
    cout<<"------Tcp断开连接--------"<<endl;
}
void MessageEvent(const TcpConnectionPtr &con)
{
    string buf=con->receive();
    cout<<"recv from client :"<<buf<<endl;
}


void test()
{
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();//此时处于监听状态
    EventLoop loop(acceptor);
    loop.setNewConectionCallback(std::move(ConnectionEvent));
    loop.setMessageCallback(std::move(MessageEvent));
    loop.setCloseCallback(std::move(CloseEvent));
    loop.loop();
}

int main(int argc, char **argv)
{
    test();
    return 0;
}
