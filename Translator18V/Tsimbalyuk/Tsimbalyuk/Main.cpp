#include "LexAnaliz.h"

int main()
{
	string FileName;
	//cin >> FileName;
	FileName = "test2.sig";
	cout << "source filename: " << FileName << endl;
	cout << "..." << endl;
	LeksAnaliz LexAn;
	LexAn.Analizator(FileName);
	return 0;
}