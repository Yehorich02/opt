#ifndef LEXICAL_ANALYZER_H
# define LEXICAL_ANALYZER_H

# include <cctype>
# include <fstream>
# include <iostream>
# include "InformationTables.h"

class LexicalAnalyser
{
    uint32_t countLines;
    uint32_t countPosition;
    std::ifstream in;
    bool errorFound;
    std::string buff;
    char symbol;

    private:
        char getSymbol();
        void addConstant(uint32_t position);
        void processIdentifier();
        void processDigits(uint32_t position);
        void processComa(uint32_t position);
        void processEXPLexeme(uint32_t position, const std::string &r = "1");
        void processComment();
        void printComplexConstants();
        void printIdentifiersTable();
        void printLexemesTable();
        void printError(const std::string &cause);

    public:
        LexicalAnalyser(const std::string &fileName);
        ~LexicalAnalyser() = default;

        bool parseFile();
        void printResultTables();

};

#endif
