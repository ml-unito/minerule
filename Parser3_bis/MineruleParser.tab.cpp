/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse mrparse
#define yylex   mrlex
#define yyerror mrerror
#define yylval  mrlval
#define yychar  mrchar
#define yydebug mrdebug
#define yynerrs mrnerrs


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
/* Tokens.  */
#define MINE 258
#define RULE 259
#define AS 260
#define SEQUENCES 261
#define SELECT 262
#define DISTINCT 263
#define BODY 264
#define HEAD 265
#define CLUSTER 266
#define FROM 267
#define ITEM 268
#define ORDER 269
#define GROUP 270
#define BY 271
#define HAVING 272
#define OR 273
#define AND 274
#define NOT 275
#define WHERE 276
#define EXTRACTING 277
#define RULES 278
#define WITH 279
#define DOTS 280
#define ITEMSET 281
#define SUPPORT 282
#define CONFIDENCE 283
#define SEQUENCE 284
#define ITEMSETS 285
#define GAP 286
#define ATTR 287
#define CARD 288
#define PARTBODY 289
#define QUOTED_VALUE 290
#define PARTHEAD 291
#define COMPARISON 292
#define ID 293
#define AGGREGATE 294
#define INT_NUMBER 295
#define DBL_NUMBER 296




/* Copy the first part of user declarations.  */
#line 1 "MineruleParser.ypp"

#include <list>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>
  
#include "Utils/MinMaxPair.h"
#include "ParsedMinerule.h" 
#include "Utils/Converter.h"
#include "ParserLibrary.h"
#include "ParsedPredicate.h"
#include "Utils/MineruleOptions.h"
#include "SupportMeasure.h"

#define PARSER_BUF_SIZE 4096
extern int mrlex();
extern void mrerror(char* s);
bool check_body_coincident_head(minerule::ParsedMinerule::ListType& , minerule::ParsedMinerule::ListType&);
using namespace std;
using namespace minerule;

extern std::map<std::string,int> keywords_to_token;
/* extern std::vector<ParsedSimplePredicate*>* CrossCond; */
extern minerule::ParsedPredicate* hc_mining;


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 28 "MineruleParser.ypp"
{
	minerule::ParsedSimplePredicate *TParsedSimplePredicate;   
	minerule::ParsedPredConjunction* TParsedPredConjunction; 
	minerule::ParsedPredicate* TParsedPredicate;
        minerule::ParsedMinerule::ListType* TListType;
	char* TString;
	minerule::MinMaxPair* TMinMaxPair;
	minerule::SupportMeasure* ev_m;
	}
