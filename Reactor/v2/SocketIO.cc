#include "SocketIO.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
SocketIO::SocketIO(int fd):_fd(fd)
{

}
SocketIO::~SocketIO()
{
    close(_fd);
}
size_t SocketIO::readn(char * buf,size_t len)
{
    int left=len;
    char *pstr=buf;
    int ret=0;

    while(left>0)
    {
        ret =read(_fd,pstr,left);
        if(ret==-1)
        {
            perror("read error -1");
            return -1;
        }
        else if(ret==0)
        {
            break;
        }
        else
        {
            pstr=pstr+ret;
            left-=ret;
        }
    }
    return len-left;
}
size_t SocketIO::readLine(char *buf,size_t len)
{
    int left=len;
    char *pstr=buf;
    int ret=0,total=0;

    while(left>0)
    {
        ret =recv(_fd,pstr,left,MSG_PEEK);
        if(ret==-1)
        {
            perror("read error -1");
            return -1;
        }
        else if(ret==0)
        {
            break;
        }
        else
        {
            for(int idx=0;idx<ret;idx++)
                if(pstr[idx]=='\n')
                {
                    int sz=idx+1;
                    readn(pstr,sz);
                    pstr+=sz;
                    *pstr='\0';
                    return total+sz;
                }
            
            readn(pstr, ret);//从内核态拷贝到用户态
            total += ret;
            pstr += ret;
            left -= ret;
        }
    }
    *pstr = '\0';
    return len-left;
}
size_t SocketIO::writen(const char * buf,size_t len)
{
    int left = len;
    const char *pstr = buf;
    int ret = 0;

    while(left > 0)
    {
        ret = write(_fd, pstr, left);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("writen error -1");
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}

int SocketIO::getFd()
{
    return _fd;
}