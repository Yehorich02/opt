#include "LexAnaliz.h"
#include "SyntaxAnaliz.h"
using namespace std;

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Error";
		return 0;
	}
	for (int i = 1; i < argc; ++i) {
		SyntaxAnaliz SynAn(argv[i]);
		SynAn.TreeListing(argv[i]);
	}

	return 0;
}