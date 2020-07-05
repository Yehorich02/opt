#include "Parser.h"

Parser::Parser(string File)
{
	ErrorStr = "";
	ErrorCounter = 0;
	Pos = 0;
	LeksAnaliz LexAn;
	LexAn.Analizator(File);
	IdentMin = LexAn.MinIdent;
	IdentMax = LexAn.LexErrorMin - 1;
	Lexems = LexAn.Lexems;
	IdentTable = LexAn.Table;
	err = false;
	start();
	if (ErrorCounter == 0)
		cout << "Parser analysis completed successfully" << endl;
}

bool Parser::start()
{
	Tree = InitTree();
	return program(Tree);
}

bool Parser::program(Node* NodePtr)
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
			N->Next = AddNode(N, -1, "", -1, -1, "<parameters-list>", false);
			N = N->Next;
			if (parameters_list(N)) {
				Pos++;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if (Lexems[Pos].Code == 59) {
					N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
					N = N->Next;
					Pos++;
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
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<declarations>", false);
	N = N->Down;
	if (declarations(N)) {
		Pos++;
		if (Lexems[Pos].Code == 402)
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
				if (Lexems[Pos].Code == 403)
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
			if (err == false)
				Errors(6, Pos);
			return false;
		}
	}
	else {
		return false;
	}
}
bool Parser::declarations(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<variable-declarations>", false);
	N = N->Down;
	if (Lexems[Pos].Code == 410) {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
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
		Pos--;
		return true;
	}
}

bool Parser::statement_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
	Pos--;
	return true;
}

bool Parser::declarations_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<declaration>", false);
	if (declaration(N->Down)) {
		N = N->Down;
		N->Next = AddNode(N, -1, "", -1, -1, "<declaraions-list>", false);
		N = N->Next;
		Pos++;
		if (declarations_list(N)) {
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

bool Parser::attributes_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<attribute>", false);
	if (attribute(N->Down)) {
		N = N->Down;
		N->Next = AddNode(N, -1, "", -1, -1, "<attributes-list>", false);
		N = N->Next;
		Pos++;
		if (attributes_list(N)) {
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

bool Parser::parameters_list(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].Code == '(') {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<declarations-list>", false);
		N = N->Next;
		if (declarations_list(N)) {
			Pos++;
			if (Lexems[Pos].Code == ')') {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				return true;
			}
			else {
				Errors(11, Pos);
				return false;
			}
		}
		else {
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

bool Parser::declaration(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	N->Down = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
	N = N->Down;
	if (identifier(N)) {
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<identifiers-list>", false);
		N = N->Next;
		if (identifiers_list(N)) {
			Pos++;
			if (Lexems[Pos].Code == ':') {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Next;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<attribute>", false);
				N = N->Next;
				if (attribute(N)) {
					Pos++;
					N->Next = AddNode(N, -1, "", -1, -1, "<attributes-list>", false);
					N = N->Next;
					if (attributes_list(N)) {
						Pos++;
						if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 59)) {
							N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
							return true;
						}
						else
						{
							err = true;
							Errors(3, Pos);
							return false;
						}
					}
					else
					{
						err = true;
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
				err = true;
				Errors(9, Pos);
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

bool Parser::attribute(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].Code >= 404) && (Lexems[Pos].Code <= 409))
	{
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::identifiers_list(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].Code == ',') {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
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
			Errors(10, Pos);
			return false;
		}
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::identifier(Node* NodePtr) {
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

void Parser::Errors(int p, int pos) {
	switch (p) {
	case 1:
		cout << "Parser Error Unexpected end of the file!" << endl;
		ErrorStr += "Parser Error Unexpected end of the file!\n";
		ErrorCounter++;
		break;
	case 2:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << "): \";\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : \";\" is passed\n";
		ErrorCounter++;
		break;
	case 3:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \";\" is passed after the \"END\"" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \";\" is passed after the \"END\"\n";
		ErrorCounter++;
		break;
	case 4:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Keyword \"PROCEDURE\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Keyword \"PROCEDURE\" is passed\n";
		ErrorCounter++;
		break;
	case 5:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Must be procedure identifier" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Must be procedure identifier\n";
		ErrorCounter++;
		break;
	case 6:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Keyword \"BEGIN\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Keyword \"BEGIN\" is passed\n";
		ErrorCounter++;
		break;
	case 7:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Keyword \"END\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Keyword \"END\" is passed\n";
		ErrorCounter++;
		break;
	case 8:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \",\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \",\" is passed\n";
		ErrorCounter++;
		break;
	case 9:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \":\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \":\" is passed\n";
		ErrorCounter++;
		break;

	case 10:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Must be variable identifier" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Must be variable identifier\n";
		ErrorCounter++;
		break;
	case 11:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \")\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \")\" is passed\n";
		ErrorCounter++;
		break;
	case 12:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Must be attribute" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Must be attribute\n";
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

Node* Parser::AddNode(Node* Nd, int code, string name, int line, int column, string Func, bool isterminal)
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

void Parser::TreeListing(string File) {
	TreeStr = "";
	File += "generated.txt";
	ofstream f;
	f.open(File);
	if (ErrorCounter > 0) {
		f << "Parser analysis completed with error" << endl;
		f << ErrorStr;
	}
	else {
		WriteTree(Tree, "");
		f << TreeStr << endl;
	}
}