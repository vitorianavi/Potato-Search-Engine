#include "libinverted_list.h"

InvListNode *allocateDoc(int id_cat, float tf) {
	InvListNode *doc_node;

	doc_node = (InvListNode*) malloc(sizeof(InvListNode));
	doc_node->doc.id = id_cat;
	doc_node->doc.tf = tf;
	doc_node->next = NULL;

	return doc_node;
}

TermNode *allocateTerm(char term[], long int offset = 0, int list_size = 0, float idf = 0) {
	TermNode *aux;

	aux = (TermNode*) malloc(sizeof(TermNode));
	strcpy(aux->term.str, term);

	aux->term.idf = idf;
	aux->term.list_size = list_size;
	aux->term.offset = offset;
	aux->list = NULL;
	aux->next_term = NULL;

	return aux;
}

TermNode *findTerm(TermNode *first, char term_str[]) {
	TermNode *aux;

	aux = first;
	while(aux) {
		if(strcmp(aux->term.str, term_str)==0) {
			break;
		}
		aux = aux->next_term;
	}

	return aux;
}

TermNode *findTermOrAlloc(TermNode *first, char term_str[], long int offset = 0, int list_size = 0, float idf = 0) {
	TermNode *aux1, *aux2;

	aux1 = first;
	while(aux1) {
		if(strcmp(aux1->term.str, term_str)==0) {
			return aux1;
		}

		aux2 = aux1;
		aux1 = aux1->next_term;
	}

	aux2->next_term = allocateTerm(term_str, offset, list_size, idf);
	return aux2->next_term;
}

void insertList(TermNode *term_node, int id_cat, float freq) {
	InvListNode *aux1, *aux2;

	if(term_node->list == NULL) {
		term_node->list = allocateDoc(id_cat, freq);
		term_node->term.list_size = 1;
	//	term_node->term.acc_frequency = freq;
	}
	else {
		aux1 = term_node->list;
		while(aux1) {
			if(aux1->doc.id == id_cat) {
				aux1->doc.tf += freq;
				//term_node->term.acc_frequency += 1;
				break;
			}
			aux2 = aux1;
			aux1 = aux1->next;
		}
		if(aux1 == NULL) {
			aux2->next = allocateDoc(id_cat, freq);
			term_node->term.list_size += 1;
			//term_node->term.acc_frequency += 1;
		}
		aux1 = NULL; aux2 = NULL;
	}
}