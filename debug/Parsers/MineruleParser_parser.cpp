/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         mrparse
#define yylex           mrlex
#define yyerror         mrerror
#define yydebug         mrdebug
#define yynerrs         mrnerrs

#define yylval          mrlval
#define yychar          mrchar
#define yylloc          mrlloc

/* Copy the first part of user declarations.  */
#line 1 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:339  */

#include <list>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>

#include "minerule/Utils/MinMaxPair.hpp"
#include "minerule/Parsers/ParsedMinerule.hpp"
#include "minerule/Utils/Converter.hpp"
#include "minerule/Parsers/ParserLibrary.hpp"
#include "minerule/Parsers/ParsedPredicate.hpp"
#include "minerule/Utils/MineruleOptions.hpp"
#include "minerule/Parsers/SupportMeasure.hpp"

#define PARSER_BUF_SIZE 4096
extern int mrlex();
extern void mrerror(char const* s);
bool check_body_coincident_head(minerule::ParsedMinerule::AttrVector& , minerule::ParsedMinerule::AttrVector&);
int find(std::vector<minerule::Bem_cond*> vect,std::string type_cond);

using namespace minerule;

extern std::map<std::string,int> keywords_to_token;
/* extern std::vector<ParsedSimplePredicate*>* CrossCond; */
extern minerule::ParsedPredicate* hc_mining;

/******************************************/
bool first_start=true;
bool first_end=true;
/******************************************/


#line 109 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "MineruleParser_parser.hpp".  */
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
#line 35 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:355  */

	minerule::ParsedSimplePredicate *TParsedSimplePredicate;
	minerule::ParsedPredConjunction* TParsedPredConjunction;
	minerule::ParsedPredicate* TParsedPredicate;
        minerule::ParsedMinerule::AttrVector* TListType;
	char* TString;
	minerule::MinMaxPair* TMinMaxPair;
	minerule::SupportMeasure* ev_m;
    minerule::ParsedMinerule::bem_c bem_condition;
	

#line 214 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:355  */
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

/* Copy the second part of user declarations.  */

#line 243 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   301

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  263

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   311

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      55,    56,     2,     2,    53,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    54,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    57,    58,
      59,    60
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    76,    76,   158,   216,   274,   275,   277,   281,   283,
     285,   287,   289,   291,   299,   301,   307,   309,   314,   315,
     321,   337,   352,   357,   365,   374,   385,   394,   397,   400,
     420,   422,   426,   433,   438,   445,   452,   458,   465,   471,
     480,   496,   505,   513,   520,   533,   541,   549,   559,   572,
     580,   588,   599,   603,   606,   611,   618,   625,   632,   642,
     647,   656,   658,   660,   664,   674,   676,   678,   682,   689,
     698,   705,   714,   721,   729,   731,   735,   737,   740,   750,
     773,   783,   794,   801,   809,   815,   819,   827,   837,   847,
     851,   862,   872,   883,   895,   905,   912,   914,   916,   918,
     920,   922,   926,   932,   938,   944
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "MINE", "RULE", "AS", "SEQUENCES",
  "SELECT", "BODY", "HEAD", "CLUSTER", "FROM", "ITEM", "ORDER", "GROUP",
  "BY", "HAVING", "OR", "AND", "NOT", "WHERE", "EXTRACTING", "RULES",
  "WITH", "DOTS", "ITEMSET", "SUPPORT", "CONFIDENCE", "SEQUENCE",
  "ITEMSETS", "GAP", "SINGLE_DOT", "BETWEEN", "BEGIN_C", "START", "END",
  "MID", "SQUARE", "LIKE", "IN", "ATTR", "CARD", "PARTBODY",
  "QUOTED_VALUE", "PARTHEAD", "COMPARISON", "ID", "AGGREGATE", "DATE_MR",
  "TIME_", "DISTINCT", "INT_NUMBER", "DBL_NUMBER", "','", "':'", "'('",
  "')'", "\"true\"", "\"TRUE\"", "\"FALSE\"", "\"false\"", "$accept",
  "query", "distinct_clause", "from_clause", "group_clause",
  "order_clause", "cluster_clause", "cluster_cond", "having_m_cond",
  "mining_cond", "having_clause", "qualified_attribute", "attr_list",
  "card", "length_clause", "gaps_clause", "value", "aggr_member",
  "or_cond", "and_cond", "predicate", "filter_cond", "filter_or_cond",
  "filter_and_cond", "filter_predicate", "base_predicate", "bool_cond",
  "between_exp", "like_exp", "in_exp", "seq_clause", "conj_cond", "cond",
  "be_cond", "mid_conj_cond", "mid_cond", "mid_card", "bem_range", "range",
  "constant_commalist", "constant", "bool_literal", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,    44,    58,    40,    41,   308,   309,   310,
     311
};
# endif

#define YYPACT_NINF -145

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-145)))

