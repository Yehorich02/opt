#include "SyntacticalAnalyzer.h"
#include <iostream>
#include <algorithm>
#include <string>
#include "tree.hh"
#include "tree_util.hh"

tree<std::string> syntaxTree;

SyntacticalAnalyzer::SyntacticalAnalyzer() :
    root(syntaxTree.begin()),
    lexemeCounter(0),
    currLexeme(nullptr),
    errorFound(false)
{
}

std::string SyntacticalAnalyzer::findKeyOfValue(const std::map<std::string, uint32_t> &table,
                                                uint32_t value)
{
    std::map<std::string, uint32_t>::const_iterator it;

    for (it = table.begin(); it != table.end(); ++it) {
        if (it->second == value) {
            return it->first;
        }
    }

    return "";
}

Lexeme *SyntacticalAnalyzer::getLexeme()
{
    currLexeme = lexemeCounter < lexemesCodes.size() ? &lexemesCodes.at(lexemeCounter) : nullptr;
    ++lexemeCounter;
    return currLexeme;
}

void SyntacticalAnalyzer::constantProcess()
{
    currNode = syntaxTree.append_child(currNode, "<constant>");

    if (currLexeme && currLexeme->code == '\'') {
        syntaxTree.append_child(currNode, "'");
        getLexeme();
    }
    else if (currLexeme &&
                currLexeme->code >= COMPLEX_NUM_START_CODE &&
                currLexeme->code < IDENTIFIER_START_CODE) {
        currNode = syntaxTree.append_child(currNode, "<unsigned-integer>");
        syntaxTree.append_child(currNode, findKeyOfValue(complexNumsTable, currLexeme->code));
        getLexeme();
        currNode = syntaxTree.parent(currNode);
        return;
    }
    else {
        printError("Expected Constant after '='");
    }
    if (errorFound) { return; currNode = syntaxTree.parent(currNode); }

    if (currLexeme &&
            currLexeme->code >= COMPLEX_NUM_START_CODE &&
            currLexeme->code < IDENTIFIER_START_CODE) {
        currNode = syntaxTree.append_child(currNode, "<complex-number>");
        syntaxTree.append_child(currNode, findKeyOfValue(complexNumsTable, currLexeme->code));
        getLexeme();
        currNode = syntaxTree.parent(currNode);
    }
    else {
        printError("Unknown constant " + currLexeme->name);
    }
    if (errorFound) { return; currNode = syntaxTree.parent(currNode); }

    if (currLexeme && currLexeme->code == '\'') {
        syntaxTree.append_child(currNode, "'");
        getLexeme();
    }
    else {
        printError("Delimeter ''' expected");
    }
    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::constantIdentifierProcess()
{
    currNode = syntaxTree.append_child(currNode, "<constant-identifier>");
    identifierProcess();
    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::constantDeclarationProcess()
{
    currNode = syntaxTree.append_child(currNode, "<constant-declaration>");
    constantIdentifierProcess();
    if (errorFound) { return; currNode = syntaxTree.parent(currNode); }

    if (currLexeme && currLexeme->code == '=') {
        syntaxTree.append_child(currNode, "=");
        getLexeme();
    }
    else {
        printError("Delimeter '=' expected");
    }
    if (errorFound) { return; currNode = syntaxTree.parent(currNode); }

    constantProcess();
    if (errorFound) { return; currNode = syntaxTree.parent(currNode); }

    if (currLexeme && currLexeme->code == ';') {
        syntaxTree.append_child(currNode, ";");
        getLexeme();
    }
    else {
        printError("Delimeter ';' expected");
    }

    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::constantDeclarationsListProcess()
{
    currNode = syntaxTree.append_child(currNode, "<constant-declarations-list>");

    if (currLexeme && currLexeme->code != BEGIN_CODE) {
        constantDeclarationProcess();
        if (errorFound) { return; currNode = syntaxTree.parent(currNode); }

        if (currLexeme && currLexeme->code == BEGIN_CODE) {
            currNode = syntaxTree.parent(currNode);
            return;
        }
        else {
            constantDeclarationsListProcess();
        }
    }
    else {
        syntaxTree.append_child(currNode, "<empty>");
    }
    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::constantDeclarationsProcess()
{
    currNode = syntaxTree.append_child(currNode, "<constant-declarations>");

    if (currLexeme && currLexeme->code == CONST_CODE) {
        syntaxTree.append_child(currNode, "CONST");
        getLexeme();
        constantDeclarationsListProcess();
    }
    else {
        syntaxTree.append_child(currNode, "<empty>");
    }
    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::declarationsProcess()
{
    currNode = syntaxTree.append_child(currNode, "<declarations>");
    constantDeclarationsProcess();
    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::statementsListProcess()
{
    currNode = syntaxTree.append_child(currNode, "<statements-list>");
    syntaxTree.append_child(currNode, "<empty>");
    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::blockProcess()
{
    currNode = syntaxTree.append_child(currNode, "<block>");
    declarationsProcess();
    if (errorFound) { return; currNode = syntaxTree.parent(currNode); }

    if (currLexeme && currLexeme->code == BEGIN_CODE) {
        syntaxTree.append_child(currNode, "BEGIN");
        getLexeme();
    }
    else {
        printError("Keyword 'BEGIN' expected");
    }
    if (errorFound) { return; currNode = syntaxTree.parent(currNode); }

    statementsListProcess();

    if (currLexeme && currLexeme->code == END_CODE) {
        syntaxTree.append_child(currNode, "END");
        getLexeme();
    }
    else {
        printError("Keyword 'END' expected");
    }
    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::identifierProcess()
{
    currNode = syntaxTree.append_child(currNode, "<identifier>");
    if (currLexeme && currLexeme->code >= IDENTIFIER_START_CODE) {
        syntaxTree.append_child(currNode, findKeyOfValue(identifiersTable, currLexeme->code));
        getLexeme();
    }
    else {
        printError("Unknown identifier " + currLexeme->name);
    }
    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::procedureIdentifierProcess()
{
    currNode = syntaxTree.append_child(currNode, "<procedure-identifier>");
    identifierProcess();
    currNode = syntaxTree.parent(currNode);
}

void SyntacticalAnalyzer::programProcess()
{
    currNode = syntaxTree.append_child(currNode, "<program>");
    getLexeme();
    if (currLexeme && currLexeme->code == PROGRAM_CODE) {
        syntaxTree.append_child(currNode, "PROGRAM");
        getLexeme();

        procedureIdentifierProcess();
        if (errorFound) { return; }

        if (currLexeme && currLexeme->code == ';') {
            syntaxTree.append_child(currNode, ";");
            getLexeme();
        }
        else {
            printError("Delimeter ';' expected");
        }
        if (errorFound) { return; }

        blockProcess();
        if (errorFound) { return; }

        if (currLexeme && currLexeme->code == '.') {
            syntaxTree.append_child(currNode, ".");
        }
        else {
            printError("Delimeter '.' expected");
        }
    }
    else {
        printError("Keyword 'PROGRAM' expected");
    }
}

void SyntacticalAnalyzer::printError(const std::string &cause)
{
    if (errorFound == false) {
        errorFound = true;
    }

    if (currLexeme) {
        std::cout << "Parser: Error (line " << currLexeme->line << ", column " <<
                     currLexeme->pos << "): " << cause << std::endl;
    }
    else {
        std::cout << "Parser: Error: " << cause << std::endl;
    }
}

bool SyntacticalAnalyzer::parseLexemesCodes()
{
    if (lexemesCodes.empty()) {
        std::cout << "Lexemes Codes is empty." << std::endl;
        return false;
    }
    std::cout << "-----------------------------------" << std::endl;
    currNode = syntaxTree.insert(root, "<signal-program>");
    programProcess();

    kptree::print_tree_bracketed(syntaxTree);
    std::cout << "-----------------------------------" << std::endl;
    return errorFound ? false : true;
}
