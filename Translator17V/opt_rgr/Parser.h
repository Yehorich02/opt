#pragma once
#include "LexAnaliz.h"

struct Tree
{
	int Code;
	string Name;
	int Line;
	int Column;
	string Func;
	bool IsTerminal;
	Tree* Next;
	Tree* Down;
};

//клас реал≥зуючий синтаксичний анал≥затор
class Parser
{
public:
	//вказ≥вник на вершину дерева розбору
	Tree* Tr;
	//таблиц€ ≥дентиф≥катор≥в згенерованих лексичним анал≥затором
	vector<Lex> Lexems;
	//поточна лексема
	int Pos;
	int IdentMin;
	int IdentMax;
	int DigitMin;
	int DigitMax;
	//л≥чильник помилок
	int Error;
	//конструктор
	Parser(string File);
	//≥н≥ц≥л≥зац≥€ дерева
	Tree* InitTree();
	//добавленн€ вузла в дерево
	Tree* AddNotTerminal(Tree* Nd, string Func, bool isterminal);
	Tree* AddTerminal(Tree* Nd, int pos, bool isterminal);
	//видаленн€ вузла з дерева
	Tree* RemoveNode(Tree* T);
	//процедура виводу дерева
	void WriteTree(Tree* Root, string sps);
	string TreeStr;
	string ErrorStr;
	//вивод дерева в файл
	void TreeListing(string File);
	//
	void Errors(int p, int pos);
	//процедури синтаксичного анал≥затора заданоњ граматики
	bool LexemsOut();
	void start(string File);
	void program(Tree* T);
	bool block(Tree* T);
	bool statements_list(Tree* T);
	int statement(Tree* T);
	bool loop_declaration(Tree* T);
	bool expression(Tree* T);
	bool summands_list(Tree* T);
	bool add_intruction(Tree* T);
	bool summand(Tree* T);
	bool integer(Tree* T);
	bool identifier(Tree* T);
	bool procedure_identifier(Tree* T);
	bool variable_identifier(Tree* T);
};