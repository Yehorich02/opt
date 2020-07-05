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
	Node* Child;
	Node* Parent;
};

class Parser
{
public:
	Node* SyntaxTree;
	vector<Lex> Lexems;
	int Pos;
	int IdentMin;
	int IdentMax;
	int IntMax;
	int IntMin;
	bool Err;
	vector<string> IdentTable;
	int ErrorCounter;
	Parser(string File);
	Node* InitilizationTree();
	Node* AddNode(Node* Nd, int code, string Name, int line, int column, string Func, bool isterminal);
	Node* RemoveNode(Node* N);
	void WriteTree(Node* Root, string sps);
	string TreeStr;
	string ErrorStr;
	void TreeListing(string File);
	void Errors(int p, int pos);
	bool Start();
	bool Program(Node* Ptr);
	bool Block(Node* Ptr);
	bool Statement_list(Node* Ptr);
	bool Identifiers_list(Node* Ptr);
	bool Identifier(Node* Ptr);
	bool Unsigned_integer(Node* Ptr);
	bool Declarations(Node* Ptr);
	bool Declarations_list(Node* Ptr);
	bool Declaration(Node* Ptr);
	bool Attribute(Node* Ptr);
	bool Attributes_list(Node* Ptr);
	bool Ranges_list(Node* Ptr);
	bool Range(Node* Ptr);
};