%{
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "ast.hpp"

extern int yylex();
void yyerror(const char *s);
Program* program;
extern FILE* yyin;
%}

%union {
    int intVal;
    char* strVal;
    ASTNode* node;
    std::vector<Statement*>* stmtList;
    Statement* stmt;
    Expression* expr;
    std::vector<Expression*>* exprList;
    std::string* str;
    std::vector<std::pair<std::string, std::string>>* paramList;
    std::pair<std::string, std::string>* param;
}

%token <intVal> NUMBER
%token <strVal> IDENTIFIER
%token LET IF ELIF ELSE WHILE RETURN DEF
%token INT_TYPE STRING_TYPE BOOL_TYPE VOID_TYPE
%token PLUS MINUS MULTIPLY ASSIGN EQ NEQ LT GT LE GE AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE COLON SEMICOLON COMMA

%type <stmtList> statement_list
%type <stmt> statement
%type <expr> expression term function_call
%type <exprList> argument_list
%type <str> type
%type <paramList> parameter_list
%type <param> parameter

%debug

%%

program:
    statement_list { program = new Program(); program->statements = *$1; delete $1; }
    ;

statement_list:
    statement { $$ = new std::vector<Statement*>(); $$->push_back($1); }
    | statement_list statement { $1->push_back($2); $$ = $1; }
    ;

statement:
    LET IDENTIFIER COLON type ASSIGN expression SEMICOLON { $$ = new LetStatement(); ((LetStatement*)$$)->identifier = $2; ((LetStatement*)$$)->type = *$4; ((LetStatement*)$$)->value = $6; delete $4; }
    | IF LPAREN expression RPAREN statement { $$ = new IfStatement(); ((IfStatement*)$$)->condition = $3; ((IfStatement*)$$)->consequence = $5; }
    | WHILE LPAREN expression RPAREN statement { $$ = new WhileStatement(); ((WhileStatement*)$$)->condition = $3; ((WhileStatement*)$$)->body = $5; }
    | RETURN expression SEMICOLON { $$ = new ReturnStatement(); ((ReturnStatement*)$$)->value = $2; }
    | function_call SEMICOLON { $$ = (Statement*)$1; }
    | LBRACE statement_list RBRACE { $$ = new BlockStatement(); ((BlockStatement*)$$)->statements = *$2; delete $2; }
    | DEF IDENTIFIER LPAREN parameter_list RPAREN COLON type LBRACE statement_list RBRACE { $$ = new FunctionDefinition(); ((FunctionDefinition*)$$)->name = $2; ((FunctionDefinition*)$$)->parameters = *$4; ((FunctionDefinition*)$$)->returnType = *$7; ((FunctionDefinition*)$$)->body = *$9; delete $4; delete $7; delete $9; }
    ;

type:
    INT_TYPE { $$ = new std::string("int"); }
    | STRING_TYPE { $$ = new std::string("string"); }
    | BOOL_TYPE { $$ = new std::string("bool"); }
    | VOID_TYPE { $$ = new std::string("void"); }
    ;

expression:
    term { $$ = $1; }
    | expression PLUS term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = "+"; ((BinaryExpression*)$$)->right = $3; }
    | expression MINUS term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = "-"; ((BinaryExpression*)$$)->right = $3; }
    | expression MULTIPLY term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = "*"; ((BinaryExpression*)$$)->right = $3; }
    | expression EQ term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = "=="; ((BinaryExpression*)$$)->right = $3; }
    | expression NEQ term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = "!="; ((BinaryExpression*)$$)->right = $3; }
    | expression LT term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = "<"; ((BinaryExpression*)$$)->right = $3; }
    | expression GT term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = ">"; ((BinaryExpression*)$$)->right = $3; }
    | expression LE term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = "<="; ((BinaryExpression*)$$)->right = $3; }
    | expression GE term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = ">="; ((BinaryExpression*)$$)->right = $3; }
    | expression AND term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = "&&"; ((BinaryExpression*)$$)->right = $3; }
    | expression OR term { $$ = new BinaryExpression(); ((BinaryExpression*)$$)->left = $1; ((BinaryExpression*)$$)->op = "||"; ((BinaryExpression*)$$)->right = $3; }
    ;

term:
    IDENTIFIER { $$ = new Identifier(); ((Identifier*)$$)->name = $1; }
    | NUMBER { $$ = new Number(); ((Number*)$$)->value = $1; }
    | function_call { $$ = $1; }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

function_call:
    IDENTIFIER LPAREN argument_list RPAREN { $$ = new FunctionCall(); ((FunctionCall*)$$)->identifier = $1; ((FunctionCall*)$$)->arguments = *$3; delete $3; }
    ;

argument_list:
    /* empty */ { $$ = new std::vector<Expression*>(); }
    | expression { $$ = new std::vector<Expression*>(); $$->push_back($1); }
    | argument_list COMMA expression { $1->push_back($3); $$ = $1; }
    ;

parameter_list:
    /* empty */ { $$ = new std::vector<std::pair<std::string, std::string>>(); }
    | parameter { $$ = new std::vector<std::pair<std::string, std::string>>(); $$->push_back(*$1); delete $1; }
    | parameter_list COMMA parameter { $1->push_back(*$3); $$ = $1; delete $3; }
    ;

parameter:
    IDENTIFIER COLON type { $$ = new std::pair<std::string, std::string>($1, *$3); delete $3; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}