#define YYTABLE_NINF -63

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      32,    89,    16,    45,    52,    -2,  -145,    51,    95,    79,
      68,  -145,   105,   109,    25,   115,    74,    76,  -145,  -145,
     138,    90,    90,   103,   139,   130,   130,  -145,   130,    47,
    -145,     6,     9,   122,  -145,  -145,   179,   155,   181,    -6,
     144,  -145,   154,   182,    90,   187,   184,   204,   130,   196,
      -2,   177,   202,    10,    92,   203,  -145,  -145,    98,   200,
     212,   168,   180,  -145,  -145,   169,  -145,  -145,    92,   183,
     185,   186,   161,   208,  -145,    92,   204,   198,   201,    -2,
     172,  -145,   215,  -145,   199,    70,   220,   188,    92,   134,
      42,    -5,   149,   110,   152,    92,    92,   129,   161,   220,
     197,   205,   199,   130,    98,   207,  -145,   189,  -145,  -145,
    -145,  -145,    70,  -145,  -145,  -145,  -145,  -145,   166,   218,
    -145,  -145,  -145,  -145,  -145,  -145,    56,    13,   224,   227,
     216,     8,  -145,  -145,  -145,   190,  -145,  -145,  -145,  -145,
    -145,  -145,  -145,  -145,  -145,  -145,   225,  -145,  -145,   231,
     -13,   -13,   211,   232,    46,  -145,   209,    70,    35,    70,
      70,   108,   170,   108,   108,   206,   108,  -145,   130,   236,
     233,   210,  -145,  -145,    92,   234,   108,   108,  -145,  -145,
    -145,   199,    -9,   -13,    49,  -145,   235,  -145,  -145,   108,
     108,   213,   238,  -145,  -145,   108,  -145,   214,   130,   246,
     230,   161,   229,   241,  -145,  -145,   108,  -145,  -145,  -145,
     242,  -145,   108,   108,    84,  -145,   214,   239,   196,   243,
     108,   247,   108,   102,  -145,   108,  -145,   244,   203,   245,
    -145,   108,  -145,  -145,  -145,   219,   204,   221,  -145,   160,
     220,   160,  -145,   231,  -145,   254,   257,   253,   130,    92,
     255,   214,   161,   252,   256,   251,   226,   160,   228,   258,
     237,   160,  -145
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,    27,     1,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,     0,    88,    87,
       0,     0,     0,     5,     0,     0,     0,     6,     0,     0,
      23,     0,     0,     0,    25,    24,     0,     0,     0,     0,
       0,    22,     0,    29,     0,     0,     0,     0,     0,    16,
      89,     0,    75,     0,     0,    14,    28,     7,    89,    53,
       0,     0,     0,    20,    21,     0,    30,    31,     0,     0,
       0,     0,    17,    33,    38,     0,     0,     0,     0,    89,
       0,    74,    77,    79,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    15,     0,
       0,     0,     0,     0,    89,     0,    81,     0,    96,   101,
      98,    99,     0,   102,   103,   104,   105,    97,    52,    54,
      59,    61,    63,    65,    66,    67,     0,   100,     0,     0,
       0,     0,    42,    41,    40,     0,    37,    45,    47,    46,
      44,    48,    43,    51,    49,    50,    34,    35,    39,    18,
       0,     0,     0,    85,     0,    76,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,    36,    32,     0,     0,     0,     0,    82,    83,
      80,     0,     0,     0,     0,    58,    55,    56,    60,     0,
       0,     0,     0,   100,    70,     0,    93,     8,     0,     0,
       0,    19,     0,     0,    91,    84,     0,    78,    86,    57,
       0,    71,     0,     0,     0,    95,     9,     0,    16,     0,
       0,     0,     0,     0,    68,     0,    72,     0,    14,     0,
      90,     0,    69,    73,    94,     0,     0,     0,    92,     0,
       0,     0,     4,    18,     3,    10,     0,    12,     0,     0,
       0,    11,    13,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -145,  -145,  -145,   -72,   -94,  -145,  -145,  -145,    50,    64,
      40,   -22,   -26,    -4,  -145,  -145,   -51,   192,   -67,  -145,
     191,  -145,  -102,  -145,   123,  -145,  -145,  -145,  -145,  -145,
    -145,   193,  -145,  -145,   106,   217,     4,  -144,   107,    78,
    -116,   -79
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    28,    52,   129,   170,   247,   250,    76,    55,
     175,    69,    31,    25,    10,    47,   117,    71,    72,    73,
      74,    86,   118,   119,   120,   121,   122,   123,   124,   125,
      59,    81,    82,    83,   152,   153,    84,   178,   167,   214,
     126,   193
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      32,    91,    33,    70,    99,   149,   127,   179,    98,    11,
     158,    36,    95,    96,    38,    60,     6,    70,    26,   176,
      43,   131,    53,   206,    70,    95,    96,   -62,   146,   147,
     -62,   -62,   177,   127,    41,     1,   166,    70,   134,   208,
      48,   139,   142,   144,    70,    70,    70,   192,   194,     9,
     196,   136,   159,   160,    56,   184,    12,   186,   187,    37,
     203,   204,    37,    37,   172,    18,   159,   160,   135,   -62,
     138,   141,   143,   210,   211,   162,    19,   154,   127,   215,
     127,   127,   127,   102,    63,     7,    64,    34,   163,   107,
     221,   185,     8,     3,   164,   165,   215,   224,    35,    37,
      13,   166,   182,    14,   230,   209,   232,   201,    15,   234,
     108,    61,    16,   109,     4,   238,    17,     5,   110,   111,
      20,    66,    67,    70,    21,   112,    22,   113,   114,   115,
     116,    77,    62,    78,    63,    79,    64,   225,    80,    65,
     226,    24,   197,    66,    67,    23,   243,    68,   108,     9,
     140,   109,    63,    27,    64,   225,   110,   111,   233,    66,
      67,    66,    67,    29,   240,   113,   114,   115,   116,    62,
      30,    63,   216,    64,   132,    39,    65,   133,    95,    96,
      66,    67,   252,   159,   160,    66,    67,    40,   242,    42,
     244,    63,   137,    64,    63,    41,    64,    44,    70,    65,
      66,    67,   189,    66,    67,    46,   258,    45,   190,   191,
     262,    66,    67,    49,    50,    51,    54,    57,    58,    75,
      85,    87,   251,    88,    90,    89,    97,   103,    92,   100,
      93,    94,   101,   104,   128,   105,   161,   150,   156,   168,
     169,   130,   171,    96,   157,   151,   173,   174,   180,   183,
     181,   198,   217,   160,   199,   202,   213,   218,   219,   220,
     222,   195,   227,   200,   246,   231,   229,    37,   212,   249,
     235,   237,   248,   239,   254,   241,   253,   256,   236,   255,
     257,   259,   228,   245,   188,   260,   145,   205,   148,   207,
     223,   261,     0,     0,     0,     0,     0,   155,     0,     0,
       0,   106
};

