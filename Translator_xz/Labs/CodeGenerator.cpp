#include "CodeGenerator.h"
#include <iostream>
#include <algorithm>
#include <string>
#include "tree.hh"
#include "tree_util.hh"

CodeGenerator::CodeGenerator() :
    root(syntaxTree.begin())
{
}

CodeGenerator::~CodeGenerator()
{
    syntaxTree.clear();
}

Lexeme *findLexeme(std::string ident)
{
    uint8_t num = 0;
    for (size_t i = 0; i < lexemesCodes.size(); ++i) {
        if (lexemesCodes[i].name == ident) {
            if (num == 1) {
            return &lexemesCodes[i];
            }
            ++num;
        }
    }
    return nullptr;
}

void CodeGenerator::generateCode(tree<std::string>::iterator curr)
{
    if (curr == syntaxTree.end()) {
        return;
    }

    if (curr.node->data == "BEGIN") {
        std::cout << std::endl;
        std::cout << "data ENDS" << std::endl << std::endl;
        std::cout << "code SEGMENT" << std::endl;
        std::cout << "\tASSUME cs:code, ds:data" << std::endl;
        std::cout << "begin:" << std::endl;
        std::cout << "\tpush	ebp" << std::endl;
        std::cout << "\tmov	ebp, esp" << std::endl;
        std::cout << "\txor	eax, eax" << std::endl;
        std::cout << "\tmov	esp, ebp" << std::endl;
        std::cout << "\tpop	ebp" << std::endl;
    }
    else if (curr.node->data == "END") {
        std::cout << std::endl;
        std::cout << "\tret   0" << std::endl << std::endl;
        std::cout << "\tmov ax, 4c00h" << std::endl;
        std::cout << "\tint 21h" << std::endl;
        std::cout << "code ENDS" << std::endl;
        std::cout << "\tend begin" << std::endl;
    }
    else if (curr.node->data == "<declarations>") {
        std::cout << ";" << programIdentifier << std::endl << std::endl;
        std::cout << "data SEGMENT" << std::endl << std::endl;
    }
    else if (curr.node->data == "<constant-declaration>") {
        tree<std::string>::iterator tmp = curr;
        std::string identifier;
        tmp = syntaxTree.child(tmp, 0);
        if (tmp == syntaxTree.end()) { return; }
        tmp = syntaxTree.child(tmp, 0);
        if (tmp == syntaxTree.end()) { return; }
        tmp = syntaxTree.child(tmp, 0);
        if (tmp == syntaxTree.end()) { return; }
        identifier = tmp.node->data;
        if (std::find(constIdentifiers.begin(), constIdentifiers.end(), identifier) !=
                constIdentifiers.end()) {
            Lexeme *lex = findLexeme(identifier);
            if (!lex) { return; }
            std::cout << "Generator: Error (line " << lex->line << ", column " <<
                         lex->pos << "): '" << identifier << "' identifier already exist" << std::endl;
            std::cout << "\nError in Code Generator. File processing stopped." << std::endl;
            exit(-3);
        }
        else if (identifier == programIdentifier) {
            Lexeme *lex = findLexeme(identifier);
            if (!lex) { return; }
            std::cout << "Generator: Error (line " << lex->line << ", column " <<
                         lex->pos << "): '" << identifier << "' is program identifier, it can`t be used for constant declaration" << std::endl;
            std::cout << "\nError in Code Generator. File processing stopped." << std::endl;
            exit(-3);
        }
        constIdentifiers.push_back(identifier);
        std::cout << identifier << "   equ   ";
        tmp = curr;
        tmp = syntaxTree.child(tmp, 2);
        if (tmp == syntaxTree.end()) { return; }
        tmp = syntaxTree.child(tmp, 1);
        if (tmp == syntaxTree.end()) { return; }
        tmp = syntaxTree.child(tmp, 0);
        if (tmp == syntaxTree.end()) { return; }
        std::cout << tmp.node->data << std::endl;
    }

    for (int i = 0; i < syntaxTree.number_of_children(curr); ++i) {
        generateCode(syntaxTree.child(curr, i));
    }
}

void CodeGenerator::compile()
{
    if (syntaxTree.empty()) {
        std::cout << "Syntax Tree is empty." << std::endl;
        std::cout << "Error in Code Generator. File processing stopped." << std::endl;
        exit(-3);
    }

    tree<std::string>::iterator currNode = syntaxTree.sibling(root, 0);

    currNode = syntaxTree.child(currNode, 0);
    if (currNode == syntaxTree.end()) { return; }
    currNode = syntaxTree.child(currNode, 1);
    if (currNode == syntaxTree.end()) { return; }
    currNode = syntaxTree.child(currNode, 0);
    if (currNode == syntaxTree.end()) { return; }
    currNode = syntaxTree.child(currNode, 0);
    if (currNode == syntaxTree.end()) { return; }

    programIdentifier = currNode.node->data;

    std::cout << "===============CODE===============" << std::endl << std::endl;

    generateCode(syntaxTree.sibling(root, 0));

}
