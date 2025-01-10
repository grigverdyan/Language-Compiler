/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MNT_C_USERS_GRIGORI_DESKTOP_PROJECTS_LANGUAGE_COMPILER_LANGUAGE_COMPILER_BUILD_PARSER_HPP_INCLUDED
# define YY_YY_MNT_C_USERS_GRIGORI_DESKTOP_PROJECTS_LANGUAGE_COMPILER_LANGUAGE_COMPILER_BUILD_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    IDENTIFIER = 259,              /* IDENTIFIER  */
    LET = 260,                     /* LET  */
    IF = 261,                      /* IF  */
    ELIF = 262,                    /* ELIF  */
    ELSE = 263,                    /* ELSE  */
    WHILE = 264,                   /* WHILE  */
    RETURN = 265,                  /* RETURN  */
    DEF = 266,                     /* DEF  */
    INT_TYPE = 267,                /* INT_TYPE  */
    STRING_TYPE = 268,             /* STRING_TYPE  */
    BOOL_TYPE = 269,               /* BOOL_TYPE  */
    VOID_TYPE = 270,               /* VOID_TYPE  */
    PLUS = 271,                    /* PLUS  */
    MINUS = 272,                   /* MINUS  */
    MULTIPLY = 273,                /* MULTIPLY  */
    ASSIGN = 274,                  /* ASSIGN  */
    EQ = 275,                      /* EQ  */
    NEQ = 276,                     /* NEQ  */
    LT = 277,                      /* LT  */
    GT = 278,                      /* GT  */
    LE = 279,                      /* LE  */
    GE = 280,                      /* GE  */
    AND = 281,                     /* AND  */
    OR = 282,                      /* OR  */
    NOT = 283,                     /* NOT  */
    LPAREN = 284,                  /* LPAREN  */
    RPAREN = 285,                  /* RPAREN  */
    LBRACE = 286,                  /* LBRACE  */
    RBRACE = 287,                  /* RBRACE  */
    COLON = 288,                   /* COLON  */
    SEMICOLON = 289,               /* SEMICOLON  */
    COMMA = 290                    /* COMMA  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 14 "src/parser.y"

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

#line 112 "/mnt/c/Users/Grigori/Desktop/Projects/Language-Compiler/language_compiler/build/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MNT_C_USERS_GRIGORI_DESKTOP_PROJECTS_LANGUAGE_COMPILER_LANGUAGE_COMPILER_BUILD_PARSER_HPP_INCLUDED  */
