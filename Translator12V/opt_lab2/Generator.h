#pragma once

#include "LexAnaliz.h"
#include "Parser.h"


class Generator {
public:
	Generator(string File);
	void Listing(string File);
	string AsmStr;
	int Error;
	string ErrStr;
	string AsmFile;
	string ProgramName;
	vector <string> Label;
	int IdnMin;
	int LabelMin;
	vector<Lex> Lexems;
	void generate(Node* Tree);
	void parameters_list(Node* Tree);
	void declarations(Node* Tree);
	void statements_list(Node* Tree);
	void statement(Node* Tree);
	void identifier(Node* Tree);
	void identifiers_list(Node* Tree);
	void labels_list(Node* Tree);
};