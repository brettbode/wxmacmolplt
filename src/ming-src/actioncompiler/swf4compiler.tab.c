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
#define yyparse swf4parse
#define yylex   swf4lex
#define yyerror swf4error
#define yylval  swf4lval
#define yychar  swf4char
#define yydebug swf4debug
#define yynerrs swf4nerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BREAK = 258,
     FOR = 259,
     CONTINUE = 260,
     IF = 261,
     ELSE = 262,
     DO = 263,
     WHILE = 264,
     THIS = 265,
     EVAL = 266,
     TIME = 267,
     RANDOM = 268,
     LENGTH = 269,
     INT = 270,
     CONCAT = 271,
     DUPLICATECLIP = 272,
     REMOVECLIP = 273,
     TRACE = 274,
     STARTDRAG = 275,
     STOPDRAG = 276,
     ORD = 277,
     CHR = 278,
     CALLFRAME = 279,
     GETURL = 280,
     GETURL1 = 281,
     LOADMOVIE = 282,
     LOADMOVIENUM = 283,
     LOADVARIABLES = 284,
     POSTURL = 285,
     SUBSTR = 286,
     GETPROPERTY = 287,
     NEXTFRAME = 288,
     PREVFRAME = 289,
     PLAY = 290,
     STOP = 291,
     TOGGLEQUALITY = 292,
     STOPSOUNDS = 293,
     GOTOFRAME = 294,
     GOTOANDPLAY = 295,
     FRAMELOADED = 296,
     SETTARGET = 297,
     ASM = 298,
     ASMADD = 299,
     ASMDIVIDE = 300,
     ASMMULTIPLY = 301,
     ASMEQUALS = 302,
     ASMLESS = 303,
     ASMLOGICALAND = 304,
     ASMLOGICALOR = 305,
     ASMLOGICALNOT = 306,
     ASMSTRINGAND = 307,
     ASMSTRINGEQUALS = 308,
     ASMSTRINGEXTRACT = 309,
     ASMSTRINGLENGTH = 310,
     ASMMBSTRINGEXTRACT = 311,
     ASMMBSTRINGLENGTH = 312,
     ASMPOP = 313,
     ASMPUSH = 314,
     ASMASCIITOCHAR = 315,
     ASMCHARTOASCII = 316,
     ASMTOINTEGER = 317,
     ASMCALL = 318,
     ASMIF = 319,
     ASMJUMP = 320,
     ASMGETVARIABLE = 321,
     ASMSETVARIABLE = 322,
     ASMGETURL2 = 323,
     ASMGETPROPERTY = 324,
     ASMGOTOFRAME2 = 325,
     ASMREMOVESPRITE = 326,
     ASMSETPROPERTY = 327,
     ASMSETTARGET2 = 328,
     ASMSTARTDRAG = 329,
     ASMWAITFORFRAME2 = 330,
     ASMCLONESPRITE = 331,
     ASMENDDRAG = 332,
     ASMGETTIME = 333,
     ASMRANDOMNUMBER = 334,
     ASMTRACE = 335,
     ASMMBASCIITOCHAR = 336,
     ASMMBCHARTOASCII = 337,
     ASMSUBSTRACT = 338,
     ASMSTRINGLESS = 339,
     TELLTARGET = 340,
     BROKENSTRING = 341,
     STRING = 342,
     NUMBER = 343,
     IDENTIFIER = 344,
     PATH = 345,
     EQ = 346,
     LE = 347,
     GE = 348,
     NE = 349,
     LAN = 350,
     LOR = 351,
     INC = 352,
     DEC = 353,
     IEQ = 354,
     DEQ = 355,
     MEQ = 356,
     SEQ = 357,
     STREQ = 358,
     STRNE = 359,
     STRCMP = 360,
     PARENT = 361,
     END = 362,
     UMINUS = 363,
     POSTFIX = 364,
     NEGATE = 365
   };
#endif
/* Tokens.  */
#define BREAK 258
#define FOR 259
#define CONTINUE 260
#define IF 261
#define ELSE 262
#define DO 263
#define WHILE 264
#define THIS 265
#define EVAL 266
#define TIME 267
#define RANDOM 268
#define LENGTH 269
#define INT 270
#define CONCAT 271
#define DUPLICATECLIP 272
#define REMOVECLIP 273
#define TRACE 274
#define STARTDRAG 275
#define STOPDRAG 276
#define ORD 277
#define CHR 278
#define CALLFRAME 279
#define GETURL 280
#define GETURL1 281
#define LOADMOVIE 282
#define LOADMOVIENUM 283
#define LOADVARIABLES 284
#define POSTURL 285
#define SUBSTR 286
#define GETPROPERTY 287
#define NEXTFRAME 288
#define PREVFRAME 289
#define PLAY 290
#define STOP 291
#define TOGGLEQUALITY 292
#define STOPSOUNDS 293
#define GOTOFRAME 294
#define GOTOANDPLAY 295
#define FRAMELOADED 296
#define SETTARGET 297
#define ASM 298
#define ASMADD 299
#define ASMDIVIDE 300
#define ASMMULTIPLY 301
#define ASMEQUALS 302
#define ASMLESS 303
#define ASMLOGICALAND 304
#define ASMLOGICALOR 305
#define ASMLOGICALNOT 306
#define ASMSTRINGAND 307
#define ASMSTRINGEQUALS 308
#define ASMSTRINGEXTRACT 309
#define ASMSTRINGLENGTH 310
#define ASMMBSTRINGEXTRACT 311
#define ASMMBSTRINGLENGTH 312
#define ASMPOP 313
#define ASMPUSH 314
#define ASMASCIITOCHAR 315
#define ASMCHARTOASCII 316
#define ASMTOINTEGER 317
#define ASMCALL 318
#define ASMIF 319
#define ASMJUMP 320
#define ASMGETVARIABLE 321
#define ASMSETVARIABLE 322
#define ASMGETURL2 323
#define ASMGETPROPERTY 324
#define ASMGOTOFRAME2 325
#define ASMREMOVESPRITE 326
#define ASMSETPROPERTY 327
#define ASMSETTARGET2 328
#define ASMSTARTDRAG 329
#define ASMWAITFORFRAME2 330
#define ASMCLONESPRITE 331
#define ASMENDDRAG 332
#define ASMGETTIME 333
#define ASMRANDOMNUMBER 334
#define ASMTRACE 335
#define ASMMBASCIITOCHAR 336
#define ASMMBCHARTOASCII 337
#define ASMSUBSTRACT 338
#define ASMSTRINGLESS 339
#define TELLTARGET 340
#define BROKENSTRING 341
#define STRING 342
#define NUMBER 343
#define IDENTIFIER 344
#define PATH 345
#define EQ 346
#define LE 347
#define GE 348
#define NE 349
#define LAN 350
#define LOR 351
#define INC 352
#define DEC 353
#define IEQ 354
#define DEQ 355
#define MEQ 356
#define SEQ 357
#define STREQ 358
#define STRNE 359
#define STRCMP 360
#define PARENT 361
#define END 362
#define UMINUS 363
#define POSTFIX 364
#define NEGATE 365




/* Copy the first part of user declarations.  */
#line 5 "./swf4compiler.y"


#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "compile.h"
#include "actiontypes.h"
#include "assembler.h"