static const yytype_int16 yycheck[] =
{
      26,    68,    28,    54,    76,    99,    85,   151,    75,     5,
     112,     5,    17,    18,     5,     5,     0,    68,    22,    32,
      26,    88,    48,    32,    75,    17,    18,    14,    95,    96,
      17,    18,    45,   112,    40,     3,    45,    88,    89,   183,
      44,    92,    93,    94,    95,    96,    97,   163,   164,    51,
     166,    56,    17,    18,    50,   157,     5,   159,   160,    53,
     176,   177,    53,    53,    56,    40,    17,    18,    90,    56,
      92,    93,    94,   189,   190,    19,    51,   103,   157,   195,
     159,   160,   161,    79,    42,    40,    44,    40,    32,    19,
     206,    56,    40,     4,    38,    39,   212,   213,    51,    53,
       5,    45,    56,    24,   220,    56,   222,   174,    40,   225,
      40,    19,     7,    43,    25,   231,     7,    28,    48,    49,
       5,    51,    52,   174,    50,    55,    50,    57,    58,    59,
      60,    33,    40,    35,    42,    37,    44,    53,    40,    47,
      56,    51,   168,    51,    52,     7,   240,    55,    40,    51,
      40,    43,    42,    50,    44,    53,    48,    49,    56,    51,
      52,    51,    52,    24,   236,    57,    58,    59,    60,    40,
      40,    42,   198,    44,    40,    53,    47,    43,    17,    18,
      51,    52,   249,    17,    18,    51,    52,     8,   239,     8,
     241,    42,    43,    44,    42,    40,    44,    53,   249,    47,
      51,    52,    32,    51,    52,    23,   257,    53,    38,    39,
     261,    51,    52,    26,    30,    11,    20,    40,    16,    16,
      20,     9,   248,    55,    55,    45,    18,    55,    45,    31,
      45,    45,    31,    18,    14,    36,    18,    40,    31,    15,
      13,    53,    26,    18,    55,    40,    56,    16,    37,    40,
      18,    15,     6,    18,    21,    21,    18,    27,    29,    18,
      18,    55,    23,    53,    10,    18,    23,    53,    55,    16,
      26,    26,    15,    54,    22,    54,    21,    26,   228,    23,
      54,    53,   218,   243,   161,    27,    94,   181,    97,   182,
     212,    54,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    84
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    62,     4,    25,    28,     0,    40,    40,    51,
      75,    97,     5,     5,    24,    40,     7,     7,    40,    51,
       5,    50,    50,     7,    51,    74,    74,    50,    63,    24,
      40,    73,    73,    73,    40,    51,     5,    53,     5,    53,
       8,    40,     8,    26,    53,    53,    23,    76,    74,    26,
      30,    11,    64,    73,    20,    70,    97,    40,    16,    91,
       5,    19,    40,    42,    44,    47,    51,    52,    55,    72,
      77,    78,    79,    80,    81,    16,    69,    33,    35,    37,
      40,    92,    93,    94,    97,    20,    82,     9,    55,    45,
      55,    79,    45,    45,    45,    17,    18,    18,    79,    64,
      31,    31,    97,    55,    18,    36,    96,    19,    40,    43,
      48,    49,    55,    57,    58,    59,    60,    77,    83,    84,
      85,    86,    87,    88,    89,    90,   101,   102,    14,    65,
      53,    79,    40,    43,    77,    72,    56,    43,    72,    77,
      40,    72,    77,    72,    77,    78,    79,    79,    81,    65,
      40,    40,    95,    96,    73,    92,    31,    55,    83,    17,
      18,    18,    19,    32,    38,    39,    45,    99,    15,    13,
      66,    26,    56,    56,    16,    71,    32,    45,    98,    98,
      37,    18,    56,    40,    83,    56,    83,    83,    85,    32,
      38,    39,   101,   102,   101,    55,   101,    73,    15,    21,
      53,    79,    21,   101,   101,    95,    32,    99,    98,    56,
     101,   101,    55,    18,   100,   101,    73,     6,    27,    29,
      18,   101,    18,   100,   101,    53,    56,    23,    70,    23,
     101,    18,   101,    56,   101,    26,    69,    26,   101,    54,
      64,    54,    77,    65,    77,    71,    10,    67,    15,    16,
      68,    73,    79,    21,    22,    23,    26,    54,    77,    53,
      27,    54,    77
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    62,    62,    63,    63,    64,    65,    66,
      67,    67,    68,    68,    69,    69,    70,    70,    71,    71,
      72,    72,    73,    73,    74,    74,    75,    75,    76,    76,
      77,    77,    78,    79,    79,    79,    79,    79,    80,    80,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    82,    82,    83,    83,    83,    83,    83,    84,
      84,    85,    85,    85,    86,    87,    87,    87,    88,    88,
      89,    89,    90,    90,    91,    91,    92,    92,    93,    93,
      93,    93,    94,    94,    95,    95,    96,    97,    97,    97,
      98,    98,    99,    99,   100,   100,   101,   101,   101,   101,
     101,   101,   102,   102,   102,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,    36,    23,    22,     0,     1,     2,     3,     3,
       0,     3,     0,     2,     0,     2,     0,     2,     0,     2,
       1,     1,     3,     1,     3,     3,     1,     0,     3,     0,
       1,     1,     4,     1,     3,     3,     4,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     0,     1,     3,     3,     4,     3,     1,
       3,     1,     1,     1,     2,     1,     1,     1,     5,     6,
       3,     4,     5,     6,     2,     0,     3,     1,     5,     1,
       4,     2,     4,     4,     3,     1,     4,     3,     3,     0,
       4,     2,     4,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 86 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
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
		    mr.bodyCardinalities = *(yyvsp[-29].TMinMaxPair);
		    mr.headCardinalities = *(yyvsp[-24].TMinMaxPair);

 		    if( !MineruleOptions::getSharedOptions().getParsers().getBodyCardinalities().contains(mr.bodyCardinalities) )
			yyerror("\nTHE CARDINALITY OF THE BODY IS INCORRECT REGARDING THE OPTION\n");

 		    if( !MineruleOptions::getSharedOptions().getParsers().getHeadCardinalities().contains(mr.headCardinalities) )
			yyerror("\nTHE CARDINALITY OF THE HEAD IS NOT CORRECTED REGARDING THE OPTION\n");

		    mr.ba = *(yyvsp[-28].TListType);
		    mr.ha = *(yyvsp[-23].TListType);

		    /* MANAGE RULE ATTRIBUTES in way to don't have duplicate attributes*/
		    minerule::ParsedMinerule::AttrVector::iterator it;
		    minerule::ParsedMinerule::AttrVector* list_ra= new minerule::ParsedMinerule::AttrVector(*(yyvsp[-28].TListType));
		    for (it=(yyvsp[-23].TListType)->begin();it!=(yyvsp[-23].TListType)->end();++it){
		    		if (find(list_ra->begin(),list_ra->end(), *it)==list_ra->end())
					list_ra->push_back(*it);

			}

		    mr.ra=*list_ra;
		    /*FINISH TO CREATE THE LIST OR RULE ATTRIBUTE, IN THIS LIST THRE ISN'T ITEM DUPLICATES*/

		    mr.tab_source=(yyvsp[-14].TString);
		    mr.tab_result=(yyvsp[-33].TString);
                    mr.body_coincident_head=check_body_coincident_head(mr.ba,mr.ha);
		    mr.mc = (yyvsp[-16].TParsedPredicate)==NULL?NULL:(yyvsp[-16].TParsedPredicate)->convert();
		    mr.ga = *(yyvsp[-13].TListType);
		    mr.gc = (yyvsp[-12].TParsedPredicate)==NULL?NULL:(yyvsp[-12].TParsedPredicate)->convert();

		    if ((yyvsp[-11].TListType)!=NULL)
				mr.ca=*(yyvsp[-11].TListType);

		    mr.cc = (yyvsp[-10].TParsedPredicate)==NULL?NULL:(yyvsp[-10].TParsedPredicate)->convert();
		    mr.sup = Converter((yyvsp[-4].TString)).toDouble();
		    mr.conf= Converter((yyvsp[0].TString)).toDouble();

                    //ev measure e il token $25

		    delete (yyvsp[-29].TMinMaxPair);
		    delete (yyvsp[-24].TMinMaxPair);
		    delete (yyvsp[-28].TListType);
		    delete (yyvsp[-23].TListType);
		    delete (yyvsp[-13].TListType);
		    if ((yyvsp[-12].TParsedPredicate)!=NULL) delete (yyvsp[-12].TParsedPredicate);
		    //if ($21!=NULL)  delete $21;
		    //if ($22!=NULL)  delete $22;
		    free((yyvsp[-33].TString));
		    free((yyvsp[-14].TString));

		   }
