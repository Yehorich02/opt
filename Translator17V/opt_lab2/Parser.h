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

//���� ���������� ������������ ���������
class Parser
{
public:
	//�������� �� ������� ������ �������
	Tree* Tr;
	//������� �������������� ������������ ��������� �����������
	vector<Lex> Lexems;
	//������� �������
	int Pos;
	int IdentMin;
	int IdentMax;
	int DigitMin;
	int DigitMax;
	//�������� �������
	int Error;
	//�����������
	Parser(string File);
	//���������� ������
	Tree* InitTree();
	//���������� ����� � ������
	Tree* AddNotTerminal(Tree* Nd, string Func, bool isterminal);
	Tree* AddTerminal(Tree* Nd, int pos, bool isterminal);
	//��������� ����� � ������
	Tree* RemoveNode(Tree* T);
	//��������� ������ ������
	void WriteTree(Tree* Root, string sps);
	string TreeStr;
	string ErrorStr;
	//����� ������ � ����
	void TreeListing(string File);
	//
	void Errors(int p, int pos);
	//��������� ������������� ���������� ������ ���������
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