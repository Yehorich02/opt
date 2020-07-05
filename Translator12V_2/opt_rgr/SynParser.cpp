#include "SynParser.h"

SynParser::SynParser(Context& context, string File)
{
	ErrorStr = "";
	ErrorCounter = 0;
	Pos = 0;
	IdentMin = 1000;
	IdentMax = 2000;
	DigitMin = 500;
	DigitMax = 999;
	Lexems = context.result;
	start();
	if (ErrorCounter == 0) {
		cout << "syntax analysis completed successfully" << endl;
	}
	TreeListing(File); //вивод дерева розбору
}

void SynParser::start()
{
	Tree = InitTree();
	program(Tree);
}

bool SynParser::program(Node* NodePtr)
{
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<program>", false);
	N = N->Down;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].tokenNumber == 401) {
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
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
				if (Lexems[Pos].tokenNumber == 59) {
					N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
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
						if ((Pos < Lexems.size()) && (Lexems[Pos].tokenNumber == 59)) {
							N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
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

bool SynParser::block(Node* NodePtr)
{
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<declarations>", false);
	N = N->Down;
	if (label_declarations(N)) {
		Pos++;
		if (Lexems[Pos].tokenNumber == 402)
		{
			N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
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
				if (Lexems[Pos].tokenNumber == 403)
				{
					N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
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
			Errors(6, Pos);
			return false;
		}
	}
	else {
		return false;
	}
}

bool SynParser::statement_list(Node* NodePtr) {
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

bool SynParser::statement(Node* NodePtr) {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	Node* N = NodePtr;
	N->Down = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
	if (integer(N->Down)) {
		N = N->Down;
		Pos++;
		if (Lexems[Pos].tokenNumber == ':') {
			N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
			N = N->Next;
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			N->Next = AddNode(N, -1, "", -1, -1, "<statement>", false);
			if (statement(N->Next)) {
				N = N->Next;
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
	else if (Lexems[Pos].tokenNumber == 405) {
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
		N = N->Next;
		if (integer(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (Lexems[Pos].tokenNumber == ';') {
				N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
				return true;
			}
			else {
				Errors(2, Pos);
				return false;
			}
		}
		else {
			Errors(8, Pos);
			return false;
		}
	}
	else  if (Lexems[Pos].tokenNumber == 406) {
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		if (Lexems[Pos].tokenNumber == ';') {
			N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
			return true;
		}
		else {
			Errors(2, Pos);
			return false;
		}
	}
	else if (Lexems[Pos].tokenNumber == ';') {
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		N = N->Down;
		return true;
	}
	else if (Lexems[Pos].tokenNumber == 301) {
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<assembly-insert-file-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			if (Lexems[Pos].tokenNumber == 302) {
				N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
				return true;
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
	else
	{
		return false;
	}
}

bool SynParser::parameters_list(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].tokenNumber == '(') {
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
		N = N->Next;
		if (identifier(N)) {
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<identifiers-list>", false);
			N = N->Next;
			if (identifiers_list(N)) {
				Pos++;
				if (Lexems[Pos].tokenNumber == ')') {
					N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
					return true;
				}
				else {
					Errors(14, Pos);
					return false;
				}
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
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool SynParser::identifiers_list(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].tokenNumber == ',') {
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		N = N->Down;
		Pos++;
		N->Next = AddNode(N, -1, "", -1, -1, "<variable-identifier>", false);
		N = N->Next;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
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

bool SynParser::label_declarations(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	N->Down = AddNode(N, -1, "", -1, -1, "<label-declarations>", false);
	N = N->Down;
	if (Lexems[Pos].tokenNumber == 404) {
		N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		N = N->Next;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
		N = N->Next;
		if (integer(N)) {
			Pos++;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			N->Next = AddNode(N, -1, "", -1, -1, "<labels-list>", false);
			N = N->Next;
			if (labels_list(N)) {
				Pos++;
				if (Pos >= Lexems.size()) {
					Errors(1, Pos);
					return false;
				}
				if (Lexems[Pos].tokenNumber == ';') {
					N->Next = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
					N = N->Next;
					return true;
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
			Errors(8, Pos);
			return false;
		}
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool SynParser::labels_list(Node* NodePtr) {
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if (Lexems[Pos].tokenNumber == ',') {
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		N = N->Down;
		Pos++;
		if (Pos >= Lexems.size()) {
			Errors(1, Pos);
			return false;
		}
		N->Next = AddNode(N, -1, "", -1, -1, "<unsigned-integer>", false);
		N = N->Next;
		if (integer(N)) {
			Pos++;
			N->Next = AddNode(N, -1, "", -1, -1, "<labels-list>", false);
			N = N->Next;
			if (Pos >= Lexems.size()) {
				Errors(1, Pos);
				return false;
			}
			if (labels_list(N)) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			Errors(8, Pos);
			return false;
		}
	}
	else {
		N->Down = AddNode(N, -1, "", -1, -1, "<empty>", false);
		Pos--;
		return true;
	}
}

bool SynParser::identifier(Node* NodePtr) { //перевірка ідентифікатора
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].tokenNumber >= IdentMin) && (Lexems[Pos].tokenNumber <= IdentMax))
	{
		N->Down = AddNode(N, -1, "", -1, -1, "<identifier>", false);
		N = N->Down;
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		return true;
	}
	else
	{
		return false;
	}
}

bool SynParser::integer(Node* NodePtr) { // перевірка на число
	Node* N = NodePtr;
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	if ((Lexems[Pos].tokenNumber >= DigitMin) && (Lexems[Pos].tokenNumber <= DigitMax))
	{
		N->Down = AddNode(N, Lexems[Pos].tokenNumber, Lexems[Pos].tokenName, Lexems[Pos].line, Lexems[Pos].column, "", true);
		N = N->Down;
		return true;
	}
	else
	{
		return false;
	}
}

void SynParser::Errors(int p, int pos) { //обробка помилок
	switch (p) {
	case 1:
		cout << "SynParser Error Unexpected end of the file!" << endl << endl;
		ErrorStr += "SynParser Error Unexpected end of the file!\n";
		ErrorCounter++;
		break;
	case 2:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << "): \";\" is passed" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") : \";\" is passed\n";
		ErrorCounter++;
		break;
	case 3:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") \";\" is passed after the \"END\"" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") \";\" is passed after the \"END\"\n";
		ErrorCounter++;
		break;
	case 4:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") : Keyword \"PROCEDURE\" is passed" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") : Keyword \"PROCEDURE\" is passed\n";
		ErrorCounter++;
		break;
	case 5:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") : Must be procedure identifier" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") : Must be procedure identifier\n";
		ErrorCounter++;
		break;
	case 6:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") : Keyword \"BEGIN\" is passed" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") : Keyword \"BEGIN\" is passed\n";
		ErrorCounter++;
		break;
	case 7:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") : Keyword \"END\" is passed" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") : Keyword \"END\" is passed\n";
		ErrorCounter++;
		break;
	case 8:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") : Must be integer" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") : Must be integer\n";
		ErrorCounter++;
		break;
	case 9:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") \",\" is passed" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") \",\" is passed\n";
		ErrorCounter++;
		break;
	case 10:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") : Must be assembly insert file identifier" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") : Must be assembly insert file identifier\n";
		ErrorCounter++;
		break;
	case 11:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") \":\" is passed" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") \":\" is passed\n";
		ErrorCounter++;
		break;

	case 12:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") : Must be identifier" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") : Must be identifier\n";
		ErrorCounter++;
		break;
	case 13:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") \"$)\" is passed" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") \"$)\" is passed\n";
		ErrorCounter++;
		break;
	case 14:
		cout << "SynParser: Error (line " << Lexems[pos].line << ", column " << Lexems[pos].column << ") \")\" is passed" << endl << endl;
		ErrorStr += "SynParser: Error (line " + to_string(Lexems[pos].line) + ", column " + to_string(Lexems[pos].column) + ") \")\" is passed\n";
		ErrorCounter++;
		break;
	}
}

Node* SynParser::InitTree()
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

Node* SynParser::AddNode(Node* Nd, int code, string name, int line, int column, string Func, bool isterminal)
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

Node* SynParser::RemoveNode(Node* N)
{
	if (N != NULL) free(N);
	return NULL;
}

void SynParser::WriteTree(Node* Root, string sps)
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

void SynParser::TreeListing(string File) {
	TreeStr = "";
	File += "generated.txt";
	ofstream f;
	f.open(File, std::ios::app);
	if (ErrorCounter > 0) {
		f << ErrorStr << endl;
	}
	else {
		WriteTree(Tree, "");
		f << TreeStr << endl;
	}
}