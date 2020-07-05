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
	//������� �����
	vector <string> TableD;
	//�������� ����� ����� ��������� �����
	int Line;
	int SaveLine;
	//�������� ����� ������� ��������� �����
	int Column;
	int SaveColumn;
	int LexErrorCode;
	//�������� ���������������
	int IdentCounter;
	//��������� ��� ��������������
	int MinIdent;
	//�������� �����
	int DigitCounter;
	//��������� ��� �����
	int MinDigit;
	//���� �������� ���
	int KeyWordsMin;
	//������� ������� � ����
	int Size;
	//������ ���� ASCCII �������
	int ASCIIArr[128];
	//������ ����������� ��������������
	vector <string> StdIdent{ "PROGRAM", "BEGIN", "END", "LABEL", "GOTO", "LINK", "IN", "OUT" };
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
	//��� �����
	void DG();
	//�������� ������� ���������
	void BCOM();
	//�������� ���������
	void COM();
	//�������� ���� ���������
	void ECOM();
	//�������� �� �������������� ���������
	void DM();
	//�������� �� �������
	void ERR(string pt);
	//����� � ������� �����
	int SearchDG(string Digit);
	//����� � ������� ���������������
	int SearchIdent(string Ident);
	//����� � �������������� ������
	int SearchStandartIdent(string Ident);
	//���� ������
	void MakeListing(string file);
};