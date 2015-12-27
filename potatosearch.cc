#include "lib/libstring_operations.cc"
#include "lib/libinverted_list.cc"
#include "lib/libindexer.cc"

int main(int argc, char* argv[]) {
	if(strcmp(argv[1], "index") == 0) {
		index(argv[2]);
	}
}