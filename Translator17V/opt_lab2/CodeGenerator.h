#pragma once
#include "LexAnaliz.h"
#include "Parser.h"

class CodeGenerator {
public:
	CodeGenerator(string File);
	string StrAsm;
	string StrError;
	Tree* T;
	int Error;
	int Label;
	vector<string> ForVariable;
	string ForVar;
	string ProgramName;
	int LabelCounter;
	
	void listing(string File);
	void init(string File);
	void start(Tree* T);
	void statements_list(Tree* T);
	void statement(Tree* T);
	void expression(Tree* T);
	void summand(Tree* T);
	void summand_list(Tree* T);


	int checkFor(string var);
};