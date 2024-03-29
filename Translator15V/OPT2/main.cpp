#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "Context.h"
#include "TokenFunctions.h"
#include "PrintFunctions.h"
#include "SyntacticalAnalyzer.h"
#include "Utils.h"

using namespace std;
namespace fs = filesystem;

void GetSymbolCategories(Context& context) //creating symbol categories
{
	for (int i = 0; i < 127; i++) {

		//category of whitespace
		if ((i > 8 && i < 14) || i == 32) {
			context.symbolCategory[i] = 0;
		}

		//category of digits
		else if (i > 47 && i < 58) {
			context.symbolCategory[i] = 1;
		}

		//category of identifiers and key words
		else if (i > 64 && i < 91 || i > 96 && i < 123) {
			context.symbolCategory[i] = 2;
		}

		//category of one-character symbols
		else if (i == 44 || i == 59 || i == 46 || i == 41) {
			context.symbolCategory[i] = 3;
		}

		//category of symbols from which multi-character delimiters begin
		else if (i == 36) {
			context.symbolCategory[i] = 4;
		}

		//category of symbols which can be both one-character and multi-character  delimiters
		else if (i == 58 || i == 40) {
			context.symbolCategory[i] = 5;
		}

		//category of forbidden symbols
		else {
			context.symbolCategory[i] = 6;
		}
	}
}


void initialize(Context& context)
{
	GetSymbolCategories(context);
	context.KeyWords.push_back({ "PROGRAM", 401 });
	context.KeyWords.push_back({ "BEGIN", 402 });
	context.KeyWords.push_back({ "END", 403 });
	context.KeyWords.push_back({ "GOTO", 404 });
	context.KeyWords.push_back({ "LINK", 405 });
	context.KeyWords.push_back({ "IN", 406 });
	context.KeyWords.push_back({ "OUT", 407 });
	context.KeyWords.push_back({ "RETURN", 408 });

	context.delim2.push_back({ ":=", 301 });
	context.delim2.push_back({ "($", 302 });
	context.delim2.push_back({ "$)", 301 });

}

void Parse(const fs::path& folder)
{
	Context context{};
	initialize(context);

	string path = folder.string() + "\\input.sig";
	//string path = "input.sig";
	ifstream file;
	file.open(path);
	ofstream out(folder.string() + "\\generated.txt");
	//ofstream out("generated.txt");

	if (!file.is_open()) //if file wasn`t opened, print error
	{
		cout << "Error";
		return;
	}
	else // if file was successfully opened
	{
		cout << "File was successfully opened" << endl;

		char character; // variable for character to check
		if (!file.get(character)) { std::cout << "EMPTY\n";  return; } //if file is empty

		while (character != EOF)
		{
			int AsciiCh = character; //variable for symbol in ASCII table

			bool work = false;

			switch (context.symbolCategory[AsciiCh])
			{
				// case of whitespaces
			case 0:
			{
				work = ParseWhitespaces(context, character, file, out);
				break;
			}
			//case of digit
			case 1:
			{
				work = ParseDigit(context, character, file, out);
				break;
			}

			//case of identifiers or key words
			case 2:
			{
				work = ParseIdentifier(context, character, file, out);
				break;
			}

			//case of one-character symbols
			case 3:
			{
				work = ParseOneCharacterSymbols(context, character, file, out);
				break;
			}
			//case of $
			case 4:
			{
				work = ParseTwoCharactersSymbols(context, character, file, out);
				break;
			}

			// case of  symbols which can be both one-character and multi-character delimiters
			case 5:
			{
				work = ParseSpecialCase(context, character, file, out);
				break;
			}

			//case of invalid character
			case 6:
			{
				std::cout << "case of invalid character";
				InvalidValue(context, character, out);
				work = false;
				break;
			}
			}
			if (!work)
			{
				break;
			}
		}
	}
	file.close();
	if(context.isError == true)
	{
		return;
	}
	else
	{
		int curPosition = 0;
		SignalProgram(context, curPosition);

		int counter = 0;
		Print(&context.tree.root, counter, out);
		out << context.ErrorString;
	}

}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Incorrect argument count";
	}
	else
	{
		Parse(argv[1]);
	}
	//Parse("tests\\test01");
	return 0;
}

