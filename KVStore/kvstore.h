#ifndef _KVSTORE_H_
#define _KVSTORE_H_

#include<stddef.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>

#define BUFFER_LENGTH 512


typedef int (*RCALLBACK)(int fd);

struct conn_item{
	int fd;

    char rbuffer[BUFFER_LENGTH];
	int rlen;
	char wbuffer[BUFFER_LENGTH];
	int wlen;

    char resource[BUFFER_LENGTH];
	
	union{
		RCALLBACK accept_callback;
		RCALLBACK recv_callback;
		}recv_t;
	RCALLBACK send_callback ;
};

int epoll_entry(void);
int kvstore_request(struct conn_item *item);
void *kvstore_malloc(size_t size);
void kvstore_free(void *ptr);
int ntyco_entry(void);
int io_uring_entry(void);
int init_server(unsigned short port);

#define NETWORK_EPOLL 0
#define NETWORK_ETYCO 1
#define NTEWORK_IOURING 2



#define ENABLE_APPAY_KVENGING 1
#define ENABLE_NETWORK_SELECT NETWORK_EPOLL


#if ENABLE_APPAY_KVENGING
struct kvs_array_item_s{
     char *key;
	 char *value;
 
};

char* kvstore_array_get(char *key);
int kvstore_array_set(char *key,char *value);

#define KVS_ARRAY_SIZE 1024
#endif

#endif