#define YYPARSE_PARAM buffer
#define YYERROR_VERBOSE 1



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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
#line 19 "./swf4compiler.y"
{
  Buffer action;
  char *str;
  SWFActionFunction function;
  SWFGetUrl2Method getURLMethod;
  int len;
}
/* Line 187 of yacc.c.  */
#line 346 "swf4compiler.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 359 "swf4compiler.tab.c"

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
# if YYENABLE_NLS
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
#define YYFINAL  112
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1707

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  129
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  188
/* YYNRULES -- Number of states.  */
#define YYNSTATES  433

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   365

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   119,     2,     2,     2,     2,   114,     2,
     126,   127,   117,   115,   108,   116,   128,   118,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   111,   125,
     112,   109,   113,   110,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   123,     2,   124,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   120,   121,   122
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    15,    19,    21,
      24,    26,    28,    30,    32,    34,    38,    39,    41,    44,
      55,    64,    74,    85,    94,   104,   112,   118,   119,   121,
     131,   137,   144,   154,   155,   157,   160,   163,   164,   167,
     169,   173,   178,   183,   188,   193,   200,   208,   215,   223,
     231,   239,   246,   261,   270,   274,   278,   282,   286,   290,
     294,   299,   304,   309,   314,   319,   324,   330,   336,   341,
     345,   350,   355,   360,   365,   370,   377,   386,   393,   396,
     399,   401,   405,   407,   410,   412,   414,   416,   420,   425,
     428,   433,   436,   439,   442,   446,   450,   454,   458,   462,
     466,   470,   474,   478,   482,   486,   490,   494,   498,   502,
     506,   510,   516,   518,   522,   524,   526,   528,   530,   532,
     534,   536,   538,   542,   544,   545,   551,   553,   556,   559,
     564,   569,   573,   577,   581,   585,   589,   595,   601,   607,
     613,   619,   621,   623,   625,   629,   631,   634,   635,   639,
     641,   643,   645,   647,   649,   651,   653,   655,   657,   659,
     661,   663,   665,   667,   669,   671,   673,   675,   677,   679,
     681,   683,   685,   687,   689,   691,   693,   695,   697,   699,
     701,   703,   705,   707,   709,   712,   715,   718,   721
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     130,     0,    -1,   131,    -1,   132,    -1,   131,   132,    -1,
     133,    -1,   123,   124,    -1,   123,   135,   124,    -1,   125,
      -1,   151,   125,    -1,   136,    -1,   138,    -1,   140,    -1,
     141,    -1,   151,    -1,   134,   108,   151,    -1,    -1,   133,
      -1,   135,   133,    -1,     6,   126,    41,   126,    88,   127,
     127,   133,     7,   133,    -1,     6,   126,    41,   126,    88,
     127,   127,   133,    -1,     6,   126,   119,    41,   126,    88,
     127,   127,   133,    -1,     6,   126,    41,   126,   153,   127,
     127,   133,     7,   133,    -1,     6,   126,    41,   126,   153,
     127,   127,   133,    -1,     6,   126,   119,    41,   126,   153,
     127,   127,   133,    -1,     6,   126,   153,   127,   133,     7,
     133,    -1,     6,   126,   153,   127,   133,    -1,    -1,   153,
      -1,     9,   126,   119,    41,   126,    88,   127,   127,   133,
      -1,     9,   126,   153,   127,   133,    -1,     8,   133,     9,
     126,   153,   127,    -1,     4,   126,   139,   125,   137,   125,
     139,   127,   133,    -1,    -1,   134,    -1,     5,   125,    -1,
       3,   125,    -1,    -1,   108,    87,    -1,   153,    -1,    21,
     126,   127,    -1,    24,   126,   148,   127,    -1,    24,   126,
      87,   127,    -1,    18,   126,   153,   127,    -1,    19,   126,
     153,   127,    -1,    25,   126,   153,   108,   153,   127,    -1,
      25,   126,   153,   108,   153,   142,   127,    -1,    26,   126,
      87,   108,    87,   127,    -1,    27,   126,   153,   108,   153,
     142,   127,    -1,    28,   126,   153,   108,   143,   142,   127,
      -1,    29,   126,   153,   108,   153,   142,   127,    -1,    20,
     126,   153,   108,   153,   127,    -1,    20,   126,   153,   108,
     153,   108,   153,   108,   153,   108,   153,   108,   153,   127,
      -1,    17,   126,   153,   108,   153,   108,   153,   127,    -1,
      33,   126,   127,    -1,    34,   126,   127,    -1,    35,   126,
     127,    -1,    36,   126,   127,    -1,    37,   126,   127,    -1,
      38,   126,   127,    -1,    39,   126,    88,   127,    -1,    39,
     126,    87,   127,    -1,    39,   126,   153,   127,    -1,    40,
     126,   153,   127,    -1,    42,   126,    87,   127,    -1,    42,
     126,   153,   127,    -1,    85,   126,    87,   127,   133,    -1,
      85,   126,   153,   127,   133,    -1,    11,   126,   153,   127,
      -1,    12,   126,   127,    -1,    13,   126,   153,   127,    -1,
      14,   126,   153,   127,    -1,    15,   126,   153,   127,    -1,
      22,   126,   153,   127,    -1,    23,   126,   153,   127,    -1,
      16,   126,   153,   108,   153,   127,    -1,    31,   126,   153,
     108,   153,   108,   153,   127,    -1,    32,   126,   153,   108,
      87,   127,    -1,   150,    97,    -1,   150,    98,    -1,   145,
      -1,   126,   147,   127,    -1,    88,    -1,   116,    88,    -1,
      87,    -1,   148,    -1,   149,    -1,   149,   128,    89,    -1,
      97,   149,   128,    89,    -1,    97,   150,    -1,    98,   149,
     128,    89,    -1,    98,   150,    -1,   116,   147,    -1,   119,
     147,    -1,   150,   109,   147,    -1,   147,   117,   147,    -1,
     147,   118,   147,    -1,   147,   115,   147,    -1,   147,   116,
     147,    -1,   147,   114,   147,    -1,   147,   112,   147,    -1,
     147,   113,   147,    -1,   147,    92,   147,    -1,   147,    93,
     147,    -1,   147,   104,   147,    -1,   147,   103,   147,    -1,
     147,   105,   147,    -1,   147,    91,   147,    -1,   147,    94,
     147,    -1,   147,    95,   147,    -1,   147,    96,   147,    -1,
     147,   110,   147,   111,   147,    -1,    89,    -1,   149,   111,
      89,    -1,    10,    -1,   128,    -1,   118,    -1,   106,    -1,
      89,    -1,    90,    -1,   148,    -1,    87,    -1,   126,   147,
     127,    -1,   146,    -1,    -1,    43,   123,   152,   156,   124,
      -1,   144,    -1,    97,   150,    -1,    98,   150,    -1,    97,
     149,   128,    89,    -1,    98,   149,   128,    89,    -1,   150,
     109,   147,    -1,   150,   101,   147,    -1,   150,   100,   147,
      -1,   150,    99,   147,    -1,   150,   102,   147,    -1,   149,
     128,    89,   109,   147,    -1,   149,   128,    89,   101,   147,
      -1,   149,   128,    89,   100,   147,    -1,   149,   128,    89,
      99,   147,    -1,   149,   128,    89,   102,   147,    -1,   147,
      -1,    87,    -1,   154,    -1,   155,   108,   154,    -1,   157,
      -1,   156,   157,    -1,    -1,    59,   158,   155,    -1,    44,
      -1,    83,    -1,    46,    -1,    45,    -1,    47,    -1,    48,
      -1,    49,    -1,    50,    -1,    51,    -1,    52,    -1,    53,
      -1,    55,    -1,    54,    -1,    56,    -1,    57,    -1,    84,
      -1,    58,    -1,    60,    -1,    61,    -1,    62,    -1,    81,
      -1,    82,    -1,    63,    -1,    66,    -1,    67,    -1,    69,
      -1,    72,    -1,    71,    -1,    73,    -1,    74,    -1,    77,
      -1,    76,    -1,    78,    -1,    79,    -1,    80,    -1,    64,
      88,    -1,    65,    88,    -1,    68,    88,    -1,    70,    88,
      -1,    75,    88,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   163,   163,   169,   170,   175,   179,   180,   181,   182,
     183,   184,   185,   186,   190,   191,   195,   196,   197,   204,
     220,   236,   248,   262,   276,   286,   297,   307,   308,   332,
     347,   359,   367,   389,   390,   394,   402,   411,   413,   420,
     428,   432,   439,   446,   450,   455,   462,   469,   478,   485,
     492,   500,   507,   519,   528,   532,   536,   540,   544,   548,
     552,   559,   566,   572,   578,   585,   589,   603,   616,   620,
     624,   628,   632,   636,   640,   644,   649,   655,   664,   675,
     689,   691,   694,   699,   705,   710,   716,   721,   729,   744,
     754,   769,   779,   784,   788,   794,   799,   804,   809,   814,
     819,   824,   829,   835,   840,   845,   849,   853,   857,   862,
     866,   870,   882,   884,   891,   894,   897,   900,   903,   906,
     911,   916,   921,   925,   928,   927,   932,   934,   942,   950,
     963,   976,   980,   987,   994,  1001,  1008,  1017,  1030,  1043,
    1056,  1071,  1075,  1079,  1080,  1084,  1085,  1089,  1089,  1095,
    1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,
    1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,
    1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,
    1126,  1127,  1128,  1129,  1131,  1135,  1139,  1142,  1146
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BREAK", "FOR", "CONTINUE", "IF", "ELSE",
  "DO", "WHILE", "THIS", "EVAL", "TIME", "RANDOM", "LENGTH", "INT",
  "CONCAT", "DUPLICATECLIP", "REMOVECLIP", "TRACE", "STARTDRAG",
  "STOPDRAG", "ORD", "CHR", "CALLFRAME", "GETURL", "GETURL1", "LOADMOVIE",
  "LOADMOVIENUM", "LOADVARIABLES", "POSTURL", "SUBSTR", "GETPROPERTY",
  "NEXTFRAME", "PREVFRAME", "PLAY", "STOP", "TOGGLEQUALITY", "STOPSOUNDS",
  "GOTOFRAME", "GOTOANDPLAY", "FRAMELOADED", "SETTARGET", "ASM", "ASMADD",
  "ASMDIVIDE", "ASMMULTIPLY", "ASMEQUALS", "ASMLESS", "ASMLOGICALAND",
  "ASMLOGICALOR", "ASMLOGICALNOT", "ASMSTRINGAND", "ASMSTRINGEQUALS",
  "ASMSTRINGEXTRACT", "ASMSTRINGLENGTH", "ASMMBSTRINGEXTRACT",
  "ASMMBSTRINGLENGTH", "ASMPOP", "ASMPUSH", "ASMASCIITOCHAR",
  "ASMCHARTOASCII", "ASMTOINTEGER", "ASMCALL", "ASMIF", "ASMJUMP",
  "ASMGETVARIABLE", "ASMSETVARIABLE", "ASMGETURL2", "ASMGETPROPERTY",
  "ASMGOTOFRAME2", "ASMREMOVESPRITE", "ASMSETPROPERTY", "ASMSETTARGET2",
  "ASMSTARTDRAG", "ASMWAITFORFRAME2", "ASMCLONESPRITE", "ASMENDDRAG",
  "ASMGETTIME", "ASMRANDOMNUMBER", "ASMTRACE", "ASMMBASCIITOCHAR",
  "ASMMBCHARTOASCII", "ASMSUBSTRACT", "ASMSTRINGLESS", "TELLTARGET",
  "BROKENSTRING", "STRING", "NUMBER", "IDENTIFIER", "PATH", "\"==\"",
  "\"<=\"", "\">=\"", "\"!=\"", "\"&&\"", "\"||\"", "\"++\"", "\"--\"",
  "\"+=\"", "\"/=\"", "\"*=\"", "\"-=\"", "\"===\"", "\"!==\"", "\"<=>\"",
  "\"..\"", "\"end\"", "','", "'='", "'?'", "':'", "'<'", "'>'", "'&'",
  "'+'", "'-'", "'*'", "'/'", "'!'", "UMINUS", "POSTFIX", "NEGATE", "'{'",
  "'}'", "';'", "'('", "')'", "'.'", "$accept", "program", "elems", "elem",
  "stmt", "assign_stmts", "statements", "if_stmt", "expr_opt", "iter_stmt",
  "assign_stmts_opt", "cont_stmt", "break_stmt", "urlmethod", "level",
  "void_function_call", "function_call", "pf_expr", "rhs_expr", "variable",
  "sprite", "lhs_expr", "assign_stmt", "@1", "expr", "push_item",
  "push_list", "opcode_list", "opcode", "@2", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,    44,    61,
      63,    58,    60,    62,    38,    43,    45,    42,    47,    33,
     363,   364,   365,   123,   125,    59,    40,    41,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   129,   130,   131,   131,   132,   133,   133,   133,   133,
     133,   133,   133,   133,   134,   134,   135,   135,   135,   136,
     136,   136,   136,   136,   136,   136,   136,   137,   137,   138,
     138,   138,   138,   139,   139,   140,   141,   142,   142,   143,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   146,   146,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   148,   148,   149,   149,   149,   149,   149,   149,
     150,   150,   150,   151,   152,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   153,   154,   155,   155,   156,   156,   158,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     3,     1,     2,
       1,     1,     1,     1,     1,     3,     0,     1,     2,    10,
       8,     9,    10,     8,     9,     7,     5,     0,     1,     9,
       5,     6,     9,     0,     1,     2,     2,     0,     2,     1,
       3,     4,     4,     4,     4,     6,     7,     6,     7,     7,
       7,     6,    14,     8,     3,     3,     3,     3,     3,     3,
       4,     4,     4,     4,     4,     4,     5,     5,     4,     3,
       4,     4,     4,     4,     4,     6,     8,     6,     2,     2,
       1,     3,     1,     2,     1,     1,     1,     3,     4,     2,
       4,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     0,     5,     1,     2,     2,     4,
       4,     3,     3,     3,     3,     3,     5,     5,     5,     5,
       5,     1,     1,     1,     3,     1,     2,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,   114,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   112,   119,     0,     0,   117,   116,     0,     8,     0,
     115,     0,     2,     3,     5,    10,    11,    12,    13,   126,
     123,   120,     0,     0,     0,    36,    33,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   124,     0,     0,   127,     0,   128,     6,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,    82,     0,     0,     0,     0,     0,    80,     0,    85,
      86,     0,     1,     4,     0,     0,    78,    79,     0,     0,
       0,     0,     0,     9,    34,     0,    14,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
      57,    58,    59,    84,    82,     0,     0,    84,     0,     0,
      84,     0,     0,     0,     7,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,    91,
      82,    92,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   122,     0,     0,   113,     0,   134,   133,   132,   135,
     131,     0,    27,     0,     0,     0,     0,     0,     0,     0,
      43,    44,     0,    42,    41,     0,     0,     0,     0,     0,
      61,    60,    62,    63,    64,    65,   149,   152,   151,   153,
     154,   155,   156,   157,   158,   159,   161,   160,   162,   163,
     165,   147,   166,   167,   168,   171,     0,     0,   172,   173,
       0,   174,     0,   176,   175,   177,   178,     0,   180,   179,
     181,   182,   183,   169,   170,   150,   164,     0,   145,     0,
       0,   129,   130,     0,    69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    81,   107,   102,   103,   108,
     109,   110,   105,   104,   106,     0,   100,   101,    99,    97,
      98,    95,    96,    87,    94,     0,     0,     0,     0,     0,
      15,     0,    28,    82,     0,     0,    26,     0,     0,    30,
       0,     0,     0,     0,    37,    37,    39,    37,     0,   184,
     185,   186,   187,   188,   125,   146,    66,    67,    68,    70,
      71,    72,     0,    73,    74,     0,     0,    88,    90,     0,
     139,   138,   137,   140,   136,    33,     0,     0,    82,     0,
       0,    31,     0,     0,     0,    51,     0,    45,     0,    47,
       0,     0,     0,   142,   143,   148,     0,     0,     0,   111,
       0,     0,     0,     0,     0,    25,     0,     0,     0,    38,
      46,    48,    49,    50,     0,    75,     0,    77,     0,    20,
      23,     0,     0,     0,    53,     0,   144,     0,    32,     0,
       0,    21,    24,    29,     0,    76,    19,    22,     0,     0,
       0,     0,    52
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    41,    42,    43,    44,   124,    89,    45,   321,    46,
     125,    47,    48,   378,   335,    49,   107,    50,   129,   109,
     110,   111,    54,   159,   130,   384,   385,   277,   278,   338
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -325
static const yytype_int16 yypact[] =
{
     829,  -118,  -110,   -93,  -109,   829,   -98,  -325,   -86,   -80,
     -79,   -76,   -72,   -71,   -64,   -61,   -57,   -56,   -48,   -47,
     -39,   -38,   -30,   -26,   -25,   -21,   -20,   -19,   -15,   -17,
    -325,   -77,  -325,     8,     8,  -325,  -325,   577,  -325,  1111,
    -325,    84,   829,  -325,  -325,  -325,  -325,  -325,  -325,  -325,
    -325,  -325,   -99,   104,   -40,  -325,   941,  -325,   975,   101,
    1230,  1111,  1111,  1111,  1111,   -14,    -7,  1111,    25,  1111,
    1111,  1111,   -12,   -11,    -5,    -4,    -3,    -2,  1244,  1111,
    1277,  -325,  1291,   -97,  -325,   -90,  -325,  -325,  -325,   703,
      -6,     1,     2,     4,     5,     9,    17,    18,    19,    20,
      38,  -325,     8,     8,  1410,  1111,  1111,  -325,    46,    39,
     -89,    43,  -325,  -325,    64,    65,  -325,  -325,  1111,  1111,
    1111,  1111,  1111,  -325,    47,    41,  -325,    42,  1064,   419,
      44,    48,  1097,    45,    57,    53,    54,    59,  -325,    56,
      60,    58,    62,    80,    81,    82,    83,  -325,  -325,  -325,
    -325,  -325,  -325,  -108,    69,    71,    87,  -101,    89,  1623,
    -100,    90,   103,   118,  -325,  -325,  1111,   100,  1111,  1111,
    1111,  1111,  1111,  1111,  1111,  1111,   -87,  -325,   -75,  -325,
    -325,  -325,  -325,   259,  1111,  1111,  1111,  1111,  1111,  1111,
    1111,  1111,  1111,  1111,  1111,  1111,  1111,  1111,  1111,  1111,
    1111,  -325,   119,  1111,  -325,   -41,   419,   419,   419,   419,
     419,   941,  1111,  1424,   102,   829,  1111,   105,   829,  1111,
    -325,  -325,  1111,  -325,  -325,  1111,   122,  1111,  1111,  1111,
    -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,
    -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,
    -325,  -325,  -325,  -325,  -325,  -325,   141,   144,  -325,  -325,
     146,  -325,   147,  -325,  -325,  -325,  -325,   148,  -325,  -325,
    -325,  -325,  -325,  -325,  -325,  -325,  -325,  1542,  -325,   829,
     829,  -325,  -325,   110,  -325,   111,   112,   113,   134,   116,
     117,   138,   139,   121,   167,   150,   107,   -24,   -24,   107,
     530,   530,   107,   107,   -24,   158,   -24,   -24,   -42,   -74,
     -74,  -325,  -325,  -325,   419,  1111,  1111,  1111,  1111,  1111,
    -325,   132,  -325,   133,   137,  1457,   258,   140,   178,  -325,
     169,  -104,  -102,   151,   174,   174,  -325,   174,   204,  -325,
    -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,  -325,
    -325,  -325,  1111,  -325,  -325,  1111,   205,  -325,  -325,  1111,
     419,   419,   419,   419,   419,   941,   166,   170,   171,   172,
     829,  -325,   173,  1111,  1111,  -325,   209,  -325,   175,  -325,
     176,   177,   179,  -325,  -325,   193,   180,   197,   181,   419,
     185,   829,   829,   186,   187,  -325,   188,   189,   210,  -325,
    -325,  -325,  -325,  -325,   204,  -325,  1111,  -325,   829,   310,
     314,   829,   829,   829,  -325,  1111,  -325,   195,  -325,   829,
     829,  -325,  -325,  -325,   215,  -325,  -325,  -325,  1111,   216,
    1111,   198,  -325
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -325,  -325,  -325,   284,    40,  -325,  -325,  -325,  -325,  -325,
     -36,  -325,  -325,  -324,  -325,  -325,  -325,  -325,   375,     0,
      15,    30,   -54,  -325,   115,   -69,  -325,  -325,    55,  -325
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -123
static const yytype_int16 yytable[] =
{
      51,  -121,   126,     7,   374,    51,   376,    55,  -121,  -121,
     380,   381,   114,   382,   114,    52,    56,    58,     7,   230,
      52,   114,   114,   375,   114,   377,   234,   279,    60,   115,
      53,   162,    57,    51,    51,    53,   114,    51,   163,   202,
      61,   293,    51,   199,   200,    59,    62,    63,    83,    85,
      64,  -118,    52,   294,    65,    66,    51,    52,   315,   316,
     317,   318,    67,    84,    86,    68,   140,    53,   319,    69,
      70,    52,    53,   197,   198,   199,   200,    88,    71,    72,
     139,   141,    31,    32,   112,   123,    53,    73,    74,    51,
     196,   197,   198,   199,   200,    30,    75,    31,    32,    35,
      76,    77,    51,    51,    52,    78,    79,    80,    81,    82,
     131,    36,   143,   138,    35,   147,   148,   176,   178,    53,
     166,    40,   149,   150,   151,   152,    36,   167,   168,   165,
     169,   170,   177,   179,    39,   171,    40,   184,   185,   186,
     187,   188,   189,   172,   173,   174,   175,  -121,  -120,   190,
     191,   192,   203,   204,   205,   211,   193,   320,   194,   195,
     196,   197,   198,   199,   200,   219,   212,   222,   213,   114,
     225,   215,   218,   201,   216,   133,   134,   135,   136,   137,
     220,   221,   142,   223,   144,   145,   146,   224,   226,   227,
     228,   229,   281,   155,   156,   158,   231,   161,   232,   185,
     186,   116,   117,   118,   119,   120,   121,   282,   313,   333,
     357,    51,   192,   122,   233,    51,   235,   280,    51,   194,
     195,   196,   197,   198,   199,   200,    52,   284,   325,   339,
      52,   328,   340,    52,   341,   342,   343,   348,   349,   350,
     351,    53,   352,   353,   354,    53,   355,   356,    53,   184,
     185,   186,   187,   188,   189,   326,   358,   365,   329,  -122,
     366,   190,   191,   192,   367,   370,   372,   371,   193,   359,
     194,   195,   196,   197,   198,   199,   200,   373,   379,    51,
      51,   283,   376,   285,   286,   287,   288,   289,   290,   291,
     292,   383,   388,   391,    52,    52,   399,   392,   393,   394,
     396,   404,   400,   401,   402,   406,   403,   405,   407,    53,
      53,   126,   408,   411,   412,   413,   414,   419,   415,   346,
     347,   420,   425,   428,   430,   432,   113,   322,   324,   390,
       0,   327,   345,     0,   330,   416,     0,   331,     0,     0,
     332,     0,   334,   336,   337,     0,     0,     0,     0,     0,
     184,   185,   186,   187,   188,   189,     0,     0,     0,     0,
       0,     0,   190,   191,   192,    51,     0,     0,     0,   193,
      51,   194,   195,   196,   197,   198,   199,   200,     0,     0,
      52,     0,     0,     0,     0,    52,   295,     0,     0,     0,
       0,    51,    51,     0,     0,    53,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,    52,    52,    51,     0,
     395,    51,    51,    51,   108,     0,     0,     0,     0,    51,
      51,    53,    53,    52,     0,     0,    52,    52,    52,     0,
       0,   409,   410,     0,    52,    52,     0,     0,    53,     0,
     369,    53,    53,    53,     0,     0,     0,     0,   418,    53,
      53,   421,   422,   423,     0,     0,     0,     0,     0,   426,
     427,     0,     0,     0,     0,     0,     0,   386,     0,     0,
     387,     0,     0,     0,     0,     0,     0,     0,     0,   181,
     182,   183,     0,     0,     0,     0,     0,     0,   397,   398,
       0,     0,     0,   206,   207,   208,   209,   210,     0,     0,
       0,     0,     0,   182,     0,     0,     0,   182,     0,     0,
     184,   185,   186,   187,   188,   189,     0,     0,     0,     0,
       0,   417,   190,   191,   192,     0,     0,     0,     0,   193,
     424,   194,   195,   196,   197,   198,   199,   200,     0,     0,
       0,     0,     0,   429,     0,   431,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,     0,     0,   314,     0,
       1,     2,     3,     4,     0,     5,     6,     7,     0,     0,
       0,     0,     0,     0,     8,     9,    10,    11,    12,     0,
       0,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,   184,   185,   186,   187,     0,     0,     0,     0,     0,
       0,     0,     0,   190,   191,   192,     0,     0,     0,     0,
       0,     0,   194,   195,   196,   197,   198,   199,   200,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,    30,     0,    31,    32,     0,     0,
       0,     0,     0,     0,    33,    34,     0,     0,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,     0,     0,
     360,   361,   362,   363,   364,    36,     0,     0,     0,     0,
      37,    87,    38,    39,     0,    40,     1,     2,     3,     4,
       0,     5,     6,     7,     0,     0,     0,     0,     0,     0,
       8,     9,    10,    11,    12,     0,     0,    13,    14,    15,
      16,    17,    18,     0,   389,     0,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
      30,     0,    31,    32,     0,     0,     0,     0,     0,     0,
      33,    34,     0,     0,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,    37,   164,    38,    39,
       0,    40,     1,     2,     3,     4,     0,     5,     6,     7,
       0,     0,     0,     0,     0,     0,     8,     9,    10,    11,
      12,     0,     0,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,    30,     0,    31,    32,
       0,     0,     0,     0,     0,     0,    33,    34,     0,     0,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     7,    37,     0,    38,    39,     0,    40,     8,     9,
      10,    11,    12,     0,     0,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     7,    90,    91,    92,    93,
      94,    95,     0,     0,     0,     0,     0,    96,    97,     0,
       0,     0,     0,     0,     0,     0,    98,    99,     0,     0,
       0,     0,     0,     0,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,    30,     0,
      31,    32,     0,     0,     0,     0,     0,     0,    33,    34,
       0,     0,     0,     0,     0,     0,     0,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
       0,     0,   100,   101,    31,    32,     0,    39,     0,    40,
       0,     0,   102,   103,     7,    90,    91,    92,    93,    94,
      95,    35,     0,     0,     0,     0,    96,    97,     0,     0,
       0,   104,     0,    36,   128,    98,    99,     0,     0,     0,
       0,   106,     0,    40,     0,   214,     0,     7,    90,    91,
      92,    93,    94,    95,     0,     0,     0,     0,     0,    96,
      97,     7,    90,    91,    92,    93,    94,    95,    98,    99,
       0,     0,     0,    96,    97,     0,     0,     0,   217,     0,
       0,     0,    98,    99,     0,     0,     0,     0,     0,     0,
       0,   100,   101,    31,    32,     0,     0,     0,     0,     0,
       0,   102,   103,     0,     0,     0,     0,     0,     0,     0,
      35,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,    36,   105,   100,   101,    31,    32,     0,     0,
     106,     0,    40,     0,   102,   103,     0,     0,   100,   101,
      31,    32,     0,    35,     0,     0,     0,     0,   102,   103,
       0,     0,     0,   104,     0,    36,   105,    35,     0,     0,
       0,     0,     0,   106,     0,    40,     0,   104,     0,    36,
     105,     0,     0,     0,     0,     0,     0,   106,     0,    40,
       7,    90,    91,    92,    93,    94,    95,     0,     0,     0,
       0,     0,    96,    97,     7,    90,    91,    92,    93,    94,
      95,    98,    99,     0,     0,     0,    96,    97,     0,     0,
       0,     0,     0,     0,     0,    98,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,    90,    91,
      92,    93,    94,    95,     0,     0,     0,     0,     0,    96,
      97,     7,    90,    91,    92,    93,    94,    95,    98,    99,
       0,     0,     0,    96,    97,     0,     0,   100,   101,    31,
      32,     0,    98,    99,     0,     0,     0,   102,   103,     0,
       0,   153,   154,    31,    32,     0,    35,     0,     0,     0,
       0,   102,   103,     0,     0,     0,   104,     0,    36,   132,
      35,     0,     0,     0,     0,     0,   106,     0,    40,     0,
     104,     0,    36,   105,   157,   101,    31,    32,     0,     0,
     106,     0,    40,     0,   102,   103,     0,     0,   160,   101,
      31,    32,     0,    35,     0,     0,     0,     0,   102,   103,
       0,     0,     0,   104,     0,    36,   105,    35,     0,     0,
       0,     0,     0,   106,     0,    40,     0,   104,     0,    36,
     105,     0,     0,     0,     0,     0,     0,   106,     0,    40,
       7,    90,    91,    92,    93,    94,    95,     0,     0,     0,
       0,     0,    96,    97,     7,    90,    91,    92,    93,    94,
      95,    98,    99,     0,     0,     0,    96,    97,     0,     0,
       0,     0,     0,     0,     0,    98,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     7,    90,    91,
      92,    93,    94,    95,     0,     0,     0,     0,     0,    96,
      97,     0,     0,     0,     0,     0,     0,     0,    98,    99,
       0,     0,     0,     0,     0,     0,     0,   100,   180,    31,
      32,     0,     0,     0,     0,     0,     0,   102,   103,     0,
       0,   100,   323,    31,    32,     0,    35,     0,     0,     0,
       0,   102,   103,     0,     0,     0,   104,     0,    36,   105,
      35,     0,     0,     0,     0,     0,   106,     0,    40,     0,
     104,     0,    36,   105,   100,   368,    31,    32,     0,     0,
     106,     0,    40,     0,   102,   103,     0,     0,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,    36,   105,     0,     0,     0,
       0,     0,     0,   106,     0,    40,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276
};

static const yytype_int16 yycheck[] =
{
       0,   109,    56,    10,   108,     5,   108,   125,   109,   109,
     334,   335,   111,   337,   111,     0,   126,   126,    10,   127,
       5,   111,   111,   127,   111,   127,   127,   127,   126,   128,
       0,   128,   125,    33,    34,     5,   111,    37,   128,   128,
     126,   128,    42,   117,   118,     5,   126,   126,    33,    34,
     126,   128,    37,   128,   126,   126,    56,    42,    99,   100,
     101,   102,   126,    33,    34,   126,    66,    37,   109,   126,
     126,    56,    42,   115,   116,   117,   118,    37,   126,   126,
      87,    66,    89,    90,     0,   125,    56,   126,   126,    89,
     114,   115,   116,   117,   118,    87,   126,    89,    90,   106,
     126,   126,   102,   103,    89,   126,   126,   126,   123,   126,
       9,   118,    87,   127,   106,   127,   127,   102,   103,    89,
     126,   128,   127,   127,   127,   127,   118,   126,   126,    89,
     126,   126,   102,   103,   126,   126,   128,    91,    92,    93,
      94,    95,    96,   126,   126,   126,   126,   109,   109,   103,
     104,   105,   109,    89,    89,   108,   110,   211,   112,   113,
     114,   115,   116,   117,   118,   108,   125,   108,   126,   111,
     108,   127,   127,   127,   126,    60,    61,    62,    63,    64,
     127,   127,    67,   127,    69,    70,    71,   127,   108,   108,
     108,   108,    89,    78,    79,    80,   127,    82,   127,    92,
      93,    97,    98,    99,   100,   101,   102,    89,    89,    87,
      89,   211,   105,   109,   127,   215,   127,   127,   218,   112,
     113,   114,   115,   116,   117,   118,   211,   127,   126,    88,
     215,   126,    88,   218,    88,    88,    88,   127,   127,   127,
     127,   211,   108,   127,   127,   215,   108,   108,   218,    91,
      92,    93,    94,    95,    96,   215,    89,   125,   218,   109,
     127,   103,   104,   105,   127,     7,    88,   127,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   108,   127,   279,
     280,   166,   108,   168,   169,   170,   171,   172,   173,   174,
     175,    87,    87,   127,   279,   280,    87,   127,   127,   127,
     127,   108,   127,   127,   127,   108,   127,   127,   127,   279,
     280,   365,   127,   127,   127,   127,   127,     7,   108,   279,
     280,     7,   127,   108,   108,   127,    42,   212,   213,   365,
      -1,   216,   277,    -1,   219,   404,    -1,   222,    -1,    -1,
     225,    -1,   227,   228,   229,    -1,    -1,    -1,    -1,    -1,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   365,    -1,    -1,    -1,   110,
     370,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
     365,    -1,    -1,    -1,    -1,   370,   127,    -1,    -1,    -1,
      -1,   391,   392,    -1,    -1,   365,    -1,    -1,    -1,    -1,
     370,    -1,    -1,    -1,    -1,    -1,   391,   392,   408,    -1,
     370,   411,   412,   413,    39,    -1,    -1,    -1,    -1,   419,
     420,   391,   392,   408,    -1,    -1,   411,   412,   413,    -1,
      -1,   391,   392,    -1,   419,   420,    -1,    -1,   408,    -1,
     325,   411,   412,   413,    -1,    -1,    -1,    -1,   408,   419,
     420,   411,   412,   413,    -1,    -1,    -1,    -1,    -1,   419,
     420,    -1,    -1,    -1,    -1,    -1,    -1,   352,    -1,    -1,
     355,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,   106,    -1,    -1,    -1,    -1,    -1,    -1,   373,   374,
      -1,    -1,    -1,   118,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    -1,   132,    -1,    -1,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
      -1,   406,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
     415,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,   428,    -1,   430,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,    -1,    -1,   203,    -1,
       3,     4,     5,     6,    -1,     8,     9,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    19,    20,    21,    -1,
      -1,    24,    25,    26,    27,    28,    29,    -1,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      43,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    87,    -1,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
     315,   316,   317,   318,   319,   118,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,    -1,   128,     3,     4,     5,     6,
      -1,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    21,    -1,    -1,    24,    25,    26,
      27,    28,    29,    -1,   359,    -1,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      87,    -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
      -1,   128,     3,     4,     5,     6,    -1,     8,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    -1,    -1,    24,    25,    26,    27,    28,    29,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    87,    -1,    89,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    10,   123,    -1,   125,   126,    -1,   128,    17,    18,
      19,    20,    21,    -1,    -1,    24,    25,    26,    27,    28,
      29,    -1,    -1,    -1,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    10,    11,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    87,    -1,
      89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    87,    88,    89,    90,    -1,   126,    -1,   128,
      -1,    -1,    97,    98,    10,    11,    12,    13,    14,    15,
      16,   106,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,   116,    -1,   118,   119,    31,    32,    -1,    -1,    -1,
      -1,   126,    -1,   128,    -1,    41,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      23,    10,    11,    12,    13,    14,    15,    16,    31,    32,
      -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,   119,    87,    88,    89,    90,    -1,    -1,
     126,    -1,   128,    -1,    97,    98,    -1,    -1,    87,    88,
      89,    90,    -1,   106,    -1,    -1,    -1,    -1,    97,    98,
      -1,    -1,    -1,   116,    -1,   118,   119,   106,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,    -1,   116,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    23,    10,    11,    12,    13,    14,    15,
      16,    31,    32,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      23,    10,    11,    12,    13,    14,    15,    16,    31,    32,
      -1,    -1,    -1,    22,    23,    -1,    -1,    87,    88,    89,
      90,    -1,    31,    32,    -1,    -1,    -1,    97,    98,    -1,
      -1,    87,    88,    89,    90,    -1,   106,    -1,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,   116,    -1,   118,   119,
     106,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,    -1,
     116,    -1,   118,   119,    87,    88,    89,    90,    -1,    -1,
     126,    -1,   128,    -1,    97,    98,    -1,    -1,    87,    88,
      89,    90,    -1,   106,    -1,    -1,    -1,    -1,    97,    98,
      -1,    -1,    -1,   116,    -1,   118,   119,   106,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,    -1,   116,    -1,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    23,    10,    11,    12,    13,    14,    15,
      16,    31,    32,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,
      -1,    87,    88,    89,    90,    -1,   106,    -1,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,   116,    -1,   118,   119,
     106,    -1,    -1,    -1,    -1,    -1,   126,    -1,   128,    -1,
     116,    -1,   118,   119,    87,    88,    89,    90,    -1,    -1,
     126,    -1,   128,    -1,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,   128,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     8,     9,    10,    17,    18,
      19,    20,    21,    24,    25,    26,    27,    28,    29,    33,
      34,    35,    36,    37,    38,    39,    40,    42,    43,    85,
      87,    89,    90,    97,    98,   106,   118,   123,   125,   126,
     128,   130,   131,   132,   133,   136,   138,   140,   141,   144,
     146,   148,   149,   150,   151,   125,   126,   125,   126,   133,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   123,   126,   149,   150,   149,   150,   124,   133,   135,
      11,    12,    13,    14,    15,    16,    22,    23,    31,    32,
      87,    88,    97,    98,   116,   119,   126,   145,   147,   148,
     149,   150,     0,   132,   111,   128,    97,    98,    99,   100,
     101,   102,   109,   125,   134,   139,   151,    41,   119,   147,
     153,     9,   119,   153,   153,   153,   153,   153,   127,    87,
     148,   149,   153,    87,   153,   153,   153,   127,   127,   127,
     127,   127,   127,    87,    88,   153,   153,    87,   153,   152,
      87,   153,   128,   128,   124,   133,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   149,   150,   149,   150,
      88,   147,   147,   147,    91,    92,    93,    94,    95,    96,
     103,   104,   105,   110,   112,   113,   114,   115,   116,   117,
     118,   127,   128,   109,    89,    89,   147,   147,   147,   147,
     147,   108,   125,   126,    41,   127,   126,    41,   127,   108,
     127,   127,   108,   127,   127,   108,   108,   108,   108,   108,
     127,   127,   127,   127,   127,   127,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,   156,   157,   127,
     127,    89,    89,   153,   127,   153,   153,   153,   153,   153,
     153,   153,   153,   128,   128,   127,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,    89,   147,    99,   100,   101,   102,   109,
     151,   137,   153,    88,   153,   126,   133,   153,   126,   133,
     153,   153,   153,    87,   153,   143,   153,   153,   158,    88,
      88,    88,    88,    88,   124,   157,   133,   133,   127,   127,
     127,   127,   108,   127,   127,   108,   108,    89,    89,   111,
     147,   147,   147,   147,   147,   125,   127,   127,    88,   153,
       7,   127,    88,   108,   108,   127,   108,   127,   142,   127,
     142,   142,   142,    87,   154,   155,   153,   153,    87,   147,
     139,   127,   127,   127,   127,   133,   127,   153,   153,    87,
     127,   127,   127,   127,   108,   127,   108,   127,   127,   133,
     133,   127,   127,   127,   127,   108,   154,   153,   133,     7,
       7,   133,   133,   133,   153,   127,   133,   133,   108,   153,
     108,   153,   127
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
# if YYLTYPE_IS_TRIVIAL
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
#line 164 "./swf4compiler.y"
    { *((Buffer *)buffer) = (yyvsp[(1) - (1)].action); }
    break;

  case 4:
#line 171 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (2)].action), (yyvsp[(2) - (2)].action)); }
    break;

  case 6:
#line 179 "./swf4compiler.y"
    { (yyval.action) = NULL; }
    break;

  case 7:
#line 180 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); }
    break;

  case 8:
#line 181 "./swf4compiler.y"
    { (yyval.action) = NULL; }
    break;

  case 15:
#line 191 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action)); }
    break;

  case 16:
#line 195 "./swf4compiler.y"
    { (yyval.action) = NULL; }
    break;

  case 18:
#line 198 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (2)].action), (yyvsp[(2) - (2)].action)); }
    break;

  case 19:
#line 205 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_WAITFORFRAME);
		  bufferWriteS16((yyval.action), 3);
		  bufferWriteS16((yyval.action), atoi((yyvsp[(5) - (10)].str)));
		  free((yyvsp[(5) - (10)].str));
		  bufferWriteU8((yyval.action), 1);		/* if not loaded, jump to.. */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(10) - (10)].action))+5);
		  bufferConcat((yyval.action), (yyvsp[(10) - (10)].action));			  /* ..here */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(8) - (10)].action)));
		  bufferConcat((yyval.action), (yyvsp[(8) - (10)].action)); }
    break;

  case 20:
