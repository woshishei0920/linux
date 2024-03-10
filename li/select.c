#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>



int main() {
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(struct sockaddr_in)); 
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(2048);


    if (-1 == bind(sockfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr))) {
		printf("bind failed: %s", strerror(errno));
		return -1;
    }


    listen(sockfd, 10); 

   printf("loop");
   fd_set  rfds,rset;
   FD_ZERO(&rfds);
   FD_SET(sockfd,&rfds);
   int maxfd=sockfd+1;
   while(1){
    rset=rfds;
	int nready=select(maxfd,&rset,NULL,NULL,NULL);

    if(FD_ISSET(sockfd,&rset)){
		struct sockaddr_in clientaddr;
		socklen_t len=sizeof(clientaddr);
		int clientfd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
	    maxfd=clientfd+1;
		FD_SET(clientfd,&rfds);
		}
	int i=0;
	for(i=sockfd+1;i<maxfd;i++){
		if(FD_ISSET(i,&rset)){


			char buffer[1024]={0};

			
			int ret=recv(i,buffer,1024,0);
			if(ret==0){
				printf("disconnect");
				close(i);
				FD_CLR(i,&rfds);
				break;}
			send(i,buffer,ret,0);
			printf("clientfd:%d,ret:%d ,buffer:%s\n",i,ret,buffer);}}}
			getchar();}
