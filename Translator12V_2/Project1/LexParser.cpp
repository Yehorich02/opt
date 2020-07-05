#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include "Utils.h"
#include "CategoryWorkers.h"
#include "TypesFmd.h"
#include "LexParser.h"
#include "SynParser.h"

void initialize(Context& context)
{
	context.keywordVec.push_back({ "PROCEDURE", 401 });
	context.keywordVec.push_back({ "BEGIN", 402 });
	context.keywordVec.push_back({ "END", 403 });
	context.keywordVec.push_back({ "LABEL", 404 });
	context.keywordVec.push_back({ "GOTO", 405 });
	context.keywordVec.push_back({ "RETURN", 406 });

	context.dm2Vec.push_back({ "($", 301 });
	context.dm2Vec.push_back({ "$)", 302 });
}

void Parse(std::string_view inputPath, std::string_view outPath, string File)
{
	Context context{};
	createSymbolCategories(context);
	initialize(context);

	ifstream file(inputPath);
	ofstream out(outPath);

	headOfTable(out);
	while (file.get(context.characterToCheck))
	{
		int asciiOfCharacter = context.characterToCheck;        //ascii character code
		if (asciiOfCharacter == 10) {                           //if a new line has started
			context.currentColumn++;
			regularBufferCheck(context, out);                   //check the buffer of the current token
			context.currentLine++;                              //increment the line counter
			context.currentColumn = 0;                          //reset the character counter
		}
		else if (context.currentCategory != 5) { // if the read character is not inside the comment
			context.currentColumn++;             //increment the character counter
			switch (context.symbolCategories[asciiOfCharacter])  //check which category the read character corresponds to
			{
			case (0):    // if the read character is from the white space category
			{
				regularBufferCheck(context, out);  //check the buffer of the current token
				break;
			}
			case (1):  //if the read character is a digit
			{
				workWithCategoryOne(context, out);
				break;
			}
			case (2): //if the read character is an uppercase letter
			{
				if (!workWithCategoryTwo(context, file, out))
				{
					return;
				}
				break;
			}
			case (3):  //if the read character is an one-character delimiter
			{
				workWithCategoryThree(context, out);
				break;
			}
			case (4): //if the read character is a two-character delimiter
			{
				if (!workWithCategoryFour(context, file, asciiOfCharacter, out))
				{
					return;
				}
				break;
			}
			case (5):  //if the read character is a comment symbol
			{
				workWithCategoryFive(context, file, asciiOfCharacter, out);
				break;
			}
			case (6):  //if the read character is a invalid symbol
			{
				workWithCategorySix(context, out);
				return;
			}
			/*case (7): //if the read character is #
			{
				workWithCategorySeven(context, file, asciiOfCharacter, out);
				break;
			}*/
			default:
				out << " !error! ";
				break;
			}
		}
		else  // if the read character is inside the comment
		{
			context.currentColumn++;
		findCommentEnd:
			if (asciiOfCharacter == 42) { //if the read character is *
				file.get(context.characterToCheck); //read the next character
				context.currentColumn++;
				asciiOfCharacter = context.characterToCheck;
				if (asciiOfCharacter == 41) { //if the next character is ')', then the comment is closed
					context.currentCategory = 0;
				}
				else if (asciiOfCharacter == 42) {  //if the next character is '*', the comment is closed again
					goto findCommentEnd;
				}
			}
		}
	}
	context.currentColumn++;
	regularBufferCheck(context, out);   //checking the last character

	if (context.currentCategory == 5) { //checking for closed comment
		unclosedCom(context, out);
		return;
	}
	SynParser Pars(context, File);
}