#line 1677 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 166 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
		   	std::cout<<"EFFETTUATA UNA MINE ITEMSET"<<std::endl<<std::endl<<std::endl;
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
		    mr.bodyCardinalities = *(yyvsp[-16].TMinMaxPair);

		   std::cout << mr.bodyCardinalities << std::endl;

		    if( !MineruleOptions::getSharedOptions().getParsers().getBodyCardinalities().contains(mr.bodyCardinalities) )
			yyerror("\nTHE CARDINALITY IS INCORRECT REGARDING THE OPTION\n");
		    mr.ba = *(yyvsp[-15].TListType);
		    mr.ra=*(yyvsp[-15].TListType);
		    /*FINISH TO CREATE THE LIST OR RULE ATTRIBUTE, IN THIS LIST THRE ISN'T ITEM DUPLICATES*/

		    mr.tab_source=(yyvsp[-8].TString);
		    mr.tab_result=(yyvsp[-20].TString);
		       mr.body_coincident_head=0;
            mr.mc = (yyvsp[-10].TParsedPredicate)==NULL?NULL:(yyvsp[-10].TParsedPredicate)->convert();
		    mr.ga = *(yyvsp[-7].TListType);
		    mr.gc = (yyvsp[-6].TParsedPredicate)==NULL?NULL:(yyvsp[-6].TParsedPredicate)->convert();
		    mr.sup = Converter((yyvsp[0].TString)).toDouble();
		    /*
		    delete $7;

		    delete $8;

		    delete $16;
		    if ($13!=NULL) delete $13;

		    free($3);
		    free($15);
		    */
		   }