#line 221 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_WAITFORFRAME);
		  bufferWriteS16((yyval.action), 3);
		  bufferWriteS16((yyval.action), atoi((yyvsp[(5) - (8)].str)));
		  free((yyvsp[(5) - (8)].str));
		  bufferWriteU8((yyval.action), 1);		/* if not loaded, jump to.. */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), 5);
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);	  /* ..here */
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(8) - (8)].action)));	  /* ..and then out */
		  bufferConcat((yyval.action), (yyvsp[(8) - (8)].action)); }
    break;

  case 21:
#line 237 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_WAITFORFRAME);
		  bufferWriteS16((yyval.action), 3);
		  bufferWriteS16((yyval.action), atoi((yyvsp[(6) - (9)].str)));
		  free((yyvsp[(6) - (9)].str));
		  bufferWriteU8((yyval.action), 1);		/* if not loaded, jump to.. */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(9) - (9)].action)));
		  bufferConcat((yyval.action), (yyvsp[(9) - (9)].action)); }
    break;

  case 22:
#line 249 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(5) - (10)].action);
		  bufferWriteU8((yyval.action), SWFACTION_WAITFORFRAME2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 1);		/* if not loaded, jump to.. */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(10) - (10)].action))+5);
		  bufferConcat((yyval.action), (yyvsp[(10) - (10)].action));			  /* ..here */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(8) - (10)].action)));
		  bufferConcat((yyval.action), (yyvsp[(8) - (10)].action)); }
    break;

  case 23:
#line 263 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(5) - (8)].action);
		  bufferWriteU8((yyval.action), SWFACTION_WAITFORFRAME2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 1);		/* if not loaded, jump to.. */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), 5);
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);	  /* ..here */
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(8) - (8)].action)));	  /* ..and then out */
		  bufferConcat((yyval.action), (yyvsp[(8) - (8)].action)); }
    break;

  case 24:
#line 277 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(6) - (9)].action);
		  bufferWriteU8((yyval.action), SWFACTION_WAITFORFRAME2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 1);		/* if not loaded, jump to.. */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(9) - (9)].action)));
		  bufferConcat((yyval.action), (yyvsp[(9) - (9)].action)); }
    break;

  case 25:
#line 287 "./swf4compiler.y"
    { bufferWriteU8((yyvsp[(3) - (7)].action), SWFACTION_IF);
		  bufferWriteS16((yyvsp[(3) - (7)].action), 2);
		  bufferWriteS16((yyvsp[(3) - (7)].action), bufferLength((yyvsp[(7) - (7)].action))+5);
		  bufferConcat((yyvsp[(3) - (7)].action), (yyvsp[(7) - (7)].action));
		  bufferWriteU8((yyvsp[(3) - (7)].action), SWFACTION_JUMP);
		  bufferWriteS16((yyvsp[(3) - (7)].action), 2);
		  bufferWriteS16((yyvsp[(3) - (7)].action), bufferLength((yyvsp[(5) - (7)].action)));
		  bufferConcat((yyvsp[(3) - (7)].action), (yyvsp[(5) - (7)].action));
		  (yyval.action) = (yyvsp[(3) - (7)].action); }
    break;

  case 26:
