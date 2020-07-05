#ifndef SYNTACTICAL_ANALYZER_H
# define SYNTACTICAL_ANALYZER_H

#include "InformationTables.h"
#include "tree.hh"

class SyntacticalAnalyzer
{
    tree<std::string>::iterator root;
    tree<std::string>::iterator currNode;
    uint32_t lexemeCounter;
    Lexeme *currLexeme;
    bool errorFound;

    private:
        Lexeme *getLexeme();
        std::string findKeyOfValue(const std::map<std::string, uint32_t> &table,
                                   uint32_t value);
        void constantProcess();
        void constantIdentifierProcess();
        void constantDeclarationProcess();
        void constantDeclarationsListProcess();
        void constantDeclarationsProcess();
        void declarationsProcess();
        void statementsListProcess();
        void blockProcess();
        void identifierProcess();
        void procedureIdentifierProcess();
        void programProcess();
        void printError(const std::string &cause);

    public:
        SyntacticalAnalyzer();
        ~SyntacticalAnalyzer() = default;

        bool parseLexemesCodes();
};

#endif
