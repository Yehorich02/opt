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
	int p;
	vector <Lex> Lexems;
	vector <string> Table;
	vector <string> TableD;
	int Line;
	int SaveLine;
	int Column;
	int SaveColumn;
	int LexErrorMin;
	int LexErrorCounter;
	int IdentCounter;
	int MinIdent;
	int DigitCounter;
	int MinDigit;
	int KeyWordsMin;
	int Size;
	int ASCIIArr[128];
	vector <Lex> KeyWords;
	vector <Lex> TableMDM;
	int Pos;
	char sbuff;
	fstream t;

	LeksAnaliz();
	~LeksAnaliz();
	void AddLeksem(int Code, int Line, int Column, string Name);
	bool SizeOut();
	void Analizator(string file);
	void INP();
	void SPACE();
	void Digit();
	void IDN();
	void BCOM();
	void COM(string Buf);
	void ECOM(string Buf);
	void MDM();
	void DM();
	void ERR(string pt);
	int SearchIdent(string Ident);
	int SearchStandartIdent(string Ident);
	int SearchDigit(string Digit);
	int SearchMDM(string MDM);
	void MakeListing(string file);
};