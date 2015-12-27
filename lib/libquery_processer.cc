#include "libquery_processer.h"

using namespace std;


void indexRetrievTerm(Index index[], Term term) {
	hash_t i;
	TermNode *aux;

	i = hashFunction(term.str) % INDEX_SIZE;

	if(index[i] == NULL) {
		index[i] = allocateTerm(term.str, term.offset, term.acc_frequency, term.list_size);
	} else {
		aux = findTermOrAlloc(index[i], term.str, term.offset, term.acc_frequency, term.list_size);
	}
}

FILE *retrieveIndex(Index index[], int *n_docs, const char file_name[]) {
	FILE *file;
	Term term;
	long int next_index;

	if((file = fopen(file_name, "rb")) == NULL) {
		printf("Fail to open index file [%s].", file_name);
		exit(1);
	}

	fread(n_docs, sizeof(int), 1, file);

	while(fread(&term, sizeof(Term), 1, file)==1) {
		indexRetrievTerm(index, term);	
		next_index = ftell(file) + term.list_size * sizeof(Doc);
		fseek(file, next_index, SEEK_SET);
	}

	fseek(file, 0, SEEK_SET);
	
	return file;
}

float calcScoreTerm(char term[], Index *index, FILE *file_index) {

}

int procQuery(char query[]) {
	int qlen, i=0, n_docs;
	char term[MAX_TERM_SIZE];

	FILE *file_index;
	Index *index;
	unordered_map<string, int> stopWordsHash;

	loadStopWordsHash(stopWordsHash, STOPWORDS_FILE);
	index = (Index*) malloc(sizeof(Index)*INDEX_SIZE);
	file_index = retrieveIndex(index, &n_docs, INDEX_FILE);

	printIndex(index, INDEX_SIZE);

	qlen = strlen(query);
	while(i < qlen) {
		getNextTerm(query, term, &i);

		if(stopWordsHash.count(string(term)) == 0) {

		}
	}
}

int readQuerys(char qfile_name[]) {
	ifstream qfile(qfile_name);
	string line, buff;
	char query[MAX_QUERY_SIZE];
	int qId, rId, rClass;

	while(getline(qfile, line)) {
		stringstream lineStream(line);

		lineStream >> buff;
		if(buff.compare("QN") == 0) {
			lineStream >> qId;
		} else if(buff.compare("QU") == 0) {
			lineStream >> ws;
			getline(lineStream, buff);

			strcpy(query, buff.c_str());
			procQuery(query);

		} else if(buff.compare("RD") == 0) {
			while(lineStream >> rId) {
				lineStream >> rClass;
			}
		}
	}
}