#line 1731 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 224 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
		    ParsedMinerule& mr = getParserOutputObj();
		    mr.miningTask = MTMineSequences;
            if ((yyvsp[-19].TMinMaxPair) != NULL)
                mr.length = *(yyvsp[-19].TMinMaxPair);
            mr.tab_result=(yyvsp[-18].TString);
            if ((yyvsp[-15].TString) != NULL)
                mr.distinct= true;
            else mr.distinct= false;
            mr.ba=*(yyvsp[-14].TListType);
            if ((yyvsp[-11].TMinMaxPair) != NULL){
               mr.sequenceAllowedGaps = *(yyvsp[-11].TMinMaxPair);
		    }
		    //mr.bodyCardinalities = $9 == NULL?NULL:*$9;
            mr.tab_source=(yyvsp[-10].TString);
            if((yyvsp[-8].TString) != NULL)
                mr.filter_condition=(yyvsp[-8].TString);
            mr.ga=*(yyvsp[-7].TListType);
            mr.oa=*(yyvsp[-6].TListType);
            mr.sup = Converter((yyvsp[0].TString)).toDouble();

            first_start=true;
            first_end=true;
           /* delete $16;
            delete $8;
            delete $15;

            delete $11;
            delete $3;*/
		 }
#line 1766 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 274 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TString)=NULL;}
#line 1772 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 275 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TString)="DISTINCT";}
#line 1778 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 277 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
			(yyval.TString)=(yyvsp[0].TString);
			}
#line 1786 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 281 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    { (yyval.TListType)=(yyvsp[0].TListType); }
#line 1792 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 283 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    { (yyval.TListType)=(yyvsp[0].TListType); }
#line 1798 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 285 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TListType)=NULL;}
#line 1804 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 287 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    { (yyval.TListType)=(yyvsp[0].TListType); }
#line 1810 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 289 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TParsedPredicate)=NULL;}
#line 1816 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 291 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
				(yyvsp[0].TParsedPredicate)->stamp();
				if	(!(yyvsp[0].TParsedPredicate)->areAllBorH())
                               			 mrerror("\nTHE CLUSTER CONDITION IS INCORRECT, LACK ONE OR MORE DECLARATION OF BODY|HEAD ATTRIBUTE\n");

                                        (yyval.TParsedPredicate)=(yyvsp[0].TParsedPredicate);
                }
#line 1828 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 299 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TParsedPredicate)=NULL;}
#line 1834 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 301 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    { if (!(yyvsp[0].TParsedPredicate)->areAllAggr_f())
					mrerror("\nTHE HAVING CONDITION OF MINING PREDICATE IS INCORRECT, COULD IS A MISTAKEN DECLARATION OF ATTRIBUTE\n");
				 (yyval.TParsedPredicate)=(yyvsp[0].TParsedPredicate);
				}
#line 1843 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 307 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TParsedPredicate)=NULL;}
#line 1849 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 309 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {  if (!(yyvsp[0].TParsedPredicate)->areAllBorH())
					mrerror("\nTHE MINING CONDITION IS INCORRECT, LACK ONE OR MORE DECLARATION OF BODY|HEAD ATTRIBUTE\n");
				(yyval.TParsedPredicate)=(yyvsp[0].TParsedPredicate);
		}
#line 1858 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 314 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    { (yyval.TParsedPredicate)=NULL; }
#line 1864 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 315 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    { if ((yyvsp[0].TParsedPredicate)->atLeastOneBorH())
					mrerror("\nTHE HAVING CONDITION IS INCORRECT, COULD IS A MISTAKEN DECLARATION OF ATTRIBUTE\n");
				(yyval.TParsedPredicate)=(yyvsp[0].TParsedPredicate);
				}
#line 1873 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 321 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
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
				strncat(text_Up,strdup((yyvsp[0].TString)+5),PARSER_BUF_SIZE);
				(yyval.TString)=strndup(text_Up,PARSER_BUF_SIZE);
				//$$=$1;
			}
#line 1894 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 337 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
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
				strncat(text_Up,strdup((yyvsp[0].TString)+5),PARSER_BUF_SIZE);
				(yyval.TString)=strndup(text_Up,PARSER_BUF_SIZE);
				}
#line 1913 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 352 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
				(yyvsp[-2].TListType)->push_back((yyvsp[0].TString));
	   			(yyval.TListType) = (yyvsp[-2].TListType);
				free((yyvsp[0].TString));
				}
#line 1923 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 357 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
				(yyval.TListType) = new ParsedMinerule::AttrVector();
				(yyval.TListType)->push_back((yyvsp[0].TString));
				free((yyvsp[0].TString));
				}
#line 1933 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 366 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
			 int n_min =Converter((yyvsp[-2].TString)).toLong();
			 int n_max = Converter((yyvsp[0].TString)).toLong();
			 if (n_min>n_max) // se la cardinalita massima e maggiore di quella minima
			     mrerror("\nTHE MIN CARDINALITY IS GREATER THAN MAX CARDINALITY\n");
			 else
			     (yyval.TMinMaxPair)=new MinMaxPair(n_min,n_max);
			}
#line 1946 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 374 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
			int n_min = Converter((yyvsp[-2].TString)).toLong();

			if( strlen((yyvsp[0].TString))!=1 || toupper((yyvsp[0].TString)[0])!='N' ) {
			    mrerror("Max cardinality must be either a number or a character in [n|N]\n");
			}

			(yyval.TMinMaxPair) = new MinMaxPair(n_min, n_min);
			(yyval.TMinMaxPair)->setDefaultMax();
		    }
