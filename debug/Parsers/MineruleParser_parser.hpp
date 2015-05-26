/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_MR_HOME_BORTI_TESI_MINERULE_DEBUG_PARSERS_MINERULEPARSER_PARSER_HPP_INCLUDED
# define YY_MR_HOME_BORTI_TESI_MINERULE_DEBUG_PARSERS_MINERULEPARSER_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int mrdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    MINE = 258,
    RULE = 259,
    AS = 260,
    SEQUENCES = 261,
    SELECT = 262,
    BODY = 263,
    HEAD = 264,
    CLUSTER = 265,
    FROM = 266,
    ITEM = 267,
    ORDER = 268,
    GROUP = 269,
    BY = 270,
    HAVING = 271,
    OR = 272,
    AND = 273,
    NOT = 274,
    WHERE = 275,
    EXTRACTING = 276,
    RULES = 277,
    WITH = 278,
    DOTS = 279,
    ITEMSET = 280,
    SUPPORT = 281,
    CONFIDENCE = 282,
    SEQUENCE = 283,
    ITEMSETS = 284,
    GAP = 285,
    SINGLE_DOT = 286,
    BETWEEN = 287,
    BEGIN_C = 288,
    START = 289,
    END = 290,
    MID = 291,
    SQUARE = 292,
    LIKE = 293,
    IN = 294,
    ATTR = 295,
    CARD = 296,
    PARTBODY = 297,
    QUOTED_VALUE = 298,
    PARTHEAD = 299,
    COMPARISON = 300,
    ID = 301,
    AGGREGATE = 302,
    DATE_MR = 303,
    TIME_ = 304,
    DISTINCT = 305,
    INT_NUMBER = 306,
    DBL_NUMBER = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 35 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1909  */

	minerule::ParsedSimplePredicate *TParsedSimplePredicate;
	minerule::ParsedPredConjunction* TParsedPredConjunction;
	minerule::ParsedPredicate* TParsedPredicate;
        minerule::ParsedMinerule::AttrVector* TListType;
	char* TString;
	minerule::MinMaxPair* TMinMaxPair;
	minerule::SupportMeasure* ev_m;
    minerule::ParsedMinerule::bem_c bem_condition;
	

#line 119 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE mrlval;
extern YYLTYPE mrlloc;
int mrparse (void);

#endif /* !YY_MR_HOME_BORTI_TESI_MINERULE_DEBUG_PARSERS_MINERULEPARSER_PARSER_HPP_INCLUDED  */
