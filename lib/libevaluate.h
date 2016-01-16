#ifndef LIBEVALUATE_H
#define LIBEVALUATE_H

typedef struct {
	int id;
	int n_relevants=0;
	int *relevants=NULL;
} Query;

#endif