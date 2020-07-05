#pragma once
#include "Context.h"
#include <fstream>
using namespace std;
void PrintResult(const Context&, ofstream&);
void ErrorInNumber(const Context& ,char tmp, ofstream&);
void ErrorInDollar(const Context&, char tmp, ofstream&);
void ErrorInDelim2(const Context&, char tmp, ofstream&);
void InvalidValue(const Context&, char tmp, ofstream&);
void UnclosedComment(const Context& context, char tmp, ofstream&);