/* Line 193 of yacc.c.  */
#line 223 "MineruleParser.tab.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 236 "MineruleParser.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   168

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  49
/* YYNRULES -- Number of states.  */
#define YYNSTATES  154

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      44,    45,     2,     2,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,    40,    64,    84,    87,    91,    95,    96,
     100,   101,   104,   105,   108,   109,   112,   113,   116,   118,
     120,   124,   126,   130,   134,   140,   141,   143,   144,   146,
     148,   153,   155,   159,   163,   168,   172,   174,   178,   182,
     186,   190,   194,   198,   202,   206,   210,   214,   218,   222
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,     3,     4,    32,     5,     7,     8,    58,
      57,     5,     9,    42,    58,    57,     5,    10,    42,    27,
      42,    28,    54,    53,    48,    49,    55,    51,    52,    22,
      23,    24,    27,    43,    61,    42,    28,    43,    61,    -1,
       3,    26,    32,     5,     7,     8,    58,    57,     5,     9,
      42,    27,    54,    53,    48,    49,    55,    22,    30,    24,
      27,    43,    61,    -1,     3,    29,    32,     5,     7,    57,
      42,    27,    60,    59,    48,    49,    50,    22,     6,    24,
      27,    43,    61,    -1,    12,    32,    -1,    15,    16,    57,
      -1,    14,    16,    57,    -1,    -1,    11,    16,    57,    -1,
      -1,    17,    63,    -1,    -1,    17,    63,    -1,    -1,    21,
      63,    -1,    -1,    17,    63,    -1,    34,    -1,    36,    -1,
      57,    42,    32,    -1,    32,    -1,    40,    25,    40,    -1,
      40,    25,    32,    -1,    24,    31,    40,    25,    40,    -1,
      -1,    21,    -1,    -1,    40,    -1,    41,    -1,    39,    44,
      56,    45,    -1,    64,    -1,    63,    18,    63,    -1,    63,
      19,    63,    -1,    20,    44,    63,    45,    -1,    44,    63,
      45,    -1,    65,    -1,    64,    19,    65,    -1,    32,    37,
      61,    -1,    32,    37,    35,    -1,    32,    37,    32,    -1,
      61,    37,    61,    -1,    61,    37,    32,    -1,    56,    37,
      35,    -1,    56,    37,    61,    -1,    56,    37,    56,    -1,
      61,    37,    56,    -1,    62,    37,    61,    -1,    62,    37,
      62,    -1,    62,    37,    56,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    60,    60,   142,   200,   245,   249,   251,   253,   255,
     257,   259,   267,   269,   275,   277,   282,   283,   289,   305,
     320,   325,   333,   342,   353,   362,   365,   367,   371,   373,
     377,   384,   389,   396,   403,   409,   416,   422,   432,   448,
     457,   465,   472,   485,   493,   501,   511,   524,   532,   540
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "MINE", "RULE", "AS", "SEQUENCES",
  "SELECT", "DISTINCT", "BODY", "HEAD", "CLUSTER", "FROM", "ITEM", "ORDER",
  "GROUP", "BY", "HAVING", "OR", "AND", "NOT", "WHERE", "EXTRACTING",
  "RULES", "WITH", "DOTS", "ITEMSET", "SUPPORT", "CONFIDENCE", "SEQUENCE",
  "ITEMSETS", "GAP", "ATTR", "CARD", "PARTBODY", "QUOTED_VALUE",
  "PARTHEAD", "COMPARISON", "ID", "AGGREGATE", "INT_NUMBER", "DBL_NUMBER",
  "','", "':'", "'('", "')'", "$accept", "query", "from_clause",
  "group_clause", "order_clause", "cluster_clause", "cluster_cond",
  "having_m_cond", "mining_cond", "having_clause", "qualified_attribute",
  "attr_list", "card", "gaps_clause", "seq_clause", "value", "aggr_member",
  "or_cond", "and_cond", "predicate", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,    44,    58,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    47,    48,    49,    50,    51,    51,
      52,    52,    53,    53,    54,    54,    55,    55,    56,    56,
      57,    57,    58,    58,    59,    59,    60,    60,    61,    61,
      62,    63,    63,    63,    63,    63,    64,    64,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,    36,    23,    19,     2,     3,     3,     0,     3,
       0,     2,     0,     2,     0,     2,     0,     2,     1,     1,
       3,     1,     3,     3,     5,     0,     1,     0,     1,     1,
       4,     1,     3,     3,     4,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     1,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    27,    20,    23,
      22,     0,     0,     0,    26,    25,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     5,     0,
       0,     0,     0,    12,     0,     0,     0,     0,     0,     0,
       0,    18,    19,     0,    28,    29,     0,     0,     0,     0,
      15,    31,    36,     0,     0,    24,     6,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13,     0,     7,     0,     0,     0,    40,    39,    38,     0,
      35,    43,    45,    44,    42,    46,    41,    49,    47,    48,
      32,    33,    37,    16,     0,     0,    34,    30,     0,     0,
       0,     0,    17,     0,     0,    14,     0,     4,    12,     0,
       0,     0,     0,     0,    16,     3,     8,     0,    10,     0,
       0,     0,     9,    11,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     2
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    44,    50,    57,   138,   141,    74,    53,   119,
      67,    19,    21,    39,    35,    68,    69,    70,    71,    72
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -88
static const yytype_int16 yypact[] =
{
      22,    25,    30,     4,    10,    12,   -88,    44,    48,    55,
      68,    70,    76,    29,    61,    53,    64,    64,   -88,    63,
      81,    53,    53,    -9,   -12,     5,     6,    67,   -88,   -88,
     -88,    86,    75,    99,   -88,    85,    69,    71,    79,   100,
      64,    87,    77,    83,   101,    53,    98,    95,   -88,   105,
     108,     8,    32,   106,    84,    53,   109,   104,   117,    88,
      91,   -88,   -88,    89,   -88,   -88,    32,    92,    93,    94,
       3,   115,   -88,    32,   100,   -88,    96,    53,   129,    97,
      32,    49,    31,    -4,    57,    46,    60,    32,    32,    -1,
       3,   101,    96,   112,   110,    -2,   -88,   -88,   -88,   102,
     -88,   -88,   -88,   -88,   -88,   -88,   -88,   -88,   -88,   -88,
     121,   -88,   -88,   124,   116,   103,   -88,   -88,    32,   120,
     107,   118,     3,   114,    62,    98,   125,   -88,   106,   126,
     100,   111,   101,    62,   124,   -88,   137,   135,   138,    53,
      32,   130,    96,     3,   133,   134,   132,   119,    62,   122,
     139,   123,    62,   -88
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -88,   -88,   -72,   -87,   -88,   -88,   -88,    33,    35,    23,
     -23,   -21,    -8,   -88,   -88,   -78,    82,   -61,   -88,    74
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      25,    26,    91,    98,   113,    83,   103,   106,   108,    22,
      31,    33,    90,    58,    87,    88,    87,    88,    27,    95,
      29,    87,    88,    28,    51,     1,   110,   111,    30,     3,
       6,    60,    45,    61,    76,    62,     7,    16,    63,    64,
      65,   100,     8,   116,     9,   134,   127,    32,    32,    10,
      32,     4,    59,    11,     5,   135,    92,   122,   132,    99,
      12,   102,   105,   107,    60,    61,    61,    62,    62,    17,
     149,    63,    64,    65,   153,    13,    66,    14,   104,   143,
      61,    96,    62,    15,    97,    18,    64,    65,    34,    64,
      65,    61,   101,    62,    61,    36,    62,    64,    65,    63,
      64,    65,    64,    65,    20,    23,    24,    28,    37,    38,
      42,    40,    43,    41,    46,    48,    49,    47,   142,    52,
      54,    55,    56,    73,    75,    77,    78,    79,    81,    84,
      85,    86,    80,    82,    89,    93,   114,   115,    32,    94,
      88,   118,   123,   120,   126,   121,   125,   117,   137,   129,
     124,   139,   144,   131,   133,   140,   145,   136,   146,   147,
     128,   130,   148,   112,   150,     0,   152,   151,   109
};

