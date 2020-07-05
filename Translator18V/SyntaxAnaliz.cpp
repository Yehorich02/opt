#include "SyntaxAnaliz.h"

SyntaxAnaliz::SyntaxAnaliz(string FileName)
{
	//FuncCounter = 0;
	ErrorStr = "";
	ErrorCounter = 0;
	Pos = 0;
	LeksAnaliz LexAn;
	LexAn.Analizator(FileName);
	LexAn.MakeListing(FileName);
	IdentMin = LexAn.MinIdent;
	ConstMin = LexAn.MinConst;
	ConstMax = LexAn.ConstMax;
	Lexems = LexAn.Lexems;
	IdentTable = LexAn.TableC;
	ConstTable = LexAn.Table;
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
		if (identifier(N)){
			Pos++;
		}
		else{
			Errors(6, Pos);
			return false;
		}
		if (Lexems[Pos].Code == 59){
			N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
			Pos++;
			N = N->Next;
			N->Next = AddNode(N, -1, "", -1, -1, "<block>", false);
			N = N->Next;
		}
		else{
			Errors(2, Pos);
			return false;
		}
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if (block(N)){
			Pos++;
		}
		else{
			return false;
		}
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 46)){
			N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
			return true;
		}
		else
		{
			Errors(3, Pos);
			return false;
		}
	}
	else{
		Errors(5, Pos);
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
	if (Lexems[Pos].Code == 410)
	{
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
		N = N->Next;
	}
	else
	{
		Errors(8, Pos);
		return false;
	}
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (statement_list(N)) {
		Pos++;
	}
	else return false;
	if (Lexems[Pos].Code == 411)
	{
		N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		return true;
	}
	else
	{
		Errors(9, Pos);
		return false;
	}
}

bool SyntaxAnaliz::identifier(Node* NodePtr) {
	if (Pos >= Lexems.size()) { 
		Errors(1, Pos);
		return false; 
	}
	if (Lexems[Pos].Code >= IdentMin)
	{
		Node* N = NodePtr;
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		return true;
	}
	else
	{
		return false;
	}
}

