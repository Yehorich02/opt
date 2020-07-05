#ifndef CODE_GENERATOR_H
# define CODE_GENERATOR_H

#include "InformationTables.h"
#include "tree.hh"

class CodeGenerator
{
    tree<std::string>::iterator root;
    std::string programIdentifier;
    std::vector<std::string> constIdentifiers;
    private:
        void generateCode(tree<std::string>::iterator curr);
    public:
        CodeGenerator();
        ~CodeGenerator();

        void compile();
};

#endif