static const yytype_int16 yycheck[] =
{
      21,    22,    74,    81,    91,    66,    84,    85,    86,    17,
       5,     5,    73,     5,    18,    19,    18,    19,    27,    80,
      32,    18,    19,    32,    45,     3,    87,    88,    40,     4,
       0,    32,    40,    34,    55,    36,    32,     8,    39,    40,
      41,    45,    32,    45,    32,   132,   124,    42,    42,     5,
      42,    26,    20,     5,    29,   133,    77,   118,   130,    82,
       5,    84,    85,    86,    32,    34,    34,    36,    36,     8,
     148,    39,    40,    41,   152,     7,    44,     7,    32,   140,
      34,    32,    36,     7,    35,    32,    40,    41,    21,    40,
      41,    34,    35,    36,    34,     9,    36,    40,    41,    39,
      40,    41,    40,    41,    40,    42,    25,    32,     9,    24,
      31,    42,    12,    42,    27,    32,    15,    40,   139,    21,
      25,    16,    14,    17,    40,    16,    22,    10,    37,    37,
      37,    37,    44,    44,    19,     6,    24,    27,    42,    42,
      19,    17,    22,    27,    30,    42,    28,    45,    11,    24,
      43,    16,    22,    27,    43,    17,    23,   134,    24,    27,
     125,   128,    43,    89,    42,    -1,    43,    28,    86
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    47,     4,    26,    29,     0,    32,    32,    32,
       5,     5,     5,     7,     7,     7,     8,     8,    32,    57,
      40,    58,    58,    42,    25,    57,    57,    27,    32,    32,
      40,     5,    42,     5,    21,    60,     9,     9,    24,    59,
      42,    42,    31,    12,    48,    58,    27,    40,    32,    15,
      49,    57,    21,    54,    25,    16,    14,    50,     5,    20,
      32,    34,    36,    39,    40,    41,    44,    56,    61,    62,
      63,    64,    65,    17,    53,    40,    57,    16,    22,    10,
      44,    37,    44,    63,    37,    37,    37,    18,    19,    19,
      63,    48,    57,     6,    42,    63,    32,    35,    61,    56,
      45,    35,    56,    61,    32,    56,    61,    56,    61,    62,
      63,    63,    65,    49,    24,    27,    45,    45,    17,    55,
      27,    42,    63,    22,    43,    28,    30,    61,    54,    24,
      53,    27,    48,    43,    49,    61,    55,    11,    51,    16,
      17,    52,    57,    63,    22,    23,    24,    27,    43,    61,
      42,    28,    43,    61
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 70 "MineruleParser.ypp"
    {
					    			    
		/*	if ($25!=NULL){
			switch ($25->getMeasureType()){
		        				case EvaluationMeasure::MONOTONE:
		          					cout<<"LA MISURA SUL SUPP E MONOTONA"<<endl;
		            			break;
		        				case EvaluationMeasure::ANTIMONOTONE:
		          					cout<<"LA MISURA SUL SUPP E ANTIMONOTONA"<<endl;
		            			break;
		        				default: break;
    		        }
		    } else cout<<"SUPPORTO NON ESPRESSO"<<endl;
		*/    
		   #warning GESTIRE HC_MINING
  		   /* hc_mining=$17; */
		    
		    ParsedMinerule& mr = getParserOutputObj();
		    mr.miningTask = MTMineRules;
		    mr.bodyCardinalities = *(yyvsp[(7) - (36)].TMinMaxPair);
		    mr.headCardinalities = *(yyvsp[(12) - (36)].TMinMaxPair);
	
 		    if( !MineruleOptions::getSharedOptions().getParsers().getBodyCardinalities().contains(mr.bodyCardinalities) )
			yyerror("\nTHE CARDINALITY OF THE BODY IS INCORRECT REGARDING THE OPTION\n");

 		    if( !MineruleOptions::getSharedOptions().getParsers().getHeadCardinalities().contains(mr.headCardinalities) )
			yyerror("\nTHE CARDINALITY OF THE HEAD IS NOT CORRECTED REGARDING THE OPTION\n");

		    mr.ba = *(yyvsp[(8) - (36)].TListType);
		    mr.ha = *(yyvsp[(13) - (36)].TListType);

		    /* MANAGE RULE ATTRIBUTES in way to don't have duplicate attributes*/		    
		    minerule::ParsedMinerule::ListType::iterator it;
		    minerule::ParsedMinerule::ListType* list_ra= new minerule::ParsedMinerule::ListType(*(yyvsp[(8) - (36)].TListType));
		    for (it=(yyvsp[(13) - (36)].TListType)->begin();it!=(yyvsp[(13) - (36)].TListType)->end();++it){
		    		if (find(list_ra->begin(),list_ra->end(), *it)==list_ra->end())
					list_ra->push_back(*it);

			}
		    
		    mr.ra=*list_ra;
		    /*FINISH TO CREATE THE LIST OR RULE ATTRIBUTE, IN THIS LIST THRE ISN'T ITEM DUPLICATES*/	
		    
		    mr.tab_source=(yyvsp[(22) - (36)].TString);
		    mr.tab_result=(yyvsp[(3) - (36)].TString);
                    mr.body_coincident_head=check_body_coincident_head(mr.ba,mr.ha);
		    mr.mc = (yyvsp[(20) - (36)].TParsedPredicate)==NULL?NULL:(yyvsp[(20) - (36)].TParsedPredicate)->convert();
		    mr.ga = *(yyvsp[(23) - (36)].TListType);
		    mr.gc = (yyvsp[(24) - (36)].TParsedPredicate)==NULL?NULL:(yyvsp[(24) - (36)].TParsedPredicate)->convert();
		
		    if ((yyvsp[(25) - (36)].TListType)!=NULL) 
				mr.ca=*(yyvsp[(25) - (36)].TListType);
		
		    mr.cc = (yyvsp[(26) - (36)].TParsedPredicate)==NULL?NULL:(yyvsp[(26) - (36)].TParsedPredicate)->convert();
		    mr.sup = Converter((yyvsp[(32) - (36)].TString)).toDouble();
		    mr.conf= Converter((yyvsp[(36) - (36)].TString)).toDouble();

                    //ev measure e il token $25

		    delete (yyvsp[(7) - (36)].TMinMaxPair);
		    delete (yyvsp[(12) - (36)].TMinMaxPair);
		    delete (yyvsp[(8) - (36)].TListType);
		    delete (yyvsp[(13) - (36)].TListType);
		    delete (yyvsp[(23) - (36)].TListType);
		    if ((yyvsp[(24) - (36)].TParsedPredicate)!=NULL) delete (yyvsp[(24) - (36)].TParsedPredicate);
		    //if ($21!=NULL)  delete $21;
		    //if ($22!=NULL)  delete $22;
		    free((yyvsp[(3) - (36)].TString));
		    free((yyvsp[(22) - (36)].TString));
		    
		   ;}
    break;

  case 3:
#line 150 "MineruleParser.ypp"
    {
		   	cout<<"EFFETTUATA UNA MINE ITEMSET"<<endl<<endl<<endl;
			/*
			if ($16!=NULL){
                        switch ($16->getMeasureType()){
                                                        case EvaluationMeasure::MONOTONE:
                                                                cout<<"LA MISURA SUL SUPP E MONOTONA"<<endl;
                                                break;
                                                        case EvaluationMeasure::ANTIMONOTONE:
                                                                cout<<"LA MISURA SUL SUPP E ANTIMONOTONA"<<endl;
                                                break;
                                                        default: break;
                        }
                    } else cout<<"SUPPORTO NON ESPRESSO"<<endl;
			*/
		    #warning GESTIRE HC_MINING
			/* hc_mining=$10;  */
		    ParsedMinerule& mr = getParserOutputObj();

		    mr.miningTask = MTMineItemsets;
		    mr.bodyCardinalities = *(yyvsp[(7) - (23)].TMinMaxPair);
		  
		   cout << mr.bodyCardinalities << endl;

		    if( !MineruleOptions::getSharedOptions().getParsers().getBodyCardinalities().contains(mr.bodyCardinalities) )
			yyerror("\nTHE CARDINALITY IS INCORRECT REGARDING THE OPTION\n");
		    mr.ba = *(yyvsp[(8) - (23)].TListType);		    
		    mr.ra=*(yyvsp[(8) - (23)].TListType);
		    /*FINISH TO CREATE THE LIST OR RULE ATTRIBUTE, IN THIS LIST THRE ISN'T ITEM DUPLICATES*/	

		    mr.tab_source=(yyvsp[(15) - (23)].TString);
		    mr.tab_result=(yyvsp[(3) - (23)].TString);
		       mr.body_coincident_head=0;
		    mr.mc = (yyvsp[(13) - (23)].TParsedPredicate)==NULL?NULL:(yyvsp[(13) - (23)].TParsedPredicate)->convert();
		    mr.ga = *(yyvsp[(16) - (23)].TListType);
		    mr.gc = (yyvsp[(17) - (23)].TParsedPredicate)==NULL?NULL:(yyvsp[(17) - (23)].TParsedPredicate)->convert();
		    mr.sup = Converter((yyvsp[(23) - (23)].TString)).toDouble();
		    /*
		    delete $7;
		    
		    delete $8;
		   
		    delete $16;
		    if ($13!=NULL) delete $13;
		
		    free($3);
		    free($15);
		    */
		   ;}
    break;

  case 4:
#line 207 "MineruleParser.ypp"
    {
		    ParsedMinerule& mr = getParserOutputObj();
		    mr.miningTask = MTMineSequences;
		    mr.tab_source=(yyvsp[(11) - (19)].TString);
                    mr.tab_result=(yyvsp[(3) - (19)].TString);
		    mr.oa=*(yyvsp[(13) - (19)].TListType);
		    mr.ba=*(yyvsp[(6) - (19)].TListType);
		    if ((yyvsp[(10) - (19)].TMinMaxPair) != NULL){
			   mr.sequenceAllowedGaps = *(yyvsp[(10) - (19)].TMinMaxPair);
		    }
		    //mr.bodyCardinalities = $9 == NULL?NULL:*$9;
		    mr.ga=*(yyvsp[(12) - (19)].TListType);  
		    mr.sup = Converter((yyvsp[(19) - (19)].TString)).toDouble();
		    delete (yyvsp[(13) - (19)].TListType);
		    delete (yyvsp[(6) - (19)].TListType);
		    delete (yyvsp[(12) - (19)].TListType); 	
		 ;}
    break;

  case 5:
#line 245 "MineruleParser.ypp"
    {
			(yyval.TString)=(yyvsp[(2) - (2)].TString);
			;}
    break;

  case 6:
#line 249 "MineruleParser.ypp"
    { (yyval.TListType)=(yyvsp[(3) - (3)].TListType); ;}
    break;

  case 7:
#line 251 "MineruleParser.ypp"
    { (yyval.TListType)=(yyvsp[(3) - (3)].TListType); ;}
    break;

  case 8:
#line 253 "MineruleParser.ypp"
    {(yyval.TListType)=NULL;;}
    break;

  case 9:
#line 255 "MineruleParser.ypp"
    { (yyval.TListType)=(yyvsp[(3) - (3)].TListType); ;}
    break;

  case 10:
#line 257 "MineruleParser.ypp"
    {(yyval.TParsedPredicate)=NULL;;}
    break;

  case 11:
#line 259 "MineruleParser.ypp"
    {
				(yyvsp[(2) - (2)].TParsedPredicate)->stamp();	
				if	(!(yyvsp[(2) - (2)].TParsedPredicate)->areAllBorH())
                               			 mrerror("\nTHE CLUSTER CONDITION IS INCORRECT, LACK ONE OR MORE DECLARATION OF BODY|HEAD ATTRIBUTE\n"); 

                                        (yyval.TParsedPredicate)=(yyvsp[(2) - (2)].TParsedPredicate);
                ;}
    break;

  case 12:
#line 267 "MineruleParser.ypp"
    {(yyval.TParsedPredicate)=NULL;;}
    break;

  case 13:
#line 269 "MineruleParser.ypp"
    { if (!(yyvsp[(2) - (2)].TParsedPredicate)->areAllAggr_f())
					mrerror("\nTHE HAVING CONDITION OF MINING PREDICATE IS INCORRECT, COULD IS A MISTAKEN DECLARATION OF ATTRIBUTE\n");
				 (yyval.TParsedPredicate)=(yyvsp[(2) - (2)].TParsedPredicate);
				;}
    break;

  case 14:
#line 275 "MineruleParser.ypp"
    {(yyval.TParsedPredicate)=NULL;;}
    break;

  case 15:
#line 277 "MineruleParser.ypp"
    {  if (!(yyvsp[(2) - (2)].TParsedPredicate)->areAllBorH())
					mrerror("\nTHE MINING CONDITION IS INCORRECT, LACK ONE OR MORE DECLARATION OF BODY|HEAD ATTRIBUTE\n");			
				(yyval.TParsedPredicate)=(yyvsp[(2) - (2)].TParsedPredicate);
		;}
    break;

  case 16:
#line 282 "MineruleParser.ypp"
    { (yyval.TParsedPredicate)=NULL; ;}
    break;

  case 17:
#line 283 "MineruleParser.ypp"
    { if ((yyvsp[(2) - (2)].TParsedPredicate)->atLeastOneBorH())
					mrerror("\nTHE HAVING CONDITION IS INCORRECT, COULD IS A MISTAKEN DECLARATION OF ATTRIBUTE\n");
				(yyval.TParsedPredicate)=(yyvsp[(2) - (2)].TParsedPredicate);
				;}
    break;

  case 18:
#line 289 "MineruleParser.ypp"
    {  
				/*
                                 * text_tUp serve per convertire il prefisso dell'attributo della mining condition
                                 * in maiuscolo in modo che dentro il MineRule la scritta BODY (HEAD) antecendente al vero
                                 * attributo sia maiuscola, mrtext+5 si riferisce alla parola contenuta in mrtext senza le prime 5
                                 * lettere (BODY. o HEAD.) in modo tale da poterlo ricopiare in con strcat in text_tUp
                                 * l'ultima istruzione serve per dire alla strcat di far ripartire il buffer dalla prima posizione
                                 */


				char text_Up[4096];
				strcpy(text_Up,"BODY.");
				strncat(text_Up,strdup((yyvsp[(1) - (1)].TString)+5),PARSER_BUF_SIZE);
				(yyval.TString)=strndup(text_Up,PARSER_BUF_SIZE);
				//$$=$1;
			;}
    break;

  case 19:
#line 305 "MineruleParser.ypp"
    {
				/*
                                 * text_tUp serve per convertire il prefisso dell'attributo della mining condition
                                 * in maiuscolo in modo che dentro il MineRule la scritta BODY (HEAD) antecendente al vero
                                 * attributo sia maiuscola, mrtext+5 si riferisce alla parola contenuta in mrtext senza le prime 5
                                 * lettere (BODY. o HEAD.) in modo tale da poterlo ricopiare in con strcat in text_tUp
                                 * l'ultima istruzione serve per dire alla strcat di far ripartire il buffer dalla prima posizione
                                 */

				char text_Up[4096];
				strcpy(text_Up,"HEAD.");
				strncat(text_Up,strdup((yyvsp[(1) - (1)].TString)+5),PARSER_BUF_SIZE);
				(yyval.TString)=strndup(text_Up,PARSER_BUF_SIZE);
				;}
    break;

  case 20:
#line 320 "MineruleParser.ypp"
    {
				(yyvsp[(1) - (3)].TListType)->push_back((yyvsp[(3) - (3)].TString));
	   			(yyval.TListType) = (yyvsp[(1) - (3)].TListType);
				free((yyvsp[(3) - (3)].TString));
				;}
    break;

  case 21:
#line 325 "MineruleParser.ypp"
    {
				(yyval.TListType) = new ParsedMinerule::ListType();
				(yyval.TListType)->push_back((yyvsp[(1) - (1)].TString));
				free((yyvsp[(1) - (1)].TString));
				;}
    break;

  case 22:
#line 334 "MineruleParser.ypp"
    {
			 int n_min =Converter((yyvsp[(1) - (3)].TString)).toLong(); 
			 int n_max = Converter((yyvsp[(3) - (3)].TString)).toLong();
			 if (n_min>n_max) // se la cardinalita massima e maggiore di quella minima
			     mrerror("\nTHE MIN CARDINALITY IS GREATER THAN MAX CARDINALITY\n");
			 else 
			     (yyval.TMinMaxPair)=new MinMaxPair(n_min,n_max);
			;}
    break;

  case 23:
#line 342 "MineruleParser.ypp"
    {
			int n_min = Converter((yyvsp[(1) - (3)].TString)).toLong();
			
			if( strlen((yyvsp[(3) - (3)].TString))!=1 || toupper((yyvsp[(3) - (3)].TString)[0])!='N' ) {
			    mrerror("Max cardinality must be either a number or a character in [n|N]\n");
			}
			    
			(yyval.TMinMaxPair) = new MinMaxPair(n_min, n_min);
			(yyval.TMinMaxPair)->setDefaultMax();
		    ;}
    break;

  case 24:
#line 354 "MineruleParser.ypp"
    {
                         int n_min =Converter((yyvsp[(3) - (5)].TString)).toLong();
                         int n_max = Converter((yyvsp[(5) - (5)].TString)).toLong();
                         if (n_min>n_max) // se la cardinalita massima e maggiore di quella minima
                             mrerror("\nTHE MIN CARDINALITY IS GREATER THAN MAX CARDINALITY\n");
                         else
                             (yyval.TMinMaxPair)=new MinMaxPair(n_min,n_max);
                        ;}
    break;

  case 25:
#line 362 "MineruleParser.ypp"
    {(yyval.TMinMaxPair)= NULL;;}
    break;

  case 26:
#line 366 "MineruleParser.ypp"
    {(yyval.TParsedPredicate)= NULL;;}
    break;

  case 27:
#line 367 "MineruleParser.ypp"
    {(yyval.TParsedPredicate)= NULL;;}
    break;

  case 28:
#line 371 "MineruleParser.ypp"
    {(yyval.TString)=(yyvsp[(1) - (1)].TString);;}
    break;

  case 29:
#line 373 "MineruleParser.ypp"
    {(yyval.TString)=(yyvsp[(1) - (1)].TString);;}
    break;

  case 30:
#line 378 "MineruleParser.ypp"
    {
		  string temp= (yyvsp[(1) - (4)].TString);
		  temp=temp+"("+(yyvsp[(3) - (4)].TString)+")";
		  (yyval.TString)=strndup(temp.c_str(),PARSER_BUF_SIZE);
		;}
    break;

  case 31:
#line 385 "MineruleParser.ypp"
    { (yyval.TParsedPredicate) = new ParsedPredicate();
		  (yyval.TParsedPredicate)->push_back((yyvsp[(1) - (1)].TParsedPredConjunction));
		;}
    break;

  case 32:
#line 390 "MineruleParser.ypp"
    {
		  *(yyvsp[(1) - (3)].TParsedPredicate) |= *(yyvsp[(3) - (3)].TParsedPredicate);
		  (yyval.TParsedPredicate) = (yyvsp[(1) - (3)].TParsedPredicate);
		  delete (yyvsp[(3) - (3)].TParsedPredicate);
		;}
    break;

  case 33:
#line 397 "MineruleParser.ypp"
    {     
		      *(yyvsp[(1) - (3)].TParsedPredicate) &= *(yyvsp[(3) - (3)].TParsedPredicate);
		      (yyval.TParsedPredicate) = (yyvsp[(1) - (3)].TParsedPredicate);
		      delete (yyvsp[(3) - (3)].TParsedPredicate);
		;}
    break;

  case 34:
#line 404 "MineruleParser.ypp"
    {
			 
			 (yyval.TParsedPredicate)=&dynamic_cast<ParsedPredicate&>(!(*(yyvsp[(3) - (4)].TParsedPredicate)));
			 delete (yyvsp[(3) - (4)].TParsedPredicate);
		;}
    break;

  case 35:
#line 410 "MineruleParser.ypp"
    { 
		  (yyval.TParsedPredicate)=(yyvsp[(2) - (3)].TParsedPredicate);
		;}
    break;

  case 36:
#line 417 "MineruleParser.ypp"
    { 
		(yyval.TParsedPredConjunction) = new ParsedPredConjunction();
		(yyval.TParsedPredConjunction)->push_back((yyvsp[(1) - (1)].TParsedSimplePredicate));
		;}
    break;

  case 37:
#line 423 "MineruleParser.ypp"
    {
			  
			  (yyvsp[(1) - (3)].TParsedPredConjunction)->push_back((yyvsp[(3) - (3)].TParsedSimplePredicate));
			  (yyval.TParsedPredConjunction)=(yyvsp[(1) - (3)].TParsedPredConjunction);
			  
		;}
    break;

  case 38:
#line 433 "MineruleParser.ypp"
    { 
			/*ParsedSimplePredicate:
				firt param: first term
				second param: relational operator
				third param: secondo term
				fourth param: true if at least one of the two term was evaluated on body or head false otherwise
				fifth param: true if at least one of the two term was an aggregation function false otherwise
			*/

			  	(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),false,false);
			  	free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString)); 
                        ;}
    break;

  case 39:
