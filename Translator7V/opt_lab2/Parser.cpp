#include "Parser.h"

Parser::Parser(string File)
{
	LeksAnaliz LexAn;
	LexAn.Analizator(File);
	IdentMin = LexAn.MinIdent;
	IdentMax = LexAn.LexErrorMin - 1;
	IntMax = LexAn.MinIdent - 1;
	IntMin = LexAn.MinDigit;
	Lexems = LexAn.Lexems;
	IdentTable = LexAn.Table;
	Err = false;
	ErrorStr = "";
	ErrorCounter = 0;
	Pos = 0;
	Start();
	TreeListing(File);
	if (ErrorCounter == 0)
		cout << "Parser analysis completed successfully" << endl;
}

bool Parser::Start()
{
	SyntaxTree = InitilizationTree();
	return Program(SyntaxTree);
}

bool Parser::Program(Node* Ptr)
{
	Node* N = Ptr;
	N->Parent = AddNode(N, -1, "", -1, -1, "<program>", false);
	N = N->Parent;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].Code == 401) {
		N->Parent = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Parent;
		Pos++;
		N->Child = AddNode(N, -1, "", -1, -1, "<procedure-identifier>", false);
		N = N->Child;
		if (Identifier(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].Code == 59) {
				N->Child = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Child;
				Pos++;
				N->Child = AddNode(N, -1, "", -1, -1, "<block>", false);
				N = N->Child;
				if (Block(N)) {
					Pos++;
					if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 46)) {
						N->Child = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
						Pos++;
						return true;
					}
					else
					{
						Errors(3, Pos);
						return false;
					}
				}
				else
					return false;
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

