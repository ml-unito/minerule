
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
     ID = 258,
     STRCONST = 259,
     INTCONST = 260,
     IOPARAM = 261,
     FLOATCONST = 262,
     CREATE = 263,
     INSERT = 264,
     OPEN = 265,
     CLOSE = 266,
     QUIT = 267,
     SELECT = 268,
     DELETE = 269,
     NL = 270,
     ERROR = 271,
     CHECK = 272,
     SPLIT = 273,
     HELP = 274,
     DUMP = 275,
     ONLY = 276,
     SET = 277,
     STRUCT = 278,
     WRITE = 279,
     ECHO_TOKEN = 280,
     EVALSPLIT = 281,
     EVALPENALTY = 282,
     LOADFILE = 283,
     PREDINFO = 284,
     CREATEANL = 285,
     OPENANL = 286,
     CLOSEANL = 287,
     WKLDSTATS = 288,
     SPLITSTATS = 289,
     PENALTYSTATS = 290,
     ANLINFO = 291
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 18 "cmdline_gram.y"

    char *string;
    shpid_t pgno;
    int number;
    float flt;



/* Line 1676 of yacc.c  */
#line 97 "cmdline_gram.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


