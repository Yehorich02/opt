#include "LexAnaliz.h"

LeksAnaliz::LeksAnaliz()
{
	Line = 1;
	Column = 1;
	KeyWordsMin = 401;
	MinDigit = 501;
	DigitCounter = MinDigit;
	MinIdent = 1001;
	IdentCounter = MinIdent;
	LexErrorMin = 2001;
	LexErrorCounter = LexErrorMin;
	ComError = 2200;
	Lex buf;

	buf.Name = "PROCEDURE";
	buf.Code = 401;
	KeyWords.push_back(buf);
	buf.Name = "BEGIN";
	buf.Code = 402;
	KeyWords.push_back(buf);
	buf.Name = "END";
	buf.Code = 403;
	KeyWords.push_back(buf);
	buf.Name = "LABEL";
	buf.Code = 404;
	KeyWords.push_back(buf);
	buf.Name = "GOTO";
	buf.Code = 405;
	KeyWords.push_back(buf);
	buf.Name = "RETURN";
	buf.Code = 406;
	KeyWords.push_back(buf);

	for (int i = 0; i < 128; i++) {
		if ((i == 9) || (i == 10) || (i == 32) || (i == 12) || (i == 13))
			ASCIIArr[i] = 0;
		else {
			if ((i < 91) && (i > 64))
				ASCIIArr[i] = 1;
			else {
				if ((i < 58) && (i > 47))
					ASCIIArr[i] = 2;
				else {
					if ((i == ')') || (i == '(') || (i == '*') || (i == '.') || (i == ';') || (i == ',') || (i == ':') || (i == '$'))
						ASCIIArr[i] = 3;
					else
						ASCIIArr[i] = 4;
				}
			}
		}
	}
}

void LeksAnaliz::AddLeksem(int Code, int Line, int Column, string Name)
{
	Lex buf;
	buf.Code = Code;
	buf.Line = Line;
	buf.Column = Column;
	buf.Name = Name;
	Lexems.push_back(buf);
}

bool LeksAnaliz::SizeOut()
{
	return !t.eof();
}

void LeksAnaliz::Analizator(string file)
{
	t.open(file + "input.sig");
	if (!t.is_open()) {
		std::cout << "failed to open " << file << '\n';
		return;
	}
	Pos = 0;
	Line = 1;
	Column = 1;
	sbuff = t.get();
	if (SizeOut())
		INP();
	MakeListing(file);
	t.close();
}

void LeksAnaliz::INP()
{
	while (SizeOut())
	{
		switch (ASCIIArr[sbuff])
		{
		case 0:
			SPACE();
			break;
		case 1:
			IDN();
			break;
		case 2:
			Digit();
			break;
		case 3:
			DM();
			break;
		case 4:
			ERR("");
			break;
		}
	}
}

void LeksAnaliz::SPACE()
{
	if (SizeOut())
	{
		while ((SizeOut()) && (ASCIIArr[sbuff] == 0))
		{
			if (sbuff == 10)
			{
				Pos++; Column++;
				sbuff = t.get();
				Line++;
				Column = 1;
			}
			else {
				Pos++; Column++;
				sbuff = t.get();
			}
		}
		return;
	}
}

void LeksAnaliz::Digit()
{
	SaveLine = Line;
	SaveColumn = Column;
	string Buf = "";
	while ((SizeOut()) && (ASCIIArr[sbuff] == 2))
	{
		Buf += sbuff;
		Pos++;
		sbuff = t.get();
		Column++;
	}

	switch (ASCIIArr[sbuff])
	{
	case 0:
	case 3:
	{
		int n = SearchDigit(Buf);
		if (n == -1)
		{
			AddLeksem(DigitCounter, SaveLine, SaveColumn, Buf);
			TableD.push_back(Buf);
			DigitCounter++;
		}
		else
		{
			AddLeksem(n + MinDigit, SaveLine, SaveColumn, Buf);
		}
		Buf = "";
		return;
	}
	break;
	case 1:
	case 4:
		ERR(Buf);
		break;
	}
}

void LeksAnaliz::IDN()
{
	int n;
	SaveLine = Line;
	SaveColumn = Column;
	string Buf = "";
	while ((SizeOut()) && ((ASCIIArr[sbuff] == 2) || (ASCIIArr[sbuff] == 1)))
	{
		Buf += sbuff;
		Pos++;
		sbuff = t.get();
		Column++;
	}
	if (ASCIIArr[sbuff] == 4)
	{
		ERR(Buf);
	}
	else
	{
		n = SearchStandartIdent(Buf);
		if (n == -1) {
			n = SearchIdent(Buf);
			if (n == -1)
			{
				AddLeksem(IdentCounter, SaveLine, SaveColumn, Buf);
				Table.push_back(Buf);
				IdentCounter++;
			}
			else
			{
				AddLeksem(n + MinIdent, SaveLine, SaveColumn, Buf);
			}
		}
		else AddLeksem(KeyWords[n].Code, SaveLine, SaveColumn, KeyWords[n].Name);
		Buf = "";
		return;
	}
}

void LeksAnaliz::BCOM()
{
	Pos++;
	sbuff = t.get();
	Column++;
	COM("(*");
}

void LeksAnaliz::COM(string Buf)
{
	while (SizeOut() && (sbuff != '*'))
	{
		if (sbuff == '\n')
		{
			Line++;
			Column = 1;
			Pos++;
			sbuff = t.get();
		}
		else {
			Buf += sbuff;
			Pos++;
			sbuff = t.get();
			Column++;
		}
	}
	if (!SizeOut())
		AddLeksem(ComError, SaveLine, SaveColumn, Buf);
	else ECOM(Buf);
}