bool SyntaxAnaliz::multiplication_instruction(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if (Lexems[Pos].Code == '*') {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		return true;
	}
	else {
		if (Lexems[Pos].Code == '/') {
			N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
			return true;
		}
		else {
			if (Lexems[Pos].Code == 470) {
				N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				return true;
			}
			else {
				return false;
			}
		}
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
	if (Lexems[Pos].Code == 440) {
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
		N = N->Next;
		if (statement_list(N)) {
			Pos++;
			if (Lexems[Pos].Code == 441) {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Next;
				Pos++;
				if (Lexems[Pos].Code == ';') {
					N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
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
			return false;
		}
	}
	else {
		if (Lexems[Pos].Code == 450) {
			N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
			N = N->Down;
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<expression>", false);
			N = N->Next;
			if (expression(N)) {
				Pos++;
				if (Lexems[Pos].Code == 455) {
					N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
					N = N->Next;
					Pos++;
					N->Next = AddNode(N, -1, "", -1, -1, "<alternatives-list>", false);
					N = N->Next;
					if (altenative_list(N)) {
						Pos++;
						if (Lexems[Pos].Code == 451) {
							N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
							N = N->Next;
							Pos++;
							if (Lexems[Pos].Code == ';') {
								N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
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
			if (ifthen(N)) {
				return true;
			}
			return false;
		}
	}
}

bool SyntaxAnaliz::expression(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<multiplier>", false);
	N = N->Down;
	if (multiplier(N)) {
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<multipliers-list>", false);
		N = N->Next;
		if (multipliers_list(N)) {
			return true;
		}
		else return false;
	}
	else {
		return false;
	}
}

bool SyntaxAnaliz::ifthen(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	if (Lexems[Pos].Code == 498)
	{
		N->Down = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<cmp>", false);
		N = N->Next;
		if (cmp(N)) {
			Pos++;
			if (Lexems[Pos].Code == 499) {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Next;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<statement-list>", false);
				N = N->Next;
				if (statement_list(N)) {
					if (Lexems[Pos].Code == 497) {
						N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
						return true;
					}
					else return false;
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
			return false;
		}
	}
	else {
		return false;
	}
}

bool SyntaxAnaliz::cmp(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
	N = N->Down;
	if (expression(N)) {
		Pos++;
		if (Lexems[Pos].Code == '=') {
			N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
			N = N->Next;
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
			N = N->Next;
			if (integer(N)) {
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
		return false;
	}
}

bool SyntaxAnaliz::alternative(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<expression>", false);
	N = N->Down;
	if (expression(N)) {
		Pos++;
		if (Lexems[Pos].Code == ':') {
			N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
			N = N->Next;
			Pos++;
			if (Lexems[Pos].Code == '/') {
				N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
				N = N->Next;
				Pos++;
				N->Next = AddNode(N, -1, "", -1, -1, "<statements-list>", false);
				N = N->Next;
				if (statement_list(N)) {
					Pos++;
					if (Lexems[Pos].Code == '\\') {
						N->Next = AddNode(N, Lexems[Pos].Code, Lexems[Pos].Name, Lexems[Pos].Line, Lexems[Pos].Column, "", true);
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

bool SyntaxAnaliz::altenative_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<alternative>", false);
	if (alternative(N->Down)) {
		Pos++;
		N = N->Down;
		N->Next = AddNode(N, -1, "", -1, -1, "<alternatives-list>", false);
		N = N->Next;
		if (altenative_list(N)) {
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

bool SyntaxAnaliz::multiplier(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
	if (identifier(N)) {
		return true;
	}
	else {
		if (integer(N)) {
			N->Down = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
			return true;
		}
		else {
			// Errors(10, Pos);
			return false;
		}
	}
}

bool SyntaxAnaliz::multipliers_list(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<multiplication-instruction>", false);
	if (multiplication_instruction(N->Down)) {
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<multipliers>", false);
		N = N->Next;
		if (multiplier(N)) {
			N->Next = AddNode(N, -1, "", -1, -1, "<multipliers-list>", false);
			N = N->Next;
			Pos++;
			if (multipliers_list(N)) {
				return true;
			}
			else return false;
		}
		else return false;
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool SyntaxAnaliz::integer(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].Code >= ConstMin) && (Lexems[Pos].Code <= ConstMax))
	{
		Node* N = NodePtr;
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
		ErrorStr += "Syntactic error: Unexpected end of the file!";
		ErrorCounter++;
		break;
	case 2:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": \";\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		//ErrorStr += ": \";\" is passed";
		ErrorCounter++;
		break;
	case 3:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": \".\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		//ErrorStr += ": \".\" is passed";
		ErrorCounter++;
		break;
	//case 4:
	//	cout << "Syntactic error: \".\" is passed after the \"END\"" << endl;
	//	break;
	case 5:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"PROGRAM\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		//ErrorStr += ": Keyword \"PROGRAM\" is passed";
		ErrorCounter++;
		break;
	case 6:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be procedure identifier" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		//ErrorStr += ": Must be procedure identifier";
		ErrorCounter++;
		break;
	case 7:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": \")\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		//ErrorStr += ": \")\" is passed";
		ErrorCounter++;
		break;
	case 8:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"BEGIN\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		//ErrorStr += ": Keyword \"BEGIN\" is passed";
		ErrorCounter++;
		break;
	case 9:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"END\" is passed" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		//ErrorStr += ": Keyword \"END\" is passed";
		ErrorCounter++;
		break;
	case 10:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be constant or identifier" << endl;
		ErrorStr += "Syntactic error on Line ";
		ErrorStr += to_string(Lexems[pos].Line);
		ErrorStr += " Column ";
		ErrorStr += to_string(Lexems[pos].Column);
		//ErrorStr += ": Must be constant or identifier";
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
	Help->Func = "<program>";
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
			TreeStr += sps + "'" + Root->Name + "'" + "\n";
		}
		else
		{
			TreeStr += sps + Root->Func + "\n";
		}
		if (Root->Down != NULL) {
			for (int i = 0; i < 6; i++) {
				sps.push_back(' ');
			}
			WriteTree(Root->Down, sps);
			for (int i = 0; i < 6; i++) {
				sps.pop_back();
			}
		}
		Root = Root->Next;
	}
}

void SyntaxAnaliz::TreeListing(string FileName) {
	TreeStr = "";
	FileName = FileName + ".lst";
	fstream f;
	f.open(FileName, std::ios::app);
	if (ErrorCounter > 0) {
		f << ErrorStr << endl;
	}
	else {
		WriteTree(Tree, "");
		f << TreeStr << endl;
	}
}