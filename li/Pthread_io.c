#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

void* Client_thread(void *arg){
   int clientfd=*(int *)arg;
   while(1){
      char buffer[1024]={0};
      int ret =recv(clientfd,buffer,1024,0);
      if (ret==0){
	  	close(clientfd);
		break;}
	  printf("ret:%d, buffer:%s", ret, buffer);
	  send(clientfd,buffer,ret,0);}}
int main(){
   int sockfd=socket(AF_INET,SOCK_STREAM,0);
   struct sockaddr_in servaddr;
   memset(&servaddr,0,sizeof(struct sockaddr_in));
   servaddr.sin_family=AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(2048);
   if (-1==bind(sockfd,(struct sockaddr *)&servaddr,sizeof(struct sockaddr))){
       printf("bind failed is %s",strerror(errno));
	   return-1;
       }

   listen(sockfd,10);
   
   while(1){
     struct sockaddr_in clientaddr;
	 socklen_t len=sizeof(clientaddr); 
	 int clientfd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);

	 pthread_t thid;
	 pthread_create(&thid,NULL,Client_thread,&clientfd);
	 }
   getchar();


}
