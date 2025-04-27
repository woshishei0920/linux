#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include<arpa/inet.h>
#include<string>
#include <cstring>
using std::string;

class InetAddress 
{
public:
    InetAddress(const string & ip,unsigned short port);
    InetAddress(struct sockaddr_in addr);
    ~InetAddress();
    string ip();
    unsigned short port();
    struct sockaddr_in * getInetAddress();
private:
    struct sockaddr_in _addr;
};
#endif 