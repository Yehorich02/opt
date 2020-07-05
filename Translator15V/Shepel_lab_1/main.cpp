#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "Context.h"
#include "ParseFunctions.h"
#include "Utils.h"
using namespace std;

void Analyze(string path)
{
	string outFile = path + "//generated.txt";
	path += "//input.sig";
	ifstream fin(path);
	ofstream out(outFile);
	Context context{};
	if (!fin.is_open())
	{
		cout << "Ошибка открытия файла" << endl;
	}
	else
	{
		char ch;
		int asci_ch;
		if (!fin.eof())
			fin.get(ch);
		//lexem.push_back(ch);
		//pos_in_row++;
		asci_ch = ch;
		while (!fin.eof())
		{
			if (asci_ch >= 48 && asci_ch <= 57)
			{
				ParseConstant(context, fin, asci_ch, ch);
			}

			else if ((asci_ch >= 97 && asci_ch <= 122) || (asci_ch >= 65 && asci_ch <= 90))//идентификаторы
			{
				ParseIdent(context, fin, asci_ch, ch);
			}

			else if (asci_ch == 44 || asci_ch == 59)  //односимвольные разделители
			{
				ParseOneSymbolDelimiter(context, fin, asci_ch, ch);
			}

			else if (asci_ch == 58|| asci_ch == 36)  //двусимвольные разделители
			{
				int help_ch = asci_ch;
				if (!ParseTwoSymbolsDelimiter(context, fin, asci_ch, ch, help_ch))
				{
					ParseError(context, fin, help_ch, ch);
					break;
				}
			}

			//_________________________________________________________________________
			else if (asci_ch == 40)  //     встречаем '('    42-*   36-$
			{
				if (!ParseSpecialCase(context, fin, asci_ch, ch))
				{
					break;
				}
			}
			//_________________________________________________________________________

			else if ((asci_ch == 32)||((asci_ch >= 8)&&(asci_ch <10))||((asci_ch >10) && (asci_ch <=13)))  //пробельные символы
			{
				fin.get(ch);
				context.pos_in_row++;
				asci_ch = ch;
			}

			else if (asci_ch == 10)  //enter
			{
				fin.get(ch);
				context.pos_in_row = 1;
				context.pos_in_column++;
				asci_ch = ch;
			}
			else
			{
				ParseError(context, fin, asci_ch, ch);
				break;
			}
			context.lexem.clear();

		}
		if (context.CommentError)
		{
			context.pos_comment_x = context.cur_pos_in_row;
			context.pos_comment_y = context.cur_pos_in_column;
		}
		//--------------------------------------------------OUTPUT
		PrintResult(context, out);

		delete[] context.OurTokens;
		delete[] context.OurErrors;
	}
	fin.close();
	out.close();
}



int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Incorrect argument count";

	}
	else
	{
		Analyze(argv[1]);
	}
	system("pause");
	return 0;
}