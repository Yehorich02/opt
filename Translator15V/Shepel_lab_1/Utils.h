#pragma once
#include <fstream>
using namespace std;
Token* AddStruct(Token* Obj, const int amount);
Errors* AddStruct_er(Errors* Obj, const int amount);

void PrintResult(Context& context, ofstream& out);