#line 1961 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 386 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            (yyval.TMinMaxPair)=(yyvsp[0].TMinMaxPair);
            if((yyval.TMinMaxPair)->getMin()==0) {
                (yyval.TMinMaxPair)->setMin(1);
                std::cout<<"Min length must be greater than zero! Resetting parameter with value: 1\n"<<std::endl;
             }
        }
#line 1973 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 394 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TMinMaxPair)= NULL;}
#line 1979 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 398 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    { (yyval.TMinMaxPair)=(yyvsp[0].TMinMaxPair); }
#line 1985 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 400 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TMinMaxPair)=NULL;}
#line 1991 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 420 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TString)=(yyvsp[0].TString);}
#line 1997 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 422 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TString)=(yyvsp[0].TString);}
#line 2003 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 427 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
		 std::string temp= (yyvsp[-3].TString);
		  temp=temp+"("+(yyvsp[-1].TString)+")";
		  (yyval.TString)=strndup(temp.c_str(),PARSER_BUF_SIZE);
		}
#line 2013 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 434 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    { (yyval.TParsedPredicate) = new ParsedPredicate();
		  (yyval.TParsedPredicate)->push_back((yyvsp[0].TParsedPredConjunction));
		}
#line 2021 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 439 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
		  *(yyvsp[-2].TParsedPredicate) |= *(yyvsp[0].TParsedPredicate);
		  (yyval.TParsedPredicate) = (yyvsp[-2].TParsedPredicate);
		  delete (yyvsp[0].TParsedPredicate);
		}
#line 2031 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 446 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
		      *(yyvsp[-2].TParsedPredicate) &= *(yyvsp[0].TParsedPredicate);
		      (yyval.TParsedPredicate) = (yyvsp[-2].TParsedPredicate);
		      delete (yyvsp[0].TParsedPredicate);
		}
#line 2041 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 453 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {

			 (yyval.TParsedPredicate)=&dynamic_cast<ParsedPredicate&>(!(*(yyvsp[-1].TParsedPredicate)));
			 delete (yyvsp[-1].TParsedPredicate);
		}
#line 2051 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 459 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
		  (yyval.TParsedPredicate)=(yyvsp[-1].TParsedPredicate);
		}
#line 2059 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 466 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
		(yyval.TParsedPredConjunction) = new ParsedPredConjunction();
		(yyval.TParsedPredConjunction)->push_back((yyvsp[0].TParsedSimplePredicate));
		}
#line 2068 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 472 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {

			  (yyvsp[-2].TParsedPredConjunction)->push_back((yyvsp[0].TParsedSimplePredicate));
			  (yyval.TParsedPredConjunction)=(yyvsp[-2].TParsedPredConjunction);

		}
#line 2079 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 481 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            /*ParsedSimplePredicate:
                firt param: first term
                second param: relational operator
                third param: secondo term
                fourth param: true if at least one of the two term was evaluated on body or head false otherwise
                fifth param: true if at least one of the two term was an aggregation function false otherwise
            */

                (yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),false,false);
                free((yyvsp[-2].TString));
                free((yyvsp[-1].TString));
                free((yyvsp[0].TString));
                        }
#line 2098 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 497 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                (yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),false,false);
                free((yyvsp[-2].TString));
                free((yyvsp[-1].TString));
                free((yyvsp[0].TString));

            }
#line 2110 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 506 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                (yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),false,false);
                free((yyvsp[-2].TString));
                free((yyvsp[-1].TString));
                free((yyvsp[0].TString));
             }
#line 2121 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 513 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                (yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),false,false);
                free((yyvsp[-2].TString));
                free((yyvsp[-1].TString));
                free((yyvsp[0].TString));
             }
#line 2132 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 521 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string op=(yyvsp[-1].TString),nop;
                if (op.compare("")==0) { nop="<";}
                else if (op==">=") { nop="<=";}
                else if (op=="<") { nop=">";}
                else if (op=="<=") { nop=">=";}
                (yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[0].TString),nop,(yyvsp[-2].TString),false,false);
                free((yyvsp[-2].TString));
                free((yyvsp[-1].TString));
                free((yyvsp[0].TString));
             }
#line 2148 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 534 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),true,false);
			  	free((yyvsp[-2].TString));
			  	free((yyvsp[-1].TString));
			  	free((yyvsp[0].TString));
			 }
#line 2159 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 542 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),true,false);
			  	free((yyvsp[-2].TString));
			  	free((yyvsp[-1].TString));
			  	free((yyvsp[0].TString));
                        }
#line 2170 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 550 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),true,false);
			  /*	ParsedSimplePredicate *p = new ParsedSimplePredicate($1,$2,$3,true,false);
								CrossCond->push_back(p); */
				free((yyvsp[-2].TString));
			  	free((yyvsp[-1].TString));
			  	free((yyvsp[0].TString));
			 }
#line 2183 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 560 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
			std::string op=(yyvsp[-1].TString), nop;
				if (op.compare(">")==0) { nop="<";}
				else if (op==">=") { nop="<=";}
				else if (op=="<") { nop=">";}
				else if (op=="<=") { nop=">=";}
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[0].TString),nop,(yyvsp[-2].TString),true,false);
				free((yyvsp[-2].TString));
			  	free((yyvsp[-1].TString));
			  	free((yyvsp[0].TString));
			 }
#line 2199 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 573 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),false,true);
			  	free((yyvsp[-2].TString));
			  	free((yyvsp[-1].TString));
			  	free((yyvsp[0].TString));
			 }
#line 2210 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 581 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),false,true);
			  	free((yyvsp[-2].TString));
			  	free((yyvsp[-1].TString));
			  	free((yyvsp[0].TString));
			 }
