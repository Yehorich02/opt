#pragma once

#include "TypesFmd.h"
#include "LexParser.h"
#include "SynParser.h"
#include "Node.h"
#include <iostream>
#include <fstream>

class SynParser
{
public:
	Node* Tree;
	int Pos;
	Context context;
	vector <info> Lexems;
	int IdentMin;
	int IdentMax;
	int DigitMin;
	int DigitMax;
	int ErrorCounter;
	SynParser(Context&, string File);
	Node* InitTree();
	Node* AddNode(Node* Nd, int code, string Name, int line, int column, string Func, bool isterminal);
	Node* RemoveNode(Node* N);
	void WriteTree(Node* Root, string sps);
	string TreeStr;
	string ErrorStr;
	void TreeListing(string File);
	void Errors(int p, int pos);
	void start();
	bool program(Node* NodePtr);
	bool block(Node* NodePtr);
	bool parameters_list(Node* NodePtr);
	bool identifiers_list(Node* NodePtr);
	bool label_declarations(Node* NodePtr);
	bool labels_list(Node* NodePtr);
	bool statement_list(Node* NodePtr);
	bool statement(Node* NodePtr);
	bool integer(Node* NodePtr);
	bool identifier(Node* NodePtr);
};