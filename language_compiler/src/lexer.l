%{
#include "ast.hpp"
#include "parser.hpp"
#include <iostream>
#include <vector>
#include <string>
%}

%option noyywrap

%%
"def"               { return DEF; }
"let"               { return LET; }
"if"                { return IF; }
"elif"              { return ELIF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"return"            { return RETURN; }
"int"               { return TYPE_INT; }
"string"            { return TYPE_STRING; }
"bool"              { return TYPE_BOOL; }
"void"              { return TYPE_VOID; }

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
":"                 { return COLON; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }
"="                 { return EQUAL; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return TIMES; }
"=="                { return EQ; }
"!="                { return NEQ; }
"<"                 { return LT; }
">"                 { return GT; }
"<="                { return LTE; }
">="                { return GTE; }

[a-zA-Z_][a-zA-Z0-9_]*  { yylval.sval = strdup(yytext); return IDENTIFIER; }
[0-9]+              { yylval.ival = atoi(yytext); return NUMBER; }

[ \t]+              ;
\r?\n               ;
"#"([^\n]*)         ;
.                   { std::cerr << "Invalid character: " << yytext << std::endl; }

%%