#line 298 "./swf4compiler.y"
    { bufferWriteU8((yyvsp[(3) - (5)].action), SWFACTION_LOGICALNOT);
		  bufferWriteU8((yyvsp[(3) - (5)].action), SWFACTION_IF);
		  bufferWriteS16((yyvsp[(3) - (5)].action), 2);
		  bufferWriteS16((yyvsp[(3) - (5)].action), bufferLength((yyvsp[(5) - (5)].action)));
		  bufferConcat((yyvsp[(3) - (5)].action), (yyvsp[(5) - (5)].action));
		  (yyval.action) = (yyvsp[(3) - (5)].action); }
    break;

  case 27:
#line 307 "./swf4compiler.y"
    { (yyval.action) = NULL; }
    break;

  case 28:
#line 308 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action); }
    break;

  case 29:
#line 333 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_WAITFORFRAME);
		  bufferWriteS16((yyval.action), 3);
		  bufferWriteS16((yyval.action), atoi((yyvsp[(6) - (9)].str)));
		  free((yyvsp[(6) - (9)].str));
		  bufferWriteU8((yyval.action), 1);		/* if not loaded, jump to.. */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(9) - (9)].action))+5);
		  bufferConcat((yyval.action), (yyvsp[(9) - (9)].action));				  /* ..here */
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), -(bufferLength((yyval.action))+2)); }
    break;

  case 30:
