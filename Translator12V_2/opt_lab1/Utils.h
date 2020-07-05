#pragma once
#include "TypesFmd.h"

void invalidCharacter(const Context&, ofstream&);
void invalidId(const Context&, ofstream&);
void unclosedCom(const Context&, ofstream&);
void outputResult(const Context&, ofstream&);
void headOfTable(ofstream& out);

void createSymbolCategories(Context&);

void currentTokenIsConstant(Context&, ofstream&);
void currentTokenIsDm1(Context&, ofstream&);
void currentTokenIsIdentOrKeyword(Context&, ofstream&);
void currentTokenIsDm2(Context&, ofstream&);
//void currentTokenIsTest(Context&, ofstream&);


void regularBufferCheck(Context&, ofstream&);

//void mustBeAnLetter(const Context& context, ofstream& out);
//void mustBePlus(const Context& context, ofstream& out);
//void mustBeDig(const Context& context, ofstream& out);
