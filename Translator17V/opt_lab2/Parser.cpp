#include "Parser.h"

Parser::Parser(string File)
{
	start(File);
	if (Error == 0) {
		cout << "syntax analysis completed successfully" << endl;
	}
	TreeListing(File);
}

void Parser::start(string File)
{
	ErrorStr = "";
	Error = 0;
	Pos = 0;
	LeksAnaliz LexAn;
	LexAn.Analizator(File);
	IdentMin = LexAn.MinIdent;
	IdentMax = LexAn.LexErrorMin - 1;
	DigitMin = LexAn.MinDigit;
	DigitMax = LexAn.MinIdent - 1;
	Lexems = LexAn.Lexems;
	Tr = InitTree();
	program(Tr);
}

bool Parser::LexemsOut() {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return true;
	}
	else {
		return false;
	}
}
void Parser::program(Tree* T) {
	if (LexemsOut())
		return;
	T->Down = AddNotTerminal(T, "<program>", false);
	T = T->Down;
	if (Lexems[Pos].Code == 401) {
		T->Down = AddTerminal(T, Pos, true);
		T = T->Down;
		Pos++;
		T->Next = AddNotTerminal(T, "<procedure-identifier>", false);
		T = T->Next;
		if (procedure_identifier(T)) {
			Pos++;
			if (LexemsOut()) {
				return;
			}
			if (Lexems[Pos].Code == 59) {
				T->Next = AddTerminal(T, Pos, true);
				Pos++;
				T = T->Next;
				T->Next = AddNotTerminal(T, "<block>", false);
				T = T->Next;
				if (LexemsOut()) {
					return;
				}
				if (block(T)) {
					Pos++;
					if (LexemsOut()) {
						return;
					}
					if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 46)) {
						T->Next = AddTerminal(T, Pos, true);
						Pos++;
						return;
					}
					else
					{
						Errors(3, Pos);
						return;
					}
				}
				else {
					return;
				}
			}
			else {
				Errors(2, Pos);
				return;
			}
		}
		else {
			Errors(5, Pos);
			return;
		}
	}
	else {
		Errors(4, Pos);
		return;
	}
}

