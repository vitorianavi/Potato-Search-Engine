#include "libindexer.h"

using namespace std;

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

void indexTerm(Index h_index[], char term_str[], int id_doc) {
	hash_t index;
	TermNode *aux;

	index = hashFunction(term_str) % INDEX_SIZE;

	if(h_index[index] == NULL) {
		h_index[index] = allocateTerm(term_str);
		insertList(h_index[index], id_doc, 1);

	} else {
		aux = findTermOrAlloc(h_index[index], term_str);
		insertList(aux, id_doc, 1);
	}
}

void calcNormDocuments(DocInfo hash_info[]) {
	int i;

	for (i = 0; i < HASH_SIZE; i++) {
		if(hash_info[i].id != 0) {
			hash_info[i].norm = sqrt(hash_info[i].norm);
		}
	}
}

void calcTfIdf(Index index[], DocInfo hash_info[], int n_docs) {
	TermNode *term_aux;
	InvListNode *doc_aux;
	float weig;
	int i;

	for (i = 0; i < INDEX_SIZE; i++) {
		term_aux = index[i];
		while(term_aux) {
			term_aux->term.idf = (float) n_docs/(float)term_aux->term.list_size;
			term_aux->term.idf = logf(1+term_aux->term.idf);

			doc_aux = term_aux->list;
			while(doc_aux) {
				doc_aux->doc.tf = 1+logf(doc_aux->doc.tf);
				weig = doc_aux->doc.tf*term_aux->term.idf;
				insertHashInfo(doc_aux->doc.id, weig*weig, hash_info, HASH_SIZE);

				doc_aux = doc_aux->next;
			}

			term_aux = term_aux->next_term;
		}
	}

	calcNormDocuments(hash_info);
}

int storeIndex(Index h_index[], const char index_name[]) {
	FILE *index_file;
	TermNode *term_aux, *termtrash;
	InvListNode *doc_aux, *doctrash;
	int i, n=0;

	if((index_file = fopen(index_name, "wb")) == NULL) {
		printf("Fail to create index file [%s].", index_name);
	}
	
	for (i = 1; i < INDEX_SIZE; i++) {
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

void storeDocInfo(DocInfo hash_info[], int n_docs, const char file_name[]) {
	FILE *docinfo_file;
	int i;

	if((docinfo_file = fopen(file_name, "wb")) == NULL) {
		printf("Fail to create index file [%s].", file_name);
	}

	fwrite(&n_docs, sizeof(int), 1, docinfo_file);

	for (i = 0; i < HASH_SIZE; i++)	{
		if(hash_info[i].id != 0) {
			fwrite(&(hash_info[i]), sizeof(DocInfo), 1, docinfo_file);
		}
	}

	fclose(docinfo_file);
}

void index(char *dir_path) {
	DIR *dir;
	struct dirent *ent;
	string line, term;
	int id, stop = 0;
	char dterm[MAX_TERM_SIZE], norm_term[MAX_TERM_SIZE], file_name[300];

	Index *index;
	DocInfo hash_info[HASH_SIZE];
	unordered_map<string, int> stopWordsHash;
	int n_docs = 0, n_terms;
	
	index = (Index*) malloc(sizeof(Index)*INDEX_SIZE);
	initIndex(index, INDEX_SIZE);
	initHashInfo(hash_info, HASH_SIZE);
	loadStopWordsHash(stopWordsHash, STOPWORDS_FILE);

	if ((dir = opendir(dir_path)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			strcpy(file_name, dir_path);
			strcat(file_name, ent->d_name);

			ifstream file(file_name, ifstream::in);
			printf("%s\n", file_name);

			while(getline(file, line)) {

				stringstream lineStream(line);

				if(!line.empty()) {
					lineStream >> term;
					
					if(term.compare("RN") == 0) {
						stop = 0;
						lineStream >> term;
						id = atoi(term.c_str());

						printf("id: %d\n", id);
						n_docs++;

					} else if(term.compare("TI") == 0 || term.compare("MJ") == 0 || term.compare("MN") == 0) {

						while(lineStream >> term) {
							strcpy(dterm, term.c_str());
							normText(dterm, norm_term);
							if(stopWordsHash.count(string(norm_term)) == 0 && *norm_term != 0) {
								indexTerm(index, norm_term, id);
							}
						}
					} else if(!stop && term.compare("PN") != 0 && term.compare("AN") != 0 && term.compare("AU") != 0 && term.compare("SO") != 0 && term.compare("RF") != 0 && term.compare("CT") != 0)	{		

						do {
							strcpy(dterm, term.c_str());
							normText(dterm, norm_term);
							if(stopWordsHash.count(string(norm_term)) == 0 && *norm_term != 0) {
								indexTerm(index, norm_term, id);
							}

						} while(lineStream >> term);

					} else if(term.compare("RF") == 0) {
						stop = 1;
					}

					term.clear();
					*dterm = 0;
					*norm_term = 0;
				}
			}
		}
		calcTfIdf(index, hash_info, n_docs);
	//	printIndex(index, INDEX_SIZE);
		n_terms = storeIndex(index, INDEX_FILE);
		storeDocInfo(hash_info, n_docs, DOCINFO_FILE);
		
		free(index);
		closedir(dir);

		printf("Documents: %d\nTerms:%d\n", n_docs, n_terms);

	} else {
		printf("Could not open directory [%s].", dir_path);
	}

}