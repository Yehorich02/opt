#include "CodeGenerator.h"

CodeGenerator::CodeGenerator(string File)
{
	SyntaxAnaliz SynAnaliz(File);
	Lexems = SynAnaliz.Lexems;
	Pos = 0;
	Label = 0;
	LabelCounter = 0;
	Err = "";
	IdentMax = SynAnaliz.IdentMax;
	IdentMin = SynAnaliz.IdentMin;
	DigitMax = SynAnaliz.DigitMax;
	DigitMin = SynAnaliz.DigitMin;
	program();
	Listing(File);
}

void CodeGenerator::Listing(string File)
{
	File += "generated.txt";
	ofstream f;
	f.open(File, std::ios::app);
	if (Err != "") {
		cout << "Code Generator completed with error" << endl << Err << endl << endl;
		f << "Code Generator completed with error" << endl << Err << endl;
	}
	else {
		cout << "code generator completed successfuly" << endl << endl;
		f << StrAsm << endl << endl;
	}
}

bool CodeGenerator::program()
{
	if (Lexems[Pos].Code == 401) {
		Pos++;
		if (identifier()) {
			Pos++;
			if (Lexems[Pos].Code == 59) {
				Pos++;
				if (block()) {
					Pos++;
					if ((Pos < Lexems.size()) && (Lexems[Pos].Code == 46)) {
						Pos++;
						return true;
					}
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

bool CodeGenerator::block()
{
	if (Lexems[Pos].Code == 402) {
		Pos++;
		StrAsm.append("data segment\ndata ends\n");
		StrAsm.append("code segment\n\tassume cs:code, ds:data\n");
		StrAsm.append(Lexems[1].Name + ":\n");
		if (statement_list()) {
			Pos++;
			if (Lexems[Pos].Code == 403)
			{
				StrAsm.append("mov ah, 4ch\nint 21h\ncode ends\nend " + Lexems[1].Name);
				return true;
			}
			else
				return false;
		}
	}
	else
		return false;
}

bool CodeGenerator::statement_list() {
	if (statement()) {
		Pos++;
		Label = LabelCounter;
		if (statement_list()) {
			return true;
		}
	}
	else {
		StrAsm.append("\tnop\n");
		Pos--;
		return true;
	}
}

bool CodeGenerator::statement() {
	if (Lexems[Pos].Code == 408) {
		StrAsm.append("?L" + to_string(Label) + ":\n");
		int whileLabel = Label;
		Label++;
		LabelCounter++;
		Pos++;
		if (conditional_expression()) {
			Pos++;
			if (Lexems[Pos].Code == 409) {
				Pos++;
				Label++;
				LabelCounter++;
				if (statement_list()) {
					Label = whileLabel;
					StrAsm.append("\tjmp ?L" + to_string(Label) + "\n");
					Pos++;
					Label++;
					StrAsm.append("?L" + to_string(Label) + ":\tnop\n");
					if (Lexems[Pos].Code == 410) {
						Pos++;
						if (Lexems[Pos].Code == ';') {
							return true;
						}
						else
							return false;
					}
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (condition_statement()) {
		Pos++;
		if (Lexems[Pos].Code == 407) {
			Pos++;
			if (Lexems[Pos].Code == ';') {
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

bool CodeGenerator::condition_statement() {
	int ifLabel = Label;
	if (incomplete_condition_statement()) {
		Pos++;
		Label = ifLabel;
		StrAsm.append("?L" + to_string(Label) + ":\tnop\n");
		Label++;
		LabelCounter;
		if (alternative_part()) {
			StrAsm.append("?L" + to_string(Label) + ":\tnop\n");
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool CodeGenerator::conditional_expression() {
	if (expression()) {
		StrAsm.append("\tmov ax, " + Lexems[Pos].Name + "\n");
		Pos++;
		StrAsm.append("\tmov bx, " + Lexems[Pos + 1].Name + "\n");
		StrAsm.append("\tcmp ax, bx\n");
		if (comparison_operator()) {
			Pos++;
			if (expression()) {
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

bool CodeGenerator::incomplete_condition_statement() {
	if (Lexems[Pos].Code == 404) {
		Pos++;
		if (conditional_expression()) {
			LabelCounter++;
			Label++;
			Pos++;
			if (Lexems[Pos].Code == 405) {
				Pos++;
				int tempLabel = Label;
				Label++;
				LabelCounter++;
				if (statement_list()) {
					tmp = Label;
					Label = tempLabel;
					StrAsm.append("\tjmp ?L" + to_string(Label) + "\n");
					return true;
				}
			}
			else {
				return false;
			}
		}
		else
			return false;
	}
	else
		return false;
}

bool CodeGenerator::alternative_part() {
	if (Lexems[Pos].Code == 406) {
		Pos++;
		int tempLabel = Label;
		Label = tmp;
		if (statement_list()) {
			Label = tempLabel;
			return true;
		}
		else
			return false;
	}
	else {
		Pos--;
		return true;
	}
}

bool CodeGenerator::comparison_operator() {
	if (((Lexems[Pos].Code <= 62) && (Lexems[Pos].Code > 59)) || ((Lexems[Pos].Code <= 400) && (Lexems[Pos].Code >= 301)))
	{
		switch (Lexems[Pos].Code)
		{
		case 60:
			StrAsm.append("\tjl ?L" + to_string(Label) + "\n");
			return true;
		case 61:
			StrAsm.append("\tle ?L" + to_string(Label) + "\n");
			return true;
		case 62:
			StrAsm.append("\tjg ?L" + to_string(Label) + "\n");
			return true;
		case 304:
			StrAsm.append("\tjle ?L" + to_string(Label) + "\n");
			return true;
		case 305:
			StrAsm.append("\tjne ?L" + to_string(Label) + "\n");
			return true;
		case 306:
			StrAsm.append("\tjge ?L" + to_string(Label) + "\n");
			return true;
		}
	}
	else
		return false;
}

bool CodeGenerator::expression() {
	if (identifier()) {
		if (Lexems[Pos].Code != Lexems[1].Code)
			return true;
		else
			Err.append("Code Generator: Error on Line " + to_string(Lexems[Pos].Line) + ", Column " + to_string(Lexems[Pos].Column) + ": Invalid identifier, don`t use name program");
		return false;
	}
	else if (integer()) {
		return true;
	}
	else
		return false;
}

bool CodeGenerator::identifier() {
	if ((Lexems[Pos].Code >= IdentMin) && (Lexems[Pos].Code <= IdentMax))
	{
		return true;
	}
	else
		return false;
}

bool CodeGenerator::integer() {
	if ((Lexems[Pos].Code >= DigitMin) && (Lexems[Pos].Code <= DigitMax))
	{
		return true;
	}
	else
		return false;
}