void LeksAnaliz::ECOM(string Buf)
{
	Buf += sbuff;
	sbuff = t.get();
	Pos++;
	Column++;
	if ((SizeOut()) && (sbuff == ')'))
	{
		Buf += sbuff;
		Pos++;
		sbuff = t.get();
		Column++;
		return;
	}
	else if (!SizeOut())
		AddLeksem(ComError, SaveLine, SaveColumn, Buf);
	else COM(Buf);
}

void LeksAnaliz::DM()
{
	SaveLine = Line;
	SaveColumn = Column;
	string Buf = "";

	if (sbuff == '(')
	{
		Buf += sbuff;
		Pos++;
		sbuff = t.get();
		Column++;
		if (sbuff == '*') {
			BCOM();
		}
		else if (sbuff == '$') {
			Buf += sbuff;
			AddLeksem(301, SaveLine, SaveColumn, Buf);
			Pos++;
			sbuff = t.get();
			Column++;
			return;
		}
		else {
			AddLeksem('(', Line, Column, Buf);
			return;
		}
	}
	else {
		if (sbuff == '*')
		{
			Buf += sbuff;
			ERR(Buf);
		}
		else if (sbuff == '$') {
			Buf += sbuff;
			Pos++;
			sbuff = t.get();
			Column++;
			if (sbuff == ')') {
				Buf += sbuff;
				AddLeksem(302, SaveLine, SaveColumn, Buf);
				Pos++;
				sbuff = t.get();
				Column++;
				return;
			}
			else {
				ERR(Buf);
				return;
			}
		}
		else
		{
			Buf += sbuff;
			AddLeksem(sbuff, Line, Column, Buf);
			Pos++;
			sbuff = t.get();
			Column++;
			return;
		}
	}
}

void LeksAnaliz::ERR(string pt)
{
	string Buf = pt;
	if ((sbuff == '*') || (sbuff == ')'))
	{
		Buf += sbuff;
		AddLeksem(LexErrorCounter, SaveLine, SaveColumn, Buf);
		LexErrorCounter++;
		Pos++;
		sbuff = t.get();
		Column++;
		Buf = "";
		INP();
	}
	else
	{
		while ((ASCIIArr[sbuff] != 0) && (ASCIIArr[sbuff] != 3) && (SizeOut()))
		{
			Buf += sbuff;
			Pos++;
			sbuff = t.get();
			Column++;
		}
		AddLeksem(LexErrorCounter, SaveLine, SaveColumn, Buf);
		LexErrorCounter++;
		Buf = "";
		INP();
	}
}

int LeksAnaliz::SearchIdent(string Ident)
{
	for (int i = 0; i < Table.size(); i++)
	{
		if (Table[i] == Ident)
			return i;
	}
	return -1;
}

int LeksAnaliz::SearchDigit(string Digit)
{
	for (int i = 0; i < TableD.size(); i++)
	{
		if (TableD[i] == Digit)
			return i;
	}
	return -1;
}

int LeksAnaliz::SearchStandartIdent(string Ident)
{
	for (int i = 0; i < KeyWords.size(); i++)
	{
		if (KeyWords[i].Name == Ident)
			return i;
	}
	return -1;
}

void LeksAnaliz::MakeListing(string file)
{
	file = file + "generated.txt";
	ofstream f(file);
	f << setw(6) << "Line" << setw(10) << "Column" << setw(8) << "Code" << setw(20) << "Name" << endl;
	f << endl;
	for (int i = 0; i < Lexems.size(); i++)
	{
		if (Lexems[i].Code - 2000 < 0)
			f << setw(4) << Lexems[i].Line << setw(10) << Lexems[i].Column << setw(10) << Lexems[i].Code << setw(21) << Lexems[i].Name << endl;
	}
	f << endl;
	f << endl;
	int p = 0;
	for (int i = 0; i < Lexems.size(); i++)
	{
		for (int j = LexErrorMin; j < LexErrorCounter; j++)
		{
			if (Lexems[i].Code == j)
			{
				f << "Lexical error on Line " << Lexems[i].Line << " Column " << Lexems[i].Column << ": Impossible combination of characters: " << Lexems[i].Name << endl;
				p++;
			}
		}
		if (Lexems[i].Code == 2200)
		{
			f << "Lexical error on Line " << Lexems[i].Line << " Column " << Lexems[i].Column << ": Not a close comment " << Lexems[i].Name << endl;
			p++;
		}
	}
	f.close();
	if (p == 0) {
		cout << "Lexical analysis completed successfully" << endl;
	}
	else {
		cout << "Lexical analysis completed with error" << endl;
		for (int i = 0; i < Lexems.size(); i++)
		{
			for (int j = LexErrorMin; j < LexErrorCounter; j++)
			{
				if (Lexems[i].Code == j)
				{
					cout << "Lexical error on Line " << Lexems[i].Line << " Column " << Lexems[i].Column << " Code " << Lexems[i].Code << " : Impossible combination of characters: " << Lexems[i].Name << endl;
				}
			}
			if (Lexems[i].Code == 2200)
			{
				cout << "Lexical error on Line " << Lexems[i].Line << " Column " << Lexems[i].Column << " Code " << Lexems[i].Code << " : Not a close comment: " << Lexems[i].Name << endl;
			}
		}
	}
}

LeksAnaliz::~LeksAnaliz() {
}