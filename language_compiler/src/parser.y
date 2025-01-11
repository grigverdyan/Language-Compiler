%{
#include "ast.hpp"
#include "semantic.hpp"
#include "codegen.hpp"

#include <iostream>
#include <vector>
#include <memory>

extern int yylex();
extern int yyparse();
extern FILE *yyin;
void yyerror(const char *s);

// Forward declaration of global variables
extern std::shared_ptr<ProgramNode> program;
extern SymbolTable globalSymbolTable;
extern CodeGenerator codeGenerator;

BinaryOperator tokenToBinaryOp(int token);
Type tokenToType(int token);

%}

%union {
    int ival;
    char *sval;
    ASTNode *node;
    Type type;
    std::vector<ASTNode*> *nodeList;
    std::vector<std::pair<std::string, Type>> *paramList;
    std::string *id;
}

%token DEF LET IF ELIF ELSE WHILE RETURN
%token TYPE_INT TYPE_STRING TYPE_BOOL TYPE_VOID
%token LPAREN RPAREN LBRACE RBRACE COLON SEMICOLON COMMA EQUAL
%token PLUS MINUS TIMES
%token EQ NEQ LT GT LTE GTE
%token <ival> NUMBER
%token <sval> IDENTIFIER
%type <node> Program Subprogram Body Statement Variable_Declaration Assignment Conditional_Statement Loop_While_Statement Return_Statement Expression Simple_Expression Term Function_Call
%type <type> Type
%type <nodeList> StatementList
%type <paramList> Parameter_List ParamList
%type <id> Identifier
%type <nodeList> ArgList
%type <node> Conditional_Statement_Rest

%%
Program:
    Subprogram { program = std::make_unique<ProgramNode>($<node>1); $$ = program.get();program->subprograms.push_back(std::unique_ptr<ASTNode>($<node>1)); }
    | Program Subprogram  { program->subprograms.push_back(std::unique_ptr<ASTNode>($<node>2)); }
    ;

Subprogram:
    DEF Identifier Parameter_List COLON Type Body { $$ = new FunctionDefNode(*$<id>2, *$<paramList>3, $<type>5, $<node>6); delete $<id>2; delete $<paramList>3; }
    ;

Parameter_List:
    LPAREN RPAREN { $$ = new std::vector<std::pair<std::string, Type>>(); }
    | LPAREN Identifier COLON Type { $$ = new std::vector<std::pair<std::string, Type>>(); $$->push_back({*$<id>2, $<type>4}); delete $<id>2; }
     | LPAREN Identifier COLON Type COMMA ParamList {  $$ = new std::vector<std::pair<std::string, Type>>(); $$->push_back({*$<id>2, $<type>4}); $$->insert($$->end(), $<paramList>6->begin(), $<paramList>6->end()); delete $<id>2; delete $<paramList>6; }
    | LPAREN  Identifier COLON Type RPAREN { $$ = new std::vector<std::pair<std::string, Type>>(); $$->push_back({*$<id>2,$<type>4}); delete $<id>2; }
    | LPAREN Identifier COLON Type COMMA ParamList RPAREN { $$ = new std::vector<std::pair<std::string, Type>>(); $$->push_back({*$<id>2, $<type>4}); $$->insert($$->end(), $<paramList>6->begin(), $<paramList>6->end()); delete $<id>2; delete $<paramList>6; }
    ;

ParamList:
    Identifier COLON Type { $$ = new std::vector<std::pair<std::string, Type>>(); $$->push_back({*$<id>1, $<type>3}); delete $<id>1;}
    | Identifier COLON Type COMMA ParamList { $$ = new std::vector<std::pair<std::string, Type>>(); $$->push_back({*$<id>1, $<type>3}); $$->insert($$->end(), $<paramList>4->begin(), $<paramList>4->end()); delete $<id>1; delete $<paramList>4; }

Body:
    LBRACE StatementList RBRACE { $$ = new BlockNode(*$<nodeList>2); delete $<nodeList>2; }
    ;
StatementList:
    { $$ = new std::vector<ASTNode*>(); }
    | StatementList Statement { $$->push_back($<node>2); }
;
Statement:
    Variable_Declaration { $$ = $<node>1; }
    | Assignment  { $$ = $<node>1; }
    | Conditional_Statement { $$ = $<node>1; }
    | Loop_While_Statement { $$ = $<node>1; }
    | Expression SEMICOLON { $$ = $<node>1; }
    | Return_Statement { $$ = $<node>1; }
;
Variable_Declaration:
    LET Identifier COLON Type SEMICOLON { $$ = new LetStatementNode(*$<id>2, $<type>4); delete $<id>2;}
    | LET Identifier COLON Type EQUAL Expression SEMICOLON { $$ = new LetStatementNode(*$<id>2, $<type>4, $<node>6); delete $<id>2; }
    ;

Assignment:
    Identifier EQUAL Expression SEMICOLON { $$ = new AssignStatementNode(*$<id>1, $<node>3); delete $<id>1; }
    ;

Conditional_Statement:
    IF LPAREN Expression RPAREN Body { $$ = new IfStatementNode($<node>3, $<node>5); }
    | IF LPAREN Expression RPAREN Body Conditional_Statement_Rest {
         IfStatementNode* ifNode = new IfStatementNode($<node>3, $<node>5);
         ifNode->elifClauses = dynamic_cast<IfStatementNode*>($<node>6)->elifClauses;
         ifNode->elseClause = dynamic_cast<IfStatementNode*>($<node>6)->elseClause;
        $$ = ifNode;
    }

