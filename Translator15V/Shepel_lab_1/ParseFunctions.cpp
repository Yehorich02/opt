#include <iostream>
#include "ParseFunctions.h"
#include "Context.h"
#include "Utils.h"
using namespace std;

void ParseConstant(Context& context, ifstream& fin, int& asci_ch, char& ch)
{
	bool found_const = false;
	context.cur_pos_in_row = context.pos_in_row;
	context.cur_pos_in_column = context.pos_in_column;
	while (asci_ch >= 48 && asci_ch <= 57)//константы
	{
		context.lexem.push_back(ch);
		fin.get(ch);
		context.pos_in_row++;
		asci_ch = ch;
	}
	string lexem_str(context.lexem.begin(), context.lexem.end());  //_______________make_lexem
	//__________________________________________________________________________________SEARCH_FOR_CONST
	for (auto it = context.ConstMap.begin(); it != context.ConstMap.end(); ++it)
	{
		if (it->first == lexem_str)
		{
			found_const = true;
			context.lexem_id = it->second;
		}
	}
	if (!found_const)
	{
		context.const_count++;
		context.lexem_id = context.const_count;
		context.ConstMap.insert(pair<string, int>(lexem_str, context.const_count));
	}
	context.OurTokens = AddStruct(context.OurTokens, context.TokenAmount);
	context.OurTokens[context.TokenAmount].pos_x = context.cur_pos_in_row;
	context.OurTokens[context.TokenAmount].pos_y = context.cur_pos_in_column;
	context.OurTokens[context.TokenAmount].id_lex = context.lexem_id;
	context.OurTokens[context.TokenAmount].lex = lexem_str;

	context.TokenAmount++;
	//________________________________________________________________________________________
}


void ParseIdent(Context& context, ifstream& fin, int& asci_ch, char& ch)
{
	context.cur_pos_in_row = context.pos_in_row;
	context.cur_pos_in_column = context.pos_in_column;
	bool found_key_words = false;
	bool found_ident = false;
	bool found_digits = false;
	while ((asci_ch >= 97 && asci_ch <= 122) || (asci_ch >= 48 && asci_ch <= 57) || (asci_ch >= 65 && asci_ch <= 90))
	{
		if (asci_ch >= 48 && asci_ch <= 57)
			found_digits = true;
		context.lexem.push_back(ch);
		fin.get(ch);
		context.pos_in_row++;
		asci_ch = ch;
	}
	string lexem_str(context.lexem.begin(), context.lexem.end());//_______________make_lexem

	//________________________________________________________________________________________SEARCH_FOR_IDN
	if (!found_digits)
	{
		for (auto jit = context.KeyWords_Map.begin(); jit != context.KeyWords_Map.end(); ++jit)
		{
			if (jit->first == lexem_str)
			{
				found_key_words = true;
				context.lexem_id = jit->second;
			}
		}
	}
	if (!found_key_words)
	{
		for (auto it = context.IdentMap.begin(); it != context.IdentMap.end(); ++it)
		{
			if (it->first == lexem_str)
			{
				found_ident = true;
				context.lexem_id = it->second;
			}
		}
		if (!found_ident)
		{
			context.const_ident++;
			context.lexem_id = context.const_ident;
			context.IdentMap.insert(pair<string, int>(lexem_str, context.const_ident));
		}
	}
	context.OurTokens = AddStruct(context.OurTokens, context.TokenAmount);
	context.OurTokens[context.TokenAmount].pos_x = context.cur_pos_in_row;
	context.OurTokens[context.TokenAmount].pos_y = context.cur_pos_in_column;
	context.OurTokens[context.TokenAmount].id_lex = context.lexem_id;
	context.OurTokens[context.TokenAmount].lex = lexem_str;
	context.TokenAmount++;
	//________________________________________________________________________________________
}
void ParseOneSymbolDelimiter(Context& context, ifstream& fin, int& asci_ch, char& ch)
{
	context.cur_pos_in_row = context.pos_in_row;
	context.cur_pos_in_column = context.pos_in_column;
	//cout << ch;
	context.lexem.push_back(ch);
	context.lexem_id = asci_ch;
	fin.get(ch);
	asci_ch = ch;
	context.pos_in_row++;
	string lexem_str(context.lexem.begin(), context.lexem.end());//_______________make_lexem
	context.OurTokens = AddStruct(context.OurTokens, context.TokenAmount);
	context.OurTokens[context.TokenAmount].pos_x = context.cur_pos_in_row;
	context.OurTokens[context.TokenAmount].pos_y = context.cur_pos_in_column;
	context.OurTokens[context.TokenAmount].id_lex = context.lexem_id;
	context.OurTokens[context.TokenAmount].lex = lexem_str;
	context.TokenAmount++;
}

