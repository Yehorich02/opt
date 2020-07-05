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
	int idn;
	int atr;
	int id_idn;
	vector<Lex> Lexems;
	void generate(Node* Tree);
	void variable_declarations(Node* Tree);
	void declarations_list(Node* Tree);
	void declaration(Node* Tree);
	void statements_list(Node* Tree);
	void identifier(Node* Tree);
	void attribute(Node* Tree);
	void attributes_list(Node* Tree);
	void identifiers_list(Node* Tree);
};