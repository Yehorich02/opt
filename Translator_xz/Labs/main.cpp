#include <iostream>
#include <fstream>
#include <cctype>

#include "InformationTables.h"
#include "LexicalAnalyzer.h"
#include "SyntacticalAnalyzer.h"
#include "CodeGenerator.h"

int main()
{
    LexicalAnalyser lexer("Parser_TrueTest1.txt");
    SyntacticalAnalyzer parser;
    CodeGenerator generator;

    if (!lexer.parseFile()) {
        std::cout << "Error in Lexical Analysis. File processing stopped." << std::endl;
        return -1;
    }

    if (!parser.parseLexemesCodes()) {
        std::cout << "Error in Syntactical Analysis. File processing stopped." << std::endl;
        return -2;
    }

    generator.compile();

    return 0;
}