Conditional_Statement_Rest:
    ELIF LPAREN Expression RPAREN Body Conditional_Statement_Rest {
         IfStatementNode* ifNode = new IfStatementNode($<node>3, $<node>5);
         ifNode->elifClauses = dynamic_cast<IfStatementNode*>($<node>6)->elifClauses;
        ifNode->elseClause = dynamic_cast<IfStatementNode*>($<node>6)->elseClause;
         $$ = ifNode;
        }
  | ELIF LPAREN Expression RPAREN Body {
      $$ = new IfStatementNode($<node>3, $<node>5);
      }
  | ELSE Body {
      IfStatementNode* ifNode = new IfStatementNode(nullptr, $<node>2);
      ifNode->elseClause = $<node>2;
      $$ = ifNode;
      }
;

Loop_While_Statement:
    WHILE LPAREN Expression RPAREN Body { $$ = new WhileStatementNode($<node>3, $<node>5); }
    ;

Return_Statement:
    RETURN Expression SEMICOLON { $$ = new ReturnStatementNode($<node>2); }
    ;

Expression:
    Simple_Expression { $$ = $<node>1; }
    | Simple_Expression EQ Simple_Expression { $$ = new BinaryExpressionNode($<node>1, tokenToBinaryOp(EQ), $<node>3); }
    | Simple_Expression NEQ Simple_Expression { $$ = new BinaryExpressionNode($<node>1, tokenToBinaryOp(NEQ), $<node>3); }
    | Simple_Expression LT Simple_Expression { $$ = new BinaryExpressionNode($<node>1, tokenToBinaryOp(LT), $<node>3); }
    | Simple_Expression GT Simple_Expression { $$ = new BinaryExpressionNode($<node>1, tokenToBinaryOp(GT), $<node>3); }
    | Simple_Expression LTE Simple_Expression { $$ = new BinaryExpressionNode($<node>1, tokenToBinaryOp(LTE), $<node>3); }
    | Simple_Expression GTE Simple_Expression { $$ = new BinaryExpressionNode($<node>1, tokenToBinaryOp(GTE), $<node>3); }
    ;

Simple_Expression:
    Term { $$ = $<node>1; }
    | Simple_Expression PLUS Term { $$ = new BinaryExpressionNode($<node>1, tokenToBinaryOp(PLUS), $<node>3); }
    | Simple_Expression MINUS Term { $$ = new BinaryExpressionNode($<node>1, tokenToBinaryOp(MINUS), $<node>3); }
    | Simple_Expression TIMES Term { $$ = new BinaryExpressionNode($<node>1, tokenToBinaryOp(TIMES), $<node>3); }
    ;

Term:
    Identifier { $$ = new IdentifierNode(*$<id>1); delete $<id>1;}
    | NUMBER { $$ = new NumberNode($<ival>1); }
    | Function_Call { $$ = $<node>1; }
    | LPAREN Expression RPAREN { $$ = $<node>2; }
    ;

Function_Call:
    Identifier LPAREN RPAREN { $$ = new FunctionCallNode(*$<id>1, std::vector<ASTNode*>()); delete $<id>1;}
    | Identifier LPAREN ArgList RPAREN { $$ = new FunctionCallNode(*$<id>1, *$<nodeList>3); delete $<id>1; delete $<nodeList>3; }
    ;

ArgList:
    Expression { $$ = new std::vector<ASTNode*>(); $$->push_back($<node>1); }
    | Expression COMMA ArgList { $$ = new std::vector<ASTNode*>(); $$->push_back($<node>1); $$->insert($$->end(), $<nodeList>3->begin(), $<nodeList>3->end()); delete $<nodeList>3; }
    ;

Type:
    TYPE_INT { $$ = tokenToType(TYPE_INT); }
    | TYPE_STRING { $$ = tokenToType(TYPE_STRING); }
    | TYPE_BOOL { $$ = tokenToType(TYPE_BOOL); }
    | TYPE_VOID { $$ = tokenToType(TYPE_VOID); }
    ;
Identifier:
    IDENTIFIER { $$ = new std::string($<sval>1); free($<sval>1); }
;

%%
BinaryOperator tokenToBinaryOp(int token) {
    switch (token) {
        case PLUS: return BinaryOperator::PLUS;
        case MINUS: return BinaryOperator::MINUS;
        case TIMES: return BinaryOperator::TIMES;
        case EQ: return BinaryOperator::EQ;
        case NEQ: return BinaryOperator::NEQ;
        case LT: return BinaryOperator::LT;
        case GT: return BinaryOperator::GT;
        case LTE: return BinaryOperator::LTE;
        case GTE: return BinaryOperator::GTE;
        default:
            std::cerr << "Error: Invalid binary operator token: " << token << std::endl;
             exit(1);
    }
}
Type tokenToType(int token) {
    switch (token) {
        case TYPE_INT: return Type::TYPE_INT;
        case TYPE_STRING: return Type::TYPE_STRING;
        case TYPE_BOOL: return Type::TYPE_BOOL;
        case TYPE_VOID: return Type::TYPE_VOID;
          default:
            std::cerr << "Error: Invalid type token: " << token << std::endl;
            exit(1);
    }
}

void yyerror(const char *s) {
    std::cerr << "Parse error: " << s << std::endl;
    exit(1);
}