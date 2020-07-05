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
	//таблиц€ лексем
	vector <Lex> Lexems;
	//таблиц€ ≥дентиф≥каторов
	vector <string> Table;
	//поточний номер р€дка вих≥дного файла
	int Line;
	int SaveLine;
	//поточний номер стовбц€ поточного файла
	int Column;
	int SaveColumn;
	//м≥н≥мальний код лекс помилки
	int LexErrorMin;
	//л≥чильник лекс помилки
	int LexErrorCounter;
	//л≥чильник ≥дентиф≥каторов
	int IdentCounter;
	//м≥н≥мальний код ≥дентиф≥катора
	int MinIdent;
	//коди ключових сл≥в
	int KeyWordsMin;
	//к≥льк≥сть символ≥в в файл≥
	int Size;
	//массив код≥в ASCCII символ≥в
	int ASCIIArr[128];
	//массив стандартних ≥дентиф≥катор≥в
	vector <string> StdIdent{ "PROCEDURE", "BEGIN", "END", "SIGNAL", "INTEGER", "FLOAT", "COMPLEX", "BLOCKFLOAT", "EXT", "VAR" };
	vector <Lex> KeyWords;
	//поточна позиц≥€ в р€дку
	int Pos;
	//поточний символ
	char sbuff;
	//файл с кодом
	fstream t;

	//конструктор
	LeksAnaliz();
	~LeksAnaliz();
	//добавл€Ї в к≥нець списка лексему
	void AddLeksem(int Code, int Line, int Column, string Name);
	//перев≥рка виходу за довжину р€дка
	bool SizeOut();
	//лексический анал≥затор
	void Analizator(string filename);
	//вв≥д чергового символа р€дки
	void INP();
	//whitespace
	void SPACE();
	//вв≥д ≥дентиф≥катора
	void IDN();
	//перев≥рка початку коментар€
	void BCOM();
	//перев≥рка коментар€
	void COM(string Buf);
	//перев≥рка к≥нц€ коментар€
	void ECOM(string Buf);
	//перев≥рка на односимвольний розд≥льник
	void DM();
	//перев≥рка на помилку
	void ERR(string pt);
	//пошук в таблиц≥ ≥дентиф≥каторов
	int SearchIdent(string Ident);
	//пошук в зарезервованих словах
	int SearchStandartIdent(string Ident);
	//друк списка
	void MakeListing(string file);
};