#include "LexAnaliz.h"
#include "Parser.h"

int main(int argc, char* argv[])
{
	cout << "This is translator of the part of SIGNAL programming language" << endl << endl;
	if (argc == 1) {
		cout << "Error";
		return 0;
	}
	for (int i = 1; i < argc; ++i) {
		Parser parser(argv[i]);
		parser.TreeListing(argv[i]);
	}
	return 0;
}