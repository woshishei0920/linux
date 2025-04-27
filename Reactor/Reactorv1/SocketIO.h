#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__
#include <cstddef>
class SocketIO
{
public:
    SocketIO(int fd);
    ~SocketIO();
    size_t readn(char * buf,size_t len);
    size_t readLine(char *buf,size_t len);
    size_t writen(const char * buf,size_t len);

private:
    int _fd;
};
#endif 