#include "LexAnaliz.h"
#include "SynAnalizer.h"

int main(int argc, char* argv[])
{
	cout << "This is translator of the part of SIGNAL programming language" << endl << endl;
	if (argc == 1) {
		cout << "Error";
		return 0;
	}
	for (int i = 1; i < argc; ++i) {
		SynAnaliz parser(argv[i]);
	}
	return 0;
}