#include "LexAnaliz.h"
#include "SyntaxAnaliz.h"
#include "CodeGenerator.h"

int main(int argc, char* argv[])
{
	cout << "This is translator of the part of SIGNAL programming language" << endl << endl;
	if (argc == 1) {
		cout << "Error must be a parameter";
		return 0;
	}
	for (int i = 1; i < argc; ++i) {
		CodeGenerator code(argv[i]);
	}
	return 0;
}