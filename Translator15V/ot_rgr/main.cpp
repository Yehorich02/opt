#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "Context.h"
#include "ParseFunctions.h"
#include "Utils.h"
using namespace std;

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cout << "Incorrect argument count";
	}
	for (int i = 1; i < argc; i++) {
		Analyze(argv[i]);
	}
	system("pause");
	return 0;
}