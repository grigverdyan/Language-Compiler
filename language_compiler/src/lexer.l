%{
#include <string>
#include <vector>
#include "ast.hpp"
#include "parser.hpp"
%}

%option noyywrap
%option debug

%%

"let"           { return LET; }
"if"            { return IF; }
"elif"          { return ELIF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"return"        { return RETURN; }
"int"           { return INT_TYPE; }
"string"        { return STRING_TYPE; }
"bool"          { return BOOL_TYPE; }
"void"          { return VOID_TYPE; }

[0-9]+          { yylval.intVal = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.strVal = strdup(yytext); return IDENTIFIER; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULTIPLY; }
"="             { return ASSIGN; }
"=="            { return EQ; }
"!="            { return NEQ; }
"<"             { return LT; }
">"             { return GT; }
"<="            { return LE; }
">="            { return GE; }
"&&"            { return AND; }
"||"            { return OR; }
"!"             { return NOT; }

"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
":"             { return COLON; }
";"             { return SEMICOLON; }
","             { return COMMA; }

[ \t\n]+        { /* skip whitespace */ }
"#".*           { /* skip comments */ }

.               { return yytext[0]; }

%%

int mywrap() {
    return 1;
}
