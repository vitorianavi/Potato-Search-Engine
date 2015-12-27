#include "libhash.h"

hash_t bernstein_hash(char *key) {
	hash_t h=0;
	while(*key) h=33*h + *key++;
	return h;
}