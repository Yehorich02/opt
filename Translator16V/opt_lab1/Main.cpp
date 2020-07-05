#include "LexAnaliz.h"

int main()
{
	string File;
	cout << "This is translator of the part of SIGNAL programming language" << endl << endl;
	cout << "Enter \"./tests/test1/ ./tests/test2/ ./tests/test3/ ./tests/test4/\"" << endl;
	cin >> File;
	LeksAnaliz LexAn;
	LexAn.Analizator(File);
	return 0;
}