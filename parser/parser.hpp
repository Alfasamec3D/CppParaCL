#pragma once

#include "INode.hpp"

#include <cstdarg>
#include <cstdio>
#include <string>

// YYSTYPE structure for passing values between lexer and parser
struct yyRet {
  std::string name;
  int value = 0;
  INode* treeNode = nullptr;
  Ops op = Ops::Plus;
};

#define YYSTYPE yyRet

#include "compiler.hpp"

extern FILE* yyin;
extern int yylineno;

int yyparse();
int yylex();
int yyerror(char const* s);
void PrintError(char const* fmt, ...);
