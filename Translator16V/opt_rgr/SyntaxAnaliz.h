#pragma once
#include "LexAnaliz.h"

//вузол дерева розбору програми
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

//клас реал≥зуючий синтаксичний анал≥затор
class SyntaxAnaliz
{
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
	int DigitMin;
	int DigitMax;
	bool err;
	vector<string> IdentTable;
	vector<string> DigitTable;
	//л≥чиш=льник помилок
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