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

#ifndef YY_YY_SRC_PARSER_PARSER_TAB_HH_INCLUDED
# define YY_YY_SRC_PARSER_PARSER_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
    ADD = 258,                     /* "+"  */
    SUB = 259,                     /* "-"  */
    MUL = 260,                     /* "*"  */
    DIV = 261,                     /* "/"  */
    MOD = 262,                     /* "%"  */
    ASSIGN = 263,                  /* "="  */
    SEMICOLON = 264,               /* ";"  */
    COMMA = 265,                   /* ","  */
    LPAREN = 266,                  /* "("  */
    RPAREN = 267,                  /* ")"  */
    LBRACE = 268,                  /* "{"  */
    RBRACE = 269,                  /* "}"  */
    LBRACK = 270,                  /* "["  */
    RBRACK = 271,                  /* "]"  */
    INT = 272,                     /* "int"  */
    VOID = 273,                    /* "void"  */
    IF = 274,                      /* "if"  */
    ELSE = 275,                    /* "else"  */
    WHILE = 276,                   /* "while"  */
    RETURN = 277,                  /* "return"  */
    LT = 278,                      /* "<"  */
    GT = 279,                      /* ">"  */
    LE = 280,                      /* "<="  */
    GE = 281,                      /* ">="  */
    EQ = 282,                      /* "=="  */
    NE = 283,                      /* "!="  */
    LAND = 284,                    /* "&&"  */
    LOR = 285,                     /* "||"  */
    NOT = 286,                     /* "!"  */
    IDENT = 287,                   /* IDENT  */
    INTCONST = 288,                /* INTCONST  */
    LOWER_THAN_ELSE = 289          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 37 "src/parser/parser.y"

    int int_val;
    char *str_val;
    BasicType btype;
    UnaryOp op;
    AST::Node *node;

#line 106 "src/parser/parser.tab.hh"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_PARSER_PARSER_TAB_HH_INCLUDED  */
