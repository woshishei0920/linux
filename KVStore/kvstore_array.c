#include"kvstore.h"

struct kvs_array_item_s array_table[KVS_ARRAY_SIZE]={0};
int array_idx=0;

int kvstore_array_set(char *key,char *value){
    if(key==NULL||value==NULL||array_idx==KVS_ARRAY_SIZE) return -1;

    char *kcopy=kvstore_malloc(strlen(key)+1);
	if(kcopy==NULL) return -1;

    strncpy(kcopy,key,strlen(key)+1);
	
	char *vcopy=kvstore_malloc(strlen(value)+1);
	if(vcopy==NULL) {
		kvstore_free(kcopy);
		return -1;
	}
	strncpy(vcopy,value,strlen(value)+1);
	
	array_table[array_idx].key=kcopy;
	array_table[array_idx].value=vcopy;
    array_idx ++;
	return 0;
	
}

char* kvstore_array_get(char *key){
    int idx=0;
    for(idx=0;idx<array_idx;idx++){
        if(strcmp(array_table[idx].key,key)==0){
			return array_table[idx].value;
		}

	}
	return NULL;

}

