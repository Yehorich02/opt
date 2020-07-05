#pragma once
#include "LexAnaliz.h"

struct Node
{
	int Code;
	string Name;
	int Line;
	int Column;
	string Func;
	bool IsTerminal;
	Node* Next;
	Node* Down;
};

class SyntaxAnaliz
{
public:
	Node* Tree;
	vector<Lex> Lexems;
	int Pos;
	int LevelCounter;
	int IdentMin;
	int IdentMax;
	int DigitMin;
	int DigitMax;
	bool err;
	vector<string> IdentTable;
	vector<string> DigitTable;
	int ErrorCounter;
	SyntaxAnaliz(string File);
	Node* InitTree();
	Node* AddNode(Node* Nd, int code, string Name, int line, int column, string Func, bool isterminal);
	Node* RemoveNode(Node* N);
	void WriteTree(Node* Root, string sps);
	string TreeStr;
	string ErrorStr;
	void TreeListing(string File);
	void Errors(int p, int pos);
	bool start();
	bool program(Node* NodePtr);
	bool block(Node* NodePtr);
	bool statement_list(Node* NodePtr);
	bool statement(Node* NodePtr);
	bool condition_statement(Node* NodePtr);
	bool conditional_expression(Node* NodePtr);
	bool incomplete_condition_statement(Node* NodePtr);
	bool alternative_part(Node* NodePtr);
	bool comparison_operator(Node* NodePtr);
	bool expression(Node* NodePtr);
	bool integer(Node* NodePtr);
	bool identifier(Node* NodePtr);
};