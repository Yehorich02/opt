#include "SyntacticalAnalyzer.h"

using namespace std;

void SignalProgram(Context& context, int& CurPosition)
{
	context.tree.root.tokenType = TokenType::SIGNAL_PROGRAM;
	context.tree.root.child = new node;
	Program(context, CurPosition, context.tree.root.child);
}

void Program(Context& context, int& CurPosition, node* CurNode)
{
	CurNode->tokenType = TokenType::PROGRAM;
	CurNode->child = new node;
	if (!isProgram(context,CurPosition))  //check whether the first word is PROGRAM
	{
		ErrorMessage(context, CurPosition, "PROGRAM");
		return;
	}
	CurNode->child->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->tokenType = TokenType::WRITE;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<identifier>"); return; }
	CurNode->child->next = new node;

	if (!isID(context, CurPosition)) //if it`s not an identifier
	{
		ErrorMessage(context, CurPosition, "<identifier>");
		return;
	}
	CurNode->child->next->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->next->tokenType = TokenType::PROCEDURE_IDENTIFIER;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return; }
	CurNode->child->next->next = new node;

	if (!isSemicolon(context, CurPosition)) //if it`s not ';'
	{
		ErrorMessage(context, CurPosition, ";");
		return;
	}
	CurNode->child->next->next->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->next->next->tokenType = TokenType::WRITE;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "BEGIN"); return; }
	CurNode->child->next->next->next = new node;
	CurNode->child->next->next->next = new node;

	if (!Block(context, CurPosition, CurNode->child->next->next->next)) //invoke Block function
	{
		return;
	}
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return; }
	CurNode->child->next->next->next->next = new node;

	if (!isSemicolon(context, CurPosition)) //if it`s not ';'
	{
		ErrorMessage(context, CurPosition, ";");
		return;
	}
	CurNode->child->next->next->next->next->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->next->next->next->next->tokenType = TokenType::WRITE;
	CurPosition++;

}

bool Block(Context& context, int& CurPosition, node* CurNode)
{
	CurNode->tokenType = TokenType::BLOCK;
	CurNode->child = new node;
	if (!isBegin(context, CurPosition)) //if the checked word isn`t BEGIN
	{
		ErrorMessage(context, CurPosition, "BEGIN");
		return false;
	}
	CurNode->child->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->tokenType = TokenType::WRITE;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }
	CurNode->child->next = new node;

	if (auto work = StatementList(context, CurPosition, CurNode->child->next);
		work && !*work) // if in statement list there is an eror
	{
		return false;
	}

	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }
	CurNode->child->next->next = new node;

	if (!isEnd(context, CurPosition)) //if the checked word isn`t END
	{
		ErrorMessage(context, CurPosition, "END");
		return false;
	}
	CurNode->child->next->next->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->next->next->tokenType = TokenType::WRITE;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return false; }

	return true;
}

