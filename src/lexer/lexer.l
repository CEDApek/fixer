%option noinput
%option nounput
%option noyywrap
%option yylineno

%{
#include "ast/tree.hpp"
#include "parser/parser.tab.hh"
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
extern FILE *input;
void yyerror(const char *s);
%}

digit       [0-9]
nzdigit     [1-9]
blank       [ \t\n\r]+
letter      [_a-zA-Z]
identifier  {letter}({letter}|{digit})*
decconst    0|{nzdigit}{digit}*
octconst    0[0-7]+
hexconst    0[xX][0-9A-Fa-f]+
linecomment \/\/[^\n]*
comment     \/\*([^*]|\*[^/])*\*\/

%%

"=="            { return EQ; }
"!="            { return NE; }
"<="            { return LE; }
">="            { return GE; }
"&&"            { return LAND; }
"||"            { return LOR; }

"0"             { yylval.int_val = 0; return INTCONST; }
[1-9]{digit}*   { yylval.int_val = atoi(yytext); return INTCONST; }
"+"             { return ADD; }
"-"             { return SUB; }
"*"             { return MUL; }
"/"             { return DIV; }
"%"             { return MOD; }
"!"             { return NOT; }
"<"             { return LT; }
">"             { return GT; }
"="             { return ASSIGN; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"["             { return LBRACK; }
"]"             { return RBRACK; }
"="             { return ASSIGN; }
","             { return COMMA; }
";"             { return SEMICOLON; }
"void"          { return VOID; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"return"        { return RETURN; }
"int"           { return INT; }

{hexconst}      { yylval.int_val = (int)strtol(yytext, nullptr, 16); return INTCONST; }
{octconst}      { yylval.int_val = (int)strtol(yytext, nullptr, 8);  return INTCONST; }
{decconst}      { yylval.int_val = atoi(yytext); return INTCONST; }

{identifier}    { yylval.str_val = strdup(yytext); return IDENT; }
{linecomment}   { }
{comment}       { }
{blank}         { }
.               { std::string msg = "unknown character '" + std::string(yytext) + "'"; yyerror(msg.c_str()); exit(1); }

%%