#line 449 "MineruleParser.ypp"
    { 
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),false,false);
				free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
				
			;}
    break;

  case 40:
#line 458 "MineruleParser.ypp"
    {
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),false,false);
			  	free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
			 ;}
    break;

  case 41:
#line 465 "MineruleParser.ypp"
    {
			  	(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),false,false);
			  	free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
			 ;}
    break;

  case 42:
#line 473 "MineruleParser.ypp"
    { 
				string op=(yyvsp[(2) - (3)].TString),nop;
				if (op.compare("")==0) { nop="<";}
				else if (op==">=") { nop="<=";}
				else if (op=="<") { nop=">";}
				else if (op=="<=") { nop=">=";}  
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(3) - (3)].TString),nop,(yyvsp[(1) - (3)].TString),false,false);
			  	free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
			 ;}
    break;

  case 43:
#line 486 "MineruleParser.ypp"
    { 
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),true,false);
			  	free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
			 ;}
    break;

  case 44:
#line 494 "MineruleParser.ypp"
    { 
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),true,false);
			  	free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
                        ;}
    break;

  case 45:
#line 502 "MineruleParser.ypp"
    { 	
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),true,false);
			  /*	ParsedSimplePredicate *p = new ParsedSimplePredicate($1,$2,$3,true,false);
								CrossCond->push_back(p); */
				free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
			 ;}
    break;

  case 46:
