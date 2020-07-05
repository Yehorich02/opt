#pragma once
#include "LexAnaliz.h"
#include "SyntaxAnaliz.h"

class CodeGenerator {
public:
	CodeGenerator(string File);
	vector<Lex> Lexems;
	string StrAsm;
	string Err;
	int Label;
	int LabelCounter;
	int tmp;
	int Pos;
	int IdentMin;
	int IdentMax;
	int DigitMin;
	int DigitMax;
	void Listing(string File);
	bool program();
	bool block();
	bool statement_list();
	bool statement();
	bool condition_statement();
	bool conditional_expression();
	bool incomplete_condition_statement();
	bool alternative_part();
	bool comparison_operator();
	bool expression();
	bool integer();
	bool identifier();
};