#line 2221 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 589 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
				(yyval.TParsedSimplePredicate)=new ParsedSimplePredicate((yyvsp[-2].TString),(yyvsp[-1].TString),(yyvsp[0].TString),false,true);
			  	free((yyvsp[-2].TString));
			  	free((yyvsp[-1].TString));
			  	free((yyvsp[0].TString));
			 }
#line 2232 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 599 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                        (yyval.TString)=(yyvsp[0].TString);
                 }
#line 2240 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 603 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TString)=NULL;}
#line 2246 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 607 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            (yyval.TString)= (yyvsp[0].TString);
            }
#line 2254 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 612 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp=(yyvsp[-2].TString);
            tmp=tmp+ " OR " + (yyvsp[0].TString);
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE); ;
            }
#line 2264 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 619 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp=(yyvsp[-2].TString);
            tmp=tmp+ " AND " + (yyvsp[0].TString);
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
            }
#line 2274 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 626 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp="NOT (";
            tmp=tmp+ (yyvsp[-1].TString) + ")";
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE); ;

            }
#line 2285 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 633 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp="NOT (";
            tmp=tmp+ (yyvsp[-1].TString) + ")";
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
            }
#line 2295 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 643 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            (yyval.TString) = (yyvsp[0].TString);
            }
#line 2303 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 648 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp=(yyvsp[-2].TString);
            tmp=tmp+ " AND " + (yyvsp[0].TString);
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
            }
#line 2313 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 665 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp= (yyvsp[-1].TString);
            tmp += " ";
            tmp += (yyvsp[0].TString);
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
        }
#line 2324 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 683 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp=(yyvsp[-4].TString);
            tmp=tmp+ " BETWEEN " + (yyvsp[-2].TString) + " AND " + (yyvsp[0].TString);
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
        }
#line 2334 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 690 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp=(yyvsp[-5].TString);
            tmp=tmp+ " NOT BETWEEN " + (yyvsp[-2].TString) + " AND " + (yyvsp[0].TString);
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
        }
#line 2344 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 699 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp=(yyvsp[-2].TString);
            tmp=tmp+ " LIKE " + (yyvsp[0].TString);
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
        }
#line 2354 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 706 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp=(yyvsp[-3].TString);
            tmp=tmp+ " NOT LIKE " + (yyvsp[0].TString);
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
        }
#line 2364 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 715 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp=(yyvsp[-4].TString);
            tmp=tmp+ " IN (" + (yyvsp[-1].TString) + ")";
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
        }
#line 2374 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 722 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
            std::string tmp=(yyvsp[-5].TString);
            tmp=tmp+ " NOT IN (" + (yyvsp[-1].TString) + ") ";
            (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
        }
#line 2384 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 729 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TString) = (yyvsp[0].TString);}
#line 2390 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 731 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.TString) = NULL;}
#line 2396 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 741 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                        ParsedMinerule& mr= getParserOutputObj();
                        Dist_cond* dc = new Dist_cond();
                        dc->function= (yyvsp[-4].TString);
                        dc->attr= *(yyvsp[-2].TListType);
                        dc->range= (yyvsp[0].TString);
                        mr.seq_dist_vect.push_back(dc);
                    }
#line 2409 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 751 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                        ParsedMinerule& mr= getParserOutputObj();
                        if((yyvsp[0].bem_condition)->type.compare("BEGIN")==0){
                           if(first_start)
                                mr.seq_bem_vect.push_back((yyvsp[0].bem_condition));
                            else{
                               int start_index= find(mr.seq_bem_vect,"BEGIN");
                               mr.seq_bem_vect[start_index]->and_c= (yyvsp[0].bem_condition);
                            }
                           first_start=false;
                        }
                        else if((yyvsp[0].bem_condition)->type.compare("END")==0){
                            if(first_end)
                                mr.seq_bem_vect.push_back((yyvsp[0].bem_condition));
                            else{
                                int end_index= find(mr.seq_bem_vect,"END");
                                mr.seq_bem_vect[end_index]->and_c= (yyvsp[0].bem_condition);
                            }
                            first_end=false;
                        }
                    }
#line 2435 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 774 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                        ParsedMinerule& mr= getParserOutputObj();
                        if((yyvsp[-2].TMinMaxPair)) {
                            (yyvsp[-1].bem_condition)->count_min= (yyvsp[-2].TMinMaxPair)->getMin();
                            (yyvsp[-1].bem_condition)->count_max= (yyvsp[-2].TMinMaxPair)->getMax();
                        }
                        mr.seq_bem_vect.push_back((yyvsp[-1].bem_condition));
                    }
#line 2448 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 784 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                        ParsedMinerule& mr= getParserOutputObj();
                        if((yyvsp[-1].TMinMaxPair)) {
                            (yyvsp[0].bem_condition)->count_min= (yyvsp[-1].TMinMaxPair)->getMin();
                            (yyvsp[0].bem_condition)->count_max= (yyvsp[-1].TMinMaxPair)->getMax();
                        }
                        mr.seq_bem_vect.push_back((yyvsp[0].bem_condition));
                    }
#line 2461 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 795 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                   (yyvsp[0].bem_condition)->type+="BEGIN";
                   (yyvsp[0].bem_condition)->attr+=(yyvsp[-1].TString);
                   (yyval.bem_condition)=(yyvsp[0].bem_condition);
                 }
#line 2471 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 802 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                   (yyvsp[0].bem_condition)->type+="END";
                   (yyvsp[0].bem_condition)->attr+=(yyvsp[-1].TString);
                   (yyval.bem_condition)=(yyvsp[0].bem_condition);
                 }
