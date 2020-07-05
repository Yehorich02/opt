#include "LexAnaliz.h"

int main(int argc, char* argv[])
{
	cout << "This is translator of the part of SIGNAL programming language" << endl << endl;
	if (argc == 1) {
		std::cout << "Error must be a parameter";
		return 0;
	}
	LeksAnaliz LexAn;
	LexAn.Analizator(argv[1]);
	return 0;
}