#include "Parser.h"

Parser::Parser(Context& contextt, string File)
{
	ErrorStr = "";
	ErrorCounter = 0;
	Pos = 0;
	context = contextt;
	IdentMin = 1000;
	IdentMax = 2000;
	DigitMin = 500;
	DigitMax = 999;
	err = false;
	start();
	if (ErrorCounter == 0) {
		cout << "syntax analysis completed successfully" << endl;
	}
	TreeListing(File); //вивод дерева розбору
}

void Parser::start()
{
	Tree = InitTree();
	program(Tree);
}

bool Parser::program(Node* NodePtr)
{
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<program>", false); // робить відступ
	N = N->Down;
	if (Pos >= context.TokenAmount) {
		Errors(1, Pos);
		return false;
	}
	if (context.OurTokens[Pos].id_lex == 400) {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<procedure-identifier>", false); // просто добавляє
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			if (Pos >= context.TokenAmount) {
				Errors(1, Pos);
				return false;
			}
			if (context.OurTokens[Pos].id_lex == 59) {
				N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
				N = N->Next;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<block>", false);
				N = N->Next;
				if (block(N)) {
					Pos++;
					if (Pos >= context.TokenAmount) {
						Errors(1, Pos);
						return false;
					}
					if ((Pos < context.TokenAmount) && (context.OurTokens[Pos].id_lex == 59)) {
						N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
						Pos++;
						return true;
					}
					else
					{
						Errors(3, Pos);
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				Errors(2, Pos);
				return false;
			}
		}
		else {
			Errors(5, Pos);
			return false;
		}
	}
	else {
		Errors(4, Pos);
		return false;
	}
}

bool Parser::block(Node* NodePtr)
{
	if (Pos >= context.TokenAmount) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if (context.OurTokens[Pos].id_lex == 401)
	{
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
		N = N->Next;
		if (statement_list(N)) {
			Pos++;
			if (Pos >= context.TokenAmount) {
				Errors(1, Pos);
				return false;
			}
			if (context.OurTokens[Pos].id_lex == 402)
			{
				N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
				return true;
			}
			else
			{
				if (err == false)
					Errors(7, Pos);
				return false;
			}
		}
		else {
			return false;
		}
	}
	else
	{
		Errors(6, Pos);
		return false;
	}
}

bool Parser::statement_list(Node* NodePtr) {
	if (Pos >= context.TokenAmount) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<statement>", false);
	if (statement(N->Down)) {
		N = N->Down;
		N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
		N = N->Next;
		Pos++;
		if (statement_list(N)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (err == false) {
			N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
			Pos--;
			return true;
		}
		else
			return false;
	}
}

bool Parser::statement(Node* NodePtr) {
	if (Pos >= context.TokenAmount) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
	if (integer(N->Down)) {
		N = N->Down;
		Pos++;
		if (context.OurTokens[Pos].id_lex == ':') {
			N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
			N = N->Next;
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<statement>", false);
			N = N->Next;
			if (statement(N)) {
				return true;
			}
			else {
				err = true;
				return false;
			}
		}
		else {
			err = true;
			Errors(11, Pos);
			return false;
		}
	}
	else if (context.OurTokens[Pos].id_lex == 403) {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
		N = N->Next;
		if (integer(N)) {
			Pos++;
			if (Pos >= context.TokenAmount) {
				Errors(1, Pos);
				return false;
			}
			if (context.OurTokens[Pos].id_lex == ';') {
				N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
				return true;
			}
			else {
				err = true;
				Errors(2, Pos);
				return false;
			}
		}
		else {
			err = true;
			Errors(8, Pos);
			return false;
		}
	}
	else if (context.OurTokens[Pos].id_lex == 404) {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			if (Pos >= context.TokenAmount) {
				Errors(1, Pos);
				return false;
			}
			if (context.OurTokens[Pos].id_lex == ',') {
				N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
				N = N->Next;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
				N = N->Next;
				if (integer(N)) {
					Pos++;
					if (context.OurTokens[Pos].id_lex == ';') {
						N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
						N = N->Next;
						return true;
					}
					else {
						err = true;
						Errors(2, Pos);
						return false;
					}
				}
				else {
					err = true;
					Errors(8, Pos);
					return false;
				}
			}
			else {
				err = true;
				Errors(9, Pos);
				return false;
			}
		}
		else {
			err = true;
			Errors(8, Pos);
			return false;
		}
	}
	else if (context.OurTokens[Pos].id_lex == 405) {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
		N = N->Next;
		if (integer(N)) {
			Pos++;
			if (Pos >= context.TokenAmount) {
				Errors(1, Pos);
				return false;
			}
			if (context.OurTokens[Pos].id_lex == ';') {
				N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
				return true;
			}
			else {
				err = true;
				Errors(2, Pos);
				return false;
			}
		}
		else {
			err = true;
			Errors(8, Pos);
			return false;
		}
	}
	else if (context.OurTokens[Pos].id_lex == 406) {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
		N = N->Next;
		if (integer(N)) {
			Pos++;
			if (Pos >= context.TokenAmount) {
				Errors(1, Pos);
				return false;
			}
			if (context.OurTokens[Pos].id_lex == ';') {
				N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
				return true;
			}
			else {
				err = true;
				Errors(2, Pos);
				return false;
			}
		}
		else {
			err = true;
			Errors(8, Pos);
			return false;
		}
	}
	else  if (context.OurTokens[Pos].id_lex == 407) {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		if (Pos >= context.TokenAmount) {
			Errors(1, Pos);
			return false;
		}
		if (context.OurTokens[Pos].id_lex == ';') {
			N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
			return true;
		}
		else {
			err = true;
			Errors(2, Pos);
			return false;
		}
	}
	else if (context.OurTokens[Pos].id_lex == ';') {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		return true;
	}
	else if (context.OurTokens[Pos].id_lex == 302) {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<assembly-insert-file-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			if (context.OurTokens[Pos].id_lex == 303) {
				N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
				return true;
			}
			else {
				err = true;
				Errors(13, Pos);
				return false;
			}
		}
		else {
			err = true;
			Errors(10, Pos);
			return false;
		}
	}
	else if (context.OurTokens[Pos + 1].id_lex == 301) {
		N->Down = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
		if (identifier(N->Down)) {
			N = N->Down;
			Pos++;
			if (Pos >= context.TokenAmount) {
				Errors(1, Pos);
				return false;
			}
			if (context.OurTokens[Pos].id_lex == 301) {
				N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
				N = N->Next;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
				N = N->Next;
				if (integer(N)) {
					Pos++;
					if (context.OurTokens[Pos].id_lex == ';') {
						N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
						N = N->Next;
						return true;
					}
					else {
						err = true;
						Errors(2, Pos);
						return false;
					}
				}
				else {
					err = true;
					Errors(8, Pos);
					return false;
				}
			}
			else {
				err = true;
				Errors(15, Pos);
				return false;
			}
		}
		else {
			err = true;
			Errors(8, Pos);
			return false;
		}
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<procedure-identifier>", false);
		if (identifier(N->Down)) {
			N = N->Down;
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<actual-arguments>", false);
			N = N->Next;
			if (actual_arguments(N)) {
				Pos++;
				if (context.OurTokens[Pos].id_lex == ';') {
					N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
					N = N->Next;
					return true;
				}
				else {
					err = true;
					Errors(2, Pos);
					return false;
				}
			}
			else {
				err = true;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

bool Parser::actual_arguments(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= context.TokenAmount) {
		Errors(1, Pos);
		return false;
	}
	if (context.OurTokens[Pos].id_lex == '(') {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<actual-arguments-list>", false);
			N = N->Next;
			if (actual_arguments_list(N)) {
				Pos++;
				if (context.OurTokens[Pos].id_lex == ')') {
					N->Next = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
					N = N->Next;
					return true;
				}
				else {
					err = true;
					Errors(14, Pos);
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			err = true;
			Errors(12, Pos);
			return false;
		}
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::actual_arguments_list(Node* NodePtr) { //перевірка ідентифікатора
	Node* N = NodePtr;
	if (Pos >= context.TokenAmount) {
		Errors(1, Pos);
		return false;
	}
	if (context.OurTokens[Pos].id_lex == ',') {
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<actual-arguments-list>", false);
			N = N->Next;
			if (actual_arguments_list(N)) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			err = true;
			Errors(12, Pos);
			return false;
		}
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::identifier(Node* NodePtr) { //перевірка ідентифікатора
	Node* N = NodePtr;
	if (Pos >= context.TokenAmount) {
		Errors(1, Pos);
		return false;
	}
	if ((context.OurTokens[Pos].id_lex >= IdentMin) && (context.OurTokens[Pos].id_lex <= IdentMax))
	{
		N->Down = AddNode(N, -1, "", -1, -1, "<identifier>", false);
		N = N->Down;
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::integer(Node* NodePtr) { // перевірка на число
	Node* N = NodePtr;
	if (Pos >= context.TokenAmount) {
		Errors(1, Pos);
		return false;
	}
	if ((context.OurTokens[Pos].id_lex >= DigitMin) && (context.OurTokens[Pos].id_lex <= DigitMax))
	{
		N->Down = AddNode(N, context.OurTokens[Pos].id_lex, context.OurTokens[Pos].lex, context.OurTokens[Pos].pos_y, context.OurTokens[Pos].pos_x, "", true);
		N = N->Down;
		return true;
	}
	else
	{
		return false;
	}
}

void Parser::Errors(int p, int pos) { //обробка помилок
	switch (p) {
	case 1:
		cout << "Parser Error: Unexpected end of the file!" << endl << endl;
		ErrorStr += "Parser Error: Unexpected end of the file!\n";
		ErrorCounter++;
		break;
	case 2:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << ": \";\" is passed" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " : \";\" is passed\n";
		ErrorCounter++;
		break;
	case 3:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " \";\" is passed after the \"END\"" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " \";\" is passed after the \"END\"\n";
		ErrorCounter++;
		break;
	case 4:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " : Keyword \"PROGRAM\" is passed" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " : Keyword \"PROGRAM\" is passed\n";
		ErrorCounter++;
		break;
	case 5:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " : Must be procedure identifier" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " : Must be procedure identifier\n";
		ErrorCounter++;
		break;
	case 6:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " : Keyword \"BEGIN\" is passed" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " : Keyword \"BEGIN\" is passed\n";
		ErrorCounter++;
		break;
	case 7:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " : Keyword \"END\" is passed" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " : Keyword \"END\" is passed\n";
		ErrorCounter++;
		break;
	case 8:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " : Must be integer" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " : Must be integer\n";
		ErrorCounter++;
		break;
	case 9:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " \",\" is passed" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " \",\" is passed\n";
		ErrorCounter++;
		break;
	case 10:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " : Must be assembly insert file identifier" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " : Must be assembly insert file identifier\n";
		ErrorCounter++;
		break;
	case 11:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " \":\" is passed" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " \":\" is passed\n";
		ErrorCounter++;
		break;

	case 12:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " : Must be identifier" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " : Must be identifier\n";
		ErrorCounter++;
		break;
	case 13:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " \"$)\" is passed" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + ") \"$)\" is passed\n";
		ErrorCounter++;
		break;
	case 14:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " \")\" is passed" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " \")\" is passed\n";
		ErrorCounter++;
		break;
	case 15:
		cout << "Parser Error: line " << context.OurTokens[pos].pos_y << ", column " << context.OurTokens[pos].pos_x << " \":=\" is passed" << endl << endl;
		ErrorStr += "Parser Error: line " + to_string(context.OurTokens[pos].pos_y) + ", column " + to_string(context.OurTokens[pos].pos_x) + " \":=\" is passed\n";
		ErrorCounter++;
		break;
	}
}

Node* Parser::InitTree()
{
	Node* Help = new Node;
	Help->Code = -1;
	Help->Column = -1;
	Help->Down = NULL;
	Help->IsTerminal = false;
	Help->Line = -1;
	Help->Name = "";
	Help->Next = NULL;
	Help->Func = "<signal-program>";
	return Help;
}

Node* Parser::AddNode(Node* Nd, int code, string name, int pos_y, int pos_x, string Func, bool isterminal)
{
	Node* Help = new Node;
	Help->Code = code;
	Help->Column = pos_x;
	Help->Down = NULL;
	Help->IsTerminal = isterminal;
	Help->Line = pos_y;
	Help->Name = name;
	Help->Next = NULL;
	Help->Func = new char[255];
	Help->Func = Func;
	return Help;
}

Node* Parser::RemoveNode(Node* N)
{
	if (N != NULL) free(N);
	return NULL;
}

void Parser::WriteTree(Node* Root, string sps)
{
	while (Root != NULL)
	{
		if (Root->IsTerminal)
		{
			TreeStr += sps + to_string(Root->Code) + "   " + Root->Name + "\n";
		}
		else
		{
			TreeStr += sps + Root->Func + "\n";
		}
		if (Root->Down != NULL) {
			for (int i = 0; i < 3; i++) {
				sps.push_back(' ');
			}
			WriteTree(Root->Down, sps);
			for (int i = 0; i < 3; i++) {
				sps.pop_back();
			}
		}
		Root = Root->Next;
	}
}

void Parser::TreeListing(string File) {
	TreeStr = "";
	ofstream f;
	f.open(File, ios::app);
	if (ErrorCounter > 0) {
		f << ErrorStr << endl;
	}
	else {
		WriteTree(Tree, "");
		f << TreeStr << endl;
	}
}