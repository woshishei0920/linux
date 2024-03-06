#include<sys/poll.h>
#include<stdio.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/epoll.h>

void *Client_Thread(void *arg){
    int clientfd=*(int *)arg;
	while(1){
		char buffer[128]={0};
		int ret=recv(clientfd,buffer,128,0);
		if (ret==0){
			close(clientfd);
			break;
			}
		printf("ret:%d ,buffer:%s",ret,buffer);
		send(clientfd,buffer,ret,0);}
		}
int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in servaddr;
	memset(&servaddr ,0,sizeof(struct sockaddr_in));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(2048);

	if(-1==bind(sockfd,(struct sockaddr *)& servaddr,sizeof(struct sockaddr))){
		printf(" bind is falid :%s",strerror(errno));
		return -1;}
listen(sockfd ,10);






#if 0
	while(1){
    struct sockaddr_in clientaddr;
	socklen_t len=sizeof(clientaddr);
	int clientfd =accept(sockfd,(struct sockaddr *)&clientaddr,&len);
    pthread_t thid;
	pthread_create(&thid,NULL,Client_Thread,&clientfd);
    }





#elif 0

    fd_set rfds,rset;
    FD_ZERO(&rfds);
	FD_SET(sockfd,&rfds);

	int maxfd=sockfd;
    printf("loop\n");
	while(1){
		rset=rfds;

		int nready=select(maxfd+1,&rset,NULL,NULL,NULL);
		printf("nready:%d\n",nready);

		if(FD_ISSET(sockfd,&rset)){
			    struct sockaddr_in clientaddr;
	            socklen_t len=sizeof(clientaddr);
				int clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);  

				printf("accept:%d",clientfd);
				FD_SET(clientfd,&rfds);
				maxfd=clientfd;}
		int i=0;
		for(i=sockfd+1;i<=maxfd;i++){
			if (FD_ISSET(i, &rset)) {
			char buffer[128]={0};

		    int ret=recv(i,buffer,128,0);
		    if (ret==0){
				printf("disconnect\n");
			    close(i);
			    break;
			}
		    
		    send(i,buffer,ret,0);
			printf("clientfd:%d,ret:%d ,buffer:%s\n",i,ret,buffer);}}
			
	}



#elif 0	
    struct pollfd fds[1024]={0};
    fds[sockfd].fd=sockfd;
	fds[sockfd].events=POLLIN;

	int maxfd=sockfd;
	while(1){
		int nready=poll(fds,maxfd+1,-1);
		if(fds[sockfd].revents & POLLIN){
			 struct sockaddr_in clientaddr;
	            socklen_t len=sizeof(clientaddr);
				int clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);  

				printf("accept:%d",clientfd);
				fds[clientfd].fd=clientfd;
	            fds[clientfd].events=POLLIN;
				maxfd=clientfd;}
				int i=0;
				for(i=sockfd+1;i<=maxfd;i++){
					if(fds[i].revents & POLLIN){
			char buffer[128]={0};

		    int ret=recv(i,buffer,128,0);
		    if (ret==0){
				printf("disconnect\n");
			    fds[i].fd=-1;
				fds[i].events=0;
			    close(i);
				continue;
			}
		    
		    send(i,buffer,ret,0);
			printf("clientfd:%d,ret:%d ,buffer:%s\n",i,ret,buffer);}}}


#else
      int epfd=epoll_create(1);//int size 
      struct epoll_event ev;
	  ev.events=EPOLLIN;
	  ev.data.fd=sockfd;

	  epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);

	  struct epoll_event events[1024]={0};
	  while(1){
          int nready=epoll_wait(epfd,events,1024,-1);
          int i=0;
		  for(i=0;i<nready;i++){
		  	int connfd=events[i].data.fd;
		  	if(sockfd==connfd){
                struct sockaddr_in clientaddr;
	            socklen_t len=sizeof(clientaddr);
				int clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);  

				ev.events=EPOLLIN;
				ev.data.fd=clientfd;
				epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&ev);
				printf("accept:%d",clientfd);
               
				
				}
			else if(events[i].events&EPOLLIN){
				char buffer[128]={0};

		        int ret=recv(connfd,buffer,128,0);
		        if (ret==0){
				    printf("disconnect\n");
			  
			     	epoll_ctl(epfd,EPOLL_CTL_DEL,connfd,NULL);
			        close(i);
				    continue;
			}
		    
		    send(connfd,buffer,ret,0);
			printf("clientfd:%d,ret:%d ,buffer:%s\n",connfd,ret,buffer);}
			}
		  
	  }
#endif
	getchar();
	 } 