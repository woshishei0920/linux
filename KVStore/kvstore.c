#include"kvstore.h"
#define KVSTORE_MAX_TOKENS 128

const char *commands[]={
  "SET","GET","DEL","MOD",

};

enum{
  KVS_CMD_START=0,
  KVS_CMD_SET=KVS_CMD_START,
  KVS_CMD_GET,
  KVS_CMD_DEL,
  KVS_CMD_MOD,
  KVS_CMD_COUNT,
  	

};

void *kvstore_malloc(size_t size){
	return malloc(size);
}
void kvstore_free(void *ptr){
	return free(ptr);
}

//rbuffer

//wbuffer

int kvstore_split_token(char *msg,char **tokens){
   if(msg==NULL||tokens==NULL) return -1;

   int index=0;
   char *token=strtok(msg," ");
   while(token!=NULL){
       tokens[index++]=token;
	   token=strtok(NULL," ");
   }

   return index;
}


int kvstore_parser_protocol(struct conn_item *item,char **tokens,int count){

	if(item ==NULL||tokens[0]==NULL||count==0) return -1;

	int cmd=KVS_CMD_START;

	for(cmd=KVS_CMD_START;cmd<KVS_CMD_COUNT;cmd++){
        if(strcmp(commands[cmd],tokens[0])==0){
			break;
		}

	}
	char *msg=item->wbuffer;
    switch(cmd){
		case KVS_CMD_SET:
			 printf("set\n");
			 int res=kvstore_array_set(tokens[1],tokens[2]);
			 printf("res:%d",res);
			 if(!res){
			 	  memset(msg,0,BUFFER_LENGTH);
                  snprintf(msg,BUFFER_LENGTH,"SUCCESS!\n");
			 }else{
                  snprintf(msg,BUFFER_LENGTH,"FAILD!\n");

			 }
			 
		     break;
		case KVS_CMD_GET:
			 printf("get\n");
			 char *value=kvstore_array_get(tokens[1]);
			 if(value){
			      snprintf(msg,BUFFER_LENGTH,"%s!\n",value);	
			 }else{
                  snprintf(msg,BUFFER_LENGTH,"FAILD!\n");

			 }
		     break;
		case KVS_CMD_DEL:
			 printf("del\n");
		     break;
		case KVS_CMD_MOD:
			 printf("mod\n");
		     break;
	    defualt:
			printf("cmd:%s\n",commands[cmd]);
			assert(0);
		
	}
}



//Set Key Value
int kvstore_request(struct conn_item *item){
	printf("recv:%s\n",item->rbuffer);
	char *msg=item->rbuffer;
	char *tokens[KVSTORE_MAX_TOKENS];
	int count=kvstore_split_token(msg,tokens);

	int idx=0;
	for(idx=0;idx<count;idx++){
	
		printf("idx:%s\n",tokens[idx]);
	}
	kvstore_parser_protocol(item,tokens,count);
	return 0;
	
}



int main(){
#if (ENABLE_NETWORK_SELECT==NETWORK_EPOLL)
	 epoll_entry();
#elif(ENABLE_NETWORK_SELECT==NETWORK_ETYCO)
     ntyco_entry();
#elif(ENABLE_NETWORK_SELECT==NETWORK_IOURING)
	io_uring_entry();
#endif

	}