bool Parser::block(Tree* T)
{
	if (LexemsOut()) {
		return false;
	}
	if (Lexems[Pos].Code == 402)
	{
		T->Down = AddTerminal(T, Pos, true);
		T = T->Down;
		Pos++;
		T->Next = AddNotTerminal(T, "<statements-list>", false);
		T = T->Next;
		if (statements_list(T)) {
			Pos++;
			if (LexemsOut()) {
				return false;
			}
			if (Lexems[Pos].Code == 403)
			{
				T->Next = AddTerminal(T, Pos, true);
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

bool Parser::statements_list(Tree* T) {
	if (LexemsOut()) {
		return false;
	}
	T->Down = AddNotTerminal(T, "<statement>", false);
	int ret = statement(T->Down);
	if (ret == 1) {
		T = T->Down;
		T->Next = AddNotTerminal(T, "<statements-list>", false);
		T = T->Next;
		Pos++;
		if (statements_list(T)) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (ret == 2) {
		T->Down = AddNotTerminal(T, "<empty>", false);
		Pos--;
		return true;
	}
	else {
		return false;
	}
}

int Parser::statement(Tree* T) {
	if (LexemsOut()) {
		return 0;
	}
	if (Lexems[Pos].Code == 404) {
		T->Down = AddTerminal(T, Pos, true);
		T = T->Down;
		Pos++;
		T->Next = AddNotTerminal(T, "<statements-list>", false);
		T = T->Next;
		if (statements_list(T)) {
			Pos++;
			if (LexemsOut()) {
				return 0;
			}
			if (Lexems[Pos].Code == 405) {
				T->Next = AddTerminal(T, Pos, true);
				T = T->Next;
				Pos++;
				if (LexemsOut()) {
					return 0;
				}
				if (Lexems[Pos].Code == 59) {
					T->Next = AddTerminal(T, Pos, true);
					T = T->Next;
					return 1;
				}
				else {
					Errors(2, Pos);
					return 0;
				}
			}
			else {
				Errors(12, Pos);
				return 0;
			}
		}
		else {
			return 0;
		}
	}
	else if (Lexems[Pos].Code == 406) {
		T->Down = AddTerminal(T, Pos, true);
		T = T->Down;
		Pos++;
		T->Next = AddNotTerminal(T, "<variable-identifier>", false);
		T = T->Next;
		if (variable_identifier(T)) {
			Pos++;
			if (LexemsOut()) {
				return 0;
			}
			if (Lexems[Pos].Code == 301) {
				T->Next = AddTerminal(T, Pos, true);
				T = T->Next;
				Pos++;
				T->Next = AddNotTerminal(T, "<loop-declaration>", false);
				T = T->Next;
				if (loop_declaration(T)) {
					Pos++;
					if (Lexems[Pos].Code == 407) {
						T->Next = AddTerminal(T, Pos, true);
						T = T->Next;
						Pos++;
						if (Lexems[Pos].Code == 59) {
							T->Next = AddTerminal(T, Pos, true);
							T = T->Next;
							return 1;
						}
						else {
							Errors(2, Pos);
							return 0;
						}
					}
					else {
						Errors(14, Pos);
						return 0;
					}
				}
				else {
					return 0;
				}
			}
			else {
				Errors(9, Pos);
				return 0;
			}
		}
		else {
			Errors(17, Pos);
			return 0;
		}
	}
	else {
		return 2;
	}
}

bool Parser::loop_declaration(Tree* T) {
	if (LexemsOut()) {
		return false;
	}
	T->Down = AddNotTerminal(T, "<expression>", false);
	T = T->Down;
	if (expression(T)) {
		Pos++;
		if (LexemsOut()) {
			return false;
		}
		if (Lexems[Pos].Code == 409) {
			T->Next = AddTerminal(T, Pos, true);
			T = T->Next;
			Pos++;
			T->Next = AddNotTerminal(T, "<expression>", false);
			T = T->Next;
			if (expression(T)) {
				Pos++;
				if (LexemsOut()) {
					return false;
				}
				if (Lexems[Pos].Code == 408) {
					T->Next = AddTerminal(T, Pos, true);
					T = T->Next;
					Pos++;
					T->Next = AddNotTerminal(T, "<statements-list>", false);
					T = T->Next;
					if (statements_list(T)) {
						return true;
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
			Errors(16, Pos);
			return false;
		}
	}
	else {
		return false;
	}
}

bool Parser::expression(Tree* T)
{
	if (LexemsOut()) {
		return false;
	}
	if (Lexems[Pos].Code == '-') {
		T->Down = AddTerminal(T, Pos, true);
		T = T->Down;
		Pos++;
		T->Next = AddNotTerminal(T, "<summand>", false);
		T = T->Next;
		if (summand(T)) {
			Pos++;
			T->Next = AddNotTerminal(T, "<summands-list>", false);
			T = T->Next;
			if (summands_list(T)) {
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
		T->Down = AddNotTerminal(T, "<summand>", false);
		if (summand(T->Down)) {
			T = T->Down;
			Pos++;
			T->Next = AddNotTerminal(T, "<summands-list>", false);
			T = T->Next;
			if (summands_list(T)) {
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
}

bool Parser::summands_list(Tree* T)
{
	if (LexemsOut()) {
		return false;
	}
	T->Down = AddNotTerminal(T, "<add-instruction>", false);
	if (add_intruction(T->Down)) {
		T = T->Down;
		Pos++;
		T->Next = AddNotTerminal(T, "<summand>", false);
		T = T->Next;
		if (summand(T)) {
			Pos++;
			T->Next = AddNotTerminal(T, "<summands-list>", false);
			T = T->Next;
			if (summands_list(T)) {
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
		T->Down = AddNotTerminal(T, "<empty>", false);
		Pos--;
		return true;
	}
}

bool Parser::add_intruction(Tree* T)
{
	if (LexemsOut()) {
		return false;
	}
	if (Lexems[Pos].Code == '+') {
		T->Down = AddTerminal(T, Pos, true);
		return true;
	}
	else if (Lexems[Pos].Code == '-') {
		T->Down = AddTerminal(T, Pos, true);
		return true;
	}
	else {

		return false;
	}
}

bool Parser::summand(Tree* T)
{
	T->Down = AddNotTerminal(T, "<variable-identifier>", false);
	if (variable_identifier(T->Down)) {
		T = T->Down;
		return true;
	}
	else
		if (integer(T)) {
			return true;
		}
		else {
			Errors(8, Pos);
			return false;
		}
}

bool Parser::procedure_identifier(Tree* T) {
	if (LexemsOut()) {
		return false;
	}

	if (identifier(T))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::variable_identifier(Tree* T) {
	if (LexemsOut()) {
		return false;
	}
	if (identifier(T))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::identifier(Tree* T) {
	if (LexemsOut()) {
		return false;
	}
	if ((Lexems[Pos].Code >= IdentMin) && (Lexems[Pos].Code <= IdentMax))
	{
		T->Down = AddNotTerminal(T, "<identifier>", false);
		T = T->Down;
		T->Down = AddTerminal(T, Pos, true);
		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::integer(Tree* T) {
	if (LexemsOut()) {
		return false;
	}
	if ((Lexems[Pos].Code >= DigitMin) && (Lexems[Pos].Code <= DigitMax))
	{
		T->Down = AddNotTerminal(T, "<unsigned-integer>", false);
		T = T->Down;
		T->Down = AddTerminal(T, Pos, true);
		T = T->Down;
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
		cout << "Syntactic error: Unexpected end of the file!" << endl;
		ErrorStr += "Syntactic error: Unexpected end of the file!\n";
		Error++;
		break;
	case 2:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": \";\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": \";\" is passed\n";
		Error++;
		break;
	case 3:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << " \".\" is passed after the \"END\"" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + " \".\" is passed after the \"END\"\n";
		Error++;
		break;
	case 4:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"PROGRAM\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \"PROGRAM\" is passed\n";
		Error++;
		break;
	case 5:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be procedure identifier" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Must be procedure identifier\n";
		Error++;
		break;
	case 6:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"BEGIN\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \"BEGIN\" is passed\n";
		Error++;
		break;
	case 7:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"END\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \"END\" is passed\n";
		Error++;
		break;
	case 8:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be a summand" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Must be a summand\n";
		Error++;
		break;
	case 9:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \":=\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \":=\" is passed\n";
		Error++;
		break;
	case 11:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"LOOP\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \"LOOP\" is passed\n";
		Error++;
		break;
	case 12:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"ENDLOOP\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \"ENDLOOP\" is passed\n";
		Error++;
		break;
	case 13:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"FOR\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \"FOR\" is passed\n";
		Error++;
		break;
	case 14:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"ENDFOR\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \"ENDFOR\" is passed\n";
		Error++;
		break;
	case 15:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"DO\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \"DO\" is passed\n";
		Error++;
		break;
	case 16:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Keyword \"TO\" is passed" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Keyword \"TO\" is passed\n";
		Error++;
		break;
	case 17:
		cout << "Syntactic error on Line " << Lexems[pos].Line << " Column " << Lexems[pos].Column << ": Must be a variable-identifier" << endl;
		ErrorStr += "Syntactic error on Line " + to_string(Lexems[pos].Line) + " Column " + to_string(Lexems[pos].Column) + ": Must be a variable-identifier\n";
		Error++;
		break;
	}
}

Tree* Parser::InitTree()
{
	Tree* Help = new Tree;
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

Tree* Parser::AddNotTerminal(Tree* Nd, string Func, bool isterminal)
{
	Tree* Help = new Tree;
	Help->Down = NULL;
	Help->IsTerminal = isterminal;
	Help->Next = NULL;
	Help->Func = Func;
	return Help;
}

Tree* Parser::AddTerminal(Tree* Nd, int pos, bool isterminal)
{
	Tree* Help = new Tree;
	Help->Code = Lexems[pos].Code;
	Help->Column = Lexems[pos].Column;
	Help->Down = NULL;
	Help->IsTerminal = isterminal;
	Help->Line = Lexems[pos].Line;
	Help->Name = Lexems[pos].Name;
	Help->Next = NULL;
	return Help;
}

Tree* Parser::RemoveNode(Tree* T)
{
	if (T != NULL) free(T);
	return NULL;
}

void Parser::WriteTree(Tree* Root, string sps)
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
	f.open(File, ios::app);
	if (Error > 0) {
		f << ErrorStr << endl;
	}
	else {
		WriteTree(Tr, "");
		f << TreeStr << endl;
	}
}