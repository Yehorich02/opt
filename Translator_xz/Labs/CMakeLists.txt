cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
project(Labs)
add_executable(${PROJECT_NAME} "main.cpp" "InformationTables.h" "LexicalAnalyzer.cpp" "LexicalAnalyzer.h"
    "SyntacticalAnalyzer.cpp" "SyntacticalAnalyzer.h" "CodeGenerator.h" "CodeGenerator.cpp"
    "tree.hh" "tree_util.hh")
