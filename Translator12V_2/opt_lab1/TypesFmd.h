#pragma once
#include <string>
#include <vector>
using namespace std;

struct keywordStruct    
{
    string keywordName;
    int keywordNumber;
};


struct dm2Struct     
{
    string dm2Name;
    int dm2Number;
};


struct info              //information about the token
{
    string tokenName;
    int tokenNumber;
    int line;
    int column;
};

struct Context
{
    int symbolCategories[127];
    vector <keywordStruct> keywordVec; 
    int currentConstantNumber = 500;
    int currentIndentNumber = 1000;
    //int currentTestNumber = 2000;
    vector <dm2Struct> dm2Vec;  
    vector <info> result;  
    char characterToCheck;    
    string currentTokenName = "";
    int currentLine = 1;
    int currentColumn = 0;
    int currentCategory = 0;
    int dm1Id = 0;
};

