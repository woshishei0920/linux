#include<stdio.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<poll.h>

int main() {
	//open
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); // io
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(struct sockaddr_in)); // 192.168.2.123
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    servaddr.sin_port = htons(2048);

    if (-1 == bind(sockfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr))) {
		printf("bind failed: %s", strerror(errno));
		return -1;
    }

    listen(sockfd, 10); 

    //sleep(10);


	struct pollfd fds[1024]={0};
	fds[sockfd].fd=sockfd;
	fds[sockfd].events=POLLIN;
    int maxfd=sockfd+1;
	int clientfd;
	while(1){
		int nready=poll(fds,maxfd,-1);
		if(fds[sockfd].revents&POLLIN){
			struct sockaddr_in clientaddr;
	        socklen_t len=sizeof(clientaddr);
			clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
			fds[clientfd].fd=clientfd;
			fds[clientfd].events=POLLIN;
			maxfd=maxfd>(clientfd+1)?maxfd:(clientfd+1);
			if(--nready==0) continue;
			}
		int i=0;
		for(i=sockfd+1;i<maxfd;i++){
			if(fds[i].revents&POLLIN){
				char buffer[1024]={0};
				int ret=recv(i,buffer,1024,0);
				if(ret==0){
					fds[i].fd=-1;
					fds[i].events=0;
					close(i);
					continue;
					} 
			send(i,buffer,ret,0);
			printf("clientfd:%d,ret:%d ,buffer:%s\n",i,ret,buffer);}}}}
