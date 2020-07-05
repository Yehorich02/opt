#pragma once
using namespace std;
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

struct Lex
{
	int Code;
	int Line;
	int Column;
	string Name;
};

class LeksAnaliz
{
public:
	int LexCounter;
	//������� ������
	vector <Lex> Lexems;
	//������� ���������������
	vector <string> Table;
	//�������� ����� ����� ��������� �����
	int Line;
	int SaveLine;
	//�������� ����� ������� ��������� �����
	int Column;
	int SaveColumn;
	//��������� ��� ���� �������
	int LexErrorMin;
	//�������� ���� �������
	int LexErrorCounter;
	//�������� ���������������
	int IdentCounter;
	//��������� ��� ��������������
	int MinIdent;
	//���� �������� ���
	int KeyWordsMin;
	//������� ������� � ����
	int Size;
	//������ ���� ASCCII �������
	int ASCIIArr[128];
	//������ ����������� ��������������
	vector <string> StdIdent{ "PROCEDURE", "BEGIN", "END", "SIGNAL", "INTEGER", "FLOAT", "COMPLEX", "BLOCKFLOAT", "EXT", "VAR" };
	vector <Lex> KeyWords;
	//������� ������� � �����
	int Pos;
	//�������� ������
	char sbuff;
	//���� � �����
	fstream t;

	//�����������
	LeksAnaliz();
	~LeksAnaliz();
	//�������� � ����� ������ �������
	void AddLeksem(int Code, int Line, int Column, string Name);
	//�������� ������ �� ������� �����
	bool SizeOut();
	//����������� ���������
	void Analizator(string filename);
	//��� ��������� ������� �����
	void INP();
	//whitespace
	void SPACE();
	//��� ��������������
	void IDN();
	//�������� ������� ���������
	void BCOM();
	//�������� ���������
	void COM(string Buf);
	//�������� ���� ���������
	void ECOM(string Buf);
	//�������� �� �������������� ���������
	void DM();
	//�������� �� �������
	void ERR(string pt);
	//����� � ������� ���������������
	int SearchIdent(string Ident);
	//����� � �������������� ������
	int SearchStandartIdent(string Ident);
	//���� ������
	void MakeListing(string file);
};