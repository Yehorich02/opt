#include "Context.h"
#include "Utils.h"

Token* AddStruct(Token* Obj, const int amount)
{
	if (amount == 0)
	{
		Obj = new Token[amount + 1]; // выделение памяти для первой структуры
	}
	else
	{
		Token* tempObj = new Token[amount + 1];

		for (int i = 0; i < amount; i++)
		{
			tempObj[i] = Obj[i]; // копируем во временный объект
		}
		delete[] Obj;

		Obj = tempObj;
	}
	return Obj;
}

Errors* AddStruct_er(Errors* Obj, const int amount)
{
	if (amount == 0)
	{
		Obj = new Errors[amount + 1]; // выделение памяти для первой структуры
	}
	else
	{
		Errors* tempObj = new Errors[amount + 1];

		for (int i = 0; i < amount; i++)
		{
			tempObj[i] = Obj[i]; // копируем во временный объект
		}
		delete[] Obj;

		Obj = tempObj;
	}
	return Obj;
}

void PrintResult(Context& context, ofstream& out)
{
	if (out.is_open())
	{
		out << endl;
		out << setw(6) << "row" << setw(9) << "column" << setw(5) << "id" << setw(9) << "lexem" << endl << endl;
		for (int i = 0; i < context.TokenAmount; i++)
		{
			out << setw(5) << context.OurTokens[i].pos_y << setw(8) << context.OurTokens[i].pos_x << setw(8) << context.OurTokens[i].id_lex << setw(8) << context.OurTokens[i].lex << endl << endl;
		}
		for (int i = 0; i < context.ErrorsAmount; i++)
		{
			out << "Lexer error: line" << setw(3) << context.OurErrors[i].pos_y << setw(8) << "column" << setw(3) << context.OurErrors[i].pos_x << setw(19) << context.OurErrors[i].text_error << setw(3) << "'" << context.OurErrors[i].err << "'" << endl << endl;
		}

		if (context.CommentError)
			out << "Lexer error: line" << setw(3) << context.pos_comment_y << setw(8) << "column" << setw(3) << context.pos_comment_x << setw(25) << "You must close comment!" << endl;
	}
}