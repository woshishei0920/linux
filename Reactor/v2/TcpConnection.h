#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include<string>
#include<memory>
#include<iostream>
#include<functional>

using namespace std;
class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback=function<void(const TcpConnectionPtr &)> ;


class TcpConnection
:public std::enable_shared_from_this<TcpConnection>
{

public:
    explicit TcpConnection(int fd);
    ~TcpConnection();
    size_t send(const string &msg);
    string receive();

    void setNewConectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);

    void handleNewConectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
    bool isClosed();
    
    InetAddress getServer();
    InetAddress getClinet();
    string toServerIp();
    unsigned short toServerPort();
    string toClientIp();
    unsigned short toClientPort();
private:
    SocketIO _sockio;
    Socket _sock;
    InetAddress _server;
    InetAddress _client;
    TcpConnectionCallback _onNewConnectionCb;   //连接建立
    TcpConnectionCallback _onCloseCb;           //连接断开
    TcpConnectionCallback _onMessageCb;         //消息到达
};
#endif 