#include "LexAnaliz.h"

int main()
{
	string File;
	cout << "This is translator of the part of SIGNAL programming language" << endl << endl;
	File = "./tests/test1/";
	LeksAnaliz LexAn;
	LexAn.Analizator(File);
	return 0;
}