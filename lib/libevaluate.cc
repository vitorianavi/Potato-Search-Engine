#include "libevaluate.h"

void initQuery(Query *q, int n_relevants) {
	int i;
	q->relevants = (int*) malloc(sizeof(int)*n_relevants);
	q->n_relevants = 0;
}

void insertRelevant(Query *q, int rId) {
	q->relevants[q->n_relevants] = rId;
	q->n_relevants++;

	//printf("q: %d r: %d\n", q->id, rId);
}

int findRelevant(int v[], int n, int id) {
	int m, start = 0, end = n-1;

	while(start <= end) {
		m = (start + end)/2;
		if(id > v[m]) {
			start = m+1;
		} 
		else if (id < v[m]) {
			end = m-1;
		}
		else {
			return 1;
		}
	}
	return 0;
}

float calcMAPQuery(Query q, DocResult ranking[], int n_docs) {
	int i, n=0, relev=0;
	float map=0;

	//printf("ndocs %d\n", n_docs);

	for (i = 0; relev != q.n_relevants; i++) {
		if(findRelevant(q.relevants, q.n_relevants, ranking[i].id)) {
			relev++;
			map += (float) relev/ (float)(i+1);
		}
	}

	map /=q.n_relevants;

	return map;
}

float calcP10Query(Query q, DocResult ranking[]) {
	int i, relev=0;

	for (i = 0; i < 10; i++) {
		if(findRelevant(q.relevants, q.n_relevants, ranking[i].id)) {
		//	printf("%d\n", i+1);
			relev++;
		}
	}

	return (float) relev/10.0;
}