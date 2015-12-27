#include "libinverted_list.h"

hash_t bernstein_hash(char *key) {
	hash_t h=0;
	while(*key) h=33*h + *key++;
	return h;
}

InvListNode *allocateDoc(int id_cat, int freq) {
	InvListNode *doc_node;

	doc_node = (InvListNode*) malloc(sizeof(InvListNode));
	doc_node->doc.id = id_cat;
	doc_node->doc.freq = freq;
	doc_node->next = NULL;

	return doc_node;
}

void insertList(TermNode *term_node, int id_cat, int freq) {
	InvListNode *aux1, *aux2;

	if(term_node->list == NULL) {
		term_node->list = allocateDoc(id_cat, freq);
		term_node->term.list_size = 1;
		term_node->term.acc_frequency = freq;
	}
	else {
		aux1 = term_node->list;
		while(aux1) {
			if(aux1->doc.id == id_cat) {
				aux1->doc.freq += freq;
				term_node->term.acc_frequency += freq;
				break;
			}
			aux2 = aux1;
			aux1 = aux1->next;
		}
		if(aux1 == NULL) {
			aux2->next = allocateDoc(id_cat, freq);
			term_node->term.list_size += 1;
			term_node->term.acc_frequency += freq;
		}
		aux1 = NULL; aux2 = NULL;
	}
}

TermNode *allocateTerm(char term[], long int offset = 0, int acc_frequency = 0, int list_size = 0) {
	TermNode *aux;

	aux = (TermNode*) malloc(sizeof(TermNode));
	strcpy(aux->term.str, term);

	aux->term.acc_frequency = acc_frequency;
	aux->term.list_size = list_size;
	aux->term.offset = offset;
	aux->list = NULL;
	aux->next_term = NULL;

	return aux;
}

TermNode *findTermOrAlloc(TermNode *first, char term_str[], long int offset = 0, int acc_frequency = 0, int list_size = 0) {
	TermNode *aux1, *aux2;

	aux1 = first;
	while(aux1) {
		if(strcmp(aux1->term.str, term_str)==0) {
			return aux1;
		}

		aux2 = aux1;
		aux1 = aux1->next_term;
	}

	aux2->next_term = allocateTerm(term_str, offset, acc_frequency, list_size);
	return aux2->next_term;
}

void indexTerm(Index h_index[], char term_str[], int id_doc, int freq, int hash_size) {
	hash_t index;
	TermNode *aux;

	index = hashFunction(term_str) % hash_size;

	if(h_index[index] == NULL) {
		h_index[index] = allocateTerm(term_str);
		insertList(h_index[index], id_doc, freq);

	} else {
		aux = findTermOrAlloc(h_index[index], term_str);
		insertList(aux, id_doc, freq);
	}
}

void initIndex(Index *index, int size) {
	int i;

	for (i = 0; i < size; i++) {
		index[i] = NULL;
	}
}

void printIndex(Index *index, int size) {
	int i;
	InvListNode *aux;

	for (i = 0; i < size; i++) {
		if(index[i] != NULL) {
			printf("[%s] -> |", index[i]->term.str);
			aux = index[i]->list;
			while(aux) {
				printf("%d| ", aux->doc.id);
				aux = aux->next;
			}
			printf("\n");
		}
	}
}

int storeIndex(Index h_index[], int n_docs, int hash_size, const char index_name[]) {
	FILE *index_file;
	TermNode *term_aux, *termtrash;
	InvListNode *doc_aux, *doctrash;
	int i, n=0;

	if((index_file = fopen(index_name, "wb")) == NULL) {
		printf("Fail to create index file [%s].", index_name);
	}

	fwrite(&n_docs, sizeof(int), 1, index_file);
	
	for (i = 1; i < hash_size; i++) {
		term_aux = h_index[i];

		while(term_aux) {
			term_aux->term.offset = ftell(index_file) + sizeof(Term);		
			fwrite(&(term_aux->term), sizeof(Term), 1, index_file);

			doc_aux = term_aux->list;

			while(doc_aux) {
				fwrite(&(doc_aux->doc), sizeof(Doc), 1, index_file);

				doctrash = doc_aux;
				doc_aux = doc_aux->next;
				free(doctrash);
			}

			termtrash = term_aux;
			term_aux = term_aux->next_term;
			free(termtrash);
			n++;
		}
	}
	fclose(index_file);

	return n;
}

