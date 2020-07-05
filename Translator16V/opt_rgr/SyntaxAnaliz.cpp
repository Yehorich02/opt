#include "SyntaxAnaliz.h"

SyntaxAnaliz::SyntaxAnaliz(string File)
{
	ErrorStr = "";
	ErrorCounter = 0;
	Pos = 0;
	LeksAnaliz LexAn;
	LexAn.Analizator(File);
	IdentMin = LexAn.MinIdent;
	IdentMax = LexAn.LexErrorMin - 1;
	DigitMin = LexAn.MinDigit;
	DigitMax = LexAn.MinIdent - 1;
	Lexems = LexAn.Lexems;
	IdentTable = LexAn.Table;
	DigitTable = LexAn.TableD;
	err = false;
	start();
	if (ErrorCounter == 0) {
		cout << "syntax analysis completed successfully" << endl;
	}
}

bool SyntaxAnaliz::start()
{
	Tree = InitTree();
	return program(Tree);
}

bool SyntaxAnaliz::program(Node* NodePtr)
{
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<program>", false);
	N = N->Down;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].Code == 401) {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<procedure-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].Code == 59) {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				Pos++;
				N = N->Next;
				N->Next = AddNode(N, -1, "", -1, -1, "<block>", false);
				N = N->Next;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if (block(N)) {
					Pos++;
					if (Pos >= Lexems.size()) {
						Errors(1, Pos);
						return false;
					}
					if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 46)) {
						N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
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

bool SyntaxAnaliz::block(Node* NodePtr)
{
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if (Lexems[Pos].Code == 402)
	{
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
		N = N->Next;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if (statement_list(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].Code == 403)
			{
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
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

bool SyntaxAnaliz::statement_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
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
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool SyntaxAnaliz::statement(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<condition-statement>", false);
	if (Lexems[Pos].Code == 408) {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<conditional-expression>", false);
		N = N->Next;
		if (conditional_expression(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].Code == 409) {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Next;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<statement-list>", false);
				N = N->Next;
				if (statement_list(N)) {
					Pos++;
					if (Pos >= Lexems.size()) {
						Errors(1, Pos);
						return false;
					}
					if (Lexems[Pos].Code == 410) {
						N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
						N = N->Next;
						Pos++;
						if (Pos >= Lexems.size()) {
							Errors(1, Pos);
							return false;
						}
						if (Lexems[Pos].Code == ';') {
							N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
							return true;
						}
						else {
							Errors(2, Pos);
							return false;
						}
					}
					else {
						Errors(16, Pos);
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				Errors(15, Pos);
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		if (condition_statement(N->Down)) {
			N = N->Down;
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].Code == 407) {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Next;
				Pos++;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if (Lexems[Pos].Code == ';') {
					N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
					return true;
				}
				else {
					Errors(2, Pos);
					return false;
				}
			}
			else {
				Errors(13, Pos);
				return false;
			}
		}
		else {
			return false;
		}
		err = true;
		Errors(14, Pos);
		return false;
	}
}

bool SyntaxAnaliz::condition_statement(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<incomplete-condition-statement>", false);
	N = N->Down;
	if (incomplete_condition_statement(N)) {
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<alternative-part>", false);
		N = N->Next;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if (alternative_part(N)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool SyntaxAnaliz::conditional_expression(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<expression>", false);
	N = N->Down;
	if (expression(N)) {
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<comparison-operator>", false);
		N = N->Next;
		if (comparison_operator(N)) {
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<expression>", false);
			N = N->Next;
			if (expression(N)) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			err = true;
			return false;
		}
	}
	else {
		err = true;
		return false;
	}
}

bool SyntaxAnaliz::incomplete_condition_statement(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if (Lexems[Pos].Code == 404) {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<conditional-expression>", false);
		N = N->Next;
		if (conditional_expression(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].Code == 405) {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Next;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<statement-list>", false);
				N = N->Next;
				if (statement_list(N)) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				err = true;
				Errors(11, Pos);
				return false;
			}
		}
		else {
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool SyntaxAnaliz::alternative_part(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if (Lexems[Pos].Code == 406) {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<statement-list>", false);
		N = N->Next;
		if (statement_list(N)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		N = N->Down;
		Pos--;
		return true;
	}
}

bool SyntaxAnaliz::comparison_operator(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if (((Lexems[Pos].Code <= 62) && (Lexems[Pos].Code >= 59)) || ((Lexems[Pos].Code <= 400) && (Lexems[Pos].Code >= 301)))
	{
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		return true;
	}
	else {
		Errors(9, Pos);
		return false;
	}
}

bool SyntaxAnaliz::expression(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
	if (identifier(N->Down)) {
		N = N->Down;
		return true;
	}
	else if (integer(N)) {
		return true;
	}
	else
	{
		Errors(8, Pos);
		return false;
	}
}

bool SyntaxAnaliz::identifier(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].Code >= IdentMin) && (Lexems[Pos].Code <= IdentMax))
	{
		N->Down = AddNode(N, -1, "", -1, -1, "<identifier>", false);
		N = N->Down;
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		return true;
	}
	else
	{
		return false;
	}
}

bool SyntaxAnaliz::integer(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].Code >= DigitMin) && (Lexems[Pos].Code <= DigitMax))
	{
		N->Down = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
		N = N->Down;
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		return true;
	}
	else
	{
		return false;
	}
}

void SyntaxAnaliz::Errors(int p, int pos) {
	switch (p) {
	case 1:
		cout << "Syntactic error: Unexpected end of the file!" << endl;
		ErrorStr += "Syntactic error: Unexpected end of the file!\n";
		ErrorCounter++;
		break;
	case 2:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": \";\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": \";\" is passed\n";
		ErrorCounter++;
		break;
	case 3:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << " \".\" is passed after the \"END\"" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += " \".\" is passed after the \"END\"\n";
		ErrorCounter++;
		break;
	case 4:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"PROGRAM\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"PROGRAM\" is passed\n";
		ErrorCounter++;
		break;
	case 5:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be procedure identifier" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Must be procedure identifier\n";
		ErrorCounter++;
		break;
	case 6:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"BEGIN\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"BEGIN\" is passed\n";
		ErrorCounter++;
		break;
	case 7:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"END\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"END\" is passed\n";
		ErrorCounter++;
		break;
	case 8:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be constant or identifier" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Must be constant or identifier\n";
		ErrorCounter++;
		break;
	case 9:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be comparison operator" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Must be comparison operator\n";
		ErrorCounter++;
		break;
	case 11:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"THEN\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"THEN\" is passed\n";
		ErrorCounter++;
		break;
	case 12:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"ELSE\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"ELSE\" is passed\n";
		ErrorCounter++;
		break;
	case 13:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"ENDIF\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"ENDIF\" is passed\n";
		ErrorCounter++;
		break;
	case 14:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"WHILE\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"WHILE\" is passed\n";
		ErrorCounter++;
		break;
	case 15:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"DO\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"DO\" is passed\n";
		ErrorCounter++;
		break;
	case 16:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"ENDWHILE\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"ENDWHILE\" is passed\n";
		ErrorCounter++;
		break;
	}
}

Node* SyntaxAnaliz::InitTree()
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

Node* SyntaxAnaliz::AddNode(Node* Nd, int code, string name, int line, int column, string Func, bool isterminal)
{
	Node* Help = new Node;
	Help->Code = code;
	Help->Column = column;
	Help->Down = NULL;
	Help->IsTerminal = isterminal;
	Help->Line = line;
	Help->Name = name;
	Help->Next = NULL;
	Help->Func = new char[255];
	Help->Func = Func;
	return Help;
}

Node* SyntaxAnaliz::RemoveNode(Node* N)
{
	if (N != NULL) free(N);
	return NULL;
}

void SyntaxAnaliz::WriteTree(Node* Root, string sps)
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
			for (int i = 0; i < 4; i++) {
				sps.push_back(' ');
			}
			WriteTree(Root->Down, sps);
			for (int i = 0; i < 4; i++) {
				sps.pop_back();
			}
		}
		Root = Root->Next;
	}
}

void SyntaxAnaliz::TreeListing(string File) {
	TreeStr = "";
	File += "generated.txt";
	ofstream f;
	f.open(File);
	if (ErrorCounter > 0) {
		f << ErrorStr << endl;
	}
	else {
		WriteTree(Tree, "");
		f << TreeStr << endl;
	}
}