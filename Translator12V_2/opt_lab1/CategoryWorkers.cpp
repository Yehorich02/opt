#include "CategoryWorkers.h"
#include "Utils.h"

//-----digits category------//
void workWithCategoryOne(Context& context, ofstream& out)
{
    if (context.currentCategory == 3) {  ///if the buffer is a delimiter, then write it to the token information
        currentTokenIsDm1(context, out);
        context.currentTokenName = "";
    }
    else if (context.currentCategory == 4) {  ///if the buffer is a delimiter, then write it to the token information
        currentTokenIsDm2(context, out);
    }
    context.currentTokenName.push_back(context.characterToCheck);   //add the carrent symbol to the buffer
    if (context.currentCategory != 2) context.currentCategory = 1;  //if the current category is not an identifier, then it is a constant
}

//-----uppercase letter category------//
bool workWithCategoryTwo(Context& context, ifstream& file, ofstream& out)
{
    if (context.currentCategory == 1) {    //ID cannot begin with a number
        currentTokenIsConstant(context, out);  //write the token constant
        context.currentTokenName.push_back(context.characterToCheck);  //add the carrent symbol to the buffer
        while (file.get(context.characterToCheck) && (context.symbolCategories[(int)context.characterToCheck] == 1 || context.symbolCategories[(int)context.characterToCheck] == 2))
        {
            context.currentTokenName.push_back(context.characterToCheck);   //write down the entire ID
        }
        invalidId(context, out);  //output error message
        return false;
    }
    else if (context.currentCategory == 3) {   //if the buffer is a delimiter, then write it to the token information
        currentTokenIsDm1(context, out);
    }
    else if (context.currentCategory == 4) {   //if the buffer is a delimiter, then write it to the token information
        currentTokenIsDm2(context, out);
    }
    context.currentTokenName.push_back(context.characterToCheck);   //add the carrent symbol to the buffer
    context.currentCategory = 2;  //set the category of IDs and keywords
    return true;
}

//-----single-character delimiter category------//
void workWithCategoryThree(Context& context, ofstream& out)
{
    regularBufferCheck(context, out);
    context.currentTokenName.push_back(context.characterToCheck);
    context.currentCategory = 3;
    context.dm1Id = (int)context.characterToCheck;
}

//-----two-character delimiter category------//
bool workWithCategoryFour(Context& context, ifstream& file, int& asciiOfCharacter, ofstream& out)
{
    regularBufferCheck(context, out);    //check the buffer of the current token
    context.currentTokenName.push_back(context.characterToCheck);  //add the symbol '$' to the buffer 
    file.get(context.characterToCheck);   //read a new character
    context.currentColumn++;
    asciiOfCharacter = context.characterToCheck;
    if (asciiOfCharacter != 41) {   //if the read symbol is not a ')'
        invalidCharacter(context, out);
        return false;
    }
    else if (asciiOfCharacter == 41) {   //if the read symbol is ')'
        context.currentTokenName.push_back(context.characterToCheck); //add the symbol ')' to the buffer 
        context.currentCategory = 4;   //set the category of two-character delimiter
    }
    return true;
}

//-----comment category------//
bool workWithCategoryFive(Context& context, ifstream& file, int& asciiOfCharacter, ofstream& out)
{
    regularBufferCheck(context, out);   //check the buffer of the current token
    if (asciiOfCharacter == 42) {  //if the read symbol is a *, then this is an incorrect symbol
        invalidCharacter(context, out);
        return 0;
    }
    else if (asciiOfCharacter == 41) {   // if the read symbol is a ')'
        context.currentTokenName.push_back(context.characterToCheck);
        context.dm1Id = (int)context.characterToCheck;
        context.currentCategory = 3;
    }
    else if (asciiOfCharacter == 40) {   // if the read symbol is a '('
    findCommentStart:
        context.currentTokenName.push_back(context.characterToCheck);  //add a symbol to the buffer
        context.dm1Id = (int)context.characterToCheck;   
        context.currentCategory = 3;   //set the category of single character delimiter, until another one turns out 
        file.get(context.characterToCheck);   //read a new character
        context.currentColumn++;
        asciiOfCharacter = context.characterToCheck;
        if (context.symbolCategories[asciiOfCharacter] == 6) {  //an incorrect character
            regularBufferCheck(context, out);  //check the buffer of the current token
            context.currentTokenName.push_back(context.characterToCheck);  //write an incorrect character to the buffer
            invalidCharacter(context, out);
            return 0;
        }
        else if (asciiOfCharacter == 36) {   // if the read symbol is a '$'
            context.currentTokenName.push_back(context.characterToCheck);  //add a symbol to the buffer
            context.currentCategory = 4;    //set the category of two-character delimiter
        }
        else if (asciiOfCharacter == 42) { // if the read symbol is a '*'
            context.currentTokenName = "";   //reset the buffer
            context.currentCategory = 5;  //set the comment category
        }
        else if (asciiOfCharacter == 40) { // if the read symbol is a'('
            currentTokenIsDm1(context, out);  //write the token '(' as a single character delimiter
            goto findCommentStart;   //continue to look for comment opening
        }
    }
}

//-----invalid character category------//
void workWithCategorySix(Context& context, ofstream& out)
{
    regularBufferCheck(context, out);  //check the buffer of the current token
    context.currentTokenName.push_back(context.characterToCheck);  //write an incorrect character to the buffer
    invalidCharacter(context, out);    //error message
}

/*void workWithCategorySeven(Context& context, ifstream& file, int& asciiOfCharacter, ofstream& out) {
    regularBufferCheck(context, out);    //check the buffer of the current token
    context.currentTokenName.push_back(context.characterToCheck);  //add the symbol '#' to the buffer 
    file.get(context.characterToCheck);   //read a new character
    context.currentColumn++;
    asciiOfCharacter = context.characterToCheck;
    if (context.symbolCategories[asciiOfCharacter] != 2) { mustBeAnLetter(context, out); }
    else {
        while (context.symbolCategories[asciiOfCharacter] == 2)
        {
            context.currentTokenName.push_back(context.characterToCheck); //adding letter
            file.get(context.characterToCheck);   //read a new character
            context.currentColumn++;
            asciiOfCharacter = context.characterToCheck;
        }
        if (asciiOfCharacter != 43) { mustBePlus(context, out); }
        else {
            context.currentTokenName.push_back(context.characterToCheck); //adding letter
            file.get(context.characterToCheck);   //read a new character
            context.currentColumn++;
            asciiOfCharacter = context.characterToCheck;
            if (context.symbolCategories[asciiOfCharacter] != 1) { mustBeDig(context, out); }
            while (context.symbolCategories[asciiOfCharacter] == 1)
            {
                context.currentTokenName.push_back(context.characterToCheck); //adding digit
                file.get(context.characterToCheck);   //read a new character
                context.currentColumn++;
                asciiOfCharacter = context.characterToCheck;
            }
            context.currentCategory = 7;
        }
    }
}
*/