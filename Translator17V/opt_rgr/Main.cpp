#include "LexAnaliz.h"
#include "Parser.h"

int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cout << "Error";
		return 0;
	}
	for (int i = 1; i < argc; i++) {
		Parser p(argv[i]);
	}
	return 0;
}