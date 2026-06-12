%{
#include "parser.hpp"
%}

%option noyywrap
%option yylineno

DIGIT [0-9]
ID    [a-zA-Z_][a-zA-Z0-9_]*
WS    [ \t\n\r]

%%
{WS}+              { /* skip whitespace */ }
"if"               { return IF; }
"while"            { return WHILE; }
"output"           { return OUTPUT; }
"input"            { return INPUT; }

">="               { yylval.op = Ops::GreaterEq; return RELOP; }
"<="               { yylval.op = Ops::LessEq; return RELOP; }
"!="               { yylval.op = Ops::NotEqual; return RELOP; }
"=="               { yylval.op = Ops::Equal; return RELOP; }
">"                { yylval.op = Ops::Greater; return RELOP; }
"<"                { yylval.op = Ops::Less; return RELOP; }

"&&"               { return AND; }
"||"               { return OR; }
"!"                { return NOT; }

"="                { return ASG; }
"+"                { return PLUS; }
"-"                { return MINUS; }
"*"                { return MUL; }
"/"                { return DIV; }
"%"                { return MOD; }

"("                { return LPAR; }
")"                { return RPAR; }
"{"                { return LCB; }
"}"                { return RCB; }
";"                { return SM; }

{ID}               { yylval.name = yytext; return NAME; }
{DIGIT}+           { yylval.value = atoi(yytext); return VALUE; }

.                  { return ILLEGAL; }
%%
