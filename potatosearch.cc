#include "lib/libstring_operations.cc"
#include "lib/libhash.cc"
#include "lib/libinverted_list.cc"
#include "lib/libindexer.cc"
#include "lib/libquery_processer.cc"

int main(int argc, char* argv[]) {
	if(strcmp(argv[1], "index") == 0) {
		index(argv[2]);
	} else if(strcmp(argv[1], "query") == 0) {
		readQuerys(argv[2]);
	}
}