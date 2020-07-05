#pragma once
#include "LexAnaliz.h"

//вузол дерева розбору програми
struct Node {
	int Code;
	string Name;
	int Line;
	int Column;
	string Func;
	bool IsTerminal;
	Node* Next;
	Node* Down;
};

//клас реал≥зуючий синтаксичний анал≥затор
class SyntaxAnaliz {
public:
	//вказ≥вник на вершину дерева розбору
	Node* Tree;
	//таблиц€ ≥дентиф≥катор≥в згенерованих лексичним анал≥затором
	vector<Lex> Lexems;
	//поточна лексема
	int Pos;
	int LevelCounter;
	int IdentMin;
	int IdentMax;
	bool err;
	vector<string> IdentTable;
	//л≥чильник помилок
	int ErrorCounter;
	//конструктор
	SyntaxAnaliz(string File);
	//≥н≥ц≥л≥зац≥€ дерева
	Node* InitTree();
	//добавленн€ вузла в дерево
	Node* AddNode(Node* Nd, int code, string Name, int line, int column, string Func, bool isterminal);
	//видаленн€ вузла з дерева
	Node* RemoveNode(Node* N);
	//процедура виводу дерева
	void WriteTree(Node* Root, string sps);
	string TreeStr;
	string ErrorStr;
	//вивод дерева в файл
	void TreeListing(string File);
	//вивод помилок
	void Errors(int p, int pos);
	//процедури синтаксичного анал≥затора заданоњ граматики
	bool start();
	bool program(Node* NodePtr);
	bool block(Node* NodePtr);
	bool statement_list(Node* NodePtr);
	bool declarations(Node* NodePtr);
	bool procedure_declarations(Node* NodePtr);
	bool procedure(Node* NodePtr);
	bool parameters_list(Node* NodePtr);
	bool declarations_list(Node* NodePtr);
	bool declaration(Node* NodePtr);
	bool identifiers_list(Node* NodePtr);
	bool attributes_list(Node* NodePtr);
	bool attribute(Node* NodePtr);
	bool identifier(Node* NodePtr);
};