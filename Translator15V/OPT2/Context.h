#pragma once
#include <string>
#include <vector>
#include "tree.h"

using namespace std;

struct Context
{
	vector <KeyWordInfo> KeyWords; // vector of key words
	vector <KeyWordInfo> ConstVector; // vector of constants
	vector <KeyWordInfo> IdVector; // vector of identifiers
	vector <TokenInfo> TokenResult; // vector of tokens
	vector <delim2info> delim2; // vector of multi-character delimiters

	Tree tree;

	int CurConstNum = 500; // counter for constants
	int CurIDNum = 1000; // counter for identifiers
	int symbolCategory[127]; // array for symbols from ASCII table


	int CurLine = 1; // counter for line
	int CurColumn = 1; // counter for column
	int CurCategory = 0; // variable for categories

	bool isError = false;

	string ErrorString;
};