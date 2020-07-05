#include <iostream>
#include <fstream>
#include <iomanip>
#include "PrintFunctions.h"
using namespace std;

void PrintResult(const Context& context, ofstream& out) // function for printing result
{
	for (const auto& token : context.TokenResult)
	{
		out << "Token name: " << setw(10) << left << token.TokenName << setw(10) << " | Token number: " << setw(10) << left << token.TokenNum << setw(10);
		out << " | line: " << setw(10) << left << token.line << " | column: " << setw(10) << left << token.column << "\n";
	}
}

void ErrorInNumber(const Context& context, char tmp, ofstream& out)
{

	out << "Error: Can not be a letter after digit on line: " << context.CurLine << " column: " << context.CurColumn << " invalid character: " << tmp << endl;
}

void ErrorInDollar(const Context& context, char tmp, ofstream& out)
{
	out << "Error: After '$' should be ')' on line: " << context.CurLine << " column: " << context.CurColumn << " invalid character: ' " <<  tmp<<"'" << endl;
}

void ErrorInDelim2(const Context& context, char tmp, ofstream& out)
{
	out << "Error: After ':' should be '=' on line: " << context.CurLine << " column: " << context.CurColumn << " invalid character: '" <<"'" << tmp << endl;
}

void InvalidValue(const Context& context, char tmp, ofstream& out)
{
	out << "Error: invalid character " << tmp << " on line: " << context.CurLine << " column: " << context.CurColumn << endl;
}

void UnclosedComment(const Context& context, char tmp, ofstream& out)
{
	out << "Error: unclosed comment" << endl;
}