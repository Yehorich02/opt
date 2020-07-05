#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

struct Errors
{
	int pos_x;
	int pos_y;
	string text_error;
	string err;
};

struct Token
{
	int pos_x;
	int pos_y;
	int id_lex;
	string lex;
};
struct Context
{
	bool CommentError = false;

	Token* OurTokens = nullptr;
	int TokenAmount = 0;

	Errors* OurErrors = nullptr;
	int ErrorsAmount = 0;

	int const_count = 500;        //     показывает, с какого элемента должен начинаться подсчет id констант
	int const_ident = 1000;       //     показывает, с какого элемента должен начинаться подсчет id идентификаторов

	map <string, int> ConstMap;   //     пустой контейнер, для констант
	map <string, int> IdentMap;   //     пустой контейнер, для идентификаторов
	map <string, int> Dm2_Map = {                 //
								  { "($", 51 },   //    контейнер с двухсимвольными разделителями
								  { ":=", 52 },   //
	};
	map <string, int> KeyWords_Map = {
									   { "PROGRAM", 400 },   //
									   { "BEGIN"  , 401 },   //
									   { "END"    , 402 },   //
									   { "GOTO"   , 403 },   //
									   { "LINK"   , 404 },   //    контейнер с ключевыми словами
									   { "IN"     , 405 },   //
									   { "OUT"    , 406 },   //
									   { "RETURN" , 407 },   //
	};
	int pos_in_row = 1;                           //    позиция курсора в строке
	int pos_in_column = 1;                        //    позиция курсора по странице
	int pos_comment_x;
	int pos_comment_y;

	vector<char> lexem;        //    вектор, в котором накапливаются символы до те пор, пока не закончится текущая лексема
	int lexem_id = 0;          //    id текущей лексемы
	int cur_pos_in_row = 1;    //    позиция текущей лексемы в строке
	int cur_pos_in_column = 1; //    позиция текущей лексемы по странице
};