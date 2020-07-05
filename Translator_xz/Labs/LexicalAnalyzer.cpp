#include "LexicalAnalyzer.h"
#include <cmath>

std::map<std::string, uint32_t> identifiersTable;
std::map<std::string, uint32_t> complexNumsTable;
std::vector<Lexeme> lexemesCodes;

LexicalAnalyser::LexicalAnalyser(const std::string &fileName) :
    countLines(1),
    countPosition(0),
    in(fileName),
    errorFound(false)
{
}

char LexicalAnalyser::getSymbol()
{
    ++countPosition;
    return in.get();
}

void LexicalAnalyser::addConstant(uint32_t position)
{
    if (complexNumsTable.find(buff) != complexNumsTable.end()) {
        Lexeme tmp = { countLines, position, buff, complexNumsTable[buff] };
        lexemesCodes.push_back(tmp);
    } else {
        complexNumsTable[buff] = complexNumCode;
        Lexeme tmp = { countLines, position, buff, complexNumCode };
        lexemesCodes.push_back(tmp);
        ++complexNumCode;
    }
}

void LexicalAnalyser::processIdentifier()
{
    buff.erase();
    buff += toupper(symbol);
    symbol = getSymbol();
    while (!in.eof() && isalnum(symbol)) {
        buff += toupper(symbol);
        symbol = getSymbol();
    }
    uint32_t correctPos = countPosition - buff.length();
    if (keyWordsTable.find(buff) != keyWordsTable.end()) {
        Lexeme tmp = { countLines, correctPos, buff, keyWordsTable.at(buff) };
        lexemesCodes.push_back(tmp);
    }
    else if (identifiersTable.find(buff) != identifiersTable.end()) {
        Lexeme tmp = { countLines, correctPos, buff, identifiersTable.at(buff) };
        lexemesCodes.push_back(tmp);
    }
    else {
        identifiersTable[buff] = identifierCode;
        Lexeme tmp = { countLines, correctPos, buff, identifierCode };
        lexemesCodes.push_back(tmp);
        ++identifierCode;
    }
}

void LexicalAnalyser::processDigits(uint32_t position)
{
    buff.erase();
    buff += symbol;
    symbol = getSymbol();
    while (!in.eof() && isdigit(symbol)) {
        buff += symbol;
        symbol = getSymbol();
    }
    if (symbol == '\'') {
        buff += ",0";
        addConstant(position);
    }
    else if (symbol == ',') {
        processComa(position);
    }
    else if (symbol == '$') {
        processEXPLexeme(position, buff);
    }
    else {
//        printError(std::string("Digits aren't insid+ std::ste quotation marks ' '") ring("'"));
//        buff += ",0";
        addConstant(position);
    }
}

void LexicalAnalyser::processComa(uint32_t position)
{
    bool isRealValueEmpty = buff.length() == 0;

    buff += symbol;
    symbol = getSymbol();
    if (!isdigit(symbol)) {
        printError("Expected digit after ','");
        return ;
    }
    while (!in.eof() && isdigit(symbol)) {
        buff += symbol;
        symbol = getSymbol();
    }
    if (isRealValueEmpty) {
        buff = "0" + buff;
    }
    addConstant(position);
}

void LexicalAnalyser::processEXPLexeme(uint32_t position, const std::string &r)
{
    symbol = getSymbol();
    if (symbol == 'E') {
        symbol = getSymbol();
        if (symbol == 'X') {
            symbol = getSymbol();
            if (symbol == 'P') {
                symbol = getSymbol();
                if (symbol == '(') {
                    symbol = getSymbol();
                    if (isdigit(symbol)) {
                        buff.erase();
                        while (!in.eof() && isdigit(symbol)) {
                            buff += symbol;
                            symbol = getSymbol();
                        }
                        if (symbol == ')') {
                            double absValue = std::stoi(r);
                            double argument = std::stoi(buff);
                            int32_t realValue = round(absValue * cos(argument));
                            int32_t imagValue = round(absValue * sin(argument));
                            buff.erase();

                            buff = std::to_string(realValue) + std::string(",") + std::to_string(imagValue);

                            addConstant(position);

                            symbol = getSymbol();
                            return ;
                        }
                        else {
                            printError(std::string("Expected ')' in $EXP() lexeme"));
                            return ;
                        }
                    }
                }
            }
        }
    }
    printError(std::string("Wrong symbol '") + std::string(1, symbol) + std::string("' in $EXP lexeme"));
}

