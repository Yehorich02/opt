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
	//м≥н≥мальний код лекс помилки
	int LexErrorMin;
	//л≥чильник лекс помилки
	int LexErrorCounter;
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
	vector <Lex> KeyWords;
	// массив багатосимвольних розд≥льник≥в
	vector <Lex> TableMDM;
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
	void Analizator(string file);
	//вв≥д чергового символа р€дки
	void INP();
	//whitespace
	void SPACE();
	//вв≥д числа
	void Digit();
	//вв≥д вдентиф≥катора
	void IDN();
	//перев≥рка початку коментар€
	void BCOM();
	//перев≥рка коментар€
	void COM(string Buf);
	//перев≥рка к≥нц€ коментар€
	void ECOM(string Buf);
	//перев≥рка на багатосимвольний розд≥льник
	void MDM();
	//перев≥рка на односимвольний розд≥льник
	void DM();
	//перев≥рка на помилку
	void Err(string pt);
	//пошук в таблиц≥ ≥дентиф≥каторов
	int SearchIdent(string Ident);
	//пошук в зарезервованих словах
	int SearchStandartIdent(string Ident);
	//пошук в таблиц≥ чисел
	int SearchDigit(string Digit);
	//пошук в таблиці багатосимвольних розд≥льник≥в
	int SearchMDM(string MDM);
	//друк списка
	void MakeListing(string file);
};