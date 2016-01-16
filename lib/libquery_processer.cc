#include "libquery_processer.h"

using namespace std;

TermNode *indexRetrievTerm(Index index[], Term term) {
	hash_t i;
	TermNode *aux;

	i = hashFunction(term.str) % INDEX_SIZE;

	if(index[i] == NULL) {
		index[i] = allocateTerm(term.str, term.offset, term.list_size, term.idf);
		aux = index[i];
	} else {
		aux = findTermOrAlloc(index[i], term.str, term.offset, term.list_size, term.idf);
	}

	return aux;
}

void retrieveIndex(Index index[], const char file_name[]) {
	FILE *file;
	Term term;
	Doc doc;
	TermNode *term_node;
	long int next_index;
	int i;

	if((file = fopen(file_name, "rb")) == NULL) {
		printf("Fail to open index file [%s].", file_name);
		exit(1);
	}

	while(fread(&term, sizeof(Term), 1, file)==1) {
		term_node = indexRetrievTerm(index, term);
		
		for(i = 0; i < term.list_size; i++) {
			fread(&doc, sizeof(Doc), 1, file);
			insertList(term_node, doc.id, doc.tf);
		}
		//next_index = ftell(file) + term.list_size * sizeof(Doc);
		//fseek(file, next_index, SEEK_SET);
	}

	fclose(file);
}

void retrieveDocInfo(DocInfo hash_info[], int *n_docs, const char file_name[]) {
	FILE *file;
	DocInfo docinfo;

	if((file = fopen(file_name, "rb")) == NULL) {
		printf("Fail to open documents info file [%s].", file_name);
		exit(1);
	}

	fread(n_docs, sizeof(int), 1, file);

	while(fread(&docinfo, sizeof(DocInfo), 1, file)==1) {
		insertHashInfo(docinfo.id, docinfo.norm, hash_info, HASH_SIZE);
	}

	fclose(file);
}

void printDocInfo(DocInfo hash_info[], int size) {
	int i;

	for(i = 0; i < size; i++) {
		if(hash_info[i].id != 0) {
			printf("doc: %d norma [%f]\n", hash_info[i].id, hash_info[i].norm);
		}
	}
}

float calcScoreTerm(char term[], int n_docs, Index *index, DocResult hash_score[], int hash_size) {
	TermNode *term_node;
	InvListNode *aux;
	Doc *list;
	DocResult result;

	int i;
	hash_t ind;

	ind = hashFunction(term) % INDEX_SIZE;
	term_node = findTerm(index[ind], term);

	if(term_node) {
		//printf("%s\n", term);
		aux = term_node->list;
		while(aux) {
			//printf("term: %s doc: %d TF: %f\n", term, aux->doc.id, aux->doc.tf);
			//printf("term: %s idf: %f\n", term, term_node->term.idf);

			result.id = aux->doc.id;
			result.score = term_node->term.idf*aux->doc.tf;
		//	printf("%f\n", result.score);

			insertHashResult(result, hash_score, hash_size);
			aux = aux->next;
		}
	}
}

DocResult *calcResultado(DocResult hash_score[], DocInfo hash_info[], int n_docs) {
	DocResult *ranking;
	int i, count=0;
	hash_t pos;

	ranking = (DocResult*) malloc(sizeof(DocResult)*n_docs);
	for (i = 0; i < HASH_SIZE; i++) {
		if(hash_score[i].id != 0) {
			pos = findPositionHashInfo(hash_score[i].id, hash_info, HASH_SIZE);
			if(hash_info[pos].norm) {
				hash_score[i].score = hash_score[i].score/hash_info[pos].norm;
			//	printf("%f\n", hash_score[i].score);
			}
			ranking[count] = hash_score[i];
			count++;
		}
	}

	qsort(ranking, count, sizeof(DocResult), compareRanking);

	return ranking;
}

void printRanking(DocResult ranking[]) {
	int i;

	printf("\n----Top 10 Results-----\n");

	for (i = 0; i < 10; i++) {
		printf("%d - [%d] score:%f\n", i+1, ranking[i].id, ranking[i].score);
	}
}

DocResult *procQuery(char query[], Index *index, DocInfo *hash_info, int *n_docs) {
	int qlen, i=0;
	char term[MAX_TERM_SIZE], norm_term[MAX_TERM_SIZE];

	DocResult hash_score[HASH_SIZE], *ranking;
	unordered_map<string, int> stopWordsHash;

	initHashResult(hash_score, HASH_SIZE);
	loadStopWordsHash(stopWordsHash, STOPWORDS_FILE);

	//printIndex(index, INDEX_SIZE);

	qlen = strlen(query);
	while(i < qlen) {
		getNextTerm(query, term, &i);
		normText(term, norm_term);

		if(stopWordsHash.count(string(norm_term)) == 0) {
			calcScoreTerm(norm_term, *n_docs, index, hash_score, HASH_SIZE);
		}
	}
	ranking = calcResultado(hash_score, hash_info, *n_docs);
	printRanking(ranking);

	return ranking;
}

void initEngine(Index **index, DocInfo **hash_info, int *n_docs) {

	*index = (Index*) malloc(sizeof(Index)*INDEX_SIZE);
	*hash_info = (DocInfo*) malloc(sizeof(DocInfo)*HASH_SIZE);
	initIndex(*index, INDEX_SIZE);
	initHashInfo(*hash_info, HASH_SIZE);

	retrieveIndex(*index, INDEX_FILE);
	retrieveDocInfo(*hash_info, n_docs, DOCINFO_FILE);
}

int readQuerys(char qfile_name[]) {
	Index *index;
	DocInfo *hash_info;
	int i, n_docs, nq=0;

	ifstream qfile(qfile_name);
	string line, buff, aux;
	char query_str[MAX_QUERY_SIZE];
	int rId, rClass, nR, stop=1;

	clock_t begin, end;
	double time_spent = 0.0;

	DocResult *ranking;
	Query query;
	float map=0, p10=0;

	initEngine(&index, &hash_info, &n_docs);

	strcpy(query_str, "");


	while(qfile >> buff) {

		qfile >> query.id;
		qfile >> buff;
		while(buff.compare("NR") != 0) {
			if(buff.compare("QU") != 0) {
				strcat(query_str, buff.c_str());
				strcat(query_str, " ");
			}
			qfile >> buff;
		}

		printf("%d - %s\n", query.id, query_str);

		qfile >> nR;
		initQuery(&query, nR);

		qfile >> buff;
		for (i = 0; i < nR; i++) {
			qfile >> rId;
			qfile >> rClass;
			insertRelevant(&query, rId);
		}

		begin = clock();
		ranking = procQuery(query_str, index, hash_info, &n_docs);	
		end = clock();

		time_spent += (double)(end - begin);

		nq++;
		
		map += calcMAPQuery(query, ranking, n_docs);
		//printf("MAP: %f\n", map);
		p10 += calcP10Query(query, ranking);
	//	printf("P@10: %f\n", );

		free(ranking);
		strcpy(query_str, "");

		printf("\n");
	}

	p10 = p10/nq;
	map = map/nq;

	free(index);
	free(hash_info);

	time_spent = time_spent/CLOCKS_PER_SEC;
	printf("\nP@10 geral: %f\n", p10);
	printf("\nMAP: %f\n", map);

	printf("\nTotal time: %lf\n", time_spent);
}