#ifndef INFORMATION_TABLES_H
# define INFORMATION_TABLES_H

# include <iostream>
# include <cstdint>
# include <map>
# include <vector>
# include "tree.hh"

typedef struct {
    uint32_t line;
    uint32_t pos;
    std::string name;
    uint32_t code;
} Lexeme;

enum Constants {
    PROGRAM_CODE = 401,
    BEGIN_CODE,
    END_CODE,
    CONST_CODE,
    COMPLEX_NUM_START_CODE = 501,
    IDENTIFIER_START_CODE = 1001
};

const std::map<std::string, uint32_t> keyWordsTable =
{ { "PROGRAM", PROGRAM_CODE }, { "BEGIN", BEGIN_CODE },
  { "END", END_CODE }, { "CONST", CONST_CODE } };

const std::map<std::string, uint32_t> separatorsTable =
{ { ";", ';' }, { ".", '.' }, { "=", '=' }, { "'", '\'' },
  { ",", ',' }, { "$", '$' }, { "(", '(' }, { ")", ')' } };

extern std::map<std::string, uint32_t> identifiersTable;

extern std::map<std::string, uint32_t> complexNumsTable;

extern std::vector<Lexeme> lexemesCodes;

extern tree<std::string> syntaxTree;

static uint32_t complexNumCode = COMPLEX_NUM_START_CODE;

static uint32_t identifierCode = IDENTIFIER_START_CODE;

#endif
