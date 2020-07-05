#pragma once
#include <string>;

using namespace std;

struct KeyWordInfo  //stucture for key words
{
	string KeyWordName;
	int KeyWordNum;
};

struct delim2info  //structure for multi-character delimiters
{
	string delim2Name;
	int delim2Num;
};

struct TokenInfo  // structure for current token
{
	string TokenName;
	int TokenNum;
	int line;
	int column;
};

enum class TokenType
{
	SIGNAL_PROGRAM, 
	PROGRAM, 
	PROCEDURE_IDENTIFIER,
	BLOCK,
	STATEMENTS_LIST,
	STATEMENT,
	EMPTY,
	UNSIGNED_INTEGER,
	VARIABLE_IDENTIFIER,
	ACTUAL_ARGUMENTS,
	ASSEMBLY_INSERT_FILE_IDINTIFIER,
	ACTUAL_ARGUMENTS_LIST,
	WRITE, 
	NOTHING,
};

struct node
{
	TokenInfo tokenValue;
	TokenType tokenType = TokenType::NOTHING;
	node* child = nullptr;
	node* next = nullptr;
};

struct Tree
{
	node root;
};


