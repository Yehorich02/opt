#include "Generator.h"

Generator::Generator(string File) {
	Parser parser(File);
	Lexems = parser.Lexems;
	Error = 0;
	ErrStr = "";
	id_idn = parser.IdentMin;
	Node* Tree = parser.SyntaxTree;
	if(parser.ErrorCounter==0)
		generate(Tree);
	Listing(File);
}

void Generator::generate(Node* Tree) {
	while (Tree != NULL)
	{
		if (!(Tree->IsTerminal))
		{
			if (Tree->Func=="<variable-declarations>")
			{
				variable_declarations(Tree->Parent);
			}
			if (Tree->Func == "<statements-list>")
			{
				statements_list(Tree->Parent);
			}
		}
		if (Tree->Parent != NULL) {
			generate(Tree->Parent);
		}
		Tree = Tree->Child;
	}
}

void Generator::variable_declarations(Node* Tree)
{
	AsmStr.append("data SEGMENT\n");
	Tree = Tree->Child;
	if (Tree->Func == "<declarations-list>") {
		declarations_list(Tree->Parent);
	}
	AsmStr.append("data ENDS\n");
	return;
}

void Generator::statements_list(Node* Tree)
{
	AsmStr.append("code SEGMENT\n");
	AsmStr.append("\tASSUME cs:code, ds:data\n");
	AsmStr.append(Lexems[1].Name+":\n");
	if (Tree->Func == "<empty>") {
		AsmStr.append("\tnop\n");
	}
	AsmStr.append("mov ah, 4ch\n");
	AsmStr.append("int 21h\n");
	AsmStr.append("code ENDS\n");
	AsmStr.append("end "+Lexems[1].Name+"\n");
	return;
}

void Generator::declarations_list(Node* Tree) {
	if (Tree->Func == "<empty>") {
		return;
	}
	if (Tree->Func == "<declaration>") {
		declaration(Tree->Parent);
	}
	Tree = Tree->Child;
	if (Tree->Func == "<declarations-list>") {
		declarations_list(Tree->Parent);
	}
	return;
}

void Generator::declaration(Node* Tree)
{
	idn = 0;
	atr = 0;
	if (Tree->Func == "<variable-identifier>"){
		identifier(Tree->Parent);
	}
	Tree = Tree->Child;
	if (Tree->Func=="<identifiers-list>") {
		identifiers_list(Tree->Parent);
	}
	Tree = Tree->Child;
	Tree = Tree->Child;
	if (Tree->Func == "<attribute>") {
		attribute(Tree->Parent);
	}
	Tree = Tree->Child;
	if (Tree->Func == "<attributes-list>") {
		attributes_list(Tree->Parent);
	}
	if (atr != idn && atr!=1) {
		Error++;
		ErrStr += "Error: number of identifiers is not equal to number of attribute\n";
	}
	return;
}

void Generator::identifier(Node* Tree)
{
	Tree = Tree->Parent;
	if (Tree->Code == Lexems[1].Code) {
		Error++;
		ErrStr += "Error: line "+to_string(Tree->Line)+" column "+to_string(Tree->Column)+" lexem \""+Tree->Name+"\": invalid identifier\n";
	}
	else {
		AsmStr.append("\t" + Tree->Name + " \tdw\t?\n");
		id_idn = Tree->Code;
		idn++;
	}
	return;
}

void Generator::attribute(Node* Tree)
{
	atr++;
	return;
}

void Generator::attributes_list(Node* Tree)
{
	if (Tree->Func == "<empty>")
	{
		return;
	}
	else {
		if (Tree->Func == "<attribute>")
		{
			attribute(Tree->Parent);
		}
		Tree = Tree->Child;
		if (Tree->Func == "<attributes-list>")
		{
			attributes_list(Tree->Parent);
			return;
		}
	}
}

void Generator::identifiers_list(Node* Tree)
{
	if (Tree->Func=="<empty>")
	{
		return;
	}
	else {
		Tree = Tree->Child;
		if (Tree->Func=="<variable-identifier>")
		{
			identifier(Tree->Parent);
		}
		Tree = Tree->Child;
		if (Tree->Func == "<identifiers-list>") {
			identifiers_list(Tree->Parent);
			return;
		}
	}
}

void Generator::Listing(string File) {
	File += "generated.txt";
	ofstream f;
	if (Error == 0) {
		f.open(File, ios::app);
		cout << "Code Generator completed sucessfully\n" << endl;
		f << AsmStr << endl;
	}
	else {
		f.open(File);
		f << "Code Generator completed with error\n" ;
		f << ErrStr;
		cout << "Code Generator completed with error" << endl;
		cout << ErrStr;
	}
}