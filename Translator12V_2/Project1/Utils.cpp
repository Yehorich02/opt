#include <iostream>
#include <fstream>
#include <iomanip>
#include "Utils.h"
#include "TypesFmd.h"

using namespace std;

///------error messages------/////
void invalidCharacter(const Context& context, ofstream& out) {
	cout << "\nThere is an error on " << context.currentLine << " line, " << context.currentColumn << " column: " << "invalid character: " << context.currentTokenName << "\n";
}
void invalidId(const Context& context, ofstream& out) {
	cout << "\nThere is an error on " << context.currentLine << " line, " << context.currentColumn << " column: " << "identifier cannot begin with a digit: " << context.currentTokenName << "\n";
}
void unclosedCom(const Context& context, ofstream& out) {
	cout << "\nThere is an error on " << context.currentLine << " line, " << context.currentColumn << " column: " << "unclosed comment!" << "\n";
}
/*void mustBeAnLetter(const Context& context, ofstream& out) {
	out << "\nThere is an error on " << context.currentLine << " line, " << context.currentColumn << " column: " << "there should be an letter!" << "\n";
}
void mustBePlus(const Context& context, ofstream& out) {
	out << "\nThere is an error on " << context.currentLine << " line, " << context.currentColumn << " column: " << "there should be '+' !" << "\n";
}
void mustBeDig(const Context& context, ofstream& out) {
	out << "\nThere is an error on " << context.currentLine << " line, " << context.currentColumn << " column: " << "there should be an digit !" << "\n";
}
*/
///-------output token information-------///
void outputResult(const Context& context, ofstream& out) {
	/*out << setw(15) << left << (context.result.back()).tokenName
		<< setw(15) << left << (context.result.back()).tokenNumber;
	out << setw(15) << left << (context.result.back()).line
		<< setw(15) << left << (context.result.back()).column << "\n";*/
}

void headOfTable(ofstream& out) {
	/*out << setw(15) << left << "Name"
		<< setw(15) << left << "Number";
	out << setw(15) << left << "Line"
		<< setw(15) << left << "Column" << "\n";*/
}

void createSymbolCategories(Context& context) {  //characters categorization
	for (int i = 0; i < 127; i++) {
		if ((i > 8 && i < 14) || (i == 32)) {   //ws or analogues
			context.symbolCategories[i] = 0;
		}
		else if (i > 47 && i < 58) {   //digits
			context.symbolCategories[i] = 1;
		}
		else if (i > 64 && i < 91) {  //uppercase letter
			context.symbolCategories[i] = 2;
		}
		else if (i == 44 || i == 58 || i == 59 || i == 46) {  //single character delimiter
			context.symbolCategories[i] = 3;
		}
		else if (i == 36) {   //two-character delimiter
			context.symbolCategories[i] = 4;
		}
		else if (i == 40 || i == 41 || i == 42) {   //comment symbol
			context.symbolCategories[i] = 5;
		}
		/* else if (i == 43) {  //symbol task
			 context.symbolCategories[i] = 7;
		 }*/
		else { context.symbolCategories[i] = 6; }  //invalid character
	}
}

void currentTokenIsConstant(Context& context, ofstream& out) {
	context.result.push_back({ context.currentTokenName, ++context.currentConstantNumber, context.currentLine, context.currentColumn - (int)context.currentTokenName.length() });
	outputResult(context, out);
	context.currentTokenName = "";
	context.currentCategory = 0;
}
void currentTokenIsDm1(Context& context, ofstream& out) {
	context.result.push_back({ context.currentTokenName, context.dm1Id, context.currentLine, context.currentColumn - 1 });
	outputResult(context, out);
	context.currentTokenName = "";
	context.currentCategory = 0;
}
void currentTokenIsIdentOrKeyword(Context& context, ofstream& out) {
	vector<keywordStruct>::iterator it = find_if(context.keywordVec.begin(), context.keywordVec.end(), [&](const auto& keyword) {return keyword.keywordName == context.currentTokenName; });
	if (it != context.keywordVec.end()) {     //this is a keyword
		context.result.push_back({ context.currentTokenName, (*it).keywordNumber, context.currentLine, context.currentColumn - (int)context.currentTokenName.length() });
	}
	else {   //this is the identifier
		context.result.push_back({ context.currentTokenName, ++context.currentIndentNumber, context.currentLine, context.currentColumn - (int)context.currentTokenName.length() });
	}
	outputResult(context, out);
	context.currentTokenName = "";
	context.currentCategory = 0;
}
void currentTokenIsDm2(Context& context, ofstream& out) {
	vector<dm2Struct>::iterator it = find_if(context.dm2Vec.begin(), context.dm2Vec.end(), [&](const auto& dm2) {return dm2.dm2Name == context.currentTokenName; });
	if (it != context.dm2Vec.end()) {
		context.result.push_back({ context.currentTokenName, (*it).dm2Number, context.currentLine, context.currentColumn - 2 });
	}
	outputResult(context, out);
	context.currentTokenName = "";
	context.currentCategory = 0;
}
/*void currentTokenIsTest(Context& context, ofstream& out) {
	context.result.push_back({ context.currentTokenName, ++context.currentTestNumber, context.currentLine, context.currentColumn - (int)context.currentTokenName.length() });
	outputResult(context, out);
	context.currentTokenName = "";
	context.currentCategory = 0;
}
*/

void regularBufferCheck(Context& context, ofstream& out) {
	switch (context.currentCategory)
	{
	case(0):
	case(5): {
		break;
	}
	case(1): {
		currentTokenIsConstant(context, out);
		break;
	}
	case(2): {
		currentTokenIsIdentOrKeyword(context, out);
		break;
	}
	case(3): {
		currentTokenIsDm1(context, out);
		break;
	}
	case(4): {
		currentTokenIsDm2(context, out);
		break;
	}
		   /*case(7): {
			   currentTokenIsTest(context, out);
			   break;
		   }*/
	default: {
		cout << " !error! ";
		break;
	}
	}
}