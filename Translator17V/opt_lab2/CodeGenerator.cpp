#include "CodeGenerator.h"

CodeGenerator::CodeGenerator(string File)
{
	init(File);
	listing(File);
}

void CodeGenerator::init(string File)
{
	Parser parser(File);
	T = parser.Tr;
	ProgramName=T->Down->Down->Next->Down->Down->Name;
	StrAsm = "";
	Error = 0;
	Label = 0;
	LabelCounter = 0;
	StrError = "";
	T = T->Down->Down->Next->Next->Next->Down->Next;
	if(parser.Error==0)
		start(T);
}

void CodeGenerator::start(Tree* T)
{
	StrAsm.append("data SEGMENT\ndata ENDS\n");
	if (T->Func == "<statements-list>") {	
		StrAsm.append("code SEGMENT\n\tASSUME cs:code, ds:data\t\n"+ProgramName+":\n");
		statements_list(T->Down);
		StrAsm.append("mov ah, 4ch\nint 21h\ncode ends\nend " + ProgramName);
	}
	else {
		Error++;
		StrError += "Code Generator Eror: line " + to_string(T->Line) + " column " + to_string(T->Column) + "\n";
		return;
	}

}

void CodeGenerator::statements_list(Tree* T)
{
	if (T->Func == "<empty>") {
		StrAsm.append("\tnop\n");
	}
	if (T->Func=="<statement>") {
		statement(T->Down);
		if (T->Next->Func == "<statements-list>") {
			statements_list(T->Next->Down);

		}
	}
}

void CodeGenerator::statement(Tree* T)
{
	if (T->Code==404) {
		Label = LabelCounter;
		StrAsm.append("\tmov cx, 0\n?L"+to_string(Label)+": nop\n");
		Label++;
		if (T->Next->Func == "<statements-list>") {
			statements_list(T->Next->Down);
			Label--;
			if(T->Next->Next ->Code==405)
			StrAsm.append("\tloop ?L"+to_string(Label)+"\n");
			LabelCounter++;
			Label = LabelCounter;
		}
	}
	if (T->Code == 406) {
		if (T->Next->Down->Down->Name == ProgramName)
		{
			Error++;
			StrError += "Code Generator Eror: line " + to_string(T->Next->Down->Down->Line) + " column " + to_string(T->Next->Down->Down->Column) + ": dont use name program for variable identifier\n";
			return;
		}
		ForVar = T->Next->Down->Down->Name;
		if (checkFor(ForVar)) {
			Error++;
			StrError += "Code Generator Eror: line " + to_string(T->Next->Down->Down->Line) + " column " + to_string(T->Next->Down->Down->Column) + ": dont use the same meter names\n";
			return;
		}
		else {
			ForVariable.push_back(ForVar);
		}
		if (T->Next->Next->Next->Func=="<loop-declaration>") {
			StrAsm.append("\tmov ax, ");
			expression(T->Next->Next->Next->Down->Down);
			StrAsm.append("\tmov "+ ForVar+", ax\n");
			StrAsm.append("\tmov ax, ");
			expression(T->Next->Next->Next->Down->Next->Next->Down);
			StrAsm.append("\tcmp ax, "+ ForVar+"\n");
			StrAsm.append("\tjl ?L"+to_string(Label)+"\n\tmov dx, 0\n\tmov .s+0, dx\n");
			Label++;
			LabelCounter++;
			StrAsm.append("\tsub ax, "+ForVar+"\n\tcwd\n\tdiv .s+0\n\tinc ax\n\tmov cx, ax\n?L"+to_string(Label)+": nop\n");
			Label ++;
			statements_list(T->Next->Next->Next->Down->Next->Next->Next->Next->Down);
			ForVariable.clear();
			Label--;
			StrAsm.append("\tmov ax, "+ForVar+"\n\tadd ax,.s+0\n\tmov "+ForVar+", ax\n\tloop ?L"+to_string(Label)+"\n");
			Label--;
			StrAsm.append("?L"+to_string(Label)+": nop\n");
			Label = LabelCounter++;
		}
	}
}

void CodeGenerator::expression(Tree* T)
{
	if (T->Code=='-') {
		StrAsm.append("-");
		summand(T->Next->Down);
		summand_list(T->Next->Next->Down);
	}
	else {
		summand(T->Down);
		summand_list(T->Next->Down);
	}
}

void CodeGenerator::summand(Tree* T)
{
	if (T->Func == "<unsigned-integer>") {
		StrAsm.append(T->Down->Name);
	}
	else if (T->Down->Down->Name==ProgramName) {
		T = T->Down->Down;
		Error++;
		StrError += "Code Generator Eror: line " + to_string(T->Line) + " column " + to_string(T->Column) + ": dont use name program for variable identifier\n";
		return;
	}
	else {
		StrAsm.append(T->Down->Down->Name);
	}
}

void CodeGenerator::summand_list(Tree* T)
{
	if (T->Func == "<empty>") {
		StrAsm.append("\n");
		return;
	}
	StrAsm.append(T->Down->Name);
	summand(T->Next->Down);
	summand_list(T->Next->Next->Down);
}

int CodeGenerator::checkFor(string var)
{
	for (int i = 0; i < ForVariable.size(); i++) {
		if (ForVariable[i] == var) {
			return 1;
		}
	}
	return 0;
}

void CodeGenerator::listing(string File)
{
	File += "generated.txt";
	ofstream f;
	f.open(File, ios::app);
	if (Error != 0) {
		cout << "Code Generator completed with error" << endl << StrError << endl << endl;
		f << "Code Generator completed with error" << endl << StrError << endl;
	}
	else {
		cout << "code generator completed successfuly" << endl << endl;
		f << StrAsm << endl << endl;
	}
}
