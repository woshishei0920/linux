#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <vector>
#include <map>
#include <memory>
#include<functional>

using namespace std;
class Acceptor;//前向声明
class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback=function<void(const TcpConnectionPtr &)>;





class EventLoop
{
public:
    EventLoop(Acceptor & acceptor);
    ~EventLoop();
    void loop();
    void unloop();

    void setNewConectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);
private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    int createEpollFd();
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);

private:
    int _epfd;
    vector<struct epoll_event> _evList;
    bool _isLooping;
    Acceptor &_acceptor;
    map<int ,TcpConnectionPtr> _conns;
    TcpConnectionCallback _onNewConnectionCb;   //连接建立
    TcpConnectionCallback _onCloseCb;           //连接断开
    TcpConnectionCallback _onMessageCb;         //消息到达
};

#endif