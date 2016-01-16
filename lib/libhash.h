#ifndef LIBHASH_H
#define LIBHASH_H

#define hashFunction bernstein_hash

typedef unsigned long int hash_t;

typedef struct doc {
	int id;
	float score;
} DocResult;

typedef struct {
	int id;
	float norm;
} DocInfo;

#endif