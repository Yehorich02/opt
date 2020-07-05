#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include "Utils.h"
#include "CategoryWorkers.h"
#include "TypesFmd.h"
#include "LexParser.h"

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char* argv[])
{
    if (argc == 1) { 
        cout << "no arguments!"; 
        return 0;
    }
    for (auto i{ 1 }; i < argc; ++i)
    {
        fs::path folder = argv[i];
        if (!fs::exists(folder))
        {
            cout << "Test is not exists";
            return 0;
        }
        const auto input = folder.string() + "\\input.sig";
        const auto generate = folder.string() + "\\generated.txt";
        Parse(input.data(), generate.data());

    }
    return 0;
}
