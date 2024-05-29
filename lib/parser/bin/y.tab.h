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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "config/rules.y"

    #include "../src/token_schema.hpp"
    int yylex(void);
    void yyerror(inst_t **, char *);

#line 55 "y.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    B = 258,                       /* B  */
    BEQ = 259,                     /* BEQ  */
    BCS = 260,                     /* BCS  */
    MOV = 261,                     /* MOV  */
    ADD = 262,                     /* ADD  */
    SUB = 263,                     /* SUB  */
    MUL = 264,                     /* MUL  */
    MULU = 265,                    /* MULU  */
    DIV = 266,                     /* DIV  */
    MOD = 267,                     /* MOD  */
    AND = 268,                     /* AND  */
    OR = 269,                      /* OR  */
    EOR = 270,                     /* EOR  */
    NOT = 271,                     /* NOT  */
    LSL = 272,                     /* LSL  */
    LSR = 273,                     /* LSR  */
    ASR = 274,                     /* ASR  */
    ROL = 275,                     /* ROL  */
    ROR = 276,                     /* ROR  */
    CMP = 277,                     /* CMP  */
    NOP = 278,                     /* NOP  */
    RTI = 279,                     /* RTI  */
    HALT = 280,                    /* HALT  */
    FLAGS = 281,                   /* FLAGS  */
    LDR = 282,                     /* LDR  */
    STR = 283,                     /* STR  */
    T_STR = 284,                   /* T_STR  */
    T_REG = 285,                   /* T_REG  */
    T_IMM = 286                    /* T_IMM  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define B 258
#define BEQ 259
#define BCS 260
#define MOV 261
#define ADD 262
#define SUB 263
#define MUL 264
#define MULU 265
#define DIV 266
#define MOD 267
#define AND 268
#define OR 269
#define EOR 270
#define NOT 271
#define LSL 272
#define LSR 273
#define ASR 274
#define ROL 275
#define ROR 276
#define CMP 277
#define NOP 278
#define RTI 279
#define HALT 280
#define FLAGS 281
#define LDR 282
#define STR 283
#define T_STR 284
#define T_REG 285
#define T_IMM 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 7 "config/rules.y"

    token_t t_value;
    inst_t *inst_head;

#line 142 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (inst_t **inst_head);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
