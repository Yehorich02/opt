#pragma once

#include "Context.h"
#include "Node.h"
#include <iostream>
#include <fstream>

class Parser
{
public:
	Node* Tree;
	int Pos;
	Context context;
	int IdentMin;
	int IdentMax;
	int DigitMin;
	int DigitMax;
	int ErrorCounter;
	bool err;
	Parser(Context&, string File);
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
	bool statement_list(Node* NodePtr);
	bool statement(Node* NodePtr);
	bool actual_arguments(Node* NodePtr);
	bool actual_arguments_list(Node* NodePtr);
	bool integer(Node* NodePtr);
	bool identifier(Node* NodePtr);
};