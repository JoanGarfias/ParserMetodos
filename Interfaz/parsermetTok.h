
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMERO = 258,
     X = 259,
     EULER = 260,
     PI = 261,
     SIN = 262,
     COS = 263,
     TAN = 264,
     SEC = 265,
     COSEC = 266,
     COT = 267,
     ASIN = 268,
     ACOS = 269,
     ATAN = 270,
     SINH = 271,
     COSH = 272,
     TANH = 273,
     LOG10 = 274,
     LOGE = 275,
     EXP = 276,
     RAIZ = 277,
     NT_DEC = 278,
     PF_DEC = 279,
     X_DEC = 280,
     FX_DEC = 281,
     FDX_DEC = 282,
     GX_DEC = 283,
     ERROR_DEC = 284,
     ERROR_T = 285
   };
#endif
/* Tokens.  */
#define NUMERO 258
#define X 259
#define EULER 260
#define PI 261
#define SIN 262
#define COS 263
#define TAN 264
#define SEC 265
#define COSEC 266
#define COT 267
#define ASIN 268
#define ACOS 269
#define ATAN 270
#define SINH 271
#define COSH 272
#define TANH 273
#define LOG10 274
#define LOGE 275
#define EXP 276
#define RAIZ 277
#define NT_DEC 278
#define PF_DEC 279
#define X_DEC 280
#define FX_DEC 281
#define FDX_DEC 282
#define GX_DEC 283
#define ERROR_DEC 284
#define ERROR_T 285




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 38 "parsermet.y"

    double val;



/* Line 1676 of yacc.c  */
#line 118 "parsermetTok.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


