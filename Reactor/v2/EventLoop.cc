#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <unistd.h>
#include <sys/epoll.h>

#include <iostream>

using namespace std;

EventLoop::EventLoop(Acceptor & acceptor)
:_epfd(createEpollFd())
,_evList(1024)
,_isLooping(false)
,_acceptor(acceptor)
,_conns()
{   int listenfd =_acceptor.getFd();
    addEpollReadFd(listenfd);

}

EventLoop::~EventLoop()
{
    close(_epfd);
}
void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}
void EventLoop::waitEpollFd()
{
    int nready = 0;
    do
    {
        nready = epoll_wait(_epfd, &*_evList.begin(), _evList.size(), 3000);
    }while(-1 == nready && errno == EINTR);
	if(-1 == nready) 
        {
			perror("epoll_wait");
			return;
		} 
    else if(0 == nready) 
        {
			printf(">> epoll_wait timeout!\n");
		} 
    else
        {
            //可以判断一下，文件描述符是不是已经达到了1024
            //如果达到1024就需要进行扩容
            if(nready == (int)_evList.size())
            {
                 _evList.reserve(2 * nready);
            }
            for(int idx = 0; idx < nready; ++idx)
            {
                if((_evList[idx].data.fd==_acceptor.getFd())&&
                    (_evList[idx].events&EPOLLIN))
                    {
                        handleNewConnection();
                    }
                else
                {
                    if(_evList[idx].events & EPOLLIN)
                    {   int fd=_evList[idx].data.fd;
                        handleMessage(fd);
                    }
                }
            }
        }
}
void EventLoop::handleNewConnection()
{
    int peerfd=_acceptor.accept();
    if(peerfd < 0)
    {
        perror("handleNewConnection accept");
        return;
    }
    TcpConnectionPtr con (new TcpConnection(peerfd));
    con->setNewConectionCallback(_onNewConnectionCb);
    con->setMessageCallback(_onMessageCb);
    con->setCloseCallback(_onCloseCb);

    con->handleNewConectionCallback();
    _conns[peerfd]=con;

    addEpollReadFd(peerfd);
}
void EventLoop::handleMessage(int  fd)
{        

    auto it = _conns.find(fd);
    if(it!=_conns.end()){
        bool flag=it->second->isClosed();
        cout<<flag<<endl;
        if(flag)
        {
            it->second->handleCloseCallback();
            delEpollReadFd(it->first);
            _conns.erase(it);
        }
        else
        {
            it->second->handleMessageCallback();
        }

    }
    else
    {
        cerr<<"handleMessage error"<<endl;
        return ;
    }
}

int  EventLoop::createEpollFd()
{
    return epoll_create(1);
}
void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event ev;
    ev.events=EPOLLIN||EPOLLOUT;
    ev.data.fd=fd;
    int ret=::epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
    if(ret < 0) 
    {
		perror("epoll_ctl");
	}
    return ;
}
void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event ev;
    ev.events=EPOLLIN||EPOLLOUT;
    ev.data.fd=fd;
    int ret=::epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
    if(ret < 0) 
    {
		perror("epoll_ctl");
	}
    return ;
}


void EventLoop::setNewConectionCallback(TcpConnectionCallback &&cb)
{  
        _onNewConnectionCb=std::move(cb);

}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
   
        _onMessageCb=std::move(cb);
    

}
void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{

        _onCloseCb=std::move(cb);
 
}