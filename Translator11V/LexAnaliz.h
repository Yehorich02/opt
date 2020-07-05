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
	//таблиц€ чисел
	vector <string> TableD;
	//поточний номер р€дка вих≥дного файла
	int Line;
	int SaveLine;
	//поточний номер стовбц€ поточного файла
	int Column;
	int SaveColumn;
	int LexErrorCode;
	//л≥чильник ≥дентиф≥каторов
	int IdentCounter;
	//м≥н≥мальний код ≥дентиф≥катора
	int MinIdent;
	//л≥чильник чисел
	int DigitCounter;
	//м≥н≥мальний код чисел
	int MinDigit;
	//коди ключових сл≥в
	int KeyWordsMin;
	//к≥льк≥сть символ≥в в файл≥
	int Size;
	//массив код≥в ASCCII символ≥в
	int ASCIIArr[128];
	//массив стандартних ≥дентиф≥катор≥в
	vector <string> StdIdent{ "PROGRAM", "BEGIN", "END", "LABEL", "GOTO", "LINK", "IN", "OUT" };
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
	//вв≥д вдентиф≥катора
	void IDN();
	//вв≥д числа
	void DG();
	//перев≥рка початку коментар€
	void BCOM();
	//перев≥рка коментар€
	void COM();
	//перев≥рка к≥нц€ коментар€
	void ECOM();
	//перев≥рка на односимвольний розд≥льник
	void DM();
	//перев≥рка на помилку
	void ERR(string pt);
	//пошук в таблиц≥ чисел
	int SearchDG(string Digit);
	//пошук в таблиц≥ ≥дентиф≥каторов
	int SearchIdent(string Ident);
	//пошук в зарезервованих словах
	int SearchStandartIdent(string Ident);
	//друк списка
	void MakeListing(string file);
};