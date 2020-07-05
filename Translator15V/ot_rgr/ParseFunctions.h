#pragma once
#include <iostream>
#include "Context.h"
#include "Parser.h"

using namespace std;
void ParseConstant(Context&, ifstream&, int&, char&);
void ParseIdent(Context&, ifstream&, int&, char&);
void ParseOneSymbolDelimiter(Context&, ifstream&, int&, char&);
bool ParseTwoSymbolsDelimiter(Context&, ifstream&, int&, char&, int&);
bool ParseSpecialCase(Context&, ifstream&, int&, char&);
void ParseError(Context&, ifstream&, int&, char&);
void Analyze(string);