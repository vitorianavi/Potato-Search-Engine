#ifndef LIBLISTA_INVERTIDA_H
#define LIBLISTA_INVERTIDA_H

#include "libgeneral.h"

#define hashFunction bernstein_hash

typedef struct {
	int id;
	int freq;
} Doc;

typedef struct list_node {
	Doc doc;
	struct list_node *next;
} InvListNode;

typedef struct {
	char str[MAX_TERM_SIZE];
	int acc_frequency;
	long int offset;
	int list_size;
} Term;

typedef struct term_node {
	Term term;
	InvListNode *list;
	struct term_node *next_term;
} TermNode;

typedef TermNode* Index;
typedef unsigned long int hash_t;

#endif