#line 348 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (5)].action);
		  bufferWriteU8((yyval.action), SWFACTION_LOGICALNOT);
		  bufferWriteU8((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (5)].action))+5);
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), -(bufferLength((yyval.action))+2));
		  bufferResolveJumps((yyval.action)); }
    break;

  case 31:
#line 360 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (6)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferWriteU8((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), -(bufferLength((yyval.action))+2));
		  bufferResolveJumps((yyval.action)); }
    break;

  case 32:
#line 368 "./swf4compiler.y"
    { if (!(yyvsp[(5) - (9)].action))
                    (yyvsp[(5) - (9)].action) = newBuffer();
                  else {
                    bufferWriteU8((yyvsp[(5) - (9)].action), SWFACTION_LOGICALNOT);
                    bufferWriteU8((yyvsp[(5) - (9)].action), SWFACTION_IF);
                    bufferWriteS16((yyvsp[(5) - (9)].action), 2);
                    bufferWriteS16((yyvsp[(5) - (9)].action), bufferLength((yyvsp[(9) - (9)].action))+bufferLength((yyvsp[(7) - (9)].action))+5);
                  }
                  bufferConcat((yyvsp[(5) - (9)].action), (yyvsp[(9) - (9)].action));
                  bufferConcat((yyvsp[(5) - (9)].action), (yyvsp[(7) - (9)].action));
                  bufferWriteU8((yyvsp[(5) - (9)].action), SWFACTION_JUMP);
                  bufferWriteS16((yyvsp[(5) - (9)].action), 2);
                  bufferWriteS16((yyvsp[(5) - (9)].action), -(bufferLength((yyvsp[(5) - (9)].action))+2));
                  bufferResolveJumps((yyvsp[(5) - (9)].action));
                  (yyval.action) = (yyvsp[(3) - (9)].action);
                  if(!(yyval.action)) (yyval.action) = newBuffer();
                  bufferConcat((yyval.action), (yyvsp[(5) - (9)].action));
                }
    break;

  case 33:
#line 389 "./swf4compiler.y"
    { (yyval.action) = NULL; }
    break;

  case 35:
#line 395 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), MAGIC_CONTINUE_NUMBER); }
    break;

  case 36:
#line 403 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), MAGIC_BREAK_NUMBER); }
    break;

  case 37:
#line 411 "./swf4compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_NOSEND; }
    break;

  case 38:
#line 413 "./swf4compiler.y"
    { if(strcmp((yyvsp[(2) - (2)].str), "GET") == 0)
				    (yyval.getURLMethod) = GETURL_METHOD_GET;
				  else if(strcmp((yyvsp[(2) - (2)].str), "POST") == 0)
				    (yyval.getURLMethod) = GETURL_METHOD_POST; }
    break;

  case 39:
#line 421 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), "_level", 7);
		  bufferConcat((yyval.action), (yyvsp[(1) - (1)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STRINGCONCAT); }
    break;

  case 40:
#line 429 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_ENDDRAG); }
    break;

  case 41:
#line 433 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteU8((yyval.action), SWFACTION_CALLFRAME);
		  bufferWriteS16((yyval.action), 0);
		  free((yyvsp[(3) - (4)].str)); }
    break;

  case 42:
#line 440 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteU8((yyval.action), SWFACTION_CALLFRAME);
		  bufferWriteS16((yyval.action), 0);
		  free((yyvsp[(3) - (4)].str)); }
    break;

  case 43:
#line 447 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_REMOVECLIP); }
    break;

  case 44:
#line 451 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_TRACE); }
    break;

  case 45:
#line 456 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (6)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), GETURL_METHOD_NOSEND); }
    break;

  case 46:
#line 463 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), (yyvsp[(6) - (7)].getURLMethod)); }
    break;

  case 47:
#line 470 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_GETURL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (6)].str)) + strlen((yyvsp[(5) - (6)].str)) + 2);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (6)].str), strlen((yyvsp[(3) - (6)].str)));
		  bufferWriteU8((yyval.action), 0);
		  bufferWriteHardString((yyval.action), (yyvsp[(5) - (6)].str), strlen((yyvsp[(5) - (6)].str)));
		  bufferWriteU8((yyval.action), 0); }
    break;

  case 48:
#line 479 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), (yyvsp[(6) - (7)].getURLMethod) | GETURL_LOADMOVIE); }
    break;

  case 49:
#line 486 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), (yyvsp[(6) - (7)].getURLMethod)); }
    break;

  case 50:
#line 493 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0xc0 + (yyvsp[(6) - (7)].getURLMethod)); }
    break;

  case 51:
#line 501 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), "0", 2); /* no constraint */
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (6)].action));
		  bufferWriteU8((yyval.action), SWFACTION_STARTDRAG); }
    break;

  case 52:
#line 508 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferConcat((yyval.action), (yyvsp[(7) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(11) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(9) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(13) - (14)].action));
		  bufferWriteString((yyval.action), "1", 2); /* has constraint */
		  bufferConcat((yyval.action), (yyvsp[(5) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (14)].action));
		  bufferWriteU8((yyval.action), SWFACTION_STARTDRAG); }
    break;

  case 53:
#line 520 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (8)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(7) - (8)].action));
		  bufferWriteWTHITProperty((yyval.action));
		  bufferWriteU8((yyval.action), SWFACTION_ADD); /* see docs for explanation */
		  bufferWriteU8((yyval.action), SWFACTION_DUPLICATECLIP); }
    break;

  case 54:
#line 529 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_NEXTFRAME); }
    break;

  case 55:
#line 533 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_PREVFRAME); }
    break;

  case 56:
#line 537 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_PLAY); }
    break;

  case 57:
#line 541 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_STOP); }
    break;

  case 58:
#line 545 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_TOGGLEQUALITY); }
    break;

  case 59:
#line 549 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_STOPSOUNDS); }
    break;

  case 60:
