#include "libindexer.h"
#include <fstream>      // std::ifstream
#include <sstream>
#include <unordered_map>

using namespace std;

void index(char *dir_path) {
	DIR *dir;
	struct dirent *ent;
	string line, term;
	int id, stop = 0;
	char dterm[MAX_TERM_SIZE], norm_term[MAX_TERM_SIZE], file_name[300];

	Index *index;
	unordered_map<string, int> stopWordsHash;
	int n_docs = 0, n_terms;
	
	index = (Index*) malloc(sizeof(Index)*INDEX_SIZE);
	initIndex(index, INDEX_SIZE);
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
							if(stopWordsHash.count(string(norm_term)) == 0) {
								indexTerm(index, norm_term, id, 1, INDEX_SIZE);
							}
						}
					} else if(!stop && term.compare("PN") != 0 && term.compare("AN") != 0 && term.compare("AU") != 0 && term.compare("SO") != 0 && term.compare("RF") != 0 && term.compare("CT") != 0)	{		

						do {
							strcpy(dterm, term.c_str());
							normText(dterm, norm_term);
							if(stopWordsHash.count(string(norm_term)) == 0) {
								indexTerm(index, norm_term, id, 1, INDEX_SIZE);
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
		//printIndex(index, INDEX_SIZE);
		n_terms = storeIndex(index, n_docs, INDEX_SIZE, INDEX_FILE);
		free(index);
		closedir(dir);

		printf("Documentos: %d\nTermos:%d\n", n_docs, n_terms);

	} else {
		printf("Could not open directory [%s].", dir_path);
	}

}