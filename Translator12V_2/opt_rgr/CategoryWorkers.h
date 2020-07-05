#pragma once
#include "TypesFmd.h"
#include <fstream>
using namespace std;
void workWithCategoryOne(Context&, ofstream&);
bool workWithCategoryTwo(Context&, ifstream&, ofstream&);
void workWithCategoryThree(Context&, ofstream&);
bool workWithCategoryFour(Context&, ifstream&, int&, ofstream&);
bool workWithCategoryFive(Context&, ifstream&, int&, ofstream&);
void workWithCategorySix(Context&, ofstream&);
//void workWithCategorySeven(Context&, ifstream&, int&, ofstream&);