#line 553 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_GOTOFRAME);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), atoi((yyvsp[(3) - (4)].str)));
		  free((yyvsp[(3) - (4)].str)); }
    break;

  case 61:
#line 560 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_GOTOLABEL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); }
    break;

  case 62:
#line 567 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_GOTOFRAME2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0); }
    break;

  case 63:
#line 573 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_GOTOFRAME2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 1); }
    break;

  case 64:
#line 579 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_SETTARGET);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); }
    break;

  case 65:
#line 586 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_SETTARGET2); }
    break;

  case 66:
#line 590 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
			/* SetTarget(STRING) */
		  bufferWriteU8((yyval.action), SWFACTION_SETTARGET);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (5)].str))+1);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (5)].str), strlen((yyvsp[(3) - (5)].str))+1);
			/* stmt */
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
			/* SetTarget('') */
		  bufferWriteU8((yyval.action), SWFACTION_SETTARGET);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0);
		  free((yyvsp[(3) - (5)].str)); }
    break;

  case 67:
#line 604 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (5)].action);
			/* SetTarget(expr) */
		  bufferWriteU8((yyval.action), SWFACTION_SETTARGET2); 
			/* stmt */
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
			/* SetTarget('') */
		  bufferWriteU8((yyval.action), SWFACTION_SETTARGET);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0); }
    break;

  case 68:
#line 617 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE); }
    break;

  case 69:
#line 621 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteU8((yyval.action), SWFACTION_GETTIME); }
    break;

  case 70:
#line 625 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_RANDOMNUMBER); }
    break;

  case 71:
#line 629 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_STRINGLENGTH); }
    break;

  case 72:
#line 633 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_INT); }
    break;

  case 73:
#line 637 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_ORD); }
    break;

  case 74:
#line 641 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteU8((yyval.action), SWFACTION_CHR); }
    break;

  case 75:
#line 645 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (6)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferWriteU8((yyval.action), SWFACTION_STRINGCONCAT); }
    break;

  case 76:
#line 650 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (8)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(7) - (8)].action));
		  bufferWriteU8((yyval.action), SWFACTION_SUBSTRING); }
    break;

  case 77:
#line 656 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferConcat((yyval.action), (yyvsp[(3) - (6)].action));
		  bufferWriteProperty((yyval.action), (yyvsp[(5) - (6)].str));
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  free((yyvsp[(5) - (6)].str)); }
    break;

  case 78:
#line 665 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE);
		  bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].action));
		  bufferConcat((yyval.action), (yyvsp[(1) - (2)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE);
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_ADD);
		  bufferWriteU8((yyval.action), SWFACTION_SETVARIABLE); }
    break;

  case 79:
#line 676 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE);
		  bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].action));
		  bufferConcat((yyval.action), (yyvsp[(1) - (2)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE);
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_SUBTRACT);
		  bufferWriteU8((yyval.action), SWFACTION_SETVARIABLE); }
    break;

  case 81:
#line 692 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); }
    break;

  case 82:
#line 695 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str)); }
    break;

  case 83:
#line 700 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), "-", 2);
		  bufferWriteString((yyval.action), (yyvsp[(2) - (2)].str), strlen((yyvsp[(2) - (2)].str))+1);
		  free((yyvsp[(2) - (2)].str)); }
    break;

  case 84:
#line 706 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str)); }
    break;

  case 85:
#line 711 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE);
		  free((yyvsp[(1) - (1)].str)); }
    break;

  case 86:
#line 717 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str)); }
    break;

  case 87:
#line 722 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (3)].str), strlen((yyvsp[(1) - (3)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (3)].str));
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  free((yyvsp[(3) - (3)].str));
		  free((yyvsp[(1) - (3)].str)); }
    break;

  case 88:
#line 730 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_ADD);
		  bufferWriteU8((yyval.action), SWFACTION_SETPROPERTY);
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  free((yyvsp[(2) - (4)].str));
		  free((yyvsp[(4) - (4)].str)); }
    break;

  case 89:
#line 745 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteU8((yyval.action), SWFACTION_PUSHDUP);
		  bufferWriteU8((yyval.action), SWFACTION_PUSHDUP);
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE);
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_ADD);
		  bufferWriteU8((yyval.action), SWFACTION_SETVARIABLE);
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE); }
    break;

  case 90:
#line 755 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_ADD);
		  bufferWriteU8((yyval.action), SWFACTION_SETPROPERTY);
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  free((yyvsp[(2) - (4)].str));
		  free((yyvsp[(4) - (4)].str)); }
    break;

  case 91:
#line 770 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteU8((yyval.action), SWFACTION_PUSHDUP);
		  bufferWriteU8((yyval.action), SWFACTION_PUSHDUP);
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE);
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_SUBTRACT);
		  bufferWriteU8((yyval.action), SWFACTION_SETVARIABLE);
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE); }
    break;

  case 92:
#line 780 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteString((yyvsp[(2) - (2)].action), "-1", 3);
		  bufferWriteU8((yyvsp[(2) - (2)].action), SWFACTION_MULTIPLY); }
    break;

  case 93:
#line 785 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteU8((yyvsp[(2) - (2)].action), SWFACTION_LOGICALNOT); }
    break;

  case 94:
#line 789 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyval.action), SWFACTION_PUSHDUP);
		  bufferWriteU8((yyval.action), SWFACTION_SETVARIABLE); }
    break;

  case 95:
#line 795 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyval.action), SWFACTION_MULTIPLY); }
    break;

  case 96:
#line 800 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyval.action), SWFACTION_DIVIDE); }
    break;

  case 97:
#line 805 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyval.action), SWFACTION_ADD); }
    break;

  case 98:
#line 810 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyval.action), SWFACTION_SUBTRACT); }
    break;

  case 99:
#line 815 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyval.action), SWFACTION_STRINGCONCAT); }
    break;

  case 100:
#line 820 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyval.action), SWFACTION_LESSTHAN); }
    break;

  case 101:
#line 825 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(1) - (3)].action));
		  bufferWriteU8((yyval.action), SWFACTION_LESSTHAN); }
    break;

  case 102:
#line 830 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(3) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(1) - (3)].action));
		  bufferWriteU8((yyval.action), SWFACTION_LESSTHAN);
		  bufferWriteU8((yyval.action), SWFACTION_LOGICALNOT); }
    break;

  case 103:
#line 836 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_LESSTHAN);
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); }
    break;

  case 104:
#line 841 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_STRINGEQ);
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); }
    break;

  case 105:
#line 846 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_STRINGEQ); }
    break;

  case 106:
#line 850 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_STRINGCOMPARE); }
    break;

  case 107:
#line 854 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_EQUAL); }
    break;

  case 108:
#line 858 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_EQUAL);
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); }
    break;

  case 109:
#line 863 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_LOGICALAND); }
    break;

  case 110:
#line 867 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_LOGICALOR); }
    break;

  case 111:
#line 871 "./swf4compiler.y"
    { bufferWriteU8((yyvsp[(1) - (5)].action), SWFACTION_IF);
		  bufferWriteS16((yyvsp[(1) - (5)].action), 2);
		  bufferWriteS16((yyvsp[(1) - (5)].action), bufferLength((yyvsp[(5) - (5)].action))+5);
		  bufferConcat((yyvsp[(1) - (5)].action), (yyvsp[(5) - (5)].action));
		  bufferWriteU8((yyvsp[(1) - (5)].action), SWFACTION_JUMP);
		  bufferWriteS16((yyvsp[(1) - (5)].action), 2);
		  bufferWriteS16((yyvsp[(1) - (5)].action), bufferLength((yyvsp[(3) - (5)].action)));
		  bufferConcat((yyvsp[(1) - (5)].action), (yyvsp[(3) - (5)].action)); }
    break;

  case 113:
#line 885 "./swf4compiler.y"
    { (yyval.str) = (yyvsp[(1) - (3)].str);
		  (yyval.str) = stringConcat((yyval.str), strdup(":"));
		  (yyval.str) = stringConcat((yyval.str), (yyvsp[(3) - (3)].str)); }
    break;

  case 114:
#line 892 "./swf4compiler.y"
    { (yyval.str) = strdup(""); }
    break;

  case 115:
#line 895 "./swf4compiler.y"
    { (yyval.str) = strdup(""); }
    break;

  case 116:
#line 898 "./swf4compiler.y"
    { (yyval.str) = strdup("/"); }
    break;

  case 117:
#line 901 "./swf4compiler.y"
    { (yyval.str) = strdup(".."); }
    break;

  case 118:
#line 904 "./swf4compiler.y"
    { (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 119:
#line 907 "./swf4compiler.y"
    { (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 120:
#line 912 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str)); }
    break;

  case 121:
#line 917 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str)); }
    break;

  case 122:
#line 921 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); }
    break;

  case 124:
#line 928 "./swf4compiler.y"
    { asmBuffer = newBuffer(); }
    break;

  case 125:
#line 930 "./swf4compiler.y"
    { (yyval.action) = asmBuffer; }
    break;

  case 127:
#line 935 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteBuffer((yyval.action), (yyvsp[(2) - (2)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE);
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_ADD);
		  bufferWriteU8((yyval.action), SWFACTION_SETVARIABLE); }
    break;

  case 128:
#line 943 "./swf4compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteBuffer((yyval.action), (yyvsp[(2) - (2)].action));
		  bufferWriteU8((yyval.action), SWFACTION_GETVARIABLE);
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_SUBTRACT);
		  bufferWriteU8((yyval.action), SWFACTION_SETVARIABLE); }
    break;

  case 129:
#line 951 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_ADD);
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteU8((yyval.action), SWFACTION_SETPROPERTY);
		  free((yyvsp[(2) - (4)].str));
		  free((yyvsp[(4) - (4)].str)); }
    break;

  case 130:
