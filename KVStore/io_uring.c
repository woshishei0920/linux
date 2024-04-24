#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<liburing.h>
#include<string.h>
#include"kvstore.h"

#define ENTRIES_LENGTH 1024
#define EVENT_ACCEPT 0
#define EVENT_READ 1
#define EVENT_WRITE 2
#define BUFFER_LENGET 1024
struct conn_info{
    int fd;
	int event;

};

int set_event_accept(struct io_uring *ring,int sockfd,struct sockaddr *addr,
                      socklen_t *addrlen,int flags){
    struct io_uring_sqe* sqe = io_uring_get_sqe(ring);                  

	struct conn_info accept_info={
		.fd=sockfd,
		.event=EVENT_ACCEPT,
	};
	io_uring_prep_accept(sqe, sockfd, (struct sockaddr*)&addr, addrlen,flags);
    memcpy(&sqe->user_data,&accept_info,sizeof(struct conn_info));

    }

int set_event_recv(struct io_uring *ring,int sockfd,void *buf,
                      size_t len,int flags){
    struct io_uring_sqe* sqe = io_uring_get_sqe(ring);                  
	struct conn_item item = { 0 };

	struct conn_info accept_info={
		.fd=sockfd,
		.event=EVENT_READ,
	};
	//io_uring_prep_recv(sqe, sockfd, buf, len,flags);
	
	//memcpy(item.rbuffer, buf,sizeof(buf));
	//item.rlen = strlen(item.rbuffer);
   // 	kvstore_request(&item);
    memcpy(&sqe->user_data,&accept_info,sizeof(struct conn_info));

    }
int set_event_send (struct io_uring *ring,int sockfd,void *buf,
                      size_t len,int flags){
    struct io_uring_sqe* sqe = io_uring_get_sqe(ring);                  

	struct conn_info accept_info={
		.fd=sockfd,
		.event=EVENT_WRITE,
	};
	io_uring_prep_send(sqe, sockfd, buf, len,flags);
    memcpy(&sqe->user_data,&accept_info,sizeof(struct conn_info));

    }
#if 0 
int init_server(unsigned short port){
    int sockfd =socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serveradd;
	memset(&serveradd,0,sizeof(struct sockaddr_in));
	serveradd.sin_family=AF_INET;
	serveradd.sin_addr.s_addr=htonl(INADDR_ANY);
	serveradd.sin_port=htons(port);

	if(-1==bind(sockfd,(struct sockaddr*)&serveradd,sizeof(struct sockaddr))){
		perror("bind");
		return -1;}

	listen(sockfd,10);
	return sockfd;


   }
#endif

#if 0
int main(int argc,char *argv[]){

	unsigned short port=2048;
	int sockfd =init_server( port);

	struct io_uring_params params;
	memset(&params,0,sizeof(struct io_uring_params));

	struct io_uring ring;
	io_uring_queue_init_params(ENTRIES_LENGTH,&ring,&params);

	
#if 0
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);

	accept(sockfd,(struct sockaddr *sockaddr)clienraddr,&len);
#else 
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);

	set_event_accept(&ring,sockfd,(struct sockaddr * )&clientaddr,&len,0);
    
#endif
char buffer[1024]={};
	while (1) {
		io_uring_submit(&ring);

		struct io_uring_cqe* cqe;
		io_uring_wait_cqe(&ring, &cqe);//调用 io_uring_wait_cqe 时，它都会阻塞，直到有 CQE 可用或超时为止

		struct io_uring_cqe* cqes[128];
		int nready=io_uring_peek_batch_cqe(&ring, cqes, 128);  //epoll_wait 立即返回可用的 CQE 数量
		int i = 0;
		for (i = 0; i < nready; i++) {
			
			
			struct io_uring_cqe *entries=cqes[i];
            struct conn_info res_info;
			memcpy(&res_info,&entries->user_data,sizeof(struct conn_info));
			if(res_info.event==EVENT_ACCEPT){
				set_event_accept(&ring,sockfd,(struct sockaddr * )&clientaddr,&len,0);
			    printf("success accept \n");

				
			    int connfd=entries->res;
				set_event_recv(&ring,connfd,buffer,BUFFER_LENGET,0);
		}
			else if(res_info.event==EVENT_READ){
				int length=entries->res;
                printf("success recv :%s,length:%d\n",buffer,length);
				if(length<=0){
					close(res_info.fd);}
				else if(length>0){
			        set_event_send(&ring,res_info.fd,buffer,length,0);	
			   }
			}
			else if(res_info.event==EVENT_WRITE){
			        int length=entries->res;
			        printf("success send :%s,length:%d\n",buffer,length);

                    set_event_recv(&ring,res_info.fd,buffer,BUFFER_LENGET,0);	
			}
			
	}

	    io_uring_cq_advance(&ring,nready);
	
	
	
	}
}
#else 
int io_uring_entry(void ) {

	unsigned short port = 2048;
	int sockfd = init_server(port);

	struct io_uring_params params;
	memset(&params, 0, sizeof(struct io_uring_params));

	struct io_uring ring;
	io_uring_queue_init_params(ENTRIES_LENGTH, &ring, &params);


#if 0
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);

	accept(sockfd, (struct sockaddr* sockaddr)clienraddr, &len);
#else 
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);

	set_event_accept(&ring, sockfd, (struct sockaddr*)&clientaddr, &len, 0);

#endif
	char buffer[1024] = {};
	while (1) {
		io_uring_submit(&ring);

		struct io_uring_cqe* cqe;
		io_uring_wait_cqe(&ring, &cqe);//调用 io_uring_wait_cqe 时，它都会阻塞，直到有 CQE 可用或超时为止

		struct io_uring_cqe* cqes[128];
		int nready = io_uring_peek_batch_cqe(&ring, cqes, 128);  //epoll_wait 立即返回可用的 CQE 数量
		int i = 0;
		for (i = 0; i < nready; i++) {


			struct io_uring_cqe* entries = cqes[i];
			struct conn_info res_info;
			memcpy(&res_info, &entries->user_data, sizeof(struct conn_info));
			if (res_info.event == EVENT_ACCEPT) {
				set_event_accept(&ring, sockfd, (struct sockaddr*)&clientaddr, &len, 0);
				printf("success accept \n");


				int connfd = entries->res;
				set_event_recv(&ring, connfd, buffer, BUFFER_LENGET, 0);
			}
			else if (res_info.event == EVENT_READ) {
				int length = entries->res;
				printf("success recv :%s,length:%d\n", buffer, length);
				if (length <= 0) {
					close(res_info.fd);
				}
				else if (length > 0) {
					set_event_send(&ring, res_info.fd, buffer, length, 0);
				}
			}
			else if (res_info.event == EVENT_WRITE) {
				int length = entries->res;
				printf("success send :%s,length:%d\n", buffer, length);

				set_event_recv(&ring, res_info.fd, buffer, BUFFER_LENGET, 0);
			}

		}

		io_uring_cq_advance(&ring, nready);



	}
}

#endif

