#include<sys/epoll.h>
#include<sys/socket.h>
#include<string.h>
#include<stdio.h>
#include<netinet/in.h>
#include<errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/time.h>
#include<stdlib.h>

#define BUFFER_LENGTH 512
#define ENABLE_HTTP_RESPONSE 1

typedef int (*RCALLBACK)(int fd);

int connect_cb(int fd,const char *ip,int port);
int recv_cb(int fd);
int send_cb(int fd);

struct conn_item{
	int fd;

    char rbuffer[BUFFER_LENGTH];
	int rlen;
	char wbuffer[BUFFER_LENGTH];
	int wlen;

    char resource[BUFFER_LENGTH];
	
	union{
		RCALLBACK connect_callback;
		RCALLBACK recv_callback;
		}recv_t;
	RCALLBACK send_callback ;
};

int epfd=0;//全局变量
struct conn_item connlist[1048576]={0};  //分配数组大于2G 会出错
struct timeval n_time;

#define TIME_SUB_MS(tv1,tv2) ((tv1.tv_sec -tv2.tv_sec)*1000+(tv1.tv_usec-tv2.tv_usec)/1000)

#if ENABLE_HTTP_RESPONSE


#define Root_Dir  "/home/share/li"

typedef struct conn_item  connection_t;

int http_request(connection_t *conn){
     



    return 0;
}

int http_response(connection_t *conn){
#if 0
   conn->wlen=sprintf(conn->wbuffer,
		"HTTP/1.1 200 0K\r\n"
		"Accept-Ranges: bytes\r\n"
		"content-Length: 78\r\n"
        "Content-Type:text/html\r\n"
        "Date:Sat,06 Aug 2022 13:16:46 GMT\r\n\r\n"
        "<html><head><title>0voice.king</title></head><body><h1>King</h1></body></html>");
#else
    int filefd=open("index.html",O_RDONLY);

    struct stat stat_buf;
	fstat(filefd,&stat_buf);
	
    conn->wlen=sprintf(conn->wbuffer,
		"HTTP/1.1 200 0K\r\n"
		"Accept-Ranges: bytes\r\n"
		"content-Length: %ld\r\n"
        "Content-Type: text/html\r\n"
        "Date:Sat,06 Aug 2023 13:16:46 GMT\r\n\r\n",stat_buf.st_size);
	int count=read(filefd,conn->wbuffer+conn->wlen,BUFFER_LENGTH-conn->wlen);
	conn->wlen+=count;
    
#endif
   return conn->wlen;
  
}

#endif


int set_events(int fd,int event,int flag){
	//flag-1 add -0 mod
	struct epoll_event ev;
	ev.data.fd=fd;
	ev.events=event;
	if(flag){
		epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
		}
	else{
		epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);
		}
}

int connect_cb(int fd,const char *ip,int port){
	struct sockaddr_in p2paddr;
	socklen_t len=sizeof(p2paddr);
	memset(&p2paddr,0,sizeof(struct sockaddr_in));
	p2paddr.sin_family=AF_INET;
	p2paddr.sin_addr.s_addr=inet_addr(ip);
	p2paddr.sin_port=htons(port);
	
    return connect(fd,(struct sockaddr*)&p2paddr,sizeof(struct sockaddr));
}

int recv_cb(int fd){
	char *buffer=connlist[fd].rbuffer;
	short idx=connlist[fd].rlen;

    int count=recv(fd,buffer+idx,BUFFER_LENGTH-idx,0);

    if(count==0){
		printf("disconnect\n");

	    epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
		close(fd);
		return -1;
    }
	connlist[fd].rlen+=count;
#if 1 //echo:need to send
    memcpy(connlist[fd].wbuffer, connlist[fd].rbuffer, connlist[fd].rlen);
	connlist[fd].wlen = connlist[fd].rlen;	
	connlist[fd].rlen -= connlist[fd].rlen;
#else
    http_request(&connlist[fd]);
    http_response(&connlist[fd]);
#endif
	set_events(fd,EPOLLOUT,0);
  	
	return count;
}

int send_cb(int fd){
	char *buffer=connlist[fd].wbuffer;
	int idx=connlist[fd].wlen;
	int count=send(fd,buffer,idx,0);

    set_events(fd,EPOLLIN,0);
	return count;
}

int init_server(unsigned short port){
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(struct sockaddr_in));
		serveraddr.sin_family = AF_INET;	
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serveraddr.sin_port = htons(port);
        if (-1 == bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr))){	
		  perror("bind");		
		  return -1;	
		}
		
		return sockfd;
}

int main(int argc, char **argv) {
	if (argc <= 2) {
		printf("Usage: %s ip port\n", argv[0]);
		exit(0);
	}
  const char *ip = argv[1];
  int port_p = atoi(argv[2]);
  int port_num=1;
  int i=0;
  epfd=epoll_create(1);
  
  int sockfd=init_server(port_p);
  connlist[sockfd].fd=sockfd;
  connlist[sockfd].recv_t.connect_callback=recv_cb;
  set_events(sockfd,EPOLLIN,1);

  gettimeofday(&n_time,NULL);
  struct epoll_event events[1024]={0};
  while(1){
  	if (0 == connect_cb(sockfd, ip, port_p)) {
		printf("connect success!\n");
		break;
		}}
	
  while(1){
  	
  	int nready=epoll_wait(epfd,events,1024,0);
	int i=0;
    for(i=0;i<nready;i++){
        int connfd=events[i].data.fd;
		//printf("connfd:%d",connfd);
		if(events[i].events&EPOLLIN){
				int count = connlist[connfd].recv_t.recv_callback(connfd);
				//printf("recv count: %d <-- buffer: %s\n", count, connlist[connfd].rbuffer);
				} 
		else if (events[i].events & EPOLLOUT) { 
			  //  printf("send --> buffer: %s\n",  connlist[connfd].wbuffer);
				int count = connlist[connfd].send_callback(connfd);
		}
	}
  }
    getchar();
}


  
