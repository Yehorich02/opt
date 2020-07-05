#include "SyntaxAnaliz.h"

SyntaxAnaliz::SyntaxAnaliz(string File)
{
	ErrorStr = "";
	ErrorCounter = 0;
	Pos = 0;
	LeksAnaliz LexAn;
	LexAn.Analizator(File);
	err = false;
	IdentMin = LexAn.MinIdent;
	IdentMax = LexAn.LexErrorMin - 1;
	Lexems = LexAn.Lexems;
	IdentTable = LexAn.Table;
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
					if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 59)) {
						N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
						Pos++;
						return true;
					}
					else
					{
						Errors(2, Pos);
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
	N->Down = AddNode(N, -1, "", -1, -1, "<declarations>", false);
	N = N->Down;
	if (declarations(N)) {
		Pos++;
		if (Lexems[Pos].Code == 403)
		{
			N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
			N = N->Next;
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
				if (Lexems[Pos].Code == 404)
				{
					N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
					return true;
				}
				else
				{
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
			if (err = false) {
				Errors(6, Pos);
				return false;
			}
			else
				return false;
		}
	}
}

bool SyntaxAnaliz::declarations(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<procedure-declarations>", false);
	N = N->Down;
	if (procedure_declarations(N)) {
		return true;
	}
	else {
		return false;
	}
}

bool SyntaxAnaliz::procedure_declarations(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<procedure>", false);
	if (procedure(N->Down)) {
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<procedure-declarations>", false);
		N = N->Next;
		if (procedure_declarations(N)) {
			return true;
		}
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool SyntaxAnaliz::procedure(Node* NodePtr) {
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
		N->Next = AddNode(N, -1, "", -1, -1, "<procedure-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<parameters-list>", false);
			N = N->Next;
			if (parameters_list(N)) {
				Pos++;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 59)) {
					N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
					return true;
				}
				else
				{
					err = false;
					Errors(2, Pos);
					return false;
				}
			}
			else {
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
		Errors(3, Pos);
		return false;
	}
}

bool SyntaxAnaliz::parameters_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if (Lexems[Pos].Code == 40) {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<declarations-list>", false);
		N = N->Next;
		if (declarations_list(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].Code == 41) {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Next;
				return true;
			}
			else {
				err = true;
				Errors(10, Pos);
				return false;
			}
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

bool SyntaxAnaliz::declarations_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<declaration", false);
	if (declaration(N->Down)) {
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<declarations-list>", false);
		N = N->Next;
		if (declarations_list(N)) {
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

bool SyntaxAnaliz::declaration(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
	if (identifier(N->Down)) {
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<identifiers-list>", false);
		N = N->Next;
		if (identifiers_list(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].Code == 58) {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Next;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<attribute>", false);
				N = N->Next;
				if (attribute(N)) {
					Pos++;
					if (Pos >= Lexems.size()) {
						Errors(1, Pos);
						return false;
					}
					N->Next = AddNode(N, -1, "", -1, -1, "<attributes-list>", false);
					N = N->Next;
					if (attributes_list(N)) {
						Pos++;
						if (Pos >= Lexems.size()) {
							Errors(1, Pos);
							return false;
						}
						if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 59)) {
							N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
							return true;
						}
						else
						{
							err == true;
							Errors(2, Pos);
							return false;
						}
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				err == true;
				Errors(9, Pos);
				return false;
			}
		}
	}
	else {
		Errors(5, Pos);
		return false;
	}
}

bool SyntaxAnaliz::identifiers_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if (Lexems[Pos].Code == 44)
	{
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			N->Next = AddNode(N, -1, "", -1, -1, "<identifiers-list>", false);
			N = N->Next;
			if (identifiers_list(N)) {
				return true;
			}
			else {
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
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		N = N->Down;
		Pos--;
		return true;
	}
}

bool SyntaxAnaliz::attributes_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<attribute>", false);
	if (attribute(N->Down)) {
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<attributes-list>", false);
		N = N->Next;
		if (attributes_list(N)) {
			return true;
		}
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		N = N->Down;
		Pos--;
		return true;
	}
}

bool SyntaxAnaliz::attribute(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if ((Lexems[Pos].Code >= 405) && (Lexems[Pos].Code <= 410)) {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		return true;
	}
	else {
		return false;
	}
}

bool SyntaxAnaliz::statement_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
	N = N->Down;
	Pos--;
	return true;
}

bool SyntaxAnaliz::identifier(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].Code >= IdentMin) && (Lexems[Pos].Code < IdentMax))
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

	case 4:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"PROGRAM\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Keyword \"PROGRAM\" is passed\n";
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
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be identifier" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Must be identifier\n";
		ErrorCounter++;
		break;
	case 9:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": \":\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": \":\" is passed\n";
		ErrorCounter++;
		break;
	case 10:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": \")\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": \")\" is passed\n";
		ErrorCounter++;
		break;
	case 11:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be attribute" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		ErrorStr += ": Must be attribute\n";
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