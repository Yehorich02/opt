#pragma once
#include "tree.h"
#include <iostream>
#include <fstream>
#include "Context.h"
using namespace std;

void Print(node*, int, ofstream&); // print a tree

void ErrorMessage(Context& context, int& CurPosition, string expectedToken); // expected 'smthg' but found 'smthg'
void ErrorisStopParse(Context& context, int& CurPosition, string expectedToken); // expected 'smthg' but found EOF
void UnexpectedEOF(Context& context, int& CurPosition); // error: unexpected eof

bool isStopParse(int CurPosition, Context& context); // check whether it`s eof
bool isProgram(Context& context, int& CurPosition); // 'PROGRAM'
bool isID(Context& context, int& CurPosition); // 'identifier'
bool isConst(Context& context, int& CurPosition); // 'unsigned integer'
bool isSemicolon(Context& context, int& CurPosition); // ';'
bool isBegin(Context& context, int& CurPosition); // 'BEGIN'
bool isEnd(Context& context, int& CurPosition); //'END'
bool isGoto(Context& context, int& CurPosition); // 'GOTO'
bool isLink(Context& context, int& CurPosition); //'LINK'
bool isIn(Context& context, int& CurPosition); // 'IN'
bool isOut(Context& context, int& CurPosition); // 'OUT'
bool isReturn(Context& context, int& CurPosition); // 'RETURN'
bool isOpenBracket(Context& context, int& CurPosition); // '('
bool isCloseBracket(Context& context, int& CurPosition); // ')'
bool isComma(Context& context, int& CurPosition); // ','
bool isColon(Context& context, int& CurPosition); // ':'
bool isDelim2(Context& context, int& CurPosition); // ':='
bool isOpenDollar(Context& context, int& CurPosition);// '($'
bool isCloseDollar(Context& context, int& CurPosition);// '$)'
