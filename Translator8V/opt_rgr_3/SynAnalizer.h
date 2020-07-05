#pragma once
#include "LexAnaliz.h"

class SynAnaliz
{
public:
	vector<Lex> Lexems;
	int Pos;
	int IdentMin;
	int IdentMax;
	int space;
	bool err;
	int ErrorCounter;
	SynAnaliz(string File);
	string Out;
	string ErrorStr;
	void TreeListing(string File);
	void Errors(int p, int pos);
	void start();
	void program();
	bool LexemsOut();
	int block();
	int statement_list();
	int statement();
	int cond();
	int else_();
	int parameters_list();
	int identifiers_list();
	int procedure_identifier();
	int variable_identifier();
	int identifier();
	int declarations();
	int declarations_list();
	int declaration();
	int attribute();

	int attributes_list();
	string tab(int sps);
};