bool Parser::Block(Node* Ptr)
{
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = Ptr;
	N->Parent = AddNode(N, -1, "", -1, -1, "<declarations>", false);
	N = N->Parent;
	if (Declarations(N)) {
		Pos++;
		if (Lexems[Pos].Code == 402)
		{
			N->Child = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
			N = N->Child;
			Pos++;
			N->Child = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
			N = N->Child;
			if (Statement_list(N)) {
				Pos++;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if (Lexems[Pos].Code == 403)
				{
					N->Child = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
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
			if (Err == false)
				Errors(6, Pos);
			return false;
		}
	}
	else {
		return false;
	}
}
bool Parser::Declarations(Node* Ptr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = Ptr;
	N->Parent = AddNode(N, -1, "", -1, -1, "<variable-declarations>", false);
	N = N->Parent;
	if (Lexems[Pos].Code == 404) {
		N->Parent = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Parent;
		Pos++;
		N->Child = AddNode(N, -1, "", -1, -1, "<declarations-list>", false);
		N = N->Child;
		if (Declarations_list(N)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		N->Parent = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::Statement_list(Node* Ptr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = Ptr;
	N->Parent = AddNode(N, -1, "", -1, -1, "<empty>", false);
	Pos--;
	return true;
}

bool Parser::Declarations_list(Node* Ptr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = Ptr;
	N->Parent = AddNode(N, -1, "", -1, -1, "<declaration>", false);
	if (Declaration(N->Parent)) {
		N = N->Parent;
		N->Child = AddNode(N, -1, "", -1, -1, "<declarations-list>", false);
		N = N->Child;
		Pos++;
		if (Declarations_list(N)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		N->Parent = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::Attributes_list(Node* Ptr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = Ptr;
	N->Parent = AddNode(N, -1, "", -1, -1, "<attribute>", false);
	if (Attribute(N->Parent)) {
		N = N->Parent;
		N->Child = AddNode(N, -1, "", -1, -1, "<attributes-list>", false);
		N = N->Child;
		Pos++;
		if (Attributes_list(N)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		N->Parent = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::Declaration(Node* Ptr) {
	Node* N = Ptr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	N->Parent = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
	N = N->Parent;
	if (Identifier(N)) {
		Pos++;
		N->Child = AddNode(N, -1, "", -1, -1, "<identifiers-list>", false);
		N = N->Child;
		if (Identifiers_list(N)) {
			Pos++;
			if (Lexems[Pos].Code == ':') {
				N->Child = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Child;
				Pos++;
				N->Child = AddNode(N, -1, "", -1, -1, "<attribute>", false);
				N = N->Child;
				if (Attribute(N)) {
					Pos++;
					N->Child = AddNode(N, -1, "", -1, -1, "<attributes-list>", false);
					N = N->Child;
					if (Attributes_list(N)) {
						Pos++;
						if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 59)) {
							N->Child = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
							return true;
						}
						else
						{
							Err = true;
							Errors(2, Pos);
							return false;
						}
					}
					else
					{
						Err = true;
						return false;
					}
				}
				else {
					Err = true;

					return false;
				}
			}
			else {
				Err = true;
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

bool Parser::Attribute(Node* Ptr) {
	Node* N = Ptr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].Code >= 405) && (Lexems[Pos].Code <= 410))
	{
		N->Parent = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		return true;
	}
	else if (Lexems[Pos].Code == '[') {
		N->Parent = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Parent;
		Pos++;
		N->Child = AddNode(N, -1, "", -1, -1, "<range>", false);
		N = N->Child;
		if (Range(N)) {
			Pos++;
			N->Child = AddNode(N, -1, "", -1, -1, "<ranges-list>", false);
			N = N->Child;
			if (Ranges_list(N)) {
				Pos++;
				if (Lexems[Pos].Code == ']') {
					N->Child = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
					return true;
				}
				else {
					Err = true;
					Errors(11, Pos);
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			Err = true;
			return false;
		}
	}
	else {
		return false;
	}
}

bool Parser::Ranges_list(Node* Ptr) {
	Node* N = Ptr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].Code == ',') {
		N->Parent = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Parent;
		Pos++;
		N->Child = AddNode(N, -1, "", -1, -1, "<range>", false);
		N = N->Child;
		if (Range(N)) {
			Pos++;
			N->Child = AddNode(N, -1, "", -1, -1, "<ranges-list>", false);
			N = N->Child;
			if (Ranges_list(N)) {
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
	else {
		N->Parent = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::Range(Node* Ptr) {
	Node* N = Ptr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	N->Parent = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
	N = N->Parent;
	if (Unsigned_integer(N)) {
		Pos++;
		if (Lexems[Pos].Code == 301) {
			N->Child = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
			N = N->Child;
			Pos++;
			N->Child = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
			N = N->Child;
			if (Unsigned_integer(N)) {
				return true;
			}
			else {
				Errors(12, Pos);
				return false;
			}
		}
		else {
			Errors(13, Pos);
			return false;
		}
	}
	else {
		Errors(12, Pos);
		return false;
	}
}

bool Parser::Identifiers_list(Node* Ptr) {
	Node* N = Ptr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].Code == ',') {
		N->Parent = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Parent;
		Pos++;
		N->Child = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
		N = N->Child;
		if (Identifier(N)) {
			Pos++;
			N->Child = AddNode(N, -1, "", -1, -1, "<identifiers-list>", false);
			N = N->Child;
			if (Identifiers_list(N)) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			Errors(12, Pos);
			return false;
		}
	}
	else {
		N->Parent = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::Unsigned_integer(Node* Ptr) {
	Node* N = Ptr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].Code >= IntMin) && (Lexems[Pos].Code <= IntMax))
	{
		N->Parent = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Parent;
		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::Identifier(Node* Ptr) {
	Node* N = Ptr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].Code >= IdentMin) && (Lexems[Pos].Code <= IdentMax))
	{
		N->Parent = AddNode(N, -1, "", -1, -1, "<identifier>", false);
		N = N->Parent;
		N->Parent = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
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
		cout << "Parser: Error unexpected end of the file!" << endl;
		ErrorStr += "Parser: Error unexpected end of the file!\n";
		ErrorCounter++;
		break;
	case 2:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << "): \";\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : \";\" is passed\n";
		ErrorCounter++;
		break;
	case 3:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \".\" is passed after the \"END\"" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \".\" is passed after the \"END\"\n";
		ErrorCounter++;
		break;
	case 4:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Keyword \"PROGRAM\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Keyword \"PROGRAM\" is passed\n";
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
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \"]\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \"]\" is passed\n";
		ErrorCounter++;
		break;
	case 12:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Must be unsigned-integer" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Must be unsigned-integer\n";
		ErrorCounter++;
		break;
	case 13:
		cout << "Parser: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \"..\" is passed" << endl;
		ErrorStr += "Parser: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \"..\" is passed\n";
		ErrorCounter++;
		break;
	}
}

Node* Parser::InitilizationTree()
{
	Node* Help = new Node;
	Help->Code = -1;
	Help->Column = -1;
	Help->Parent = NULL;
	Help->IsTerminal = false;
	Help->Line = -1;
	Help->Name = "";
	Help->Child = NULL;
	Help->Func = "<signal-program>";
	return Help;
}

Node* Parser::AddNode(Node* Nd, int code, string name, int line, int column, string Func, bool isterminal)
{
	Node* Help = new Node;
	Help->Code = code;
	Help->Column = column;
	Help->Parent = NULL;
	Help->IsTerminal = isterminal;
	Help->Line = line;
	Help->Name = name;
	Help->Child = NULL;
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
		if (Root->Parent != NULL) {
			for (int i = 0; i < 4; i++) {
				sps.push_back(' ');
			}
			WriteTree(Root->Parent, sps);
			for (int i = 0; i < 4; i++) {
				sps.pop_back();
			}
		}
		Root = Root->Child;
	}
}

void Parser::TreeListing(string File) {
	TreeStr = "";
	File += "generated.txt";
	ofstream f;
	f.open(File, ios::app);
	if (ErrorCounter > 0) {
		f << "Parser analysis completed with error" << endl;
		f << ErrorStr;
	}
	else {
		WriteTree(SyntaxTree, "");
		f << TreeStr << endl;
	}
}