#pragma once
#include <iostream>
#include <fstream>
#include <optional>
#include "Context.h"
#include "Utils.h"

void SignalProgram(Context&, int&);
void Program(Context&, int&, node*);
bool Block(Context&, int&, node*);
optional<bool> StatementList(Context&, int&, node*);
optional<bool> Statement(Context&, int&, node*);
bool Argument(Context&, int&, node*);
optional<bool> ArgumentList(Context&, int&, node*);