bool ParseTwoSymbolsDelimiter(Context& context, ifstream& fin, int& asci_ch, char& ch, int& help_ch)
{
	context.cur_pos_in_row = context.pos_in_row;
	context.cur_pos_in_column = context.pos_in_column;
	context.lexem.push_back(ch);
	fin.get(ch);
	context.pos_in_row++;
	asci_ch = ch;
	if (asci_ch == 61 && help_ch == 58)
	{
		context.lexem.push_back(ch);
		fin.get(ch);
		context.pos_in_row++;
		asci_ch = ch;
		context.lexem_id = 301;
	}
	else if (help_ch == 58)
	{
		context.lexem_id = 58;
	}
	else if (asci_ch == 41 && help_ch == 36)
	{
		context.lexem.push_back(ch);
		fin.get(ch);
		context.pos_in_row++;
		asci_ch = ch;
		context.lexem_id = 303;
	}
	else
	{
		//ParseError(context, fin, help_ch, ch);
		return false;
	}

	string lexem_str(context.lexem.begin(), context.lexem.end());//_______________make_lexem
	context.OurTokens = AddStruct(context.OurTokens, context.TokenAmount);
	context.OurTokens[context.TokenAmount].pos_x = context.cur_pos_in_row;
	context.OurTokens[context.TokenAmount].pos_y = context.cur_pos_in_column;
	context.OurTokens[context.TokenAmount].id_lex = context.lexem_id;
	context.OurTokens[context.TokenAmount].lex = lexem_str;
	context.TokenAmount++;
	return true;
}

bool ParseSpecialCase(Context& context, ifstream& fin, int& asci_ch, char& ch)
{
	context.cur_pos_in_row = context.pos_in_row;
	context.cur_pos_in_column = context.pos_in_column;
	bool isComment = false;

	context.lexem.push_back(ch);
	fin.get(ch);
	context.pos_in_row++;
	asci_ch = ch;

	if (asci_ch == 36)  // если встречаем '$'
	{
		context.lexem.push_back(ch);
		fin.get(ch);
		context.pos_in_row++;
		asci_ch = ch;
		context.lexem_id = 302;
	}
	else if (asci_ch == 42)   //  если встречаем '*', то 
	{
		isComment = true;
		fin.get(ch);
		context.pos_in_row++;
		asci_ch = ch;
		if (fin.eof())
		{
			context.CommentError = true;
			return false;
		}
		while (asci_ch != 42) // идем пока не встретим вторую '*' 
		{

			fin.get(ch);
			if (fin.eof())
			{
				context.CommentError = true;
				break;
			}
			context.pos_in_row++;
			if (asci_ch == 10)  //enter
			{
				context.pos_in_row = 1;
				context.pos_in_column++;
			}
			asci_ch = ch;
		}
		//cout << ch;
		fin.get(ch);
		if (fin.eof())
		{
			context.CommentError = true;
			return false;
		}
		context.pos_in_row++;
		asci_ch = ch;
		if (asci_ch == 10)  //enter
		{
			context.pos_in_row = 1;
			context.pos_in_column++;
		}
		while (asci_ch != 41)   // пока не встретим ')'
		{
			while (asci_ch != 42)// пока не встретим '*' 
			{
				fin.get(ch);
				if (fin.eof())
				{
					context.CommentError = true;
					break;
				}
				context.pos_in_row++;
				if (asci_ch == 10)  //enter
				{
					context.pos_in_row = 1;
					context.pos_in_column++;
				}
				asci_ch = ch;
			}
			fin.get(ch);
			if (fin.eof())
			{
				context.CommentError = true;
				break;
			}
			context.pos_in_row++;
			if (asci_ch == 10)  //enter
			{
				context.pos_in_row = 1;
				context.pos_in_column++;
			}
			asci_ch = ch;
		}
		fin.get(ch);
		context.pos_in_row++;
		if (asci_ch == 10)  //enter
		{
			context.pos_in_row = 1;
			context.pos_in_column++;
		}
		asci_ch = ch;
	}
	else
	{
		context.lexem_id = 40;

	}
	if (fin.eof())
	{
		context.CommentError = true;
		return false;
	}
	if (!isComment)
	{
		string lexem_str(context.lexem.begin(), context.lexem.end());//_______________make_lexem
		context.OurTokens = AddStruct(context.OurTokens, context.TokenAmount);
		context.OurTokens[context.TokenAmount].pos_x = context.cur_pos_in_row;
		context.OurTokens[context.TokenAmount].pos_y = context.cur_pos_in_column;
		context.OurTokens[context.TokenAmount].id_lex = context.lexem_id;
		context.OurTokens[context.TokenAmount].lex = lexem_str;
		context.TokenAmount++;
	}
	return true;
}

void ParseError(Context& context, ifstream& fin, int& asci_ch, char& ch)
{
	context.lexem.push_back(ch);
	string lexem_str(context.lexem.begin(), context.lexem.end());
	context.OurErrors = AddStruct_er(context.OurErrors, context.ErrorsAmount);
	context.OurErrors[context.ErrorsAmount].pos_x = context.pos_in_row;
	context.OurErrors[context.ErrorsAmount].pos_y = context.pos_in_column;
	context.OurErrors[context.ErrorsAmount].text_error = "invalid character";
	context.OurErrors[context.ErrorsAmount].err = lexem_str;
	context.ErrorsAmount++;
	fin.get(ch);
	context.pos_in_row++;
	asci_ch = ch;
}