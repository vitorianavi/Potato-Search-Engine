#ifndef LIBQUERY_PROCESSER_H
#define LIBQUERY_PROCESSER_H

#include <fstream>      // std::ifstream
#include <sstream>
#include <unordered_map>
#include <math.h>
#include <time.h>

int compareRanking(const void *a, const void *b) {
	DocResult *reg1 = (DocResult *)a;
	DocResult *reg2 = (DocResult *)b;
	
	if (reg1->score > reg2->score) return -1;
	if (reg1->score < reg2->score) return 1;
	return 0;
}

#endif