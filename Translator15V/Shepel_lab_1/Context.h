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

	int const_count = 500;        //     ����������, � ������ �������� ������ ���������� ������� id ��������
	int const_ident = 1000;       //     ����������, � ������ �������� ������ ���������� ������� id ���������������

	map <string, int> ConstMap;   //     ������ ���������, ��� ��������
	map <string, int> IdentMap;   //     ������ ���������, ��� ���������������
	map <string, int> Dm2_Map = {                 //
								  { "($", 51 },   //    ��������� � ��������������� �������������
								  { ":=", 52 },   //
	};
	map <string, int> KeyWords_Map = {
									   { "PROGRAM", 400 },   //
									   { "BEGIN"  , 401 },   //
									   { "END"    , 402 },   //
									   { "GOTO"   , 403 },   //
									   { "LINK"   , 404 },   //    ��������� � ��������� �������
									   { "IN"     , 405 },   //
									   { "OUT"    , 406 },   //
									   { "RETURN" , 407 },   //
	};
	int pos_in_row = 1;                           //    ������� ������� � ������
	int pos_in_column = 1;                        //    ������� ������� �� ��������
	int pos_comment_x;
	int pos_comment_y;

	vector<char> lexem;        //    ������, � ������� ������������� ������� �� �� ���, ���� �� ���������� ������� �������
	int lexem_id = 0;          //    id ������� �������
	int cur_pos_in_row = 1;    //    ������� ������� ������� � ������
	int cur_pos_in_column = 1; //    ������� ������� ������� �� ��������
};