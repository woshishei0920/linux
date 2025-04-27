#include "InetAddress.h"
InetAddress::InetAddress(const string & ip, unsigned short port){
    memset(&_addr,0,sizeof(_addr));
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(port);
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());

}
InetAddress::InetAddress(struct sockaddr_in addr):_addr(addr)
{

}
InetAddress::~InetAddress()
{

}
string InetAddress::ip(){
    return inet_ntoa(_addr.sin_addr);
}
unsigned short InetAddress::port(){
    return ntohs(_addr.sin_port);
}
struct sockaddr_in * InetAddress::getInetAddress(){
    return &_addr;
}