#pragma once
#include "Context.h"
#include <fstream>
using namespace std;
bool ParseWhitespaces(Context&, char&, ifstream&, ofstream&);
bool ParseDigit(Context&, char&, ifstream&, ofstream&);
bool ParseIdentifier(Context&, char&, ifstream&, ofstream&);
bool ParseOneCharacterSymbols(Context&, char&, ifstream&, ofstream&);
bool ParseTwoCharactersSymbols(Context&, char&, ifstream&, ofstream&);
bool ParseSpecialCase(Context&, char&, ifstream&, ofstream&);