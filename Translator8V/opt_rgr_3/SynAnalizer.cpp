#include "SynAnalizer.h"

SynAnaliz::SynAnaliz(string File) {
	LeksAnaliz LexAn;
	LexAn.Analizator(File);
	IdentMin = LexAn.MinIdent;
	IdentMax = LexAn.LexErrorMin - 1;
	Lexems = LexAn.Lexems;
	start();
	TreeListing(File);
	if (ErrorCounter == 0)
		cout << "SynAnaliz analysis completed successfully" << endl;
}


//<statements - list> -- > <statement><statements - list> | <empty>
//<statement> -- > IF <cond> THEN <statements - list> <else> ENDIF;
//<cond> -- > <variable - identifier> = <variable - identifier>
//<else> -- > ELSE <statements - list> | <empty>

void SynAnaliz::start() {
	ErrorStr = "";
	ErrorCounter = 0;
	Pos = 0;
	space = 0;
	Out = (tab(space) + "<signal-program>\n");
	Out += (tab(space+1) + "<program>\n");
	space+=2;
	program();
}

string SynAnaliz::tab(int sps) {
	string Tab = "";
	for (int i = 0; i < sps; i++)
		Tab += "    ";
	return Tab;
}

bool SynAnaliz::LexemsOut() {
	if (Pos >= Lexems.size()) {
		Errors(1, Pos);
		return false;
	}
	else {
		return true;
	}
}

