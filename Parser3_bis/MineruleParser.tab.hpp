
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
     MINE = 258,
     RULE = 259,
     AS = 260,
     SEQUENCES = 261,
     SELECT = 262,
     DISTINCT = 263,
     BODY = 264,
     HEAD = 265,
     CLUSTER = 266,
     FROM = 267,
     ITEM = 268,
     ORDER = 269,
     GROUP = 270,
     BY = 271,
     HAVING = 272,
     OR = 273,
     AND = 274,
     NOT = 275,
     WHERE = 276,
     EXTRACTING = 277,
     RULES = 278,
     WITH = 279,
     DOTS = 280,
     ITEMSET = 281,
     SUPPORT = 282,
     CONFIDENCE = 283,
     SEQUENCE = 284,
     ITEMSETS = 285,
     GAP = 286,
     ATTR = 287,
     CARD = 288,
     PARTBODY = 289,
     QUOTED_VALUE = 290,
     PARTHEAD = 291,
     COMPARISON = 292,
     ID = 293,
     AGGREGATE = 294,
     INT_NUMBER = 295,
     DBL_NUMBER = 296
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 28 "MineruleParser.ypp"

	minerule::ParsedSimplePredicate *TParsedSimplePredicate;   
	minerule::ParsedPredConjunction* TParsedPredConjunction; 
	minerule::ParsedPredicate* TParsedPredicate;
        minerule::ParsedMinerule::ListType* TListType;
	char* TString;
	minerule::MinMaxPair* TMinMaxPair;
	minerule::SupportMeasure* ev_m;
	


/* Line 1676 of yacc.c  */
#line 105 "MineruleParser.tab.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE mrlval;


