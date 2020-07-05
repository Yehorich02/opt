#include "TokenFunctions.h"
#include "Context.h"
#include "PrintFunctions.h"
#include <fstream>

using namespace std;

bool ParseWhitespaces(Context& context, char& character, ifstream& file, ofstream& out)
{
	bool work = false;
	if (character == '\n')
	{
		context.CurLine++;
		context.CurColumn = 0;
	}
	while (file.get(character)) {
		context.CurColumn++;
		if ((context.symbolCategory[(int)character]) != 0)
		{
			work = true;
			break;
		}
		else if (character == '\n')
		{
			context.CurLine++;
			context.CurColumn = 0;
		}
	}
	if (!work) {
		
		return false;
	}
	return true;
}

bool ParseDigit(Context& context, char& character, ifstream& file, ofstream& out)
{
	bool work = false;
	string constant{ character };
	while (file.get(character))
	{
		context.CurColumn++;
		if ((context.symbolCategory[(int)character]) != 1) {
			if (context.symbolCategory[character] == 2)
			{
				context.isError = true;
				ErrorInNumber(context, character, out);
				return false;
			}
			work = true;
			break;
		}

		constant += character;
	}
	auto it = find_if(context.ConstVector.begin(), context.ConstVector.end(), [&](const auto& Const) {return Const.KeyWordName == constant; });
	if (it != context.ConstVector.end())//already exists in ConstVector
	{
		context.TokenResult.push_back({ constant, (*it).KeyWordNum, context.CurLine, context.CurColumn - (int)constant.length()+ (work?0:1) });
	}
	else
	{
		context.TokenResult.push_back({ constant, ++context.CurConstNum, context.CurLine, context.CurColumn - (int)constant.length() + (work ? 0 : 1) });
		context.ConstVector.push_back({ constant, context.CurConstNum });
	}

	if (!work) {
		return false;
	}
	return true;
}

bool ParseIdentifier(Context& context, char& character, ifstream& file, ofstream& out)
{
	string id{ character };
	bool work = false;
	while (file.get(character))
	{
		if ((context.symbolCategory[(int)character]) != 1 && (context.symbolCategory[(int)character]) != 2) {
			work = true;
			break;
		}
		context.CurColumn++;
		id += character;
	}
	context.CurColumn++;
	auto it = find_if(context.KeyWords.begin(), context.KeyWords.end(), [&](const auto& KeyWord) {return KeyWord.KeyWordName == id; });
	if (it == context.KeyWords.end()) // no matches in vector with Key words. this is identifier
	{
		auto tmp = find_if(context.IdVector.begin(), context.IdVector.end(), [&](const auto& IDVector) {return IDVector.KeyWordName == id; });
		if (tmp != context.IdVector.end())//already exists in IdVector
		{
			context.TokenResult.push_back({ id, (*tmp).KeyWordNum, context.CurLine, context.CurColumn - (int)id.length() });
		}
		else
		{
			context.TokenResult.push_back({ id, ++context.CurIDNum, context.CurLine, context.CurColumn - (int)id.length() });
			context.IdVector.push_back({ id, context.CurIDNum });
		}

	}
	else {
		context.TokenResult.push_back({ id, (*it).KeyWordNum, context.CurLine, context.CurColumn - (int)id.length() });
	}
	if (!work) {
		return false;
	}
	return true;
}

bool ParseOneCharacterSymbols(Context& context, char& character, ifstream& file, ofstream& out)
{
	context.TokenResult.push_back({ string{character},(int)character, context.CurLine, context.CurColumn - 1 });
	if (!file.get(character)) {
		return false;
	}
	context.CurColumn++;
	return true;
}

bool ParseTwoCharactersSymbols(Context& context, char& character, ifstream& file, ofstream& out)
{
	string tmp{ character };
	if (!file.get(character)) {
		return 0; //error
	}
	context.CurColumn++;
	if (character != ')') {
		context.isError = true;
		ErrorInDollar(context, character, out);
	}
	tmp += character;
	auto it = find_if(context.delim2.begin(), context.delim2.end(), [&](const auto& delim) {return delim.delim2Name == tmp; });
	context.TokenResult.push_back({ tmp, (*it).delim2Num, context.CurLine, context.CurColumn - 1 });
	if (!file.get(character)) {
		return false;
	}
	context.CurColumn++;
	return true;
}

bool ParseSpecialCase(Context& context, char& character, ifstream& file, ofstream& out)
{
	// если : то проверить что дальше = как в 4 кейсе
	string tmp{ character };
	bool work = false;
	if (character == ':')
	{
		file.get(character);
		context.CurColumn++;

		if (character != '=')
		{
			context.TokenResult.push_back({ tmp, (int)character, context.CurLine, context.CurColumn-1 });
			return true;
		}

		tmp += character;
		auto it = find_if(context.delim2.begin(), context.delim2.end(), [&](const auto& delim) {return delim.delim2Name == tmp; });
		context.TokenResult.push_back({ tmp, (*it).delim2Num, context.CurLine, context.CurColumn - 1 });

		if (!file.get(character))
		{

			return false;
		}
		context.CurColumn++;
	}
	//если скобочка, проверить что дальше : если * то это коммент; если доллар то сохранить как в 4 кейсе; если что-то другое то сохранить скобочку и пойти дальше
	else if (character == '(')
	{

		if (!file.get(character))
		{
			return false;
		}
		context.CurColumn++;

		if (character == '*')
		{
			bool isStartFound = false;

			while (file.get(character))
			{
				context.CurColumn++;
				if (character == '*' || isStartFound)
				{
					if (isStartFound && character == ')')
					{
						work = true;
						if (!file.get(character))
						{
							return false;
						}
						context.CurColumn++;
						break;

					}
					file.get(character);
					context.CurColumn++;
					if (character == ')')
					{
						work = true;
						if (!file.get(character))
						{
							return false;
						}

						context.CurColumn++;
						break;
					}
					else if (character == '*')
					{
						isStartFound = true;
					}
				}
				else if (character == '\n')
				{
					context.CurLine++;
					context.CurColumn = 0;
				}
				else
				{
					isStartFound = false;
				}
			}
			if (!work)
			{
				context.isError = true;
				UnclosedComment(context, character, out);
			}
		}
		else if (character == '$')
		{
			tmp += character;
			auto it = find_if(context.delim2.begin(), context.delim2.end(), [&](const auto& delim) {return delim.delim2Name == tmp; });
			context.TokenResult.push_back({ tmp, (*it).delim2Num, context.CurLine, context.CurColumn - 1 });

			if (!file.get(character)) {
				return false;
			}
			work = true;
			context.CurColumn++;
		}
		else
		{
			context.TokenResult.push_back({ "(", 40, context.CurLine, context.CurColumn - 1 });
			work = true;
		}

		if (!work) {
			return false;
		}
	}
	return true;
}