#include<stdio.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/epoll.h>
int main(){
int sockfd =socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in servaddr;
memset(&servaddr,0,sizeof(struct sockaddr_in));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
servaddr.sin_port=htons(2048);
if(-1==bind(sockfd,(struct sockaddr*)&servaddr,sizeof(struct sockaddr))){
	printf("bind is failed %s",strerror(errno));
	return -1;}
listen(sockfd ,10);

int epfd=epoll_create(1);
struct epoll_event ev;
ev.events=EPOLLIN;
ev.data.fd=sockfd;
int maxfd=sockfd;
epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
struct epoll_event events[1024]={0};;
while(1){
	int nready=epoll_wait(epfd,events,maxfd,-1);
	if(nready<=0) continue;
	int i=0;
	for(i;i<nready;i++){
	    int connfd=events[i].data.fd;
		if(connfd==sockfd){
			struct sockaddr_in clientaddr;
			socklen_t len=sizeof(clientaddr);
			int clientfd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
			ev.data.fd=clientfd;
			ev.events=EPOLLIN|EPOLLET;
			epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&ev);}
		else{
			char buffer[1024]={0};
			short len=0;
			recv(connfd,&len,2,0);
			len=ntohs(len);
			int ret =recv(connfd,buffer,len,0);
			if(ret==0){
				epoll_ctl(epfd,EPOLL_CTL_DEL,connfd,NULL);
				close(connfd);
				printf("disconnect");
				continue;}
			send(connfd,buffer,ret,0);
			printf("clientfd:%d,ret:%d ,buffer:%s\n",connfd,ret,buffer);}}}
			getchar();}
			


