
#include<sys/time.h>
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<arpa/inet.h>

#define TEST_MESSAGE "THIS IS TCP MEssSSAGETHIS IS TCP MESSAGETHIS IS TCP MESSAGEllll\n"
#define DUFFER_LENGTH 2048
#define TIME_SUB_MS(tv1, tv2)  ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec))
#define WBUFFER_LENGTH 2048


typedef struct test_context{
	char serverip[16];
	int port;
	int threadnum;
	int connectnum;
	int requestion;
#if 1
  int faild;
#endif

} test_context_t;

int connect_tcpserver(const char *ip ,int port){
    int connfd=socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in tcpserver_addr;
	memset(&tcpserver_addr,0,sizeof(struct sockaddr_in));
	
    tcpserver_addr.sin_family=AF_INET;
	tcpserver_addr.sin_addr.s_addr=inet_addr(ip);
	tcpserver_addr.sin_port=htons(port);

	int ret=connect(connfd,(struct sockaddr*)&tcpserver_addr,sizeof(struct sockaddr));
	if(ret<0){
	   perror("connect\n");
	   return -1;
	   }
    
	 return connfd; 
 
}

int send_recv_tcppkt(int fd){


#if 0
     int res=send(fd,TEST_MESSAGE,strlen(TEST_MESSAGE),0);
	 if(res<0){
	 	exit(1);
	 }

	 char buffer[DUFFER_LENGTH]={0};
     res=recv(fd,buffer,DUFFER_LENGTH,0);
	 if(res<=0){
	 	exit(1);
	 }
	 if(strcmp(buffer,TEST_MESSAGE)!=0){
        printf("faild :%s !=%s",buffer,TEST_MESSAGE);
		return -1;

	 }
#else
	char wbuffer[WBUFFER_LENGTH]={0};
	int i=0;
	for(i=0;i<3;i++){
       
	   strcpy(wbuffer+i*strlen(TEST_MESSAGE),TEST_MESSAGE);
	}
    int res=send(fd,wbuffer,strlen(wbuffer),0);
	if(res<0){
	 	exit(1);
	 }

	 char rbuffer[DUFFER_LENGTH]={0};
     res=recv(fd,rbuffer,DUFFER_LENGTH,0);
	 if(res<=0){
	 	exit(1);
	 }
	 if(strcmp(rbuffer,wbuffer)!=0){
        printf("faild :%s !=%s",rbuffer,wbuffer);
		return -1;

	 }    
	return 0;
#endif
}

static void *test_qps_entry(void *arg){
    
	test_context_t *pctx=(test_context_t *)arg;
    int conn_num=pctx->connectnum/pctx->threadnum;
	int num=0;
	int *connfd=malloc(conn_num*sizeof(int));
	for(num=0;num<conn_num;num++){
		connfd[num]=connect_tcpserver( pctx->serverip, pctx->port);
		if (connfd[num]<0){
		   printf("connfd is faild\n");
		   return NULL;
		}
		int count=pctx->requestion/pctx->threadnum;
		int i=0;
		for(i=0;i<count;i++){
		 	int res=send_recv_tcppkt(connfd[num]);
			if(res!=0){
				printf("send_recv is faild\n");
				pctx->faild++;
				continue;
			}   
		}
	}
	free(connfd);
    return NULL;
}


int main(int argc,char *argv[]){
   int ret=0;
   int opt;
   test_context_t ctx={0};
   while((opt=getopt(argc,argv,"s:p:t:c:n:"))!=-1){
        switch(opt){
        case 's':
			printf("-s:%s\n",optarg);
			strcpy(ctx.serverip,optarg);
			break;
		case 'p':
			printf("-p:%s\n",optarg);
			ctx.port=atoi(optarg);
			break;
		case 't':
			printf("-t:%s\n",optarg);
			ctx.threadnum=atoi(optarg);
			break;
		case 'c':
			printf("-c:%s\n",optarg);
			ctx.connectnum=atoi(optarg);
			break;
		case 'n':
			printf("-n:%s\n",optarg);
			ctx.requestion=atoi(optarg);
			break;
		default:
			return -1;
		}
    }



    pthread_t *ptid=malloc(ctx.threadnum*sizeof(pthread_t));
    int i=0;
	struct timeval tv_begin;
	gettimeofday(&tv_begin, NULL);
	for(i=0;i<ctx.threadnum;i++){
		pthread_create(&ptid[i],NULL,test_qps_entry,&ctx);
		}
    struct timeval tv_end;
	gettimeofday(&tv_end, NULL);
	int time_used=TIME_SUB_MS(tv_end, tv_begin);
    printf("success,time:%d,qps :%d\n",time_used,ctx.requestion*1000/time_used);

cleanup:	
	free(ptid);

	return 0;

}
