#include "LexAnaliz.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;
using namespace std;

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "Error must be a parameter";
		return 0;
	}
	for (int i = 1; i < argc; ++i) {
		LeksAnaliz LexAn;
		LexAn.Analizator(argv[i]);
	}

	return 0;
}