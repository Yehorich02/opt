#include "Utils.h"

void PrintDots(int counter, ofstream& out)
{
	for (int i = 0; i < counter; i++)
	{
		out << "_ ";
	}
}

void PrintNode(node* CurNode, int counter, ofstream& out)
{
	if (CurNode)
	{
		switch (CurNode->tokenType)
		{
			case TokenType::SIGNAL_PROGRAM :
			{
				out << "<signal-program>\n";
				break;
			}
			case TokenType::PROGRAM:
			{
				out << "<program>\n";
				break;
			}
			case TokenType::PROCEDURE_IDENTIFIER:
			{
				out << "<procedure-identifier>\n";
				PrintDots(counter + 3, out);
				out << CurNode->tokenValue.TokenNum<<" "<<CurNode->tokenValue.TokenName<<endl;
				break;
			}
			case TokenType::BLOCK:
			{
				out << "<block>\n";
				break;
			}
			case TokenType::STATEMENTS_LIST:
			{
				out << "<statements-list>\n";
				break;
			}
			case TokenType::STATEMENT:
			{
				out << "<statement>\n";
				break;
			}
			case TokenType::EMPTY:
			{
				out << "<empty>\n";
				break;
			}
			case TokenType::UNSIGNED_INTEGER:
			{
				out << "<unsigned-integer>\n";
				PrintDots(counter + 3, out);
				out << CurNode->tokenValue.TokenNum << " " << CurNode->tokenValue.TokenName << endl;
				break;
			}
			case TokenType::VARIABLE_IDENTIFIER:
			{
				out << "<variable-identifier>\n";
				PrintDots(counter + 3, out);
				out << CurNode->tokenValue.TokenNum << " " << CurNode->tokenValue.TokenName << endl;
				break;
			}
			case TokenType::ACTUAL_ARGUMENTS:
			{
				out << "<actual-arguments>\n";
				break;
			}
			case TokenType::ASSEMBLY_INSERT_FILE_IDINTIFIER:
			{
				out << "<assembly-insert-file-identifier>\n";
				break;
			}
			case TokenType::ACTUAL_ARGUMENTS_LIST:
			{
				out << "<actual-arguments-list>\n";
				break;
			}
			case TokenType::WRITE:
			{
				out << CurNode->tokenValue.TokenNum << " " << CurNode->tokenValue.TokenName << endl;
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

void Print(node* CurNode, int counter, ofstream& out)
{
	if (CurNode)
	{
		if (CurNode->tokenType != TokenType::NOTHING)
		{
			PrintDots(counter, out);
		}
		PrintNode(CurNode, counter, out);
		Print(CurNode->child, counter+3, out);
		Print(CurNode->next, counter, out);
	}
}

/////////////////////////////////////////////////////////////////////////////////
void ErrorMessage(Context& context, int& CurPosition, string expectedToken)
{
	context.ErrorString.append("Error (line: ").append(to_string(context.TokenResult[CurPosition].line))
		.append(" column: ").append(to_string(context.TokenResult[CurPosition].column))
		.append(") : expected '").append(expectedToken).append("' but '").append(context.TokenResult[CurPosition].TokenName)
		.append("' found\n");
}

void ErrorisStopParse(Context& context, int& CurPosition, string expectedToken)
{
	context.ErrorString.append("Error (line: ").append(to_string(context.TokenResult[CurPosition - 1].line))
		.append(" column: ").append(to_string(context.TokenResult[CurPosition - 1].column))
		.append(") : expected '").append(expectedToken).append("' but EOF found\n");
}

void UnexpectedEOF(Context& context, int& CurPosition)
{
	context.ErrorString.append("Error (line: ").append(to_string(context.TokenResult[CurPosition - 1].line))
		.append(" column: ").append(to_string(context.TokenResult[CurPosition - 1].column))
		.append(") unexpected EOF\n");
}

///////////////////////////////////////////////////////////////////////////////////////////

bool isStopParse(int CurPosition, Context& context)
{
	return CurPosition == context.TokenResult.size();
}

bool isProgram(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "PROGRAM";
}

bool isID(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	auto tmp = find_if(context.IdVector.begin(), context.IdVector.end(), [&](const auto& IDVector) {return IDVector.KeyWordName == token.TokenName; });
	return tmp != context.IdVector.end();
}

bool isConst(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	auto tmp = find_if(context.ConstVector.begin(), context.ConstVector.end(), [&](const auto& ConstVector) {return ConstVector.KeyWordName == token.TokenName; });
	return tmp != context.ConstVector.end();
}

bool isSemicolon(Context& context, int& CurPosition) // ';'
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenNum == 59;
}

bool isBegin(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "BEGIN";
}

bool isEnd(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "END";
}

bool isGoto(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "GOTO";
}

bool isLink(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "LINK";
}

bool isIn(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "IN";
}

bool isOut(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "OUT";
}

bool isReturn(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "RETURN";
}

bool isOpenBracket(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "(";
}

bool isCloseBracket(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == ")";
}

bool isComma(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == ",";
}

bool isColon(Context& context, int& CurPosition)
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == ":";
}

bool isDelim2(Context& context, int& CurPosition) // ':='
{
	auto token = context.TokenResult[CurPosition];

	return token.TokenName == ":=";
}

bool isOpenDollar(Context& context, int& CurPosition) // '$'
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "($";
}

bool isCloseDollar(Context& context, int& CurPosition) // '$'
{
	auto token = context.TokenResult[CurPosition];
	return token.TokenName == "$)";
}