void SynAnaliz::program() {
	if (LexemsOut()) {
		if (Lexems[Pos].Code == 401) {
			Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
			Pos++;
			if (procedure_identifier()==1) {
				Pos++;
				Out += (tab(space) + "<parameters-list>\n");
				if (parameters_list()==1) {
					Pos++;
					if (LexemsOut()) {
						if (Lexems[Pos].Code == 59) {
							Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
							Pos++;
							Out += (tab(space) + "<block>\n");
							if (block()==1) {
								Pos++;
								if (LexemsOut()) {
									if (Lexems[Pos].Code == 59) {
										Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
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
								return;
							}
						}
						else {
							Errors(2, Pos);
							return;
						}
					}
					else {
						return;
					}
				}
				else {
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
	else
	{
		return;
	}
}

int SynAnaliz::block() {
	if (LexemsOut()) {
		space++;
		if (declarations()==1) {
			Pos++;
			if (Lexems[Pos].Code == 402)
			{
				Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
				Pos++;
				Out += (tab(space) + "<statements-list>\n");
				if (statement_list()==1) {
					Pos++;
					if (LexemsOut()) {
						if (Lexems[Pos].Code == 403)
						{
							Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
							space--;
							return 1;
						}
						else
						{
							Errors(7, Pos);
							return 2;
						}
					}
					else {
						return 2;
					}
				}
				else {
					return 2;
				}
			}
			else
			{
				Errors(6, Pos);
				return 2;
			}
		}
		else {
			space--;
			return 2;
		}
	}
	else {
		return 0;
	}
}
int SynAnaliz::declarations() {
	if (LexemsOut()) {
		Out += (tab(space) + "<declarations>\n");
		space++;
		if (Lexems[Pos].Code == 410) {
			Out += (tab(space) + "<variable-declarations>\n");
			space++;
			Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
			Pos++;
			if (declarations_list()==1) {
				space -= 2;
				return 1;
			}
			else {
				return 2;
			}
		}
		else {
			Out += (tab(space) + "<empty>");
			Pos--;
			space--;
			return 1;
		}
	}
	else {
		return 0;
	}
}

int SynAnaliz::statement_list() {
	if (LexemsOut()) {
		space++;
		int ret = statement();
		if (ret == 1) {
			Pos++;
			Out += (tab(space) + "<statements-list>\n");
			ret = statement_list();
			if (ret == 1) {
				space--;
				return 1;
			}
			else if (ret == 2) {
				return 2;
			}
			else
				return 0;
		}
		else if (ret == 0) {
			Out += (tab(space) + "<empty>\n");
			Pos--;
			space--;
			return 1;
		}
		else {
			return 2;
		}
	}
	else {
		return 0;
	}
}

int SynAnaliz::statement() {
	if (LexemsOut()) {
		if (Lexems[Pos].Code == 411) {
			Out += (tab(space) + "<statement>\n");
			space++;
			Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
			Pos++;
			int ret = cond();
			if (ret==1) {
				Pos++;
				if (Lexems[Pos].Code == 414) {
					Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
					Pos++;
					Out += (tab(space) + "<statements-list>\n");
					if (statement_list() == 1) {
						Pos++;
						ret = else_();
						if (ret == 1) {
							Pos++;
							if (Lexems[Pos].Code == 413) {
								Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
								Pos++;
								if (Lexems[Pos].Code == 59) {
									Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
									space--;
									return 1;
								}
								else {
									Errors(2, Pos);
									return 2;
								}
							}
							else {
								Errors(14, Pos);
								return 2;
							}
						}
						else {
							return 2;
						}
					}
					else {
						return 2;
					}
				}
				else {
					Errors(13, Pos);
					return 2;
				}
			}
			else {
				return 2;
			}
		}
		else {
			return 0;
		}
		
	}
	else {
		return 0;
	}

}

int SynAnaliz::cond() {
	if (LexemsOut()) {
		Out += (tab(space) + "<cond>\n");
		space++;
		if (variable_identifier()) {
			Pos++;
			if (Lexems[Pos].Code == '=') {
				Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
				Pos++;
				if (variable_identifier()) {
					space--;
					return 1;
				}
				else {
					Errors(10, Pos);
					return 2;
				}
			}
			else {
				Errors(10, Pos);
				return 2;
			}
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}


int SynAnaliz::else_() {
	if (LexemsOut()) {
		Out += (tab(space) + "<else>\n");
		space++;
		if (Lexems[Pos].Code == 412) {
			Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
			Pos++;
			Out += (tab(space) + "<statements-list>\n");
			if (statement_list() == 1) {
				space--;
				return 1;
			}
			else {
				return 2;
			}
		}
		else {
			Out += (tab(space) + "<empty>\n");
			Pos--;
			space--;
			return 1;
		}

	}
	else {
		return 0;
	}
}

int SynAnaliz::declarations_list() {
	if (LexemsOut()) {
		Out += (tab(space) + "<declarations-list>\n");
		space++;
		int ret = declaration();
		if (ret==1) {
			Pos++;
			ret = declarations_list();
			if (ret == 1) {
				space--;
				return 1;
			}
			else if (ret == 2) {
				return 2;
			}
			else
				return 0;
		}
		else if (ret==0){
			Out += (tab(space) + "<empty>\n");
			Pos--;
			space--;
			return 1;
		}
		else {
			return 2;
		}
	}
	else {
		return 0;
	}
}

int SynAnaliz::attributes_list() {
	if (LexemsOut()) {
		space++;
		if (attribute()==1) {
			Out += (tab(space) + "<attributes-list>\n");
			Pos++;
			if (attributes_list()==1) {
				space--;
				return 1;
			}
			else {
				return 2;
			}
		}
		else {
			Out += (tab(space) + "<empty>\n");
			Pos--;
			space--;
			return 1;
		}
	}
	else {
		return 0;
	}
}

int SynAnaliz::parameters_list() {
	if (LexemsOut()) {
		space++;
		if (Lexems[Pos].Code == '(') {
			Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
			Pos++;
			if (declarations_list()==1) {
				Pos++;
				if (Lexems[Pos].Code == ')') {
					Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
					space--;
					return 1;
				}
				else {
					Errors(11, Pos);
					return 2;
				}
			}
			else {
				return 2;
			}
		}
		else {
			Out += (tab(space) + "<empty>\n");
			Pos--;
			space--;
			return 1;
		}
	}
	else {
		return 0;
	}
}

int SynAnaliz::declaration() {
	if (LexemsOut()) {
		if ((Lexems[Pos].Code >= IdentMin) && (Lexems[Pos].Code <= IdentMax)) {
			Out += (tab(space) + "<declaration>\n");
			space++;
			if (variable_identifier()==1) {
				Pos++;
				Out += (tab(space) + "<identifiers-list>\n");
				if (identifiers_list()==1) {
					Pos++;
					if (Lexems[Pos].Code == ':') {
						Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
						Pos++;
						if (attribute()==1) {
							Pos++;
							Out += (tab(space) + "<attributes-list>\n");
							if (attributes_list()==1) {
								Pos++;
								if (LexemsOut()) {
									if (Lexems[Pos].Code == 59) {
										Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
										space--;
										return 1;
									}
									else
									{
										Errors(3, Pos);
										return 2;
									}
								}
							}
							else
							{
								return 2;
							}
						}
						else {
							Errors(12, Pos);
							return 2;
						}
					}
					else {
						Errors(9, Pos);
						return 2;
					}
				}
				else {
					return 2;
				}
			}
			else {
				space--;
				return 2;
			}
		}
		else
		{
			return 0;
		}
	}
	else {
		return 0;
	}
}

int SynAnaliz::attribute() {
	if (LexemsOut()) {
		if ((Lexems[Pos].Code >= 404) && (Lexems[Pos].Code <= 409))
		{
			Out += (tab(space) + "<attribute>\n");
			Out += (tab(space+1) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else {
		return 0;
	}
}

int SynAnaliz::identifiers_list() {
	if (LexemsOut()) {
		space++;
		if (Lexems[Pos].Code == ',') {
			Out += (tab(space) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
			Pos++;
			if (variable_identifier()==1) {
				Pos++;
				Out += (tab(space) + "<identifiers-list>\n");
				if (identifiers_list()==1) {
					space--;
					return 1;
				}
				else {
					return 2;
				}
			}
			else {
				Errors(10, Pos);
				return 2;
			}
		}
		else {
			Out += (tab(space) + "<empty>\n");
			space--;
			Pos--;
			return 1;
		}
	}
	else {
		return 0;
	}
}

int SynAnaliz::procedure_identifier() {
	Out += (tab(space) + "<procedure-identifier>\n");
	if (identifier())
		return 1;
	else
		return 0;
}

int SynAnaliz::variable_identifier() {
	Out += (tab(space) + "<variable-identifier>\n");
	if (identifier())
		return 1;
	else
		return 0;
}
int SynAnaliz::identifier() {
	if (LexemsOut()) {
		if ((Lexems[Pos].Code >= IdentMin) && (Lexems[Pos].Code <= IdentMax))
		{
			space++;
			Out += (tab(space) + "<identifier>\n");
			Out += (tab(space+1) + to_string(Lexems[Pos].Code) + "   " + Lexems[Pos].Name + "\n");
			space --;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else {
		return 0;
	}
}

void SynAnaliz::Errors(int p, int pos) {
	switch (p) {
	case 1:
		cout << "SynAnaliz Error Unexpected end of the file!" << endl;
		ErrorStr += "SynAnaliz Error Unexpected end of the file!\n";
		ErrorCounter++;
		break;
	case 2:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << "): \";\" is passed" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : \";\" is passed\n";
		ErrorCounter++;
		break;
	case 3:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \";\" is passed after the \"END\"" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \";\" is passed after the \"END\"\n";
		ErrorCounter++;
		break;
	case 4:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Keyword \"PROCEDURE\" is passed" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Keyword \"PROCEDURE\" is passed\n";
		ErrorCounter++;
		break;
	case 5:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Must be procedure identifier" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Must be procedure identifier\n";
		ErrorCounter++;
		break;
	case 6:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Keyword \"BEGIN\" is passed" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Keyword \"BEGIN\" is passed\n";
		ErrorCounter++;
		break;
	case 7:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Keyword \"END\" is passed" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Keyword \"END\" is passed\n";
		ErrorCounter++;
		break;
	case 8:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \",\" is passed" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \",\" is passed\n";
		ErrorCounter++;
		break;
	case 9:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \":\" is passed" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \":\" is passed\n";
		ErrorCounter++;
		break;

	case 10:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Must be variable identifier" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Must be variable identifier\n";
		ErrorCounter++;
		break;
	case 11:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") \")\" is passed" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") \")\" is passed\n";
		ErrorCounter++;
		break;
	case 12:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Must be attribute" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Must be attribute\n";
		ErrorCounter++;
		break;
	case 13:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Keyword \"THEN\" is passed" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Keyword \"THEN\" is passed\n";
		ErrorCounter++;
		break;
	case 14:
		cout << "SynAnaliz: Error (line " << Lexems[pos].Line << ", column " << Lexems[pos].Column << ") : Keyword \"ENDIF\" is passed" << endl;
		ErrorStr += "SynAnaliz: Error (line " + to_string(Lexems[pos].Line) + ", column " + to_string(Lexems[pos].Column) + ") : Keyword \"ENDIF\" is passed\n";
		ErrorCounter++;
		break;

	}
}

void SynAnaliz::TreeListing(string File) {
	File += "generated.txt";
	ofstream f;
	f.open(File);
	if (ErrorCounter > 0) {
		f << "SynAnaliz analysis completed with error" << endl;
		f << ErrorStr;
	}
	else {
		f << Out << endl;
	}
}