#line 964 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  bufferWriteString((yyval.action), "1", 2);
		  bufferWriteU8((yyval.action), SWFACTION_SUBTRACT);
		  bufferWriteString((yyval.action), (yyvsp[(2) - (4)].str), strlen((yyvsp[(2) - (4)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(4) - (4)].str));
		  bufferWriteU8((yyval.action), SWFACTION_SETPROPERTY);
		  free((yyvsp[(2) - (4)].str));
		  free((yyvsp[(4) - (4)].str)); }
    break;

  case 131:
#line 977 "./swf4compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_SETVARIABLE); }
    break;

  case 132:
#line 981 "./swf4compiler.y"
    { bufferWriteBuffer((yyvsp[(1) - (3)].action), (yyvsp[(1) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_GETVARIABLE);
		  bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_MULTIPLY);
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_SETVARIABLE); }
    break;

  case 133:
#line 988 "./swf4compiler.y"
    { bufferWriteBuffer((yyvsp[(1) - (3)].action), (yyvsp[(1) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_GETVARIABLE);
		  bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_DIVIDE);
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_SETVARIABLE); }
    break;

  case 134:
#line 995 "./swf4compiler.y"
    { bufferWriteBuffer((yyvsp[(1) - (3)].action), (yyvsp[(1) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_GETVARIABLE);
		  bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_ADD);
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_SETVARIABLE); }
    break;

  case 135:
#line 1002 "./swf4compiler.y"
    { bufferWriteBuffer((yyvsp[(1) - (3)].action), (yyvsp[(1) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_GETVARIABLE);
		  bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_SUBTRACT);
		  bufferWriteU8((yyvsp[(1) - (3)].action), SWFACTION_SETVARIABLE); }
    break;

  case 136:
#line 1009 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (5)].str), strlen((yyvsp[(1) - (5)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (5)].str));
		  bufferConcat((yyval.action),(yyvsp[(5) - (5)].action));
		  bufferWriteU8((yyval.action), SWFACTION_SETPROPERTY);
		  free((yyvsp[(1) - (5)].str));
		  free((yyvsp[(3) - (5)].str)); }
    break;

  case 137:
#line 1018 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (5)].str), strlen((yyvsp[(1) - (5)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (5)].str));
		  bufferWriteString((yyval.action), (yyvsp[(1) - (5)].str), strlen((yyvsp[(1) - (5)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (5)].str));
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
		  bufferWriteU8((yyval.action), SWFACTION_MULTIPLY);
		  bufferWriteU8((yyval.action), SWFACTION_SETPROPERTY);
		  free((yyvsp[(1) - (5)].str));
		  free((yyvsp[(3) - (5)].str)); }
    break;

  case 138:
#line 1031 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (5)].str), strlen((yyvsp[(1) - (5)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (5)].str));
		  bufferWriteString((yyval.action), (yyvsp[(1) - (5)].str), strlen((yyvsp[(1) - (5)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (5)].str));
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
		  bufferWriteU8((yyval.action), SWFACTION_DIVIDE);
		  bufferWriteU8((yyval.action), SWFACTION_SETPROPERTY);
		  free((yyvsp[(1) - (5)].str));
		  free((yyvsp[(3) - (5)].str)); }
    break;

  case 139:
#line 1044 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (5)].str), strlen((yyvsp[(1) - (5)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (5)].str));
		  bufferWriteString((yyval.action), (yyvsp[(1) - (5)].str), strlen((yyvsp[(1) - (5)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (5)].str));
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
		  bufferWriteU8((yyval.action), SWFACTION_ADD);
		  bufferWriteU8((yyval.action), SWFACTION_SETPROPERTY);
		  free((yyvsp[(1) - (5)].str));
		  free((yyvsp[(3) - (5)].str)); }
    break;

  case 140:
#line 1057 "./swf4compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (5)].str), strlen((yyvsp[(1) - (5)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (5)].str));
		  bufferWriteString((yyval.action), (yyvsp[(1) - (5)].str), strlen((yyvsp[(1) - (5)].str))+1);
		  bufferWriteProperty((yyval.action), (yyvsp[(3) - (5)].str));
		  bufferWriteU8((yyval.action), SWFACTION_GETPROPERTY);
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
		  bufferWriteU8((yyval.action), SWFACTION_SUBTRACT);
		  bufferWriteU8((yyval.action), SWFACTION_SETPROPERTY);
		  free((yyvsp[(1) - (5)].str));
		  free((yyvsp[(3) - (5)].str)); }
    break;

  case 142:
#line 1075 "./swf4compiler.y"
    { (yyval.len) = bufferWriteU8(asmBuffer, PUSH_STRING);
				  (yyval.len) += bufferWriteHardString(asmBuffer, (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str)) + 1); }
    break;

  case 143:
#line 1079 "./swf4compiler.y"
    { (yyval.len) = (yyvsp[(1) - (1)].len); }
    break;

  case 144:
#line 1080 "./swf4compiler.y"
    { (yyval.len) = (yyvsp[(1) - (3)].len) + (yyvsp[(3) - (3)].len); }
    break;

  case 146:
#line 1085 "./swf4compiler.y"
    { (yyval.len) = (yyvsp[(1) - (2)].len) + (yyvsp[(2) - (2)].len); }
    break;

  case 147:
#line 1089 "./swf4compiler.y"
    { (yyval.len) = bufferWritePushOp(asmBuffer);
				  (yyval.len) += bufferWriteS16(asmBuffer, 0); }
    break;

  case 148:
#line 1091 "./swf4compiler.y"
    { (yyval.len) = (yyvsp[(2) - (3)].len) + (yyvsp[(3) - (3)].len);
			
				  bufferPatchLength(asmBuffer, (yyvsp[(3) - (3)].len)); }
    break;

  case 149:
#line 1095 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_ADD); }
    break;

  case 150:
#line 1096 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SUBTRACT); }
    break;

  case 151:
#line 1097 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MULTIPLY); }
    break;

  case 152:
#line 1098 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_DIVIDE); }
    break;

  case 153:
#line 1099 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_EQUAL); }
    break;

  case 154:
#line 1100 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LESSTHAN); }
    break;

  case 155:
#line 1101 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALAND); }
    break;

  case 156:
#line 1102 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALOR); }
    break;

  case 157:
#line 1103 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALNOT); }
    break;

  case 158:
#line 1104 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGCONCAT); }
    break;

  case 159:
#line 1105 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGEQ); }
    break;

  case 160:
#line 1106 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGLENGTH); }
    break;

  case 161:
#line 1107 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SUBSTRING); }
    break;

  case 162:
#line 1108 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBSUBSTRING); }
    break;

  case 163:
#line 1109 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBLENGTH); }
    break;

  case 164:
#line 1110 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGCOMPARE); }
    break;

  case 165:
#line 1111 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_POP); }
    break;

  case 166:
#line 1112 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_CHR); }
    break;

  case 167:
#line 1113 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_ORD); }
    break;

  case 168:
#line 1114 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_INT); }
    break;

  case 169:
#line 1115 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBCHR); }
    break;

  case 170:
#line 1116 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBORD); }
    break;

  case 171:
#line 1117 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_CALLFRAME); }
    break;

  case 172:
#line 1118 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETVARIABLE); }
    break;

  case 173:
#line 1119 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETVARIABLE); }
    break;

  case 174:
#line 1120 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETPROPERTY); }
    break;

  case 175:
#line 1121 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETPROPERTY); }
    break;

  case 176:
#line 1122 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_REMOVECLIP); }
    break;

  case 177:
#line 1123 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETTARGET2); }
    break;

  case 178:
#line 1124 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STARTDRAG); }
    break;

  case 179:
#line 1125 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_ENDDRAG); }
    break;

  case 180:
#line 1126 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_DUPLICATECLIP); }
    break;

  case 181:
#line 1127 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETTIME); }
    break;

  case 182:
#line 1128 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_RANDOMNUMBER); }
    break;

  case 183:
#line 1129 "./swf4compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_TRACE); }
    break;

  case 184:
#line 1131 "./swf4compiler.y"
    { (yyval.len) = ( 
					bufferWriteOp(asmBuffer, SWFACTION_IF)
					+ bufferWriteS16(asmBuffer, 2)
					+ bufferWriteS16(asmBuffer, atoi((yyvsp[(2) - (2)].str)))); }
    break;

  case 185:
#line 1135 "./swf4compiler.y"
    { (yyval.len) =  ( 
					bufferWriteOp(asmBuffer, SWFACTION_JUMP)
					+ bufferWriteS16(asmBuffer, 2)
					+ bufferWriteS16(asmBuffer, atoi((yyvsp[(2) - (2)].str)))); }
    break;

  case 186:
#line 1139 "./swf4compiler.y"
    { (yyval.len) =  (bufferWriteOp(asmBuffer, SWFACTION_GETURL2)
					+ bufferWriteS16(asmBuffer, 1) 
					+ bufferWriteU8(asmBuffer, atoi((yyvsp[(2) - (2)].str)))); }
    break;

  case 187:
#line 1142 "./swf4compiler.y"
    { (yyval.len) =  (bufferWriteOp(asmBuffer, SWFACTION_GOTOFRAME2) 
					+ bufferWriteS16(asmBuffer, 1)
					+ bufferWriteU8(asmBuffer, atoi((yyvsp[(2) - (2)].str)))); 
					/* SceneBias missing */ }
    break;

  case 188:
#line 1146 "./swf4compiler.y"
    { (yyval.len) = (bufferWriteOp(asmBuffer, SWFACTION_WAITFORFRAME2) 
					+ bufferWriteS16(asmBuffer, 1)
					+ bufferWriteU8(asmBuffer, atoi((yyvsp[(2) - (2)].str)))); }
    break;


/* Line 1267 of yacc.c.  */
#line 3531 "swf4compiler.tab.c"
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


#line 1152 "./swf4compiler.y"