#line 2481 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 810 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                    (yyvsp[-2].bem_condition)->and_c=(yyvsp[0].bem_condition);
                    (yyval.bem_condition)=(yyvsp[-2].bem_condition);
                  }
#line 2490 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 816 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {(yyval.bem_condition)=(yyvsp[0].bem_condition);}
#line 2496 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 820 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                     (yyvsp[0].bem_condition)->type+="MID";
                     (yyvsp[0].bem_condition)->attr+=(yyvsp[-1].TString);
                     (yyval.bem_condition)=(yyvsp[0].bem_condition);
                    }
#line 2506 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 828 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                     int n_min =Converter((yyvsp[-2].TString)).toLong();
                     int n_max = Converter((yyvsp[0].TString)).toLong();
                     if (n_min>n_max) // se la cardinalita massima e maggiore di quella minima
                           mrerror("\nTHE MIN CARDINALITY IS GREATER THAN MAX CARDINALITY\n");
                     else
                         (yyval.TMinMaxPair)=new MinMaxPair(n_min,n_max);
                   }
#line 2519 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 838 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                       int n_min =Converter((yyvsp[-2].TString)).toLong();
                       if( strlen((yyvsp[0].TString))!=1 || toupper((yyvsp[0].TString)[0])!='N')
                           mrerror("Max cardinality must be either a number or a character in [n|N]\n");
                       else
                            (yyval.TMinMaxPair)=new MinMaxPair(n_min,std::numeric_limits<int>::max()-1);

                    }
#line 2532 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 847 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    { (yyval.TMinMaxPair)=NULL; }
#line 2538 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 852 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                 std::string tmp1 = (yyvsp[-2].TString);
                 tmp1 += " AND ";
                 tmp1 += (yyvsp[0].TString);
                 Bem_cond* bc= new Bem_cond();
                 bc->op+="BETWEEN";
                 bc->val+=tmp1;
                 (yyval.bem_condition)=bc;
                }
#line 2552 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 863 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                 Bem_cond* bc= new Bem_cond();
                 bc->op+=(yyvsp[-1].TString);
                 bc->val+=(yyvsp[0].TString);
                 (yyval.bem_condition)=bc;

                 }
#line 2564 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 873 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
               std::string tmp = "BETWEEN ";
               tmp += (yyvsp[-2].TString);
               tmp += " AND ";
               tmp += (yyvsp[0].TString);
               char * cstr = new char [tmp.length()+1];
               strcpy (cstr, tmp.c_str());
               (yyval.TString) = cstr;
              }
#line 2578 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 884 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
               std::string tmp = (yyvsp[-1].TString);
               tmp =tmp+ " ";
               tmp += (yyvsp[0].TString);
               char * cstr = new char [tmp.length()+1];
               strcpy (cstr, tmp.c_str());
               (yyval.TString) = cstr;
              }
#line 2591 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 896 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                  std::string tmp = (yyvsp[-2].TString);
                  tmp =tmp+ ", ";
                  tmp += (yyvsp[0].TString);
                  char * cstr = new char [tmp.length()+1];
                  strcpy (cstr, tmp.c_str());
                  (yyval.TString) = cstr;
              }
#line 2604 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 906 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                  (yyval.TString) = (yyvsp[0].TString);
              }
#line 2612 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 927 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                 std::string tmp="TRUE";
                 (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
                 }
#line 2621 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 933 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                 std::string tmp="TRUE";
                 (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
                 }
#line 2630 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 939 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                 std::string tmp="FALSE";
                 (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
                 }
#line 2639 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 945 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1646  */
    {
                 std::string tmp="FALSE";
                 (yyval.TString)= strndup(tmp.c_str(),PARSER_BUF_SIZE);
                 }
#line 2648 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
    break;


#line 2652 "/home/borti/Tesi/minerule/debug/Parsers/MineruleParser_parser.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
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

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
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
  return yyresult;
}
#line 953 "/home/borti/Tesi/minerule/src/Parsers/MineruleParser.ypp" /* yacc.c:1906  */



namespace minerule{
void init_mrparser() {
	/* initialization of keywords_to_token table */
	if(keywords_to_token.size() > 0)
		return;

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
    keywords_to_token["BEGIN"]=BEGIN_C;
    keywords_to_token["END"]=END;
    keywords_to_token["MID"]=MID;
    keywords_to_token["BETWEEN"]=BETWEEN;
    keywords_to_token["LIKE"]=LIKE;
    keywords_to_token["IN"]=IN;

}
}


bool check_body_coincident_head(minerule::ParsedMinerule::AttrVector& a, minerule::ParsedMinerule::AttrVector& b){
	minerule::ParsedMinerule::AttrVector* a1 = new minerule::ParsedMinerule::AttrVector(a);
	minerule::ParsedMinerule::AttrVector* b1	= new minerule::ParsedMinerule::AttrVector(b);
sort(a1->begin(), a1->end());
sort(b1->begin(), b1->end());
if (a1->size()!=b1->size())
	return false;
minerule::ParsedMinerule::AttrVector::iterator it1,it2;
for (it1=a1->begin(),it2=b1->begin(); (it1!=a1->end())&&(*it1==*it2);++it1, ++it2);//notare il punto e virgola

      return (it1==a1->end());

}

bool containing(std::vector<std::string> vect, std::string el){
    for(int i=0; i<vect.size(); ++i)
        if(vect[i].compare(el)==0) return true;
    return false;
}

int find(std::vector<minerule::Bem_cond*> vect,std::string type_cond){
    for(int i=0;i<vect.size();++i)
        if(vect[i]->type.compare(type_cond)==0)
            return i;
    return -1;
}