optional<bool> StatementList(Context& context, int& CurPosition, node* CurNode)
{
	CurNode->tokenType = TokenType::STATEMENTS_LIST;
	CurNode->child = new node;

	if (auto status = Statement(context, CurPosition, CurNode->child);
		!status)//if statement is empty
	{
		CurNode->child->tokenType = TokenType::EMPTY;
		return nullopt;  //statemest list is empty
	}
	else if (!*status) // there is an error
	{
		return false;
	}
	CurNode->child->next = new node;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	if ( auto status = StatementList(context, CurPosition, CurNode->child->next);
		status && !*status)
	{
		return false;
	}

	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	return true;
}

 optional<bool> Statement1(Context& context, int& CurPosition, node* CurNode)
{
	 
	if (!isConst(context, CurPosition))
	{
		return nullopt; //empty
	}
	CurNode->tokenType = TokenType::UNSIGNED_INTEGER;
	CurNode->tokenValue = context.TokenResult[CurPosition];
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ":"); return false; }
	CurNode->next = new node;
	
	if (!isColon(context, CurPosition))
	{
		ErrorMessage(context, CurPosition, ":");
		return false; //error
	}
	CurNode->next->tokenType = TokenType::WRITE;
	CurNode->next->tokenValue = context.TokenResult[CurPosition];
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "another statement"); return false; }
	CurNode->next->next = new node;

	if (!Statement(context, CurPosition, CurNode->next->next))
	{
		return false; 
	}
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return  false; }

	return true;
}

 optional<bool> Statement2(Context& context, int& CurPosition, node* CurNode)
 {
	 if (!isID(context, CurPosition))
	 {
		 return nullopt; //empty
	 }
	 CurNode->tokenType = TokenType::VARIABLE_IDENTIFIER;
	 CurNode->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ":="); return false; }
	 CurNode->next = new node;

	 if (!isDelim2(context, CurPosition))
	 {
		 CurPosition--;
		 return nullopt; 
	 }
	 CurNode->next->tokenType = TokenType::WRITE;
	 CurNode->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<unsigned integer>"); return false; }
	 CurNode->next->next = new node;

	 if (!isConst(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, "<unsigned integer>");
		 return false;
	 }
	 CurNode->next->next->tokenType = TokenType::UNSIGNED_INTEGER;
	 CurNode->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return false; }
	 CurNode->next->next->next = new node;

	 if (!isSemicolon(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, ";");
		 return false;
	 }
	 CurNode->next->next->next->tokenType = TokenType::WRITE;
	 CurNode->next->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	 return true;
 }

 optional<bool> Statement3(Context& context, int& CurPosition, node* CurNode)
 {
	 if (!isID(context, CurPosition))
	 {
		 return nullopt; //empty
	 }
	 CurNode->tokenType = TokenType::PROCEDURE_IDENTIFIER;
	 CurNode->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { UnexpectedEOF(context, CurPosition); return false; }
	 CurNode->next = new node;

	 if (!Argument(context, CurPosition, CurNode->next))
	 {
		 return false;
	 }

	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return false; }
	 CurNode->next->next = new node;

	 if (!isSemicolon(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, ";");
		 return false;
	 }
	 CurNode->next->next->tokenType = TokenType::WRITE;
	 CurNode->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	 return true;
 }

 optional<bool> Statement4(Context& context, int& CurPosition, node* CurNode)
 {
	 if (!isGoto(context, CurPosition))
	 {
		 return nullopt; //empty
	 }
	 CurNode->tokenType = TokenType::WRITE;
	 CurNode->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<unsigned integer>"); return false; }
	 CurNode->next = new node;

	 if (!isConst(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, "<unsigned-integer>");
		 return false;
	 }
	 CurNode->next->tokenType = TokenType::UNSIGNED_INTEGER;
	 CurNode->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return false; }
	 CurNode->next->next = new node;

	 if (!isSemicolon(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, ";");
		 return false;
	 }
	 CurNode->next->next->tokenType = TokenType::WRITE;
	 CurNode->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	 return true;
 }

 optional<bool> Statement5(Context& context, int& CurPosition, node* CurNode)
 {
	 if (!isLink(context, CurPosition))
	 {
		 return nullopt; //empty
	 }
	 CurNode->tokenType = TokenType::WRITE;
	 CurNode->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<identifier>"); return false; }
	 CurNode->next = new node;

	 if (!isID(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, "<identifier>");
		 return false;
	 }
	 CurNode->next->tokenType = TokenType::VARIABLE_IDENTIFIER;
	 CurNode->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ","); return false; }
	 CurNode->next->next = new node;

	 if (!isComma(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, ",");
		 return false;
	 }
	 CurNode->next->next->tokenType = TokenType::WRITE;
	 CurNode->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<unsigned integer>"); return false; }
	 CurNode->next->next->next = new node;

	 if (!isConst(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, "<unsigned integer>");
		 return false;
	 }
	 CurNode->next->next->next->tokenType = TokenType::UNSIGNED_INTEGER;
	 CurNode->next->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return false; }
	 CurNode->next->next->next->next = new node;

	 if (!isSemicolon(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, ";");
		 return false;
	 }
	 CurNode->next->next->next->next->tokenType = TokenType::WRITE;
	 CurNode->next->next->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	 return true;
 }

 optional<bool> Statement6(Context& context, int& CurPosition, node* CurNode)
 {
	 if (!isIn(context, CurPosition))
	 {
		 return nullopt; //empty
	 }
	 CurNode->tokenType = TokenType::WRITE;
	 CurNode->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<unsigned integer>"); return false; }
	 CurNode->next = new node;

	 if (!isConst(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, "<unsigned integer>");
		 return false;
	 }
	 CurNode->next->tokenType = TokenType::UNSIGNED_INTEGER;
	 CurNode->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return false; }
	 CurNode->next->next = new node;

	 if (!isSemicolon(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, ";");
		 return false;
	 }
	 CurNode->next->next->tokenType = TokenType::WRITE;
	 CurNode->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	 return true;
 }

 optional<bool> Statement7(Context& context, int& CurPosition, node* CurNode)
 {
	 if (!isOut(context, CurPosition))
	 {
		 return nullopt; //empty
	 }
	 CurNode->tokenType = TokenType::WRITE;
	 CurNode->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<unsigned integer>"); return false; }
	 CurNode->next = new node;

	 if (!isConst(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, "<unsigned integer>");
		 return false;
	 }
	 CurNode->next->tokenType = TokenType::UNSIGNED_INTEGER;
	 CurNode->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return false; }
	 CurNode->next->next = new node;

	 if (!isSemicolon(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, ";");
		 return false;
	 }
	 CurNode->next->next->tokenType = TokenType::WRITE;
	 CurNode->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	 return true;
 }

 optional<bool> Statement8(Context& context, int& CurPosition, node* CurNode)
 {
	 if (!isReturn(context, CurPosition))
	 {
		 return nullopt; //empty
	 }
	 CurNode->tokenType = TokenType::WRITE;
	 CurNode->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ";"); return false; }
	 CurNode->next = new node;

	 if (!isSemicolon(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, ";");
		 return false;
	 }
	 CurNode->next->tokenType = TokenType::WRITE;
	 CurNode->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	 return true;
 }

 optional<bool> Statement9(Context& context, int& CurPosition, node* CurNode)
 {
	 if (!isSemicolon(context, CurPosition))
	 {
		 return nullopt;
	 }
	 CurNode->tokenType = TokenType::WRITE;
	 CurNode->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	 return true;
 }

 optional<bool> Statement10(Context& context, int& CurPosition, node* CurNode)
 {
	 if (!isOpenDollar(context, CurPosition))
	 {
		 return nullopt; 
	 }
	 CurNode->tokenType = TokenType::WRITE;
	 CurNode->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<identifier>"); return false; }
	 CurNode->next = new node;
	 if (!isID(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, "<identifier>");
		 return false;
	 }
	 CurNode->next->tokenType = TokenType::ASSEMBLY_INSERT_FILE_IDINTIFIER;
	 CurNode->next->child = new node;
	 CurNode->next->child->tokenValue = context.TokenResult[CurPosition];
	 CurNode->next->child->tokenType = TokenType::WRITE;
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "$)"); return false; }
	 CurNode->next->next = new node;

	 if (!isCloseDollar(context, CurPosition))
	 {
		 ErrorMessage(context, CurPosition, "$)");
		 return false;
	 }
	 CurNode->next->next->tokenType = TokenType::WRITE;
	 CurNode->next->next->tokenValue = context.TokenResult[CurPosition];
	 CurPosition++;
	 if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	 return true;
 }

 optional<bool> Statement(Context& context, int& CurPosition, node* CurNode)
 {
	 
	 CurNode->child = new node;
	 if (auto status = Statement1(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }

	 if (auto status = Statement2(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }

	 if (auto status = Statement3(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }

	 if (auto status = Statement4(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }

	 if (auto status = Statement5(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }

	 if (auto status = Statement6(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }

	 if (auto status = Statement7(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }

	 if (auto status = Statement8(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }

	 if (auto status = Statement9(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }

	 if (auto status = Statement10(context, CurPosition, CurNode->child);
		 status)
	 {
		 CurNode->tokenType = TokenType::STATEMENT;
		 return *status;
	 }
	 return nullopt;
 }

bool Argument(Context& context, int& CurPosition, node* CurNode)
{
	CurNode->tokenType = TokenType::ACTUAL_ARGUMENTS;
	CurNode->child = new node;
	if (!isOpenBracket(context, CurPosition))
	{
		ErrorMessage(context, CurPosition, "(");
		return false;
	}
	CurNode->child->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->tokenType = TokenType::WRITE;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<identifier>"); return false; }
	CurNode->child->next = new node;
	
	if (!isID(context, CurPosition))
	{
		ErrorMessage(context, CurPosition, "<identifier>");
		return false;
	}
	CurNode->child->next->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->next->tokenType = TokenType::VARIABLE_IDENTIFIER;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { UnexpectedEOF(context, CurPosition); return false; }
	CurNode->child->next->next = new node;
	CurNode->child->next->next->child = new node;

	if (auto status = ArgumentList(context, CurPosition, CurNode->child->next->next->child);
		status && !*status)
	{
		return false;
	}

	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, ")"); return false; }
	CurNode->child->next->next->next = new node;

	if (!isCloseBracket(context, CurPosition))
	{		
		ErrorMessage(context, CurPosition, ")");
		return false;
	}
	CurNode->child->next->next->next->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->next->next->next->tokenType = TokenType::WRITE;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }
	return true;
}

optional<bool> ArgumentList(Context& context, int& CurPosition, node* CurNode)
{
	CurNode->tokenType = TokenType::ACTUAL_ARGUMENTS_LIST;
	CurNode->child = new node;
	if (!isComma(context, CurPosition))
	{
		CurNode->child->tokenType = TokenType::EMPTY;
		return nullopt;
	}
	CurNode->child->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->tokenType = TokenType::WRITE;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "<identifier>"); return false; }
	CurNode->child->next = new node;

	if (!isID(context, CurPosition))
	{
		ErrorMessage(context, CurPosition, "<identifier>");
		return false;
	}
	CurNode->child->next->tokenValue = context.TokenResult[CurPosition];
	CurNode->child->next->tokenType = TokenType::VARIABLE_IDENTIFIER;
	CurPosition++;
	if (isStopParse(CurPosition, context)) { UnexpectedEOF(context, CurPosition); return false; }
	CurNode->child->next->next = new node;
	CurNode->child->next->next->child = new node;

	if (auto status = ArgumentList(context, CurPosition, CurNode->child->next->next->child);
		status && !*status)
	{
		return false;
	}
	if (isStopParse(CurPosition, context)) { ErrorisStopParse(context, CurPosition, "END"); return false; }

	return true;

}