#line 512 "MineruleParser.ypp"
    { 	
				string op=(yyvsp[(2) - (3)].TString), nop;
				if (op.compare(">")==0) { nop="<";}
				else if (op==">=") { nop="<=";}
				else if (op=="<") { nop=">";}
				else if (op=="<=") { nop=">=";}  			
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(3) - (3)].TString),nop,(yyvsp[(1) - (3)].TString),true,false);
				free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
			 ;}
    break;

  case 47:
#line 525 "MineruleParser.ypp"
    { 
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),false,true);
			  	free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
			 ;}
    break;

  case 48:
#line 533 "MineruleParser.ypp"
    { 
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),false,true);
			  	free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
			 ;}
    break;

  case 49:
#line 541 "MineruleParser.ypp"
    { 
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[(1) - (3)].TString),(yyvsp[(2) - (3)].TString),(yyvsp[(3) - (3)].TString),false,true);
			  	free((yyvsp[(1) - (3)].TString));
			  	free((yyvsp[(2) - (3)].TString));
			  	free((yyvsp[(3) - (3)].TString));
			 ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2108 "MineruleParser.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 550 "MineruleParser.ypp"
 


namespace minerule{
void init_mrparser() {
	/* initialization of keywords_to_token table */
	keywords_to_token["MINE"]=MINE;
	keywords_to_token["RULE"]=RULE;
	keywords_to_token["AS"]=AS;
	keywords_to_token["FROM"]=FROM;
	keywords_to_token["WHERE"]=WHERE;
	keywords_to_token["GROUP"]=GROUP;
        keywords_to_token["BY"]=BY;
	keywords_to_token["HAVING"]=HAVING;
	keywords_to_token["EXTRACTING"]=EXTRACTING;
	keywords_to_token["RULES"]=RULES;
	keywords_to_token["WITH"]=WITH;
	keywords_to_token["DISTINCT"]=DISTINCT;
	keywords_to_token["SELECT"]=SELECT;
	keywords_to_token["BODY"]=BODY;
	keywords_to_token["HEAD"]=HEAD;
	keywords_to_token["NOT"]=NOT;
	keywords_to_token["AND"]=AND;
	keywords_to_token["OR"]=OR;
	keywords_to_token["CLUSTER"]=CLUSTER;
	keywords_to_token["ITEMSET"]=ITEMSET;
	keywords_to_token["ITEMSETS"]=ITEMSETS;
	keywords_to_token["SUPPORT"]=SUPPORT;
	keywords_to_token["CONFIDENCE"]=CONFIDENCE;
	keywords_to_token["SEQUENCE"]=SEQUENCE;
	keywords_to_token["SEQUENCES"]=SEQUENCES;
	keywords_to_token["ORDER"]=ORDER;
	keywords_to_token["GAP"]=GAP;

}
}


bool check_body_coincident_head(minerule::ParsedMinerule::ListType& a, minerule::ParsedMinerule::ListType& b){
	minerule::ParsedMinerule::ListType* a1 = new minerule::ParsedMinerule::ListType(a);
	minerule::ParsedMinerule::ListType* b1	= new minerule::ParsedMinerule::ListType(b);
sort(a1->begin(), a1->end());
sort(b1->begin(), b1->end());
if (a1->size()!=b1->size())
	return false;
minerule::ParsedMinerule::ListType::iterator it1,it2;
for (it1=a1->begin(),it2=b1->begin(); (it1!=a1->end())&&(*it1==*it2);++it1, ++it2);//notare il punto e virgola

      return (it1==a1->end());

}



