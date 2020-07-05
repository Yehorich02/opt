#include "Generator.h"

Generator::Generator(string File) {
	Parser parser(File);
	Lexems = parser.Lexems;
	Error = 0;
	ErrStr = "";
	Node* Tree = parser.Tree;
	ProgramName = Tree->Down->Down->Next->Down->Down->Name;
	IdnMin = Tree->Down->Down->Next->Down->Down->Code;
	LabelMin = 500;
	if(parser.ErrorCounter==0)
		generate(Tree);
	Listing(File);
}

void Generator::generate(Node* Tree) {

	while (Tree != NULL)
	{
		if (!(Tree->IsTerminal))
		{
			if (Tree->Func=="<parameters-list>")
			{
				parameters_list(Tree->Down);
			}
			if (Tree->Func == "<declarations>")
			{
				declarations(Tree->Down);
			}
			if (Tree->Func == "<statements-list>")
			{
				AsmStr += "code SEGMENT\n ASSUME cs:code, ds:data\n";
				statements_list(Tree->Down);
				AsmStr += "?L1\tnop\n";
				return;
			}
		}
		if (Tree->Down != NULL) {
			generate(Tree->Down);
		}
		Tree = Tree->Next;
	}
}

void Generator::parameters_list(Node* Tree)
{
	if (Tree->Func=="<empty>") {
		return;
	}
	else if (Tree->Next->Func=="<variable-identifier>") {
		AsmStr += "data SEGMENT\n";
		identifier(Tree->Next->Down->Down);
		if (Tree->Next->Next->Func == "<identifiers-list>")
			identifiers_list(Tree->Next->Next->Down);
		AsmStr += "data ENDS\n";
	}
}

void Generator::declarations(Node* Tree)
{
	if (Tree->Down->Func=="<empty>") {
		return;
	}
	else if (Tree->Down->Next->Func=="<unsigned-integer>") {
		if (Tree->Down->Next->Down->Code > LabelMin) {
			LabelMin = Tree->Down->Next->Down->Code;
			if (Tree->Down->Next->Next->Func == "<labels-list>") {
				labels_list(Tree->Down->Next->Next->Down);
			}
		}
	}
}

void Generator::statements_list(Node* Tree)
{
	if (Tree->Func == "<statement>") {
		statement(Tree->Down);
		statements_list(Tree->Next->Down);
	}
	else if (Tree->Func=="<empty>") {
		AsmStr += "\tnop\n";
	}
	
}

void Generator::statement(Node* Tree)
{
	if (Tree->Func == "<unsigned-integer>") {
		if (Tree->Down->Code <= LabelMin ) {
			AsmStr += Tree->Down->Name + ":\n";
			statement(Tree->Next->Next->Down);
		}
		else {
			Error++;
			ErrStr += "Code generator error on line " + to_string(Tree->Line) + "column " + to_string(Tree->Column) + " : use undeclaretad label\n";

		}
	}
	else if (Tree->Code == 405) {
		if (Tree->Next->Down->Code <= LabelMin) {
			AsmStr += "\tjmp " + Tree->Next->Down->Name + "\n";
		}
	}
	else if (Tree->Code == 406) {
		AsmStr += "\tjmp ?L1\n";
	}
	else if (Tree->Code == 59) {
		AsmStr += "\tnop\n";
	}
	else if (Tree->Code==301){
		if (Tree->Next->Down->Down->Name != ProgramName) {
			fstream file;
			string pr = Tree->Next->Down->Down->Name + ".asm";
			file.open(pr);
			if (!file.is_open()) {
				cout << "failed to open " << Tree->Next->Down->Down->Name << ".asm\n";
				return;
			}
			while (getline(file, AsmFile)) {
				AsmStr += AsmFile + "\n";
			}
			file.close();
		}
	}
}

void Generator::identifier(Node* Tree)
{
	if (Tree->Name!=ProgramName && Tree->Code>IdnMin) {
		AsmStr += "\t"+Tree->Name+" \tdw\t?\n";
		IdnMin = Tree->Code;
	}
	else {
		Error++;
		ErrStr += "Code generator error on line "+to_string(Tree->Line)+"column "+to_string(Tree->Column)+" : invalid variable identifier!\n";
	}
}

void Generator::identifiers_list(Node* Tree)
{
	if (Tree->Func=="<empty>") {
		return;
	}
	else if(Tree->Next->Func=="<variable-identifier>"){
		identifier(Tree->Next->Down->Down);
		if (Tree->Next->Next->Func == "<identifier-list>")
			identifiers_list(Tree->Next->Next->Down);
	}
}

void Generator::labels_list(Node* Tree)
{
	if (Tree->Func=="<empty>") {
		return;
	}
	else if(Tree-> Next->Func=="<unsigned-integer>"){
		LabelMin = Tree->Next->Down->Code;
		if (Tree->Next->Next->Func == "<labels-list>") {
			labels_list(Tree->Next->Next->Down);
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