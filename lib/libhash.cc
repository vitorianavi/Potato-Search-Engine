#include "libhash.h"

hash_t bernstein_hash(char *key) {
	hash_t h=0;
	while(*key) h=33*h + *key++;
	return h;
}


void initHashResult(DocResult hash[], int size) {
	int i;
	for(i = 0; i < size; i++) {
		hash[i].id = 0;
		hash[i].score = 0.0;
	}
}

hash_t findPositionHashResult(int id_doc, DocResult *hash, int size) {
	hash_t position = id_doc % size;
	
	while(1) {
		if((hash[position].id == 0) || (hash[position].id == id_doc)){
			return position;
		} else{
			position++;
			if(position == size) { 
				position = 0; 
			}
		}
	}
}

void insertHashResult(DocResult result, DocResult *hash, int size) {
	hash_t position;
	
	position = findPositionHashResult(result.id, hash, size);
	
	if(hash[position].id == 0){
		hash[position].id = result.id;
	}
	
	hash[position].score += result.score;
}

void initHashInfo(DocInfo hash[], int size) {
	int i;
	for(i = 0; i < size; i++) {
		hash[i].id = 0;
		hash[i].norm = 0.0;
	}
}

hash_t findPositionHashInfo(int id_doc, DocInfo *hash, int size) {
	hash_t position = id_doc % size;
	
	while(1) {
		if((hash[position].id == 0) || (hash[position].id == id_doc)){
			return position;
		} else{
			position++;
			if(position == size) { 
				position = 0; 
			}
		}
	}
}

void insertHashInfo(int doc_id, float norm, DocInfo *hash, int size) {
	hash_t position;
	
	position = findPositionHashInfo(doc_id, hash, size);
	
	if(hash[position].id == 0){
		hash[position].id = doc_id;
	}
	
	hash[position].norm += norm;
}