void LexicalAnalyser::processComment()
{
    symbol = getSymbol();
    if (symbol == '*') {
        symbol = getSymbol();
        while (!in.eof()) {
            if (symbol == '*') {
                symbol = getSymbol();
                if (!in.eof() && symbol == ')') {
                    symbol = getSymbol();
                    break;
                }
            }
            else if (symbol == '\n') {
                ++countLines;
                countPosition = 0;
                symbol = getSymbol();
            }
            else {
                symbol = getSymbol();
            }
        }
        if (in.eof()) {
            printError("Incomplete comment");
        }
    }
    else {
        printError("After '(' is expected '*' for comment");
    }
}

void LexicalAnalyser::printComplexConstants()
{
    for (auto it = complexNumsTable.begin(); it != complexNumsTable.end(); ++it)
    {
        std::cout << it->first << " : " << it->second << std::endl;
    }
}

void LexicalAnalyser::printIdentifiersTable()
{
    for (auto it = identifiersTable.begin(); it != identifiersTable.end(); ++it)
    {
        std::cout << it->first << " : " << it->second << std::endl;
    }
}

void LexicalAnalyser::printLexemesTable()
{
    for (auto it = lexemesCodes.begin(); it != lexemesCodes.end(); ++it)
    {
        std::cout << it->line << ":" << it->pos << " " << it->name << " " << it->code << std::endl;
    }
}

void LexicalAnalyser::printError(const std::string &cause)
{
    if (!errorFound) {
        errorFound = true;
    }
    std::cout << "Lexer: Error (line " << countLines << ", column " <<
                 countPosition << "): " << cause << std::endl;
}

bool LexicalAnalyser::parseFile()
{
    if (!in.is_open()) {
        std::cout << "Error: Cannot open file!" << std::endl;
        return false;
    }

    symbol = getSymbol();
    if (in.eof()) {
        std::cout << "Error: File is empty!" << std::endl;
        return false;
    }

    do
    {
        buff.erase();
        if (symbol == '\n') {
            ++countLines;
            countPosition = 0;
            symbol = getSymbol();
        }
        else if (isspace(symbol)) {
            symbol = getSymbol();
        }
        else if (isalpha(symbol)) {
            processIdentifier();
        }
        else if (isdigit(symbol)) {
            processDigits(countPosition);
        }
        else if (symbol == ',') {
            processComa(countPosition);
        }
        else if (symbol == '$') {
            processEXPLexeme(countPosition);
        }
        else if (symbol == '(') {
            processComment();
        }
        else if (separatorsTable.find(std::string(1, symbol)) != separatorsTable.end()) {
            if (symbol == '\'' && lexemesCodes.size() > 1 &&
                                  lexemesCodes[lexemesCodes.size() - 1].code == '\'') {
                buff.clear();
                buff += "0,0";
                addConstant(countPosition);
            }
            Lexeme tmp = { countLines, countPosition, std::string(1, symbol), separatorsTable.at(std::string(1, symbol)) };
            lexemesCodes.push_back(tmp);
            symbol = getSymbol();
        }
        else {
            printError(std::string("Illegal symbol '") + std::string(1, symbol) + std::string("'"));
            symbol = getSymbol();
        }
    } while (!in.eof());

    in.close();
    printResultTables();

    return errorFound ? false : true;
}

void LexicalAnalyser::printResultTables()
{
    std::cout << "----------Constants Table----------" << std::endl;
    printComplexConstants();
    std::cout << "--------Identifiers Table-------" << std::endl;
    printIdentifiersTable();
    std::cout << "--------Lexemes Codes Table--------" << std::endl;
    printLexemesTable();
    std::cout << "-----------------------------------" << std::endl;
}
