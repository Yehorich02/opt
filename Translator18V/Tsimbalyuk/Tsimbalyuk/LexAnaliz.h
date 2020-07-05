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
	//таблица лексем
	vector <Lex> Lexems;
	//таблица идентификаторов
	vector <string> Table;
	//таблица констант
	vector <string> TableC;
	//текущий номер строки исходного файла
	int Line;
	int SaveLine;
	//текущий номер колонки текущего файла
	int Column;
	int SaveColumn;
	int LexErrorCode;
	//Счётчик идентиффикаторов
	int IdentCounter;	
	//минимальный код идентификатора 
	int MinIdent;
	//Счётчик констант
	int ConstCounter;	
	//Минимальный код константы
	int MinConst;
	//Коды ключевых слов
	int KeyWordsMin;
	//Конечная позиция констант в массиве
	int ConstMax;
	//количество символов в файле
	int Size;
	//массив кодов ASCCII символов
	int ASCIIArr[128];
	//массив стандартных идентиффикаторов
	vector <string> StdIdent { "PROGRAM",  "BEGIN", "END", "CASE", "ENDCASE", "LOOP", "ENDLOOP", "OF", "MOD" };
	//текущая позиция в сроке
	int Pos;
	//строка с исходным кодом
	//string Str;
	//поточный символ
	char sbuff;
	//файл с кодом
	fstream t;

	//конструктор
	LeksAnaliz();
	~LeksAnaliz();
	//добавляет в конец списка лексему
	void AddLeksem(int Code, string Name, int Line, int Column);
	//проверка выхода за длину строки
	bool SizeOut();
	//лексический анализатор
	void Analizator(string filename);
	//ввод очередного символа строки
	void INP();
	//whitespace
	void SEP();
	//ввод константы
	void CONST();
	//ввод идентиффикатора
	void IDN();
	//проверка начала коментария
	void BCOM();
	//проверка коментария
	void COM();
	//проверка конца коментария
	void ECOM();
	//проверка на односимвольный разделитель
	void DM();
	//проверка на ошибку
	void ERR(string pt);
	//поиск в таблице идентиффикаторов
	int SearchIdent(string Ident);
	//поиск среди зарезервированных слов
	int SearchStandartIdent(string Ident);
	//поиск в таблице констант
	int SearchConst(string Const);
	//печать списка
	void MakeListing(string filename);
};