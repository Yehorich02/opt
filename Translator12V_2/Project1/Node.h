#pragma once

#include <string>
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