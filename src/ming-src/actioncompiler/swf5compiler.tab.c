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
#define yyparse swf5parse
#define yylex   swf5lex
#define yyerror swf5error
#define yylval  swf5lval
#define yychar  swf5char
#define yydebug swf5debug
#define yynerrs swf5nerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BREAK = 258,
     CONTINUE = 259,
     FUNCTION = 260,
     ELSE = 261,
     SWITCH = 262,
     CASE = 263,
     DEFAULT = 264,
     FOR = 265,
     IN = 266,
     IF = 267,
     WHILE = 268,
     DO = 269,
     VAR = 270,
     NEW = 271,
     DELETE = 272,
     TARGETPATH = 273,
     RETURN = 274,
     END = 275,
     WITH = 276,
     ASM = 277,
     EVAL = 278,
     RANDOM = 279,
     GETTIMER = 280,
     LENGTH = 281,
     CONCAT = 282,
     SUBSTR = 283,
     TRACE = 284,
     INT = 285,
     ORD = 286,
     CHR = 287,
     GETURL = 288,
     GETURL1 = 289,
     NEXTFRAME = 290,
     PREVFRAME = 291,
     PLAY = 292,
     STOP = 293,
     TOGGLEQUALITY = 294,
     STOPSOUNDS = 295,
     DUP = 296,
     SWAP = 297,
     POP = 298,
     PUSH = 299,
     SETREGISTER = 300,
     CALLFUNCTION = 301,
     CALLMETHOD = 302,
     AND = 303,
     OR = 304,
     XOR = 305,
     MODULO = 306,
     ADD = 307,
     SWFLESSTHAN = 308,
     EQUALS = 309,
     INC = 310,
     DEC = 311,
     TYPEOF = 312,
     INSTANCEOF = 313,
     ENUMERATE = 314,
     INITOBJECT = 315,
     INITARRAY = 316,
     GETMEMBER = 317,
     SETMEMBER = 318,
     SHIFTLEFT = 319,
     SHIFTRIGHT = 320,
     SHIFTRIGHT2 = 321,
     VAREQUALS = 322,
     OLDADD = 323,
     SUBTRACT = 324,
     MULTIPLY = 325,
     DIVIDE = 326,
     OLDEQUALS = 327,
     OLDLESSTHAN = 328,
     LOGICALAND = 329,
     LOGICALOR = 330,
     NOT = 331,
     STRINGEQ = 332,
     STRINGLENGTH = 333,
     SUBSTRING = 334,
     GETVARIABLE = 335,
     SETVARIABLE = 336,
     SETTARGETEXPRESSION = 337,
     DUPLICATEMOVIECLIP = 338,
     REMOVEMOVIECLIP = 339,
     STRINGLESSTHAN = 340,
     MBLENGTH = 341,
     MBSUBSTRING = 342,
     MBORD = 343,
     MBCHR = 344,
     BRANCHALWAYS = 345,
     BRANCHIFTRUE = 346,
     GETURL2 = 347,
     POST = 348,
     GET = 349,
     LOADVARIABLES = 350,
     LOADMOVIE = 351,
     LOADVARIABLESNUM = 352,
     LOADMOVIENUM = 353,
     CALLFRAME = 354,
     STARTDRAG = 355,
     STOPDRAG = 356,
     GOTOANDSTOP = 357,
     GOTOANDPLAY = 358,
     SETTARGET = 359,
     GETPROPERTY = 360,
     SETPROPERTY = 361,
     TONUMBER = 362,
     TOSTRING = 363,
     TRY = 364,
     THROW = 365,
     CATCH = 366,
     FINALLY = 367,
     EXTENDS = 368,
     NULLVAL = 369,
     UNDEFINED = 370,
     INTEGER = 371,
     DOUBLE = 372,
     BOOLEAN = 373,
     REGISTER = 374,
     STRING = 375,
     IDENTIFIER = 376,
     EQ = 377,
     EEQ = 378,
     NEE = 379,
     LE = 380,
     GE = 381,
     NE = 382,
     LAN = 383,
     LOR = 384,
     INCR = 385,
     DECR = 386,
     IEQ = 387,
     DEQ = 388,
     MEQ = 389,
     SEQ = 390,
     REQ = 391,
     AEQ = 392,
     OEQ = 393,
     SHL = 394,
     SHR = 395,
     SHR2 = 396,
     SHLEQ = 397,
     SHREQ = 398,
     SHR2EQ = 399,
     NOELSE = 400,
     UMINUS = 402,
     POSTFIX = 403
   };
#endif
/* Tokens.  */
#define BREAK 258
#define CONTINUE 259
#define FUNCTION 260
#define ELSE 261
#define SWITCH 262
#define CASE 263
#define DEFAULT 264
#define FOR 265
#define IN 266
#define IF 267
#define WHILE 268
#define DO 269
#define VAR 270
#define NEW 271
#define DELETE 272
#define TARGETPATH 273
#define RETURN 274
#define END 275
#define WITH 276
#define ASM 277
#define EVAL 278
#define RANDOM 279
#define GETTIMER 280
#define LENGTH 281
#define CONCAT 282
#define SUBSTR 283
#define TRACE 284
#define INT 285
#define ORD 286
#define CHR 287
#define GETURL 288
#define GETURL1 289
#define NEXTFRAME 290
#define PREVFRAME 291
#define PLAY 292
#define STOP 293
#define TOGGLEQUALITY 294
#define STOPSOUNDS 295
#define DUP 296
#define SWAP 297
#define POP 298
#define PUSH 299
#define SETREGISTER 300
#define CALLFUNCTION 301
#define CALLMETHOD 302
#define AND 303
#define OR 304
#define XOR 305
#define MODULO 306
#define ADD 307
#define SWFLESSTHAN 308
#define EQUALS 309
#define INC 310
#define DEC 311
#define TYPEOF 312
#define INSTANCEOF 313
#define ENUMERATE 314
#define INITOBJECT 315
#define INITARRAY 316
#define GETMEMBER 317
#define SETMEMBER 318
#define SHIFTLEFT 319
#define SHIFTRIGHT 320
#define SHIFTRIGHT2 321
#define VAREQUALS 322
#define OLDADD 323
#define SUBTRACT 324
#define MULTIPLY 325
#define DIVIDE 326
#define OLDEQUALS 327
#define OLDLESSTHAN 328
#define LOGICALAND 329
#define LOGICALOR 330
#define NOT 331
#define STRINGEQ 332
#define STRINGLENGTH 333
#define SUBSTRING 334
#define GETVARIABLE 335
#define SETVARIABLE 336
#define SETTARGETEXPRESSION 337
#define DUPLICATEMOVIECLIP 338
#define REMOVEMOVIECLIP 339
#define STRINGLESSTHAN 340
#define MBLENGTH 341
#define MBSUBSTRING 342
#define MBORD 343
#define MBCHR 344
#define BRANCHALWAYS 345
#define BRANCHIFTRUE 346
#define GETURL2 347
#define POST 348
#define GET 349
#define LOADVARIABLES 350
#define LOADMOVIE 351
#define LOADVARIABLESNUM 352
#define LOADMOVIENUM 353
#define CALLFRAME 354
#define STARTDRAG 355
#define STOPDRAG 356
#define GOTOANDSTOP 357
#define GOTOANDPLAY 358
#define SETTARGET 359
#define GETPROPERTY 360
#define SETPROPERTY 361
#define TONUMBER 362
#define TOSTRING 363
#define TRY 364
#define THROW 365
#define CATCH 366
#define FINALLY 367
#define EXTENDS 368
#define NULLVAL 369
#define UNDEFINED 370
#define INTEGER 371
#define DOUBLE 372
#define BOOLEAN 373
#define REGISTER 374
#define STRING 375
#define IDENTIFIER 376
#define EQ 377
#define EEQ 378
#define NEE 379
#define LE 380
#define GE 381
#define NE 382
#define LAN 383
#define LOR 384
#define INCR 385
#define DECR 386
#define IEQ 387
#define DEQ 388
#define MEQ 389
#define SEQ 390
#define REQ 391
#define AEQ 392
#define OEQ 393
#define SHL 394
#define SHR 395
#define SHR2 396
#define SHLEQ 397
#define SHREQ 398
#define SHR2EQ 399
#define NOELSE 400
#define UMINUS 402
#define POSTFIX 403




/* Copy the first part of user declarations.  */
#line 5 "./swf5compiler.y"


#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "compile.h"
#include "actiontypes.h"
#include "assembler.h"

#define YYPARSE_PARAM buffer
//#define DEBUG 1

Buffer bf, bc;



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
#line 22 "./swf5compiler.y"
{
  Buffer action;
  char *str;
  SWFGetUrl2Method getURLMethod;
  int op;
  int intVal;
  int len;
  double doubleVal;

  struct
  {
    Buffer buffer;
    int count;
  } exprlist;
  struct switchcase   switchcase;
  struct switchcases switchcases;
  struct
  {
	Buffer obj, ident, memexpr;
  } lval;
}
/* Line 187 of yacc.c.  */
#line 436 "swf5compiler.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 449 "swf5compiler.tab.c"

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   6423

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  173
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  60
/* YYNRULES -- Number of rules.  */
#define YYNRULES  378
/* YYNRULES -- Number of states.  */
#define YYNSTATES  678

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   403

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   161,     2,     2,     2,   160,   153,     2,
     171,   172,   158,   156,   146,   157,   165,   159,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   150,   170,
     151,   147,   152,   149,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   166,     2,   167,   155,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   168,   154,   169,   162,     2,     2,     2,
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
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   148,   163,   164
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     8,    10,    13,    15,    17,
      19,    22,    25,    27,    31,    33,    36,    38,    40,    42,
      44,    46,    48,    50,    53,    55,    59,    62,    70,    75,
      85,    93,    96,   100,   102,   106,   114,   120,   121,   123,
     125,   133,   134,   137,   144,   149,   153,   155,   157,   159,
     161,   163,   165,   167,   169,   171,   173,   175,   177,   179,
     181,   183,   185,   187,   189,   191,   193,   195,   197,   199,
     201,   203,   205,   207,   209,   211,   213,   215,   217,   219,
     221,   223,   225,   227,   229,   231,   233,   235,   237,   239,
     241,   243,   245,   247,   249,   251,   253,   255,   257,   259,
     261,   263,   265,   267,   269,   271,   273,   275,   277,   279,
     281,   283,   285,   287,   289,   291,   293,   295,   297,   299,
     301,   303,   305,   307,   309,   311,   313,   315,   317,   318,
     320,   324,   326,   333,   336,   338,   340,   342,   344,   346,
     347,   348,   354,   361,   372,   380,   389,   390,   392,   395,
     398,   399,   402,   405,   408,   410,   412,   417,   422,   425,
     430,   436,   441,   446,   454,   462,   470,   478,   486,   491,
     498,   513,   517,   526,   531,   538,   542,   546,   550,   554,
     558,   562,   567,   572,   577,   582,   587,   592,   597,   602,
     607,   612,   615,   620,   626,   631,   635,   640,   645,   650,
     655,   660,   667,   676,   681,   682,   684,   688,   694,   701,
     709,   713,   715,   719,   721,   723,   725,   727,   729,   731,
     733,   735,   737,   739,   741,   743,   745,   747,   749,   751,
     753,   757,   762,   764,   767,   770,   773,   777,   781,   785,
     789,   793,   797,   801,   805,   809,   813,   817,   821,   825,
     829,   833,   837,   841,   845,   849,   853,   857,   863,   867,
     871,   873,   876,   882,   887,   893,   901,   910,   914,   916,
     920,   922,   924,   927,   930,   934,   937,   939,   942,   944,
     946,   948,   950,   952,   956,   958,   962,   966,   968,   969,
     975,   978,   980,   982,   984,   987,   990,   994,   998,  1000,
    1003,  1004,  1009,  1011,  1013,  1015,  1017,  1019,  1021,  1023,
    1025,  1029,  1030,  1034,  1036,  1039,  1041,  1043,  1045,  1047,
    1049,  1051,  1053,  1055,  1057,  1059,  1061,  1063,  1065,  1067,
    1069,  1071,  1073,  1075,  1077,  1079,  1081,  1083,  1085,  1087,
    1089,  1091,  1093,  1095,  1097,  1099,  1101,  1103,  1105,  1107,
    1109,  1111,  1113,  1115,  1117,  1119,  1121,  1123,  1125,  1127,
    1129,  1131,  1133,  1135,  1137,  1139,  1141,  1143,  1145,  1147,
    1149,  1151,  1153,  1155,  1157,  1159,  1161,  1163,  1166
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     174,     0,    -1,    -1,   175,   176,    -1,    -1,   177,    -1,
     176,   177,    -1,   180,    -1,   195,    -1,   180,    -1,   178,
     180,    -1,   168,   169,    -1,   179,    -1,   168,   178,   169,
      -1,   170,    -1,   224,   170,    -1,   186,    -1,   202,    -1,
     204,    -1,   205,    -1,   189,    -1,   184,    -1,   183,    -1,
     182,   170,    -1,   181,    -1,   110,   220,   170,    -1,   109,
     180,    -1,   109,   180,   111,   171,   192,   172,   180,    -1,
     109,   180,   112,   180,    -1,   109,   180,   111,   171,   192,
     172,   180,   112,   180,    -1,    21,   171,   219,   172,   168,
     178,   169,    -1,    19,   170,    -1,    19,   220,   170,    -1,
     224,    -1,   185,   146,   224,    -1,    12,   171,   219,   172,
     180,     6,   180,    -1,    12,   171,   219,   172,   180,    -1,
      -1,   219,    -1,     7,    -1,   188,   171,   219,   172,   168,
     190,   169,    -1,    -1,   190,   191,    -1,     8,   219,   150,
     178,     3,   170,    -1,     8,   219,   150,   178,    -1,     9,
     150,   178,    -1,   121,    -1,    16,    -1,    17,    -1,    18,
      -1,    24,    -1,    25,    -1,    26,    -1,    27,    -1,    28,
      -1,    29,    -1,    30,    -1,    31,    -1,    32,    -1,    33,
      -1,    34,    -1,    35,    -1,    36,    -1,    37,    -1,    38,
      -1,    39,    -1,    40,    -1,    41,    -1,    42,    -1,    43,
      -1,    44,    -1,    45,    -1,    46,    -1,    47,    -1,    48,
      -1,    49,    -1,    50,    -1,    51,    -1,    52,    -1,    53,
      -1,    54,    -1,    55,    -1,    56,    -1,    57,    -1,    58,
      -1,    59,    -1,    60,    -1,    61,    -1,    62,    -1,    63,
      -1,    64,    -1,    65,    -1,    66,    -1,    67,    -1,    68,
      -1,    69,    -1,    70,    -1,    71,    -1,    72,    -1,    73,
      -1,    74,    -1,    75,    -1,    76,    -1,    77,    -1,    78,
      -1,    79,    -1,    80,    -1,    81,    -1,    82,    -1,    83,
      -1,    84,    -1,   100,    -1,   101,    -1,    85,    -1,    86,
      -1,    87,    -1,    88,    -1,    89,    -1,    90,    -1,    91,
      -1,    92,    -1,    93,    -1,    94,    -1,    95,    -1,    96,
      -1,   113,    -1,   102,    -1,   103,    -1,    -1,   192,    -1,
     193,   146,   192,    -1,     5,    -1,   194,   192,   171,   193,
     172,   180,    -1,    11,   197,    -1,   218,    -1,   209,    -1,
     212,    -1,    13,    -1,    14,    -1,    -1,    -1,   198,   171,
     219,   172,   180,    -1,   199,   180,    13,   171,   219,   172,
      -1,    10,   171,   203,   170,   187,   170,   203,   172,   200,
     180,    -1,    10,   171,   192,   196,   172,   201,   180,    -1,
      10,   171,    15,   192,   196,   172,   201,   180,    -1,    -1,
     185,    -1,     4,   170,    -1,     3,   170,    -1,    -1,   146,
      94,    -1,   146,    93,    -1,   146,   120,    -1,   116,    -1,
     219,    -1,   192,   171,   210,   172,    -1,    18,   171,   121,
     172,    -1,    17,   121,    -1,    17,   217,   165,   121,    -1,
      17,   217,   166,   219,   167,    -1,    29,   171,   220,   172,
      -1,    33,   171,   219,   172,    -1,    33,   171,   219,   146,
     219,   206,   172,    -1,    95,   171,   219,   146,   219,   206,
     172,    -1,    97,   171,   219,   146,   207,   206,   172,    -1,
      96,   171,   219,   146,   219,   206,   172,    -1,    98,   171,
     219,   146,   207,   206,   172,    -1,    99,   171,   219,   172,
      -1,   100,   171,   219,   146,   219,   172,    -1,   100,   171,
     219,   146,   219,   146,   219,   146,   219,   146,   219,   146,
     219,   172,    -1,   101,   171,   172,    -1,    83,   171,   219,
     146,   219,   146,   219,   172,    -1,    84,   171,   219,   172,
      -1,    34,   171,   120,   146,   120,   172,    -1,    35,   171,
     172,    -1,    36,   171,   172,    -1,    37,   171,   172,    -1,
      38,   171,   172,    -1,    40,   171,   172,    -1,    39,   171,
     172,    -1,   103,   171,   116,   172,    -1,   102,   171,   116,
     172,    -1,   103,   171,   120,   172,    -1,   102,   171,   120,
     172,    -1,   103,   171,   219,   172,    -1,   102,   171,   219,
     172,    -1,   104,   171,   120,   172,    -1,   104,   171,   219,
     172,    -1,   121,   171,   210,   172,    -1,    18,   171,   121,
     172,    -1,    17,   121,    -1,    17,   217,   165,   121,    -1,
      17,   217,   166,   219,   167,    -1,    23,   171,   219,   172,
      -1,    25,   171,   172,    -1,    24,   171,   219,   172,    -1,
      26,   171,   220,   172,    -1,    30,   171,   219,   172,    -1,
      31,   171,   219,   172,    -1,    32,   171,   219,   172,    -1,
      27,   171,   219,   146,   219,   172,    -1,    79,   171,   219,
     146,   219,   146,   219,   172,    -1,    57,   171,   220,   172,
      -1,    -1,   220,    -1,   210,   146,   220,    -1,   194,   171,
     193,   172,   180,    -1,   217,   165,   192,   171,   210,   172,
      -1,   217,   166,   219,   167,   171,   210,   172,    -1,   192,
     150,   220,    -1,   213,    -1,   214,   146,   213,    -1,   132,
      -1,   135,    -1,   134,    -1,   133,    -1,   136,    -1,   137,
      -1,   138,    -1,   148,    -1,   142,    -1,   143,    -1,   144,
      -1,   130,    -1,   131,    -1,   218,    -1,   209,    -1,   212,
      -1,   192,    -1,   217,   165,   192,    -1,   217,   166,   219,
     167,    -1,   221,    -1,   157,   219,    -1,   162,   219,    -1,
     161,   219,    -1,   219,   129,   219,    -1,   219,   128,   219,
      -1,   219,   158,   219,    -1,   219,   159,   219,    -1,   219,
     160,   219,    -1,   219,   156,   219,    -1,   219,   157,   219,
      -1,   219,   153,   219,    -1,   219,   154,   219,    -1,   219,
     155,   219,    -1,   219,   151,   219,    -1,   219,   152,   219,
      -1,   219,   125,   219,    -1,   219,   126,   219,    -1,   219,
     122,   219,    -1,   219,   123,   219,    -1,   219,   127,   219,
      -1,   219,   124,   219,    -1,   219,   139,   219,    -1,   219,
     140,   219,    -1,   219,   141,   219,    -1,   219,   149,   219,
     150,   219,    -1,   218,   147,   220,    -1,   219,    58,   217,
      -1,   219,    -1,    16,   121,    -1,    16,   121,   171,   210,
     172,    -1,    16,   217,   165,   121,    -1,    16,   217,   166,
     219,   167,    -1,    16,   217,   165,   121,   171,   210,   172,
      -1,    16,   217,   166,   219,   167,   171,   210,   172,    -1,
     166,   210,   167,    -1,   179,    -1,   168,   214,   169,    -1,
     211,    -1,   217,    -1,   216,   218,    -1,   218,   216,    -1,
     171,   219,   172,    -1,   157,   116,    -1,   116,    -1,   157,
     117,    -1,   117,    -1,   118,    -1,   114,    -1,   115,    -1,
     120,    -1,   218,   215,   219,    -1,   223,    -1,   222,   146,
     223,    -1,   192,   147,   220,    -1,   192,    -1,    -1,    22,
     168,   225,   226,   169,    -1,    15,   222,    -1,   208,    -1,
     209,    -1,   212,    -1,   216,   218,    -1,   218,   216,    -1,
     218,   147,   220,    -1,   218,   215,   219,    -1,   231,    -1,
     226,   231,    -1,    -1,    21,   228,   226,    20,    -1,   120,
      -1,   116,    -1,   117,    -1,   118,    -1,   114,    -1,   115,
      -1,   119,    -1,   229,    -1,   230,   146,   229,    -1,    -1,
      44,   232,   230,    -1,   227,    -1,    45,   119,    -1,    46,
      -1,    19,    -1,    47,    -1,    48,    -1,    49,    -1,    50,
      -1,    51,    -1,    52,    -1,    53,    -1,    54,    -1,    55,
      -1,    56,    -1,    57,    -1,    58,    -1,    59,    -1,    17,
      -1,    16,    -1,    61,    -1,    60,    -1,    62,    -1,    63,
      -1,    64,    -1,    65,    -1,    66,    -1,    15,    -1,   113,
      -1,    68,    -1,    69,    -1,    70,    -1,    71,    -1,    72,
      -1,    73,    -1,    74,    -1,    75,    -1,    76,    -1,    77,
      -1,    78,    -1,    79,    -1,    30,    -1,    41,    -1,    42,
      -1,    43,    -1,    80,    -1,    81,    -1,   105,    -1,   106,
      -1,   107,    -1,   108,    -1,    82,    -1,    27,    -1,    83,
      -1,    84,    -1,    29,    -1,    85,    -1,    24,    -1,    86,
      -1,    31,    -1,    32,    -1,    25,    -1,    87,    -1,    88,
      -1,    89,    -1,    90,   120,    -1,    91,   120,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   162,   162,   162,   170,   174,   175,   179,   181,   186,
     189,   195,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   215,   219,   229,   240,   251,
     266,   277,   287,   299,   300,   304,   315,   325,   326,   330,
     335,   349,   352,   360,   365,   370,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   467,   470,
     476,   484,   489,   518,   523,   540,   541,   545,   550,   556,
     561,   565,   578,   591,   626,   660,   695,   696,   702,   713,
     726,   728,   730,   732,   739,   746,   754,   766,   774,   781,
     789,   797,   801,   813,   824,   831,   838,   845,   852,   858,
     865,   876,   881,   889,   893,   903,   907,   911,   915,   919,
     923,   927,   934,   941,   949,   957,   963,   969,   976,   985,
     996,  1004,  1010,  1016,  1021,  1025,  1029,  1033,  1037,  1041,
    1045,  1049,  1054,  1060,  1073,  1076,  1081,  1090,  1117,  1125,
    1134,  1142,  1146,  1152,  1153,  1154,  1155,  1156,  1157,  1158,
    1159,  1160,  1161,  1162,  1166,  1167,  1185,  1203,  1204,  1209,
    1216,  1223,  1232,  1234,  1239,  1244,  1248,  1253,  1258,  1263,
    1268,  1273,  1278,  1283,  1288,  1293,  1298,  1303,  1308,  1314,
    1319,  1323,  1327,  1332,  1337,  1341,  1345,  1349,  1359,  1386,
    1394,  1396,  1407,  1418,  1430,  1442,  1454,  1465,  1470,  1475,
    1483,  1485,  1487,  1532,  1577,  1580,  1584,  1588,  1592,  1596,
    1600,  1604,  1608,  1612,  1661,  1663,  1669,  1676,  1685,  1684,
    1689,  1692,  1694,  1698,  1702,  1741,  1780,  1801,  1846,  1847,
    1852,  1851,  1859,  1862,  1864,  1866,  1870,  1873,  1876,  1884,
    1885,  1889,  1889,  1894,  1896,  1903,  1905,  1907,  1909,  1911,
    1913,  1915,  1917,  1919,  1921,  1923,  1925,  1927,  1929,  1931,
    1933,  1935,  1937,  1939,  1941,  1943,  1945,  1947,  1949,  1951,
    1953,  1957,  1958,  1959,  1960,  1961,  1962,  1963,  1964,  1965,
    1966,  1967,  1968,  1969,  1970,  1971,  1972,  1973,  1974,  1975,
    1976,  1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,  1985,
    1986,  1987,  1988,  1989,  1990,  1991,  1992,  1995,  1999
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BREAK", "CONTINUE", "FUNCTION", "ELSE",
  "SWITCH", "CASE", "DEFAULT", "FOR", "IN", "IF", "WHILE", "DO", "VAR",
  "NEW", "DELETE", "TARGETPATH", "RETURN", "END", "WITH", "ASM", "EVAL",
  "RANDOM", "GETTIMER", "LENGTH", "CONCAT", "SUBSTR", "TRACE", "INT",
  "ORD", "CHR", "GETURL", "GETURL1", "NEXTFRAME", "PREVFRAME", "PLAY",
  "STOP", "TOGGLEQUALITY", "STOPSOUNDS", "DUP", "SWAP", "POP", "PUSH",
  "SETREGISTER", "CALLFUNCTION", "CALLMETHOD", "AND", "OR", "XOR",
  "MODULO", "ADD", "LESSTHAN", "EQUALS", "INC", "DEC", "TYPEOF",
  "INSTANCEOF", "ENUMERATE", "INITOBJECT", "INITARRAY", "GETMEMBER",
  "SETMEMBER", "SHIFTLEFT", "SHIFTRIGHT", "SHIFTRIGHT2", "VAREQUALS",
  "OLDADD", "SUBTRACT", "MULTIPLY", "DIVIDE", "OLDEQUALS", "OLDLESSTHAN",
  "LOGICALAND", "LOGICALOR", "NOT", "STRINGEQ", "STRINGLENGTH",
  "SUBSTRING", "GETVARIABLE", "SETVARIABLE", "SETTARGETEXPRESSION",
  "DUPLICATEMOVIECLIP", "REMOVEMOVIECLIP", "STRINGLESSTHAN", "MBLENGTH",
  "MBSUBSTRING", "MBORD", "MBCHR", "BRANCHALWAYS", "BRANCHIFTRUE",
  "GETURL2", "POST", "GET", "LOADVARIABLES", "LOADMOVIE",
  "LOADVARIABLESNUM", "LOADMOVIENUM", "CALLFRAME", "STARTDRAG", "STOPDRAG",
  "GOTOANDSTOP", "GOTOANDPLAY", "SETTARGET", "GETPROPERTY", "SETPROPERTY",
  "TONUMBER", "TOSTRING", "TRY", "THROW", "CATCH", "FINALLY", "EXTENDS",
  "NULLVAL", "UNDEFINED", "INTEGER", "DOUBLE", "BOOLEAN", "REGISTER",
  "STRING", "IDENTIFIER", "\"==\"", "\"===\"", "\"!==\"", "\"<=\"",
  "\">=\"", "\"!=\"", "\"&&\"", "\"||\"", "\"++\"", "\"--\"", "\"+=\"",
  "\"/=\"", "\"*=\"", "\"-=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"<<\"",
  "\">>\"", "\">>>\"", "\"<<=\"", "\">>=\"", "\">>>=\"", "NOELSE", "','",
  "'='", "\"^=\"", "'?'", "':'", "'<'", "'>'", "'&'", "'|'", "'^'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "UMINUS", "POSTFIX", "'.'",
  "'['", "']'", "'{'", "'}'", "';'", "'('", "')'", "$accept", "program",
  "@1", "code", "anycode", "stmts", "emptybraces", "stmt", "throw_stmt",
  "try_catch_stmt", "with_stmt", "return_stmt", "assign_stmts", "if_stmt",
  "expr_opt", "switch_init", "switch_stmt", "switch_cases", "switch_case",
  "identifier", "formals_list", "function_init", "function_decl", "inpart",
  "obj_ref", "while_init", "do_init", "for_init", "for_in_init",
  "iter_stmt", "assign_stmts_opt", "cont_stmt", "break_stmt", "urlmethod",
  "level", "void_function_call", "function_call", "expr_list",
  "anon_function_decl", "method_call", "objexpr", "objexpr_list",
  "assignop", "incdecop", "lvalue_expr", "lvalue", "expr", "expr_or_obj",
  "primary", "init_vars", "init_var", "assign_stmt", "@2", "opcode_list",
  "with", "@3", "push_item", "push_list", "opcode", "@4", 0
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
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,    44,    61,   401,    63,
      58,    60,    62,    38,   124,    94,    43,    45,    42,    47,
      37,    33,   126,   402,   403,    46,    91,    93,   123,   125,
      59,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   173,   175,   174,   174,   176,   176,   177,   177,   178,
     178,   179,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   181,   182,   182,   182,   182,
     183,   184,   184,   185,   185,   186,   186,   187,   187,   188,
     189,   190,   190,   191,   191,   191,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   193,   193,
     193,   194,   195,   196,   197,   197,   197,   198,   199,   200,
     201,   202,   202,   202,   202,   202,   203,   203,   204,   205,
     206,   206,   206,   206,   207,   207,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   210,   210,   210,   211,   212,   212,
     213,   214,   214,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   216,   216,   217,   217,   217,   218,
     218,   218,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   222,   222,   223,   223,   225,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   226,   226,
     228,   227,   229,   229,   229,   229,   229,   229,   229,   230,
     230,   232,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     1,     2,     1,     1,     1,
       2,     2,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     2,     7,     4,     9,
       7,     2,     3,     1,     3,     7,     5,     0,     1,     1,
       7,     0,     2,     6,     4,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       3,     1,     6,     2,     1,     1,     1,     1,     1,     0,
       0,     5,     6,    10,     7,     8,     0,     1,     2,     2,
       0,     2,     2,     2,     1,     1,     4,     4,     2,     4,
       5,     4,     4,     7,     7,     7,     7,     7,     4,     6,
      14,     3,     8,     4,     6,     3,     3,     3,     3,     3,
       3,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     2,     4,     5,     4,     3,     4,     4,     4,     4,
       4,     6,     8,     4,     0,     1,     3,     5,     6,     7,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     1,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     5,     3,     3,
       1,     2,     5,     4,     5,     7,     8,     3,     1,     3,
       1,     1,     2,     2,     3,     2,     1,     2,     1,     1,
       1,     1,     1,     3,     1,     3,     3,     1,     0,     5,
       2,     1,     1,     1,     2,     2,     3,     3,     1,     2,
       0,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     1,     0,     0,   131,    39,     0,     0,
     137,   138,     0,    47,    48,    49,     0,     0,     0,     0,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,     0,     0,     0,   111,   112,   126,   127,
       0,     0,     0,   125,    46,   224,   225,     0,    14,     3,
       5,    12,     7,    24,     0,    22,    21,    16,     0,    20,
     229,     0,     8,     0,     0,    17,    18,    19,   291,   292,
     293,     0,     0,   226,     0,   149,   148,   146,     0,    48,
      49,    50,    51,    52,    53,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    83,   105,   109,
     110,   123,   124,   111,   112,   126,   127,    46,   287,   290,
     284,    48,    49,   158,   229,   227,   228,     0,   226,     0,
      47,   280,   281,   276,   278,   279,   282,     0,     0,     0,
     204,     0,    31,     0,   268,     0,   270,     0,   271,   226,
     260,     0,   232,     0,   288,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    26,     0,   204,    11,
       0,     9,     6,    23,     0,   204,     0,     0,     0,   294,
       0,     0,   213,   216,   215,   214,   217,   218,   219,   221,
     222,   223,     0,   220,     0,   295,    15,     0,   147,   229,
       0,    33,     0,     0,     0,   191,     0,     0,     0,     0,
       0,   261,     0,   275,   277,   233,   235,   234,     0,   205,
       0,   211,     0,     0,   128,   272,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    32,     0,     0,     0,     0,   195,     0,     0,
       0,     0,     0,     0,     0,     0,   175,   176,   177,   178,
     180,   179,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   171,   276,   282,     0,   276,   282,     0,   282,
       0,     0,     0,    25,     0,    13,    10,     0,     0,   128,
       0,     0,   230,     0,   296,   297,   287,     0,     0,     0,
      37,     0,   286,   285,     0,     0,     0,    46,     0,   157,
     204,     0,     0,     0,   267,     0,     0,   269,   274,   129,
       0,   258,   283,   259,   250,   251,   253,   248,   249,   252,
     237,   236,   254,   255,   256,     0,   246,   247,   243,   244,
     245,   241,   242,   238,   239,   240,     0,   339,   331,   330,
     316,   300,   369,   373,   364,   367,   353,   371,   372,   354,
     355,   356,   311,     0,   315,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   333,   332,
     334,   335,   336,   337,   338,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   357,   358,   363,
     365,   366,   368,   370,   374,   375,   376,     0,     0,   359,
     360,   361,   362,   340,     0,   313,   298,   194,   196,   197,
       0,   161,   198,   199,   200,     0,   162,     0,   203,     0,
       0,   173,     0,     0,     0,     0,   168,     0,   182,   184,
     186,   181,   183,   185,   187,   188,     0,    28,   189,     0,
     156,     0,     0,     0,   204,   231,     0,    34,   133,   227,
     228,   226,   140,     0,    38,    36,   192,     0,   190,   160,
       0,   263,     0,   206,   210,   212,     0,     0,     0,     0,
       0,     0,   314,   377,   378,   289,   299,     0,   150,     0,
       0,     0,   150,   150,   276,   150,   155,   150,     0,     0,
      41,     0,   141,     0,     0,   204,   140,     0,   146,     0,
     193,   262,   204,   264,   130,   207,   257,     0,     0,   306,
     307,   303,   304,   305,   308,   302,   309,   312,   201,     0,
       0,   174,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,   132,   142,   208,     0,     0,   144,     0,    35,
       0,   204,    30,   301,     0,   152,   151,   153,   163,     0,
       0,   164,   166,   165,   167,     0,    27,     0,     0,    40,
      42,   209,   145,   139,   265,     0,   310,   202,   172,     0,
       0,     0,     0,     0,   266,     0,    29,     0,    45,   143,
       0,    44,     0,     0,     0,   149,     0,   170
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   109,   110,   240,   111,   241,   113,   114,
     115,   116,   268,   117,   543,   118,   119,   621,   650,   174,
     400,   195,   122,   379,   538,   123,   124,   663,   587,   125,
     270,   126,   127,   610,   575,   128,   175,   288,   196,   176,
     291,   292,   264,   197,   198,   199,   200,   289,   202,   169,
     170,   134,   324,   494,   495,   560,   606,   607,   496,   561
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -538
static const yytype_int16 yypact[] =
{
      34,    48,  1338,  -538,  -110,   -98,  -538,  -538,   -79,   -67,
    -538,  -538,  5112,  -538,  4717,   -32,  1043,   -30,   -21,   -19,
      -9,    10,    12,    14,  -538,    16,    20,    21,    22,    24,
      26,    27,    29,    30,    31,    32,    35,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,    36,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,    37,  -538,  -538,  -538,    38,
      39,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,    41,    44,    47,    50,    51,    52,    54,    55,    56,
      57,  2001,  2308,  -538,    58,  -538,  -538,  1497,  -538,  1338,
    -538,  -538,  -538,  -538,    -1,  -538,  -538,  -538,    60,  -538,
      63,  5112,  -538,    66,  2001,  -538,  -538,  -538,  -538,  -134,
     -91,  4816,   -88,   203,    49,  -538,  -538,  3653,  2466,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,    70,     3,
    -538,  4915,    68,   -95,  -538,  -538,  -538,   -75,  -538,    65,
    5014,  -538,  -538,  -538,  -538,  -538,  -538,  2615,  2466,  2466,
    2308,  3505,  -538,  2466,  -538,    71,  -538,  4816,   -88,   405,
    6069,    73,  -538,  2466,  -538,  2466,  2466,    72,  2308,  2466,
    2308,  2466,  2466,  2466,  2466,    64,    74,    77,    79,    81,
      85,    92,  2308,  2466,  2466,  2466,  2466,  2466,  2466,  2466,
    2466,  2466,    94,  2764,  2913,  3062,   -69,    75,  2308,  -538,
    1665,  -538,  -538,  -538,  2466,  2308,    96,  2466,   192,   -64,
    5112,  2466,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  2308,  -538,  2466,  -538,  -538,  5112,   123,    -5,
     100,  -538,   870,  2308,  5112,   -95,   -42,   150,  5210,  2466,
     102,   -78,   -34,  -538,  -538,   219,   219,   219,  -109,  -538,
     133,  -538,  -139,  1093,  5112,   -64,  2308,  2466,  -538,  4816,
    2466,  2466,  2466,  2466,  2466,  2466,  2466,  2466,  2466,  2466,
    2466,  2466,  2466,  2466,  2466,  2466,  2466,  2466,  2466,  2466,
    2466,  2466,  -538,  1150,  6310,  3329,  3810,  -538,   113,  5445,
     117,  3849,  3888,  3927,   -43,   144,  -538,  -538,  -538,  -538,
    -538,  -538,   121,  5484,  5523,  3966,  5562,  5601,  5640,  5679,
    4005,  5718,  -538,   127,   128,  4044,   129,   130,  4083,   132,
    4122,   135,  2001,  -538,  -128,  -538,  -538,  4161,  -127,  5112,
    4200,   136,   146,  4434,  -538,  6069,    -3,  3763,  4816,   148,
    2466,  2001,  -538,  -538,  5308,  2466,   152,  -132,  4480,    -7,
    2308,  5406,  2466,  2308,  -538,  2308,  5112,  -538,  -538,  -538,
    -126,  -538,  6069,   -88,   804,   804,   804,   156,   156,   804,
     303,   303,    15,    15,    15,  5757,   156,   156,   122,   122,
     122,   -22,   -22,   219,   219,   219,   126,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,   184,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,   202,   205,  -538,
    -538,  -538,  -538,  -538,  3359,  -538,  -538,  -538,  -538,  -538,
    2466,  -538,  -538,  -538,  -538,  2466,  -538,   206,  -538,  2466,
    2466,  -538,  2466,  2466,  3211,  3211,  -538,  2466,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  5112,  -538,  -538,   155,
    -538,  -123,  2001,  2466,  2308,   157,   158,  -538,  -538,   159,
     171,   172,  -538,   178,  6069,   323,   -71,  4526,  -538,   -20,
    -122,   -11,  4572,  -538,  -538,  -538,  5112,  2001,  2466,  2001,
    6310,   -51,  -538,  -538,  -538,  -538,  -538,  4239,  5796,   180,
    5835,  5874,  5796,  5796,  -121,   207,  6069,   207,   350,   182,
    -538,  2001,  -538,  4278,  -120,  2308,  -538,  2001,  3763,  2001,
     157,     2,  2308,    11,  -538,  -538,  6069,  1833,  6215,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,   212,  -538,   -81,
     187,  -538,  2466,  2466,   190,   193,   194,   196,  2466,  -538,
    2001,    -6,  -538,  -538,  -538,  -119,  2001,  -538,   197,  -538,
    -118,  2308,  -538,  -538,   -51,  -538,  -538,  -538,  -538,  4317,
    4356,  -538,  -538,  -538,  -538,  5913,   252,  2466,   220,  -538,
    -538,  -538,  -538,  -538,  -538,  -117,  -538,  -538,  -538,  2466,
    2001,  5952,  2001,  2001,    13,  5991,  -538,  2001,  2001,  -538,
    2466,  2160,  6030,   204,  2466,    -4,  4395,  -538
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -538,  -538,  -538,  -538,   264,  -537,    46,   131,  -538,  -538,
    -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,  -538,    -2,
       7,     9,  -538,     1,  -538,  -538,  -538,  -538,  -208,  -538,
    -205,  -538,  -538,  -516,  -135,  -538,   151,  -229,  -538,   248,
     -12,  -538,   188,    87,    19,    33,   670,    25,  -538,  -538,
     112,  -136,  -538,  -172,  -538,  -538,  -244,  -538,  -477,  -538
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -232
static const yytype_int16 yytable[] =
{
     120,   271,   647,   648,   -43,   -43,   378,   396,   378,   364,
     168,   121,   635,   636,  -159,   299,   368,   566,   393,   393,
     556,   132,   597,   556,   393,  -154,   393,   393,   393,   393,
     397,  -227,  -227,   177,    -4,   133,   299,   393,  -159,   637,
    -159,   201,   361,   362,   528,   530,   557,   178,     3,   581,
     591,  -154,   624,   651,   654,   664,   614,   615,   394,   616,
     135,   617,   194,   599,   600,   601,   602,   603,   604,   605,
     -46,   -46,   136,   299,  -228,  -228,   238,   250,   251,   300,
     301,   302,   303,   304,   305,   306,   307,   -46,   -46,   131,
     278,   279,   137,   390,   -46,   -46,   308,   309,   310,   120,
     -46,  -226,  -226,   505,   138,   120,   311,   120,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   121,   246,
     132,   566,   120,   384,   385,   668,   132,   237,   132,   506,
     671,   391,   392,   112,   133,   269,   319,   320,   321,   179,
     133,   203,   133,   132,   273,  -193,  -193,   204,   194,   274,
     132,   585,   205,   129,   -46,   -46,   132,   133,  -190,  -190,
     592,   550,   206,   649,   249,   -43,   245,  -189,  -189,   243,
     133,   317,   318,   319,   320,   321,  -231,  -231,  -209,  -209,
     299,   207,   631,   208,   335,   209,   280,   210,   131,   290,
     276,   211,   212,   213,   131,   214,   131,   215,   216,   282,
     217,   218,   219,   220,   178,   371,   221,   222,   223,   224,
     225,   131,   226,   178,   299,   227,   132,   273,   228,   266,
     265,   229,   230,   231,   131,   232,   233,   234,   235,   238,
     295,   244,   236,   328,   245,   330,   194,   247,   120,   277,
     112,   537,   294,   322,   327,   363,   336,   342,   372,   337,
     130,   338,   129,   339,   194,   248,   194,   340,   129,   132,
     129,   308,   309,   310,   341,   376,   352,   369,   194,   377,
     380,   386,   168,   133,   389,   129,   372,   299,   317,   318,
     319,   320,   321,   395,   194,   499,   298,   374,   129,   501,
     507,   194,   399,   508,   559,   308,   309,   310,   382,   518,
     519,   521,   522,   562,   524,   584,   526,   533,   194,   314,
     315,   316,   317,   318,   319,   320,   321,   534,   403,   194,
     542,   401,   563,   580,   548,   564,   569,   131,   585,   589,
     586,  -135,   178,   105,   106,   252,   253,   254,   255,   256,
     257,   258,   194,  -136,  -134,   259,   260,   261,   588,   130,
     262,   263,   611,   609,   620,   130,   625,   130,   634,   638,
     120,   299,   641,   630,   660,   642,   643,   399,   644,   653,
     662,   366,   130,   242,   675,   120,   531,   536,   626,   120,
     577,   132,   372,   628,   555,   130,   383,   297,   598,   372,
     656,   129,     0,     0,   290,   133,   132,   132,     0,     0,
     132,     0,   655,     0,     0,     0,     0,     0,   299,     0,
     133,   541,     0,     0,   133,     0,     0,     0,   553,     0,
     554,     0,     0,     0,     0,   300,   301,   302,   303,   304,
     305,     0,     0,     0,     0,     0,   194,     0,     0,   194,
       0,   194,   308,   309,   310,     0,     0,     0,     0,   131,
       0,     0,   271,     0,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   131,     0,     0,     0,   131,     0,
       0,     0,   300,   301,   302,   303,   304,   305,   306,   307,
       0,     0,     0,     0,     0,     0,     0,     0,   130,   308,
     309,   310,     0,   527,     0,     0,   618,     0,     0,   311,
       0,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,     0,   545,   129,     0,     0,     0,     0,     0,     0,
       0,     0,   619,     0,   579,     0,     0,     0,   129,   539,
     120,     0,   129,     0,     0,   105,   106,   252,   253,   254,
     255,   256,   257,   258,     0,     0,     0,   259,   260,   261,
       0,   132,   296,   263,   594,   120,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   132,     0,   132,   120,
     194,     0,     0,     0,     0,   120,   120,   120,     0,     0,
     133,     0,   133,     0,     0,   120,     0,     0,     0,     0,
     132,     0,     0,     0,     0,     0,   132,   132,   132,     0,
     130,     0,     0,     0,   133,     0,   132,     0,   120,   131,
     133,   133,   133,     0,   120,   130,   540,     0,     0,   130,
     133,   194,     0,     0,     0,     0,     0,     0,   194,   132,
       0,     0,     0,     0,   131,   132,   131,     0,     0,     0,
       0,     0,     0,   133,     0,     0,     0,     0,   120,   133,
     120,   120,     0,   582,     0,   120,   120,     0,   131,   120,
       0,     0,     0,     0,   131,   131,   131,   194,     0,   132,
       0,   132,   132,   129,   131,     0,   132,   132,   595,     0,
     132,     0,     0,   133,     0,   133,   133,     0,     0,     0,
     133,   133,     0,     0,   133,     0,     0,   131,   129,     0,
     129,     0,   622,   131,     0,     0,     0,     0,   627,     0,
     629,     0,     0,     0,     0,     0,     0,     0,   366,     0,
       0,     0,   129,     0,     0,     0,     0,     0,   129,   129,
     129,     0,     0,     0,     0,     0,     0,   131,   129,   131,
     131,   646,     0,     0,   131,   131,     0,   652,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   129,     0,     0,     0,     0,     0,   129,     0,     0,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   666,     0,     0,   669,     0,     0,     0,     0,   366,
       0,     0,   366,     0,     0,   130,     0,   130,   272,     0,
       0,   129,     0,   129,   129,     0,     0,     0,   129,   129,
       0,     0,   129,     0,     0,     0,     0,     0,     0,   130,
       0,     0,     0,     0,     0,   130,   130,   130,     0,     0,
       0,     0,     0,     0,     0,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   285,   286,   287,
       0,     0,   299,   293,     0,     0,     0,     0,   130,     0,
       0,     0,     0,   323,   130,   325,   326,     0,     0,   329,
       0,   331,   332,   333,   334,     0,     0,     0,     0,     0,
       0,     0,     0,   343,   344,   345,   346,   347,   348,   349,
     350,   351,     0,   355,   358,   360,     0,     0,   130,     0,
     130,   130,     0,     0,   367,   130,   130,   370,     0,   130,
       0,   373,     0,     0,     0,     0,     0,     0,   299,   303,
     304,     0,     0,     0,   375,     0,     0,     0,     0,     0,
       0,     0,     0,   308,   309,   310,     0,     0,     0,   388,
       0,     0,     0,     0,     0,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,     0,     0,   402,     0,     0,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     424,   425,   300,   301,   302,   303,   304,   305,   306,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   308,
     309,   310,     0,     0,     0,     0,     0,     0,     0,   311,
       0,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,     0,     0,     0,     0,     0,     6,     0,
     544,     0,     0,     0,     0,   547,     0,     0,     0,   180,
     171,   172,   552,     0,     0,     0,    19,    20,    21,    22,
      23,    24,   145,    26,    27,    28,   149,   150,   151,   152,
     153,   154,   155,   156,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,   159,   160,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,   161,   162,
       0,     0,     0,   163,   164,   165,   166,     0,     0,     0,
       0,   299,     0,     0,     0,     0,   103,   181,   182,   183,
     184,   185,     0,   186,   104,     0,     0,     0,     0,     0,
     567,     0,     0,   105,   106,   568,     0,     0,     0,   570,
     571,     0,   572,   573,   576,   576,     0,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,     0,     0,   583,   188,   189,     0,     0,   299,   190,
       0,   191,     0,   192,   193,   300,   301,   302,   303,   304,
     305,   306,   307,     0,     0,     0,     0,     0,   596,     0,
       0,     0,   308,   309,   310,     0,     0,     0,     0,     0,
       0,     0,   311,     0,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,   300,   301,   302,   303,   304,   305,   306,   307,
       0,     0,   639,   640,     0,     0,     0,     0,   645,   308,
     309,   310,     0,     0,     0,     0,     0,     0,     0,   311,
       0,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,     0,     0,     0,     0,     0,     0,   661,     0,     0,
       0,     0,   426,     0,     0,     0,     0,     0,     0,   665,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     672,     4,     5,     6,   676,     7,     0,     0,     8,     0,
       9,    10,    11,    12,    13,    14,    15,    16,     0,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,   102,     0,
       0,   103,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,   105,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     0,     0,     7,     0,   107,     8,   108,     9,
      10,    11,    12,    13,    14,    15,    16,     0,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,   102,     0,     0,
     103,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,     0,     0,     0,     0,     0,   105,   106,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   107,   239,   108,     4,     5,
       0,     0,     7,     0,     0,     8,     0,     9,    10,    11,
      12,    13,    14,    15,    16,     0,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,   102,     0,     0,   103,     0,
       0,     0,     0,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,   105,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   107,   365,   108,     4,     5,     0,     0,
       7,     0,     0,     8,     0,     9,    10,    11,    12,    13,
      14,    15,    16,     0,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,   102,     0,     0,   103,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,   105,   106,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   107,   632,   108,     4,     5,     0,     0,     7,     0,
       0,     8,     0,     9,    10,    11,    12,    13,    14,    15,
      16,     0,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,   102,     0,     0,   103,     0,     0,     0,     0,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
       0,   105,   106,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   673,     5,     0,     0,     7,     0,   107,
       8,   108,     9,    10,    11,    12,    13,    14,    15,    16,
       0,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
     102,     0,     0,   103,     0,     0,     0,     0,     0,     0,
       0,   104,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   171,   172,     0,   107,     0,
     108,    19,    20,    21,    22,    23,    24,   145,    26,    27,
      28,   149,   150,   151,   152,   153,   154,   155,   156,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,   159,   160,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,   161,   162,     0,     0,     0,   163,   164,
     165,   166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   103,   181,   182,   183,   184,   185,     0,   186,   104,
       0,     0,     0,     0,     0,     0,     0,     0,   105,   106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   187,     0,     0,     0,   188,
     189,     6,     0,     0,   190,     0,   191,     0,     0,   193,
       0,     0,    13,   171,   172,     0,     0,     0,     0,    19,
      20,    21,    22,    23,    24,   145,    26,    27,    28,   149,
     150,   151,   152,   153,   154,   155,   156,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,   159,
     160,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   161,   162,     0,     0,     0,   163,   164,   165,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
     181,   182,   183,   184,   185,     0,   186,   104,     0,     0,
       0,     0,     0,     0,     0,     0,   105,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     0,     0,   187,     0,     0,     0,   188,   189,     0,
       0,    13,   171,   172,     0,     0,     0,   193,    19,    20,
      21,    22,    23,    24,   145,    26,    27,    28,   149,   150,
     151,   152,   153,   154,   155,   156,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,   159,   160,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     161,   162,     0,     0,     0,   163,   164,   165,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,   181,
     182,   283,   284,   185,     0,   186,   104,     0,     0,     0,
       0,     0,     0,     0,     0,   105,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       0,     0,   187,     0,     0,     0,   188,   189,     0,     0,
      13,   171,   172,     0,     0,     0,   193,    19,    20,    21,
      22,    23,    24,   145,    26,    27,    28,   149,   150,   151,
     152,   153,   154,   155,   156,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,   159,   160,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,   161,
     162,     0,     0,     0,   163,   164,   165,   166,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   103,   181,   182,
     353,   184,   185,     0,   354,   104,     0,     0,     0,     0,
       0,     0,     0,     0,   105,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     6,     0,
       0,   187,     0,     0,     0,   188,   189,     0,     0,    13,
     171,   172,     0,     0,     0,   193,    19,    20,    21,    22,
      23,    24,   145,    26,    27,    28,   149,   150,   151,   152,
     153,   154,   155,   156,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,   159,   160,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,   161,   162,
       0,     0,     0,   163,   164,   165,   166,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   103,   181,   182,   356,
     184,   185,     0,   357,   104,     0,     0,     0,     0,     0,
       0,     0,     0,   105,   106,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     6,     0,     0,
     187,     0,     0,     0,   188,   189,     0,     0,    13,   171,
     172,     0,     0,     0,   193,    19,    20,    21,    22,    23,
      24,   145,    26,    27,    28,   149,   150,   151,   152,   153,
     154,   155,   156,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,   159,   160,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,   161,   162,     0,
       0,     0,   163,   164,   165,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   103,   181,   182,   183,   184,
     185,     0,   359,   104,     0,     0,     0,     0,     0,     0,
       0,     0,   105,   106,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     0,     0,   187,
       0,     0,     0,   188,   189,     0,     0,    13,   171,   172,
       0,     0,     0,   193,    19,    20,    21,    22,    23,    24,
     145,    26,    27,    28,   149,   150,   151,   152,   153,   154,
     155,   156,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,   159,   160,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,   161,   162,     0,     0,
       0,   163,   164,   165,   166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   103,   181,   182,   574,   184,   185,
       0,   186,   104,     0,     0,     0,     0,     0,     0,     0,
       0,   105,   106,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
       0,     0,   188,   189,   427,   428,   429,     0,   430,     0,
     431,     0,   193,   432,   433,     0,   434,   299,   435,   436,
     437,   438,     0,     0,     0,     0,     0,     0,     0,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,     0,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   300,   301,   302,   303,   304,   305,   306,   307,     0,
       0,     0,     0,     0,   489,   490,   491,   492,   308,   309,
     310,     0,   493,     0,     0,     0,     0,     0,   311,     0,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   497,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    13,   139,   140,     0,     0,     0,     0,   565,   141,
     142,   143,   144,    24,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,   157,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,   158,    76,    77,    78,   159,   160,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     161,   162,     0,     0,     0,   163,   164,   165,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,     0,
       0,     0,     0,     0,     0,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   267,    13,
      14,    15,     0,     0,   239,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   103,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,    12,    13,
      14,    15,     0,   105,   106,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   299,     0,
       0,     0,     0,     0,     0,     0,   103,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,   105,   106,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   299,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   300,   301,   302,   303,   304,   305,   306,   307,
       0,     0,     0,     0,     0,     0,   299,     0,     0,   308,
     309,   310,     0,     0,     0,     0,     0,     0,     0,   311,
       0,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   300,   301,   302,   303,   304,   305,   306,   307,     0,
       0,     0,   498,     0,     0,   299,     0,     0,   308,   309,
     310,     0,     0,     0,     0,     0,     0,     0,   311,     0,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     300,   301,   302,   303,   304,   305,   306,   307,     0,     0,
       0,   502,     0,     0,   299,     0,     0,   308,   309,   310,
       0,     0,     0,     0,     0,     0,     0,   311,     0,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   300,
     301,   302,   303,   304,   305,   306,   307,     0,     0,     0,
     503,     0,     0,   299,     0,     0,   308,   309,   310,     0,
       0,     0,     0,     0,     0,     0,   311,     0,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   300,   301,
     302,   303,   304,   305,   306,   307,     0,     0,     0,   504,
       0,     0,   299,     0,     0,   308,   309,   310,     0,     0,
       0,     0,     0,     0,     0,   311,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   300,   301,   302,
     303,   304,   305,   306,   307,     0,     0,     0,   511,     0,
       0,   299,     0,     0,   308,   309,   310,     0,     0,     0,
       0,     0,     0,     0,   311,     0,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   300,   301,   302,   303,
     304,   305,   306,   307,     0,     0,     0,   516,     0,     0,
     299,     0,     0,   308,   309,   310,     0,     0,     0,     0,
       0,     0,     0,   311,     0,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   300,   301,   302,   303,   304,
     305,   306,   307,     0,     0,     0,   520,     0,     0,   299,
       0,     0,   308,   309,   310,     0,     0,     0,     0,     0,
       0,     0,   311,     0,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   300,   301,   302,   303,   304,   305,
     306,   307,     0,     0,     0,   523,     0,     0,   299,     0,
       0,   308,   309,   310,     0,     0,     0,     0,     0,     0,
       0,   311,     0,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   300,   301,   302,   303,   304,   305,   306,
     307,     0,     0,     0,   525,     0,     0,   299,     0,     0,
     308,   309,   310,     0,     0,     0,     0,     0,     0,     0,
     311,     0,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   300,   301,   302,   303,   304,   305,   306,   307,
       0,     0,     0,   529,     0,     0,   299,     0,     0,   308,
     309,   310,     0,     0,     0,     0,     0,     0,     0,   311,
       0,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   300,   301,   302,   303,   304,   305,   306,   307,     0,
       0,     0,   532,     0,     0,   299,     0,     0,   308,   309,
     310,     0,     0,     0,     0,     0,     0,     0,   311,     0,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     300,   301,   302,   303,   304,   305,   306,   307,     0,     0,
       0,   608,     0,     0,   299,     0,     0,   308,   309,   310,
       0,     0,     0,     0,     0,     0,     0,   311,     0,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   300,
     301,   302,   303,   304,   305,   306,   307,     0,     0,     0,
     623,     0,     0,   299,     0,     0,   308,   309,   310,     0,
       0,     0,     0,     0,     0,     0,   311,     0,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   300,   301,
     302,   303,   304,   305,   306,   307,     0,     0,     0,   657,
       0,     0,   299,     0,     0,   308,   309,   310,     0,     0,
       0,     0,     0,     0,     0,   311,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   300,   301,   302,
     303,   304,   305,   306,   307,     0,     0,     0,   658,     0,
       0,     0,     0,     0,   308,   309,   310,     0,   299,     0,
       0,     0,     0,     0,   311,     0,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   300,   301,   302,   303,
     304,   305,   306,   307,     0,     0,     0,   677,     0,     0,
       0,     0,     0,   308,   309,   310,     0,     0,     0,     0,
       0,     0,     0,   311,   299,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,     0,     0,     0,     0,     0,
       0,   535,   300,   301,   302,   303,   304,   305,   306,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   308,
     309,   310,     0,     0,     0,     0,     0,     0,     0,   311,
     299,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,     0,     0,     0,     0,     0,     0,   549,   300,   301,
     302,   303,   304,   305,   306,   307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,   309,   310,     0,     0,
       0,     0,     0,     0,     0,   311,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,     0,     0,     0,
       0,     0,     0,   590,   300,   301,   302,   303,   304,   305,
     306,   307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,   309,   310,     0,     0,     0,     0,     0,     0,
       0,   311,     0,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,    13,   171,   172,     0,     0,     0,   593,
      19,    20,    21,    22,    23,    24,   145,    26,    27,    28,
     149,   150,   151,   152,   153,   154,   155,   156,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
     159,   160,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,   161,   162,     0,     0,     0,   163,   164,   165,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103,     0,    13,   171,   172,     0,     0,     0,   173,    19,
      20,    21,    22,    23,    24,   145,    26,    27,    28,   149,
     150,   151,   152,   153,   154,   155,   156,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,   159,
     160,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   161,   162,     0,     0,     0,   163,   164,   165,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
       0,    13,   171,   172,     0,     0,     0,   104,    19,    20,
      21,    22,    23,    24,   145,    26,    27,    28,   149,   150,
     151,   152,   153,   154,   155,   156,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,   159,   160,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     161,   162,     0,     0,     0,   163,   164,   165,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,     0,
      13,   171,   172,     0,     0,     0,   275,    19,    20,    21,
      22,    23,    24,   145,    26,    27,    28,   149,   150,   151,
     152,   153,   154,   155,   156,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,   159,   160,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,   161,
     162,     0,     0,     0,   163,   164,   165,   166,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   103,    13,   139,
     140,     0,     0,     0,     0,   281,   141,   142,   143,   144,
      24,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,   157,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,   158,    76,    77,    78,   159,   160,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,   161,   162,     0,
       0,     0,   163,   164,   165,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   103,    13,   139,   140,     0,
       0,     0,     0,   167,   141,   142,   143,   144,    24,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,   157,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,   158,
      76,    77,    78,   159,   160,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,   161,   162,     0,     0,     0,
     163,   164,   165,   166,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   103,    13,   139,   140,     0,     0,     0,
       0,   387,   141,   142,   143,   144,    24,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,   157,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,   158,    76,    77,
      78,   159,   160,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,   161,   162,     0,     0,     0,   163,   164,
     165,   166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   103,    13,   139,   140,     0,     0,     0,     0,   546,
     141,   142,   143,   144,    24,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,   157,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,   158,    76,    77,    78,   159,
     160,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,   161,   162,   299,     0,     0,   163,   164,   165,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
       0,     0,     0,     0,     0,     0,     0,   551,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   299,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   300,   301,   302,
     303,   304,   305,   306,   307,     0,     0,     0,     0,     0,
       0,   299,     0,     0,   308,   309,   310,     0,     0,     0,
       0,   500,     0,     0,   311,     0,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   300,   301,   302,   303,
     304,   305,   306,   307,     0,     0,     0,     0,     0,     0,
     299,     0,     0,   308,   309,   310,     0,     0,     0,     0,
     509,     0,     0,   311,     0,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   300,   301,   302,   303,   304,
     305,   306,   307,     0,     0,     0,     0,     0,     0,   299,
       0,     0,   308,   309,   310,     0,     0,     0,     0,   510,
       0,     0,   311,     0,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   300,   301,   302,   303,   304,   305,
     306,   307,     0,     0,     0,     0,     0,     0,   299,     0,
       0,   308,   309,   310,     0,     0,     0,     0,   512,     0,
       0,   311,     0,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   300,   301,   302,   303,   304,   305,   306,
     307,     0,     0,     0,     0,     0,     0,   299,     0,     0,
     308,   309,   310,     0,     0,     0,     0,   513,     0,     0,
     311,     0,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   300,   301,   302,   303,   304,   305,   306,   307,
       0,     0,     0,     0,     0,     0,   299,     0,     0,   308,
     309,   310,     0,     0,     0,     0,   514,     0,     0,   311,
       0,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   300,   301,   302,   303,   304,   305,   306,   307,     0,
       0,     0,     0,     0,     0,   299,     0,     0,   308,   309,
     310,     0,     0,     0,     0,   515,     0,     0,   311,     0,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     300,   301,   302,   303,   304,   305,   306,   307,     0,     0,
       0,     0,     0,     0,   299,     0,     0,   308,   309,   310,
       0,     0,     0,     0,   517,     0,     0,   311,     0,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   300,
     301,   302,   303,   304,   305,   306,   307,     0,     0,     0,
       0,     0,     0,   299,     0,     0,   308,   309,   310,     0,
       0,     0,     0,     0,     0,     0,   311,   558,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   300,   301,
     302,   303,   304,   305,   306,   307,     0,     0,     0,     0,
       0,     0,   299,     0,     0,   308,   309,   310,     0,     0,
       0,     0,   609,     0,     0,   311,     0,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   300,   301,   302,
     303,   304,   305,   306,   307,     0,     0,     0,     0,     0,
       0,   299,     0,     0,   308,   309,   310,     0,     0,     0,
       0,   612,     0,     0,   311,     0,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   300,   301,   302,   303,
     304,   305,   306,   307,     0,     0,     0,     0,     0,     0,
     299,     0,     0,   308,   309,   310,     0,     0,     0,     0,
     613,     0,     0,   311,     0,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   300,   301,   302,   303,   304,
     305,   306,   307,     0,     0,     0,     0,     0,     0,   299,
       0,     0,   308,   309,   310,     0,     0,     0,     0,   659,
       0,     0,   311,     0,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   300,   301,   302,   303,   304,   305,
     306,   307,     0,     0,     0,     0,     0,     0,   299,     0,
       0,   308,   309,   310,     0,     0,     0,     0,     0,     0,
       0,   311,   667,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   300,   301,   302,   303,   304,   305,   306,
     307,     0,     0,     0,     0,     0,     0,   299,     0,     0,
     308,   309,   310,     0,     0,     0,     0,   670,     0,     0,
     311,     0,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   300,   301,   302,   303,   304,   305,   306,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   308,
     309,   310,     0,     0,     0,     0,   674,     0,     0,   311,
       0,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   300,   301,   302,   303,   304,   305,   306,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   308,   309,
     310,     0,     0,     0,     0,     0,     0,     0,   311,     0,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     427,   428,   429,     0,   430,   633,   431,     0,     0,   432,
     433,     0,   434,     0,   435,   436,   437,   438,     0,     0,
       0,     0,     0,     0,     0,     0,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,     0,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     489,   490,   491,   492,     0,   427,   428,   429,   493,   430,
       0,   431,     0,     0,   432,   433,     0,   434,     0,   435,
     436,   437,   438,     0,     0,     0,     0,     0,     0,     0,
       0,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,     0,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   489,   490,   491,   492,     0,
       0,     0,     0,   493
};

static const yytype_int16 yycheck[] =
{
       2,   137,     8,     9,     8,     9,    11,   146,    11,   238,
      12,     2,    93,    94,   146,    58,   245,   494,   146,   146,
     146,     2,   559,   146,   146,   146,   146,   146,   146,   146,
     169,   165,   166,    14,     0,     2,    58,   146,   170,   120,
     172,    16,   111,   112,   172,   172,   172,    14,     0,   172,
     172,   172,   172,   172,   172,   172,   572,   573,   167,   575,
     170,   577,    16,   114,   115,   116,   117,   118,   119,   120,
     165,   166,   170,    58,   165,   166,   171,   165,   166,   122,
     123,   124,   125,   126,   127,   128,   129,   165,   166,     2,
     165,   166,   171,   171,   165,   166,   139,   140,   141,   101,
     171,   165,   166,   146,   171,   107,   149,   109,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   109,   121,
     101,   598,   124,   165,   166,   662,   107,   102,   109,   172,
     667,   165,   166,     2,   101,   137,   158,   159,   160,   171,
     107,   171,   109,   124,   147,   165,   166,   168,   102,   146,
     131,   171,   171,     2,   165,   166,   137,   124,   165,   166,
     171,   390,   171,   169,   131,   169,   171,   165,   166,   170,
     137,   156,   157,   158,   159,   160,   165,   166,   165,   166,
      58,   171,   171,   171,   120,   171,   121,   171,   101,   191,
     171,   171,   171,   171,   107,   171,   109,   171,   171,   180,
     171,   171,   171,   171,   171,    13,   171,   171,   171,   171,
     171,   124,   171,   180,    58,   171,   197,   147,   171,   170,
     133,   171,   171,   171,   137,   171,   171,   171,   171,   171,
     197,   171,   101,   208,   171,   210,   190,   171,   240,   171,
     109,   377,   171,   170,   172,   170,   172,   222,   250,   172,
       2,   172,   101,   172,   208,   124,   210,   172,   107,   240,
     109,   139,   140,   141,   172,   267,   172,   171,   222,   146,
     170,   121,   274,   240,   172,   124,   278,    58,   156,   157,
     158,   159,   160,   150,   238,   172,   199,   262,   137,   172,
     146,   245,   294,   172,   168,   139,   140,   141,   273,   172,
     172,   172,   172,   119,   172,   534,   171,   171,   262,   153,
     154,   155,   156,   157,   158,   159,   160,   171,   299,   273,
     172,   296,   120,   168,   172,   120,   120,   240,   171,     6,
     172,   172,   299,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   296,   172,   172,   142,   143,   144,   170,   101,
     147,   148,   172,   146,   172,   107,   585,   109,   146,   172,
     362,    58,   172,   592,   112,   172,   172,   369,   172,   172,
     150,   240,   124,   109,   170,   377,   369,   376,   586,   381,
     515,   362,   384,   588,   396,   137,   274,   199,   560,   391,
     634,   240,    -1,    -1,   396,   362,   377,   378,    -1,    -1,
     381,    -1,   631,    -1,    -1,    -1,    -1,    -1,    58,    -1,
     377,   378,    -1,    -1,   381,    -1,    -1,    -1,   393,    -1,
     395,    -1,    -1,    -1,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,   390,    -1,    -1,   393,
      -1,   395,   139,   140,   141,    -1,    -1,    -1,    -1,   362,
      -1,    -1,   588,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   377,    -1,    -1,    -1,   381,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   240,   139,
     140,   141,    -1,   362,    -1,    -1,   146,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,   381,   362,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   526,    -1,    -1,    -1,   377,   378,
     532,    -1,   381,    -1,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,    -1,    -1,   142,   143,   144,
      -1,   532,   147,   148,   556,   557,    -1,   559,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   532,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   557,    -1,   559,   581,
     534,    -1,    -1,    -1,    -1,   587,   588,   589,    -1,    -1,
     557,    -1,   559,    -1,    -1,   597,    -1,    -1,    -1,    -1,
     581,    -1,    -1,    -1,    -1,    -1,   587,   588,   589,    -1,
     362,    -1,    -1,    -1,   581,    -1,   597,    -1,   620,   532,
     587,   588,   589,    -1,   626,   377,   378,    -1,    -1,   381,
     597,   585,    -1,    -1,    -1,    -1,    -1,    -1,   592,   620,
      -1,    -1,    -1,    -1,   557,   626,   559,    -1,    -1,    -1,
      -1,    -1,    -1,   620,    -1,    -1,    -1,    -1,   660,   626,
     662,   663,    -1,   532,    -1,   667,   668,    -1,   581,   671,
      -1,    -1,    -1,    -1,   587,   588,   589,   631,    -1,   660,
      -1,   662,   663,   532,   597,    -1,   667,   668,   557,    -1,
     671,    -1,    -1,   660,    -1,   662,   663,    -1,    -1,    -1,
     667,   668,    -1,    -1,   671,    -1,    -1,   620,   557,    -1,
     559,    -1,   581,   626,    -1,    -1,    -1,    -1,   587,    -1,
     589,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   597,    -1,
      -1,    -1,   581,    -1,    -1,    -1,    -1,    -1,   587,   588,
     589,    -1,    -1,    -1,    -1,    -1,    -1,   660,   597,   662,
     663,   620,    -1,    -1,   667,   668,    -1,   626,   671,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   620,    -1,    -1,    -1,    -1,    -1,   626,    -1,    -1,
     532,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   660,    -1,    -1,   663,    -1,    -1,    -1,    -1,   668,
      -1,    -1,   671,    -1,    -1,   557,    -1,   559,   138,    -1,
      -1,   660,    -1,   662,   663,    -1,    -1,    -1,   667,   668,
      -1,    -1,   671,    -1,    -1,    -1,    -1,    -1,    -1,   581,
      -1,    -1,    -1,    -1,    -1,   587,   588,   589,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   597,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,   188,   189,
      -1,    -1,    58,   193,    -1,    -1,    -1,    -1,   620,    -1,
      -1,    -1,    -1,   203,   626,   205,   206,    -1,    -1,   209,
      -1,   211,   212,   213,   214,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   223,   224,   225,   226,   227,   228,   229,
     230,   231,    -1,   233,   234,   235,    -1,    -1,   660,    -1,
     662,   663,    -1,    -1,   244,   667,   668,   247,    -1,   671,
      -1,   251,    -1,    -1,    -1,    -1,    -1,    -1,    58,   125,
     126,    -1,    -1,    -1,   264,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,   279,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,   297,    -1,    -1,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,     5,    -1,
     380,    -1,    -1,    -1,    -1,   385,    -1,    -1,    -1,    16,
      17,    18,   392,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
     500,    -1,    -1,   130,   131,   505,    -1,    -1,    -1,   509,
     510,    -1,   512,   513,   514,   515,    -1,   517,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,   533,   161,   162,    -1,    -1,    58,   166,
      -1,   168,    -1,   170,   171,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,    -1,   558,    -1,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,   612,   613,    -1,    -1,    -1,    -1,   618,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   647,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,   659,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     670,     3,     4,     5,   674,     7,    -1,    -1,    10,    -1,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,    -1,    -1,     7,    -1,   168,    10,   170,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   168,   169,   170,     3,     4,
      -1,    -1,     7,    -1,    -1,    10,    -1,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
      -1,    -1,    -1,    -1,   109,   110,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   168,   169,   170,     3,     4,    -1,    -1,
       7,    -1,    -1,    10,    -1,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,   169,   170,     3,     4,    -1,    -1,     7,    -1,
      -1,    10,    -1,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,    -1,    -1,     7,    -1,   168,
      10,   170,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    16,    17,    18,    -1,   168,    -1,
     170,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    -1,    -1,    -1,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,   115,   116,   117,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,    -1,   161,
     162,     5,    -1,    -1,   166,    -1,   168,    -1,    -1,   171,
      -1,    -1,    16,    17,    18,    -1,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    -1,    -1,    -1,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       5,    -1,    -1,   157,    -1,    -1,    -1,   161,   162,    -1,
      -1,    16,    17,    18,    -1,    -1,    -1,   171,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    -1,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,
      -1,    -1,   157,    -1,    -1,    -1,   161,   162,    -1,    -1,
      16,    17,    18,    -1,    -1,    -1,   171,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,    -1,
      -1,   157,    -1,    -1,    -1,   161,   162,    -1,    -1,    16,
      17,    18,    -1,    -1,    -1,   171,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,    -1,    -1,
     157,    -1,    -1,    -1,   161,   162,    -1,    -1,    16,    17,
      18,    -1,    -1,    -1,   171,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    -1,
      -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     5,    -1,    -1,   157,
      -1,    -1,    -1,   161,   162,    -1,    -1,    16,    17,    18,
      -1,    -1,    -1,   171,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,    -1,   161,   162,    15,    16,    17,    -1,    19,    -1,
      21,    -1,   171,    24,    25,    -1,    27,    58,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,    -1,   105,   106,   107,   108,   139,   140,
     141,    -1,   113,    -1,    -1,    -1,    -1,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    16,    17,    18,    -1,    -1,    -1,    -1,   169,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    -1,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,
      17,    18,    -1,    -1,   169,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    15,    16,
      17,    18,    -1,   130,   131,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,    -1,   172,    -1,    -1,    58,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     122,   123,   124,   125,   126,   127,   128,   129,    -1,    -1,
      -1,   172,    -1,    -1,    58,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,    -1,
     172,    -1,    -1,    58,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   122,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,    -1,   172,
      -1,    -1,    58,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,    -1,    -1,   172,    -1,
      -1,    58,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   122,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,    -1,   172,    -1,    -1,
      58,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,    -1,    -1,   172,    -1,    -1,    58,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,    -1,   172,    -1,    -1,    58,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,    -1,   172,    -1,    -1,    58,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,   172,    -1,    -1,    58,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,    -1,   172,    -1,    -1,    58,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     122,   123,   124,   125,   126,   127,   128,   129,    -1,    -1,
      -1,   172,    -1,    -1,    58,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,    -1,
     172,    -1,    -1,    58,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   122,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,    -1,   172,
      -1,    -1,    58,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,    -1,    58,    -1,
      -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   122,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,    -1,   172,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    58,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   167,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
      58,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   167,   122,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,   167,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    16,    17,    18,    -1,    -1,    -1,   167,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    -1,    -1,    -1,   100,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    16,    17,    18,    -1,    -1,    -1,   121,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    -1,    -1,    -1,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    16,    17,    18,    -1,    -1,    -1,   121,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    -1,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      16,    17,    18,    -1,    -1,    -1,   121,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    -1,    -1,    -1,   100,   101,   102,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    16,    17,
      18,    -1,    -1,    -1,    -1,   121,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    -1,
      -1,    -1,   100,   101,   102,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    16,    17,    18,    -1,
      -1,    -1,    -1,   121,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    -1,    -1,    -1,
     100,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    16,    17,    18,    -1,    -1,    -1,
      -1,   121,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    -1,    -1,    -1,   100,   101,
     102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    16,    17,    18,    -1,    -1,    -1,    -1,   121,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    58,    -1,    -1,   100,   101,   102,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   122,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     122,   123,   124,   125,   126,   127,   128,   129,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,   139,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   122,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,   139,   140,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   122,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   122,   123,   124,
     125,   126,   127,   128,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   122,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      -1,    -1,   139,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,
      -1,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
     139,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   122,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
      15,    16,    17,    -1,    19,    20,    21,    -1,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,    -1,    15,    16,    17,   113,    19,
      -1,    21,    -1,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,   113
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   174,   175,     0,     3,     4,     5,     7,    10,    12,
      13,    14,    15,    16,    17,    18,    19,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   109,   110,   113,   121,   130,   131,   168,   170,   176,
     177,   179,   180,   181,   182,   183,   184,   186,   188,   189,
     192,   194,   195,   198,   199,   202,   204,   205,   208,   209,
     212,   216,   217,   218,   224,   170,   170,   171,   171,    17,
      18,    24,    25,    26,    27,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    57,    79,    83,
      84,    95,    96,   100,   101,   102,   103,   121,   192,   222,
     223,    17,    18,   121,   192,   209,   212,   217,   218,   171,
      16,   114,   115,   116,   117,   118,   120,   157,   161,   162,
     166,   168,   170,   171,   179,   194,   211,   216,   217,   218,
     219,   220,   221,   171,   168,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   180,   220,   171,   169,
     178,   180,   177,   170,   171,   171,   192,   171,   180,   218,
     165,   166,   132,   133,   134,   135,   136,   137,   138,   142,
     143,   144,   147,   148,   215,   216,   170,    15,   185,   192,
     203,   224,   219,   147,   146,   121,   217,   171,   165,   166,
     121,   121,   217,   116,   117,   219,   219,   219,   210,   220,
     192,   213,   214,   219,   171,   218,   147,   215,   216,    58,
     122,   123,   124,   125,   126,   127,   128,   129,   139,   140,
     141,   149,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   170,   219,   225,   219,   219,   172,   220,   219,
     220,   219,   219,   219,   219,   120,   172,   172,   172,   172,
     172,   172,   220,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   172,   116,   120,   219,   116,   120,   219,   120,
     219,   111,   112,   170,   210,   169,   180,   219,   210,   171,
     219,    13,   192,   219,   220,   219,   192,   146,    11,   196,
     170,   172,   220,   223,   165,   166,   121,   121,   219,   172,
     171,   165,   166,   146,   167,   150,   146,   169,   172,   192,
     193,   220,   219,   217,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   172,    15,    16,    17,
      19,    21,    24,    25,    27,    29,    30,    31,    32,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,   105,
     106,   107,   108,   113,   226,   227,   231,   172,   172,   172,
     146,   172,   172,   172,   172,   146,   172,   146,   172,   146,
     146,   172,   146,   146,   146,   146,   172,   146,   172,   172,
     172,   172,   172,   172,   172,   172,   171,   180,   172,   172,
     172,   193,   172,   171,   171,   167,   196,   224,   197,   209,
     212,   218,   172,   187,   219,   180,   121,   219,   172,   167,
     210,   121,   219,   220,   220,   213,   146,   172,   150,   168,
     228,   232,   119,   120,   120,   169,   231,   219,   219,   120,
     219,   219,   219,   219,   116,   207,   219,   207,   219,   192,
     168,   172,   180,   219,   210,   171,   172,   201,   170,     6,
     167,   172,   171,   167,   192,   180,   219,   178,   226,   114,
     115,   116,   117,   118,   119,   120,   229,   230,   172,   146,
     206,   172,   146,   146,   206,   206,   206,   206,   146,   172,
     172,   190,   180,   172,   172,   210,   201,   180,   203,   180,
     210,   171,   169,    20,   146,    93,    94,   120,   172,   219,
     219,   172,   172,   172,   172,   219,   180,     8,     9,   169,
     191,   172,   180,   172,   172,   210,   229,   172,   172,   146,
     112,   219,   150,   200,   172,   219,   180,   150,   178,   180,
     146,   178,   219,     3,   146,   170,   219,   172
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
#line 162 "./swf5compiler.y"
    { bf = newBuffer();
		bc = newBuffer();
	;}
    break;

  case 3:
#line 165 "./swf5compiler.y"
    { Buffer b = newBuffer();
		  bufferWriteConstants(b);
		  bufferConcat(b, bf);
		  bufferConcat(b, bc);
		  *((Buffer *)buffer) = b; ;}
    break;

  case 4:
#line 170 "./swf5compiler.y"
    { Buffer b = newBuffer(); *((Buffer *)buffer) = b; ;}
    break;

  case 7:
#line 180 "./swf5compiler.y"
    { bufferConcat(bc, (yyvsp[(1) - (1)].action)); ;}
    break;

  case 8:
#line 182 "./swf5compiler.y"
    { bufferConcat(bf, (yyvsp[(1) - (1)].action)); ;}
    break;

  case 9:
#line 187 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action); ;}
    break;

  case 10:
#line 190 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (2)].action);
		  bufferConcat((yyval.action), (yyvsp[(2) - (2)].action)); ;}
    break;

  case 11:
#line 195 "./swf5compiler.y"
    { ;}
    break;

  case 12:
#line 199 "./swf5compiler.y"
    { (yyval.action) = NULL; ;}
    break;

  case 13:
#line 200 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); ;}
    break;

  case 14:
#line 201 "./swf5compiler.y"
    { (yyval.action) = NULL; ;}
    break;

  case 15:
#line 202 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (2)].action); ;}
    break;

  case 25:
#line 215 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); bufferWriteOp((yyval.action), SWFACTION_THROW); ;}
    break;

  case 26:
#line 219 "./swf5compiler.y"
    { 	(yyval.action) = newBuffer();
									bufferWriteOp((yyval.action), SWFACTION_TRY);
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (2)].action))+8);
									bufferWriteU8((yyval.action), 0);
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (2)].action)));
									bufferWriteS16((yyval.action), 0);
									bufferWriteS16((yyval.action), 0);
									bufferWriteU8((yyval.action), 0); /* catch name here? - empty string */
									bufferConcat((yyval.action), (yyvsp[(2) - (2)].action));
								 ;}
    break;

  case 27:
#line 229 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
									bufferWriteOp((yyval.action), SWFACTION_TRY);
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (7)].action))+strlen((yyvsp[(5) - (7)].str))+bufferLength((yyvsp[(7) - (7)].action))+8);
									bufferWriteU8((yyval.action), 0);
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (7)].action)));
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (7)].action)));
									bufferWriteS16((yyval.action), 0);
									bufferWriteHardString((yyval.action), (yyvsp[(5) - (7)].str), strlen((yyvsp[(5) - (7)].str))+1);
									bufferConcat((yyval.action), (yyvsp[(2) - (7)].action));
									bufferConcat((yyval.action), (yyvsp[(7) - (7)].action));
								;}
    break;

  case 28:
#line 240 "./swf5compiler.y"
    {	(yyval.action) = newBuffer();
									bufferWriteOp((yyval.action), SWFACTION_TRY);
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (4)].action))+bufferLength((yyvsp[(4) - (4)].action))+8);
									bufferWriteU8((yyval.action), 0);
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (4)].action)));
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(4) - (4)].action)));
									bufferWriteS16((yyval.action), 0);
									bufferWriteU8((yyval.action), 0); /* catch name here? - empty string */
									bufferConcat((yyval.action), (yyvsp[(2) - (4)].action));
									bufferConcat((yyval.action), (yyvsp[(4) - (4)].action));
								 ;}
    break;

  case 29:
#line 251 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
									bufferWriteOp((yyval.action), SWFACTION_TRY);
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (9)].action))+strlen((yyvsp[(5) - (9)].str))+bufferLength((yyvsp[(7) - (9)].action))+bufferLength((yyvsp[(9) - (9)].action))+8);
									bufferWriteU8((yyval.action), 0);
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (9)].action)));
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (9)].action)));
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(9) - (9)].action)));
									bufferWriteHardString((yyval.action), (yyvsp[(5) - (9)].str), strlen((yyvsp[(5) - (9)].str))+1);
									bufferConcat((yyval.action), (yyvsp[(2) - (9)].action));
									bufferConcat((yyval.action), (yyvsp[(7) - (9)].action));
									bufferConcat((yyval.action), (yyvsp[(9) - (9)].action));
								;}
    break;

  case 30:
#line 267 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferWriteOp((yyval.action), SWFACTION_WITH);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(6) - (7)].action)));
		  bufferConcat((yyval.action), (yyvsp[(6) - (7)].action)); ;}
    break;

  case 31:
#line 278 "./swf5compiler.y"
    { int tmp = chkctx(CTX_FUNCTION);
		  if(tmp < 0)
			swf5error("return outside function");
		  (yyval.action) = newBuffer();
		  while(--tmp >= 0)
			bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferWriteNull((yyval.action));
		  bufferWriteOp((yyval.action), SWFACTION_RETURN); ;}
    break;

  case 32:
#line 288 "./swf5compiler.y"
    { int tmp = chkctx(CTX_FUNCTION);
		  if(tmp < 0)
			swf5error("return outside function");
		  (yyval.action) = newBuffer();
		  while(--tmp >= 0)
			bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferConcat((yyval.action), (yyvsp[(2) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_RETURN); ;}
    break;

  case 34:
#line 300 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action)); ;}
    break;

  case 35:
#line 305 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferWriteOp((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (7)].action))+5);
		  bufferConcat((yyval.action), (yyvsp[(7) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (7)].action)));
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action)); ;}
    break;

  case 36:
#line 316 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (5)].action);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT);
		  bufferWriteOp((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (5)].action)));
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action)); ;}
    break;

  case 37:
#line 325 "./swf5compiler.y"
    { (yyval.action) = NULL; ;}
    break;

  case 38:
#line 326 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action); ;}
    break;

  case 39:
#line 331 "./swf5compiler.y"
    { addctx(CTX_SWITCH); ;}
    break;

  case 40:
#line 337 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferResolveSwitch((yyval.action), &(yyvsp[(6) - (7)].switchcases));
		  bufferResolveJumps((yyval.action));
		  bufferWriteOp((yyval.action), SWFACTION_POP);
		  delctx(CTX_SWITCH);
 /* FIXME: continue in switch continues surrounding loop, if any */
	;}
    break;

  case 41:
#line 349 "./swf5compiler.y"
    { (yyval.switchcases).count = 0;
		  (yyval.switchcases).list = 0; ;}
    break;

  case 42:
#line 353 "./swf5compiler.y"
    { (yyval.switchcases) = (yyvsp[(1) - (2)].switchcases);
		  (yyval.switchcases).list = (struct switchcase*) realloc((yyval.switchcases).list, ((yyval.switchcases).count+1) * sizeof(struct switchcase));
		  (yyval.switchcases).list[(yyval.switchcases).count] = (yyvsp[(2) - (2)].switchcase);
		  (yyval.switchcases).count++; ;}
    break;

  case 43:
#line 361 "./swf5compiler.y"
    { (yyval.switchcase).cond = (yyvsp[(2) - (6)].action);
		  (yyval.switchcase).action = (yyvsp[(4) - (6)].action);
		  (yyval.switchcase).isbreak = 1; ;}
    break;

  case 44:
#line 366 "./swf5compiler.y"
    { (yyval.switchcase).cond = (yyvsp[(2) - (4)].action);
		  (yyval.switchcase).action = (yyvsp[(4) - (4)].action);
		  (yyval.switchcase).isbreak = 0; ;}
    break;

  case 45:
#line 371 "./swf5compiler.y"
    { (yyval.switchcase).cond = NULL;
		  (yyval.switchcase).action = (yyvsp[(3) - (3)].action);
		  (yyval.switchcase).isbreak = 0; ;}
    break;

  case 47:
#line 382 "./swf5compiler.y"
    { (yyval.str) = strdup("new"); ;}
    break;

  case 48:
#line 383 "./swf5compiler.y"
    { (yyval.str) = strdup("delete"); ;}
    break;

  case 49:
#line 384 "./swf5compiler.y"
    { (yyval.str) = strdup("targetPath"); ;}
    break;

  case 50:
#line 385 "./swf5compiler.y"
    { (yyval.str) = strdup("random"); ;}
    break;

  case 51:
#line 386 "./swf5compiler.y"
    { (yyval.str) = strdup("getTimer"); ;}
    break;

  case 52:
#line 387 "./swf5compiler.y"
    { (yyval.str) = strdup("length"); ;}
    break;

  case 53:
#line 388 "./swf5compiler.y"
    { (yyval.str) = strdup("concat"); ;}
    break;

  case 54:
#line 389 "./swf5compiler.y"
    { (yyval.str) = strdup("substr"); ;}
    break;

  case 55:
#line 390 "./swf5compiler.y"
    { (yyval.str) = strdup("trace"); ;}
    break;

  case 56:
#line 391 "./swf5compiler.y"
    { (yyval.str) = strdup("int"); ;}
    break;

  case 57:
#line 392 "./swf5compiler.y"
    { (yyval.str) = strdup("ord"); ;}
    break;

  case 58:
#line 393 "./swf5compiler.y"
    { (yyval.str) = strdup("chr"); ;}
    break;

  case 59:
#line 394 "./swf5compiler.y"
    { (yyval.str) = strdup("getURL"); ;}
    break;

  case 60:
#line 395 "./swf5compiler.y"
    { (yyval.str) = strdup("getURL1"); ;}
    break;

  case 61:
#line 396 "./swf5compiler.y"
    { (yyval.str) = strdup("nextFrame"); ;}
    break;

  case 62:
#line 397 "./swf5compiler.y"
    { (yyval.str) = strdup("prevFrame"); ;}
    break;

  case 63:
#line 398 "./swf5compiler.y"
    { (yyval.str) = strdup("play"); ;}
    break;

  case 64:
#line 399 "./swf5compiler.y"
    { (yyval.str) = strdup("stop"); ;}
    break;

  case 65:
#line 400 "./swf5compiler.y"
    { (yyval.str) = strdup("toggleQuality"); ;}
    break;

  case 66:
#line 401 "./swf5compiler.y"
    { (yyval.str) = strdup("stopSounds"); ;}
    break;

  case 67:
#line 402 "./swf5compiler.y"
    { (yyval.str) = strdup("dup"); ;}
    break;

  case 68:
#line 403 "./swf5compiler.y"
    { (yyval.str) = strdup("swap"); ;}
    break;

  case 69:
#line 404 "./swf5compiler.y"
    { (yyval.str) = strdup("pop"); ;}
    break;

  case 70:
#line 405 "./swf5compiler.y"
    { (yyval.str) = strdup("push"); ;}
    break;

  case 71:
#line 406 "./swf5compiler.y"
    { (yyval.str) = strdup("setRegister"); ;}
    break;

  case 72:
#line 407 "./swf5compiler.y"
    { (yyval.str) = strdup("callFunction"); ;}
    break;

  case 73:
#line 408 "./swf5compiler.y"
    { (yyval.str) = strdup("callMethod"); ;}
    break;

  case 74:
#line 409 "./swf5compiler.y"
    { (yyval.str) = strdup("and"); ;}
    break;

  case 75:
#line 410 "./swf5compiler.y"
    { (yyval.str) = strdup("or"); ;}
    break;

  case 76:
#line 411 "./swf5compiler.y"
    { (yyval.str) = strdup("xor"); ;}
    break;

  case 77:
#line 412 "./swf5compiler.y"
    { (yyval.str) = strdup("modulo"); ;}
    break;

  case 78:
#line 413 "./swf5compiler.y"
    { (yyval.str) = strdup("add"); ;}
    break;

  case 79:
#line 414 "./swf5compiler.y"
    { (yyval.str) = strdup("lessThan"); ;}
    break;

  case 80:
#line 415 "./swf5compiler.y"
    { (yyval.str) = strdup("equals"); ;}
    break;

  case 81:
#line 416 "./swf5compiler.y"
    { (yyval.str) = strdup("inc"); ;}
    break;

  case 82:
#line 417 "./swf5compiler.y"
    { (yyval.str) = strdup("dec"); ;}
    break;

  case 83:
#line 418 "./swf5compiler.y"
    { (yyval.str) = strdup("typeof"); ;}
    break;

  case 84:
#line 419 "./swf5compiler.y"
    { (yyval.str) = strdup("instanceof"); ;}
    break;

  case 85:
#line 420 "./swf5compiler.y"
    { (yyval.str) = strdup("enumerate"); ;}
    break;

  case 86:
#line 421 "./swf5compiler.y"
    { (yyval.str) = strdup("initobject"); ;}
    break;

  case 87:
#line 422 "./swf5compiler.y"
    { (yyval.str) = strdup("initarray"); ;}
    break;

  case 88:
#line 423 "./swf5compiler.y"
    { (yyval.str) = strdup("getmember"); ;}
    break;

  case 89:
#line 424 "./swf5compiler.y"
    { (yyval.str) = strdup("setmember"); ;}
    break;

  case 90:
#line 425 "./swf5compiler.y"
    { (yyval.str) = strdup("shiftleft"); ;}
    break;

  case 91:
#line 426 "./swf5compiler.y"
    { (yyval.str) = strdup("shiftright"); ;}
    break;

  case 92:
#line 427 "./swf5compiler.y"
    { (yyval.str) = strdup("shiftright2"); ;}
    break;

  case 93:
#line 428 "./swf5compiler.y"
    { (yyval.str) = strdup("varequals"); ;}
    break;

  case 94:
#line 429 "./swf5compiler.y"
    { (yyval.str) = strdup("oldAdd"); ;}
    break;

  case 95:
#line 430 "./swf5compiler.y"
    { (yyval.str) = strdup("subtract"); ;}
    break;

  case 96:
#line 431 "./swf5compiler.y"
    { (yyval.str) = strdup("multiply"); ;}
    break;

  case 97:
#line 432 "./swf5compiler.y"
    { (yyval.str) = strdup("divide"); ;}
    break;

  case 98:
#line 433 "./swf5compiler.y"
    { (yyval.str) = strdup("oldequals"); ;}
    break;

  case 99:
#line 434 "./swf5compiler.y"
    { (yyval.str) = strdup("oldlessthan"); ;}
    break;

  case 100:
#line 435 "./swf5compiler.y"
    { (yyval.str) = strdup("logicaland"); ;}
    break;

  case 101:
#line 436 "./swf5compiler.y"
    { (yyval.str) = strdup("logicalor"); ;}
    break;

  case 102:
#line 437 "./swf5compiler.y"
    { (yyval.str) = strdup("not"); ;}
    break;

  case 103:
#line 438 "./swf5compiler.y"
    { (yyval.str) = strdup("stringeq"); ;}
    break;

  case 104:
#line 439 "./swf5compiler.y"
    { (yyval.str) = strdup("stringlength"); ;}
    break;

  case 105:
#line 440 "./swf5compiler.y"
    { (yyval.str) = strdup("substring"); ;}
    break;

  case 106:
#line 441 "./swf5compiler.y"
    { (yyval.str) = strdup("getvariable"); ;}
    break;

  case 107:
#line 442 "./swf5compiler.y"
    { (yyval.str) = strdup("setvariable"); ;}
    break;

  case 108:
#line 443 "./swf5compiler.y"
    { (yyval.str) = strdup("settargetexpression"); ;}
    break;

  case 109:
#line 444 "./swf5compiler.y"
    { (yyval.str) = strdup("duplicatemovieclip"); ;}
    break;

  case 110:
#line 445 "./swf5compiler.y"
    { (yyval.str) = strdup("removemovieclip"); ;}
    break;

  case 111:
#line 446 "./swf5compiler.y"
    { (yyval.str) = strdup("startdrag"); ;}
    break;

  case 112:
#line 447 "./swf5compiler.y"
    { (yyval.str) = strdup("stopdrag"); ;}
    break;

  case 113:
#line 448 "./swf5compiler.y"
    { (yyval.str) = strdup("stringlessthan"); ;}
    break;

  case 114:
#line 449 "./swf5compiler.y"
    { (yyval.str) = strdup("mblength"); ;}
    break;

  case 115:
#line 450 "./swf5compiler.y"
    { (yyval.str) = strdup("mbsubstring"); ;}
    break;

  case 116:
#line 451 "./swf5compiler.y"
    { (yyval.str) = strdup("mbord"); ;}
    break;

  case 117:
#line 452 "./swf5compiler.y"
    { (yyval.str) = strdup("mbchr"); ;}
    break;

  case 118:
#line 453 "./swf5compiler.y"
    { (yyval.str) = strdup("branchalways"); ;}
    break;

  case 119:
#line 454 "./swf5compiler.y"
    { (yyval.str) = strdup("branchiftrue"); ;}
    break;

  case 120:
#line 455 "./swf5compiler.y"
    { (yyval.str) = strdup("getURL2"); ;}
    break;

  case 121:
#line 456 "./swf5compiler.y"
    { (yyval.str) = strdup("post"); ;}
    break;

  case 122:
#line 457 "./swf5compiler.y"
    { (yyval.str) = strdup("get"); ;}
    break;

  case 123:
#line 458 "./swf5compiler.y"
    { (yyval.str) = strdup("loadvariables"); ;}
    break;

  case 124:
#line 459 "./swf5compiler.y"
    { (yyval.str) = strdup("loadmovie"); ;}
    break;

  case 125:
#line 460 "./swf5compiler.y"
    { (yyval.str) = strdup("extends"); ;}
    break;

  case 126:
#line 461 "./swf5compiler.y"
    { (yyval.str) = strdup("gotoAndStop"); ;}
    break;

  case 127:
#line 462 "./swf5compiler.y"
    { (yyval.str) = strdup("gotoAndPlay"); ;}
    break;

  case 128:
#line 467 "./swf5compiler.y"
    { (yyval.exprlist).buffer = newBuffer();
		  (yyval.exprlist).count = 0; ;}
    break;

  case 129:
#line 471 "./swf5compiler.y"
    { (yyval.exprlist).buffer = newBuffer();
		  bufferWriteHardString((yyval.exprlist).buffer, (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  (yyval.exprlist).count = 1;
		  free((yyvsp[(1) - (1)].str)); ;}
    break;

  case 130:
#line 477 "./swf5compiler.y"
    { (yyval.exprlist) = (yyvsp[(1) - (3)].exprlist);
		  bufferWriteHardString((yyval.exprlist).buffer, (yyvsp[(3) - (3)].str), strlen((yyvsp[(3) - (3)].str))+1);
		  ++(yyval.exprlist).count;
		  free((yyvsp[(3) - (3)].str)); ;}
    break;

  case 131:
#line 485 "./swf5compiler.y"
    { addctx(CTX_FUNCTION); ;}
    break;

  case 132:
#line 490 "./swf5compiler.y"
    {
		(yyval.action) = newBuffer();
		if(swfVersion > 6)
		{
			// TODO: let user control which flags to use ?
			// Don't preload any variable in registers, or we'll need to track all uses of 
			// those variables in this function context turning them into register accesses
			int flags = 0;
			int num_regs = 0;
			bufferWriteDefineFunction2((yyval.action), (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].exprlist).buffer, (yyvsp[(6) - (6)].action), flags, num_regs);
		}
		else
		{
			bufferWriteOp((yyval.action), SWFACTION_DEFINEFUNCTION);
			bufferWriteS16((yyval.action), strlen((yyvsp[(2) - (6)].str)) +
				bufferLength((yyvsp[(4) - (6)].exprlist).buffer) + 5);
			bufferWriteHardString((yyval.action), (yyvsp[(2) - (6)].str), strlen((yyvsp[(2) - (6)].str))+1);
			bufferWriteS16((yyval.action), (yyvsp[(4) - (6)].exprlist).count);
			bufferConcat((yyval.action), (yyvsp[(4) - (6)].exprlist).buffer);
			bufferWriteS16((yyval.action), bufferLength((yyvsp[(6) - (6)].action)));
			bufferConcat((yyval.action), (yyvsp[(6) - (6)].action));
			delctx(CTX_FUNCTION);
			free((yyvsp[(2) - (6)].str)); // should be done for function2 as well ?
		}
	;}
    break;

  case 133:
#line 519 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action); ;}
    break;

  case 134:
#line 524 "./swf5compiler.y"
    { if((yyvsp[(1) - (1)].lval).obj)
		  {
		    (yyval.action) = (yyvsp[(1) - (1)].lval).obj;

		    if((yyvsp[(1) - (1)].lval).ident)
		      bufferConcat((yyval.action), (yyvsp[(1) - (1)].lval).ident);
		    else
		      bufferConcat((yyval.action), (yyvsp[(1) - (1)].lval).memexpr);

		    bufferWriteOp((yyval.action), SWFACTION_GETMEMBER);
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (1)].lval).ident;
		  }
		;}
    break;

  case 137:
#line 546 "./swf5compiler.y"
    { addctx(CTX_LOOP); ;}
    break;

  case 138:
#line 551 "./swf5compiler.y"
    { addctx(CTX_LOOP); ;}
    break;

  case 139:
#line 556 "./swf5compiler.y"
    { addctx(CTX_LOOP); ;}
    break;

  case 140:
#line 561 "./swf5compiler.y"
    { addctx(CTX_FOR_IN); ;}
    break;

  case 141:
#line 566 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (5)].action);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT);
		  bufferWriteOp((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (5)].action))+5);
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
		  bufferWriteOp((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), -(bufferLength((yyval.action))+2));
		  bufferResolveJumps((yyval.action));
		  delctx(CTX_LOOP); ;}
    break;

  case 142:
#line 579 "./swf5compiler.y"
    { if((yyvsp[(2) - (6)].action))
			{	(yyval.action) = (yyvsp[(2) - (6)].action);
		  		bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
			}
			else
				(yyval.action) = (yyvsp[(5) - (6)].action);
		  bufferWriteOp((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), -(bufferLength((yyval.action))+2));
		  bufferResolveJumps((yyval.action));
		  delctx(CTX_LOOP); ;}
    break;

  case 143:
#line 592 "./swf5compiler.y"
    {
		  if((yyvsp[(3) - (10)].action))
		    (yyval.action) = (yyvsp[(3) - (10)].action);
		  else
		    (yyval.action) = newBuffer();

		  if((yyvsp[(7) - (10)].action))
		  {
                    bufferWriteOp((yyval.action), SWFACTION_JUMP);
                    bufferWriteS16((yyval.action), 2);
                    bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (10)].action)));
		  }
		  else
		    (yyvsp[(7) - (10)].action) = newBuffer();

		  if((yyvsp[(5) - (10)].action))
		  {
		    bufferConcat((yyvsp[(7) - (10)].action), (yyvsp[(5) - (10)].action));
                    bufferWriteOp((yyvsp[(7) - (10)].action), SWFACTION_LOGICALNOT);
                    bufferWriteOp((yyvsp[(7) - (10)].action), SWFACTION_IF);
                    bufferWriteS16((yyvsp[(7) - (10)].action), 2);
                    bufferWriteS16((yyvsp[(7) - (10)].action), bufferLength((yyvsp[(10) - (10)].action))+5);
                  }

                  bufferConcat((yyvsp[(7) - (10)].action), (yyvsp[(10) - (10)].action));
                  bufferWriteOp((yyvsp[(7) - (10)].action), SWFACTION_JUMP);
                  bufferWriteS16((yyvsp[(7) - (10)].action), 2);
                  bufferWriteS16((yyvsp[(7) - (10)].action), -(bufferLength((yyvsp[(7) - (10)].action))+2));
                  bufferResolveJumps((yyvsp[(7) - (10)].action));

                  bufferConcat((yyval.action), (yyvsp[(7) - (10)].action));
				  delctx(CTX_LOOP);
                ;}
    break;

  case 144:
#line 627 "./swf5compiler.y"
    { Buffer b2, b3;
		  int tmp;

		  (yyval.action) = (yyvsp[(4) - (7)].action);
		  bufferWriteOp((yyval.action), SWFACTION_ENUMERATE);	

		  b2 = newBuffer();
		  bufferWriteSetRegister(b2, 0);
		  bufferWriteOp(b2, SWFACTION_PUSH);
		  bufferWriteS16(b2, 1);
		  bufferWriteU8(b2, 2);
		  bufferWriteOp(b2, SWFACTION_EQUALS2);
		  bufferWriteOp(b2, SWFACTION_IF);
		  bufferWriteS16(b2, 2);

		  b3 = newBuffer();
/* basically a lvalue could be used here rather than an ident !!! */
/* probably by using reg1 for the test rather than reg0 */
		  bufferWriteString(b3, (yyvsp[(3) - (7)].str), strlen((yyvsp[(3) - (7)].str))+1);
		  bufferWriteRegister(b3, 0);
		  bufferWriteOp(b3, SWFACTION_SETVARIABLE);
		  bufferConcat(b3, (yyvsp[(7) - (7)].action));
		  bufferWriteS16(b2, bufferLength(b3) + 5);
		  tmp = bufferLength(b2) + bufferLength(b3) + 5;
		  bufferConcat((yyval.action), b2);
		  bufferWriteOp(b3, SWFACTION_JUMP);
		  bufferWriteS16(b3, 2);
		  bufferWriteS16(b3, -tmp);
		  bufferResolveJumps(b3);
		  bufferConcat((yyval.action), b3);
		  delctx(CTX_FOR_IN);
		  free((yyvsp[(3) - (7)].str)); ;}
    break;

  case 145:
#line 661 "./swf5compiler.y"
    { Buffer b2, b3;
		  int tmp;

		  (yyval.action) = (yyvsp[(5) - (8)].action);
		  bufferWriteOp((yyval.action), SWFACTION_ENUMERATE);	

		  b2 = newBuffer();
		  bufferWriteSetRegister(b2, 0);
		  bufferWriteOp(b2, SWFACTION_PUSH);
		  bufferWriteS16(b2, 1);
		  bufferWriteU8(b2, 2);
		  bufferWriteOp(b2, SWFACTION_EQUALS2);
		  bufferWriteOp(b2, SWFACTION_IF);
		  bufferWriteS16(b2, 2);
		  // add size later

		  b3 = newBuffer();
		  bufferWriteString(b3, (yyvsp[(4) - (8)].str), strlen((yyvsp[(4) - (8)].str))+1);
		  bufferWriteRegister(b3, 0);
		  bufferWriteOp(b3, SWFACTION_DEFINELOCAL);
		  bufferConcat(b3, (yyvsp[(8) - (8)].action));
		  bufferWriteS16(b2, bufferLength(b3) + 5);
		  tmp = bufferLength(b2) + bufferLength(b3) + 5;
		  bufferConcat((yyval.action), b2);
		  bufferWriteOp(b3, SWFACTION_JUMP);
		  bufferWriteS16(b3, 2);
		  bufferWriteS16(b3, -tmp);
		  bufferResolveJumps(b3);
		  bufferConcat((yyval.action), b3);
		  delctx(CTX_FOR_IN);
		  free((yyvsp[(4) - (8)].str)); ;}
    break;

  case 146:
#line 695 "./swf5compiler.y"
    { (yyval.action) = NULL; ;}
    break;

  case 148:
#line 703 "./swf5compiler.y"
    { if(chkctx(CTX_CONTINUE) < 0)
			swf5error("continue outside loop");
		  (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), MAGIC_CONTINUE_NUMBER); ;}
    break;

  case 149:
#line 714 "./swf5compiler.y"
    { int tmp = chkctx(CTX_BREAK);
		  if(tmp < 0)
			swf5error("break outside switch / loop");
		  (yyval.action) = newBuffer();
		  if(tmp)	/* break out of a for .. in */
			bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferWriteOp((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), MAGIC_BREAK_NUMBER); ;}
    break;

  case 150:
#line 726 "./swf5compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_NOSEND; ;}
    break;

  case 151:
#line 728 "./swf5compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_GET; ;}
    break;

  case 152:
#line 730 "./swf5compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_POST; ;}
    break;

  case 153:
#line 732 "./swf5compiler.y"
    { if(strcmp((yyvsp[(2) - (2)].str), "GET") == 0)
				    (yyval.getURLMethod) = GETURL_METHOD_GET;
				  else if(strcmp((yyvsp[(2) - (2)].str), "POST") == 0)
				    (yyval.getURLMethod) = GETURL_METHOD_POST; ;}
    break;

  case 154:
#line 740 "./swf5compiler.y"
    { char *lvlstring = (char*) malloc(12*sizeof(char));
		  sprintf(lvlstring, "_level%d", (yyvsp[(1) - (1)].intVal));
		  (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), lvlstring, strlen(lvlstring)+1);
		  free(lvlstring); ;}
    break;

  case 155:
#line 747 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), "_level", 7);
		  bufferConcat((yyval.action), (yyvsp[(1) - (1)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STRINGCONCAT); ;}
    break;

  case 156:
#line 755 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("void_function_call: IDENTIFIER '(' expr_list ')'\n");
#endif
		  (yyval.action) = (yyvsp[(3) - (4)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(3) - (4)].exprlist).count);
		  bufferWriteString((yyval.action), (yyvsp[(1) - (4)].str), strlen((yyvsp[(1) - (4)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_CALLFUNCTION);
		  bufferWriteOp((yyval.action), SWFACTION_POP);
		  free((yyvsp[(1) - (4)].str)); ;}
    break;

  case 157:
#line 767 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str));
		  bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE); 
		  bufferWriteOp((yyval.action), SWFACTION_TARGETPATH); 
		  bufferWriteOp((yyval.action), SWFACTION_POP); ;}
    break;

  case 158:
#line 775 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(2) - (2)].str), strlen((yyvsp[(2) - (2)].str))+1);
		  free((yyvsp[(2) - (2)].str));
		  bufferWriteOp((yyval.action), SWFACTION_DELETE2);
		  bufferWriteOp((yyval.action), SWFACTION_POP); ;}
    break;

  case 159:
#line 782 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (4)].action);
		  // bufferWriteOp($$, SWFACTION_GETVARIABLE);
		  bufferWriteString((yyval.action), (yyvsp[(4) - (4)].str), strlen((yyvsp[(4) - (4)].str))+1);
		  free((yyvsp[(4) - (4)].str));
		  bufferWriteOp((yyval.action), SWFACTION_DELETE);
		  bufferWriteOp((yyval.action), SWFACTION_POP); ;}
    break;

  case 160:
#line 790 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (5)].action);
		  // bufferWriteOp($$, SWFACTION_GETVARIABLE);
		  bufferConcat((yyval.action), (yyvsp[(4) - (5)].action));
		  // bufferWriteOp($$, SWFACTION_GETVARIABLE);
		  bufferWriteOp((yyval.action), SWFACTION_DELETE); 
		  bufferWriteOp((yyval.action), SWFACTION_POP); ;}
    break;

  case 161:
#line 798 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_TRACE); ;}
    break;

  case 162:
#line 802 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("void_function_call: GETURL '(' expr ')'\n");
#endif
		  (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_PUSH);
		  bufferWriteS16((yyval.action), 2); bufferWriteS16((yyval.action), 0); // two 0 bytes	
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0); ;}
    break;

  case 163:
#line 814 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("void_function_call: GETURL '(' expr ',' expr urlmethod ')'\n");
#endif
		  (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), (yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 164:
#line 825 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0xc0+(yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 165:
#line 832 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0x80+(yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 166:
#line 839 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0x40+(yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 167:
#line 846 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), (yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 168:
#line 853 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_CALLFRAME);
		  bufferWriteS16((yyval.action), 0); ;}
    break;

  case 169:
#line 859 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), "0", 2); /* no constraint */
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (6)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STARTDRAG); ;}
    break;

  case 170:
#line 866 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferConcat((yyval.action), (yyvsp[(7) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(11) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(9) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(13) - (14)].action));
		  bufferWriteString((yyval.action), "1", 2); /* has constraint */
		  bufferConcat((yyval.action), (yyvsp[(5) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (14)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STARTDRAG); ;}
    break;

  case 171:
#line 877 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_ENDDRAG); ;}
    break;

  case 172:
#line 882 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (8)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(7) - (8)].action));
		  bufferWriteInt((yyval.action), 16384); /* magic number */
		  bufferWriteOp((yyval.action), SWFACTION_ADD);
		  bufferWriteOp((yyval.action), SWFACTION_DUPLICATECLIP); ;}
    break;

  case 173:
#line 890 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_REMOVECLIP); ;}
    break;

  case 174:
#line 894 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GETURL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (6)].str)) + strlen((yyvsp[(5) - (6)].str)) + 2);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (6)].str), strlen((yyvsp[(3) - (6)].str)));
		  bufferWriteU8((yyval.action), 0);
		  bufferWriteHardString((yyval.action), (yyvsp[(5) - (6)].str), strlen((yyvsp[(5) - (6)].str)));
		  bufferWriteU8((yyval.action), 0); ;}
    break;

  case 175:
#line 904 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_NEXTFRAME); ;}
    break;

  case 176:
#line 908 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_PREVFRAME); ;}
    break;

  case 177:
#line 912 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_PLAY); ;}
    break;

  case 178:
#line 916 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_STOP); ;}
    break;

  case 179:
#line 920 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_STOPSOUNDS); ;}
    break;

  case 180:
#line 924 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_TOGGLEQUALITY); ;}
    break;

  case 181:
#line 928 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOFRAME);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), (yyvsp[(3) - (4)].intVal)-1); /* GOTOFRAME target is 0-based */
		  bufferWriteOp((yyval.action), SWFACTION_PLAY); ;}
    break;

  case 182:
#line 935 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOFRAME);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), (yyvsp[(3) - (4)].intVal)-1);  /* GOTOFRAME target is 0-based */
		  bufferWriteOp((yyval.action), SWFACTION_STOP); ;}
    break;

  case 183:
#line 942 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOLABEL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); 
		  bufferWriteOp((yyval.action), SWFACTION_PLAY); ;}
    break;

  case 184:
#line 950 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOLABEL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); 
		  bufferWriteOp((yyval.action), SWFACTION_STOP); ;}
    break;

  case 185:
#line 958 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_GOTOFRAME2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 1); ;}
    break;

  case 186:
#line 964 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_GOTOFRAME2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0); ;}
    break;

  case 187:
#line 970 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_SETTARGET);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); ;}
    break;

  case 188:
#line 977 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_SETTARGET2); ;}
    break;

  case 189:
#line 986 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("function_call: %s '(' expr_list ')'\n", (yyvsp[(1) - (4)].str));
#endif
		  (yyval.action) = (yyvsp[(3) - (4)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(3) - (4)].exprlist).count);
		  bufferWriteString((yyval.action), (yyvsp[(1) - (4)].str), strlen((yyvsp[(1) - (4)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_CALLFUNCTION);
		  free((yyvsp[(1) - (4)].str)); ;}
    break;

  case 190:
#line 997 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str));
		  bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE); 
		  bufferWriteOp((yyval.action), SWFACTION_TARGETPATH); ;}
    break;

  case 191:
#line 1005 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(2) - (2)].str), strlen((yyvsp[(2) - (2)].str))+1);
		  free((yyvsp[(2) - (2)].str));
		  bufferWriteOp((yyval.action), SWFACTION_DELETE2);  ;}
    break;

  case 192:
#line 1011 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (4)].action);
		  bufferWriteString((yyval.action), (yyvsp[(4) - (4)].str), strlen((yyvsp[(4) - (4)].str))+1);
		  free((yyvsp[(4) - (4)].str));
		  bufferWriteOp((yyval.action), SWFACTION_DELETE); ;}
    break;

  case 193:
#line 1017 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (5)].action);
		  bufferConcat((yyval.action), (yyvsp[(4) - (5)].action));
		  bufferWriteOp((yyval.action), SWFACTION_DELETE);  ;}
    break;

  case 194:
#line 1022 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE); ;}
    break;

  case 195:
#line 1026 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GETTIME); ;}
    break;

  case 196:
#line 1030 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_RANDOMNUMBER); ;}
    break;

  case 197:
#line 1034 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_STRINGLENGTH); ;}
    break;

  case 198:
#line 1038 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_INT); ;}
    break;

  case 199:
#line 1042 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_ORD); ;}
    break;

  case 200:
#line 1046 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_CHR); ;}
    break;

  case 201:
#line 1050 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (6)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STRINGCONCAT); ;}
    break;

  case 202:
#line 1055 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (8)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(7) - (8)].action));
		  bufferWriteOp((yyval.action), SWFACTION_SUBSTRING); ;}
    break;

  case 203:
#line 1061 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("function_call: TYPEOF '(' expr_or_obj ')'\n");
#endif
		  (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_TYPEOF); ;}
    break;

  case 204:
#line 1073 "./swf5compiler.y"
    { (yyval.exprlist).buffer = newBuffer();
		  (yyval.exprlist).count = 0; ;}
    break;

  case 205:
#line 1077 "./swf5compiler.y"
    { (yyval.exprlist).buffer = (yyvsp[(1) - (1)].action);
		  (yyval.exprlist).count = 1; ;}
    break;

  case 206:
#line 1082 "./swf5compiler.y"
    { Buffer tmp = newBuffer();
		  bufferConcat(tmp, (yyvsp[(3) - (3)].action));
		  bufferConcat(tmp, (yyval.exprlist).buffer);
		  (yyval.exprlist).buffer = tmp;
		  ++(yyval.exprlist).count;  ;}
    break;

  case 207:
#line 1091 "./swf5compiler.y"
    {
		(yyval.action) = newBuffer();
		if(swfVersion > 6)
		{
			// TODO: let user control which flags to use ?
			// Don't preload any variable in registers, or we'll need to track all uses of 
			// those variables in this function context turning them into register accesses
			int flags = 0;
			int num_regs = 0;
			bufferWriteDefineFunction2((yyval.action), NULL, (yyvsp[(3) - (5)].exprlist).buffer, (yyvsp[(5) - (5)].action), flags, num_regs);
		}
		else
		{
			bufferWriteOp((yyval.action), SWFACTION_DEFINEFUNCTION);
			bufferWriteS16((yyval.action), bufferLength((yyvsp[(3) - (5)].exprlist).buffer) + 5); /* what's 5 here ? */
			bufferWriteU8((yyval.action), 0); /* empty function name */
			bufferWriteS16((yyval.action), (yyvsp[(3) - (5)].exprlist).count);
			bufferConcat((yyval.action), (yyvsp[(3) - (5)].exprlist).buffer);
			bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (5)].action)));
			bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
			delctx(CTX_FUNCTION); // should be done for function2 as well ?
		}
	;}
    break;

  case 208:
#line 1118 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(5) - (6)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(5) - (6)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(1) - (6)].action));
		  bufferWriteString((yyval.action), (yyvsp[(3) - (6)].str), strlen((yyvsp[(3) - (6)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_CALLMETHOD);
		  free((yyvsp[(3) - (6)].str)); ;}
    break;

  case 209:
#line 1126 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(6) - (7)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(6) - (7)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(1) - (7)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_CALLMETHOD); ;}
    break;

  case 210:
#line 1135 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (3)].str), strlen((yyvsp[(1) - (3)].str))+1);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  free((yyvsp[(1) - (3)].str)); ;}
    break;

  case 211:
#line 1143 "./swf5compiler.y"
    { (yyval.exprlist).buffer = (yyvsp[(1) - (1)].action);
		  (yyval.exprlist).count = 1; ;}
    break;

  case 212:
#line 1147 "./swf5compiler.y"
    { bufferConcat((yyval.exprlist).buffer, (yyvsp[(3) - (3)].action));
		  ++(yyval.exprlist).count;  ;}
    break;

  case 213:
#line 1152 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_ADD2; ;}
    break;

  case 214:
#line 1153 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_SUBTRACT; ;}
    break;

  case 215:
#line 1154 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_MULTIPLY; ;}
    break;

  case 216:
#line 1155 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_DIVIDE; ;}
    break;

  case 217:
#line 1156 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_MODULO; ;}
    break;

  case 218:
#line 1157 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_BITWISEAND; ;}
    break;

  case 219:
#line 1158 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_BITWISEOR; ;}
    break;

  case 220:
#line 1159 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_BITWISEXOR; ;}
    break;

  case 221:
#line 1160 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_SHIFTLEFT; ;}
    break;

  case 222:
#line 1161 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_SHIFTRIGHT; ;}
    break;

  case 223:
#line 1162 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_SHIFTRIGHT2; ;}
    break;

  case 224:
#line 1166 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_INCREMENT; ;}
    break;

  case 225:
#line 1167 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_DECREMENT; ;}
    break;

  case 226:
#line 1186 "./swf5compiler.y"
    { if((yyvsp[(1) - (1)].lval).obj)
		  {
		    (yyval.action) = (yyvsp[(1) - (1)].lval).obj;

		    if((yyvsp[(1) - (1)].lval).ident)
		      bufferConcat((yyval.action), (yyvsp[(1) - (1)].lval).ident);
		    else
		      bufferConcat((yyval.action), (yyvsp[(1) - (1)].lval).memexpr);

		    bufferWriteOp((yyval.action), SWFACTION_GETMEMBER);
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (1)].lval).ident;
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		  }
		;}
    break;

  case 229:
#line 1210 "./swf5compiler.y"
    { (yyval.lval).ident = newBuffer();
		  bufferWriteString((yyval.lval).ident, (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str));
		  (yyval.lval).obj = 0;
		  (yyval.lval).memexpr = 0; ;}
    break;

  case 230:
#line 1217 "./swf5compiler.y"
    { (yyval.lval).obj = (yyvsp[(1) - (3)].action);
		  (yyval.lval).ident = newBuffer();
		  bufferWriteString((yyval.lval).ident, (yyvsp[(3) - (3)].str), strlen((yyvsp[(3) - (3)].str))+1);
		  free((yyvsp[(3) - (3)].str));
		  (yyval.lval).memexpr = 0; ;}
    break;

  case 231:
#line 1224 "./swf5compiler.y"
    { (yyval.lval).obj = (yyvsp[(1) - (4)].action);
		  (yyval.lval).memexpr = (yyvsp[(3) - (4)].action);
		  (yyval.lval).ident = 0; ;}
    break;

  case 233:
#line 1235 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteInt((yyvsp[(2) - (2)].action), -1);
		  bufferWriteOp((yyvsp[(2) - (2)].action), SWFACTION_MULTIPLY); ;}
    break;

  case 234:
#line 1240 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteInt((yyvsp[(2) - (2)].action), 0xffffffff);
		  bufferWriteOp((yyvsp[(2) - (2)].action), SWFACTION_BITWISEXOR); ;}
    break;

  case 235:
#line 1245 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteOp((yyvsp[(2) - (2)].action), SWFACTION_LOGICALNOT); ;}
    break;

  case 236:
#line 1249 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALOR); ;}
    break;

  case 237:
#line 1254 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALAND); ;}
    break;

  case 238:
#line 1259 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_MULTIPLY); ;}
    break;

  case 239:
#line 1264 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_DIVIDE); ;}
    break;

  case 240:
#line 1269 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_MODULO); ;}
    break;

  case 241:
#line 1274 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_ADD2); ;}
    break;

  case 242:
#line 1279 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_SUBTRACT); ;}
    break;

  case 243:
#line 1284 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BITWISEAND); ;}
    break;

  case 244:
#line 1289 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BITWISEOR); ;}
    break;

  case 245:
#line 1294 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BITWISEXOR); ;}
    break;

  case 246:
#line 1299 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_LESS2); ;}
    break;

  case 247:
#line 1304 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(1) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_LESS2); ;}
    break;

  case 248:
#line 1309 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(1) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_LESS2);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT); ;}
    break;

  case 249:
#line 1315 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LESS2);
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); ;}
    break;

  case 250:
#line 1320 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_EQUALS2); ;}
    break;

  case 251:
#line 1324 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_STRICTEQUALS); ;}
    break;

  case 252:
#line 1328 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_EQUALS2);
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); ;}
    break;

  case 253:
#line 1333 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_STRICTEQUALS); 
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); ;}
    break;

  case 254:
#line 1338 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_SHIFTLEFT); ;}
    break;

  case 255:
#line 1342 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_SHIFTRIGHT); ;}
    break;

  case 256:
#line 1346 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_SHIFTRIGHT2); ;}
    break;

  case 257:
#line 1350 "./swf5compiler.y"
    { bufferWriteOp((yyvsp[(1) - (5)].action), SWFACTION_IF);
		  bufferWriteS16((yyvsp[(1) - (5)].action), 2);
		  bufferWriteS16((yyvsp[(1) - (5)].action), bufferLength((yyvsp[(5) - (5)].action))+5);
		  bufferConcat((yyvsp[(1) - (5)].action), (yyvsp[(5) - (5)].action));
		  bufferWriteOp((yyvsp[(1) - (5)].action), SWFACTION_JUMP);
		  bufferWriteS16((yyvsp[(1) - (5)].action), 2);
		  bufferWriteS16((yyvsp[(1) - (5)].action), bufferLength((yyvsp[(3) - (5)].action)));
		  bufferConcat((yyvsp[(1) - (5)].action), (yyvsp[(3) - (5)].action)); ;}
    break;

  case 258:
#line 1360 "./swf5compiler.y"
    { if((yyvsp[(1) - (3)].lval).obj) /* obj[memexpr] or obj.ident */
		  {
		    (yyval.action) = (yyvsp[(1) - (3)].lval).obj;

		    if((yyvsp[(1) - (3)].lval).ident)
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).ident);
		    else
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).memexpr);

		    bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		    bufferWriteSetRegister((yyval.action), 0);
		    bufferWriteOp((yyval.action), SWFACTION_SETMEMBER);
		    bufferWriteRegister((yyval.action), 0);
		  }
		  else /* just ident */
		  {
		    (yyval.action) = (yyvsp[(3) - (3)].action);
		    bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);
		    bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
/* tricky case missing here: lvalue ASSIGN expr */
/* like in x = y += z; */
		;}
    break;

  case 259:
#line 1387 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_INSTANCEOF); ;}
    break;

  case 261:
#line 1397 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW %s\n", (yyvsp[(2) - (2)].str));
#endif
		  (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferWriteString((yyval.action), (yyvsp[(2) - (2)].str), strlen((yyvsp[(2) - (2)].str))+1);
		  free((yyvsp[(2) - (2)].str));
		  bufferWriteOp((yyval.action), SWFACTION_NEWOBJECT); ;}
    break;

  case 262:
#line 1408 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW %s '(' expr_list ')'\n", (yyvsp[(2) - (5)].str));
#endif
		  (yyval.action) = (yyvsp[(4) - (5)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(4) - (5)].exprlist).count);
		  bufferWriteString((yyval.action), (yyvsp[(2) - (5)].str), strlen((yyvsp[(2) - (5)].str))+1);
		  free((yyvsp[(2) - (5)].str));
		  bufferWriteOp((yyval.action), SWFACTION_NEWOBJECT); ;}
    break;

  case 263:
#line 1419 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW lvalue_expr '.' %s\n", (yyvsp[(4) - (4)].str));
#endif
		  (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferConcat((yyval.action), (yyvsp[(2) - (4)].action));
		  bufferWriteString((yyval.action), (yyvsp[(4) - (4)].str), strlen((yyvsp[(4) - (4)].str))+1);
		  free((yyvsp[(4) - (4)].str));
		  bufferWriteOp((yyval.action), SWFACTION_NEWMETHOD); ;}
    break;

  case 264:
#line 1431 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW lvalue_expr '[' expr ']'\n");
#endif
		  (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferConcat((yyval.action), (yyvsp[(2) - (5)].action));
		  bufferConcat((yyval.action), (yyvsp[(4) - (5)].action));
		  bufferWriteOp((yyval.action), SWFACTION_NEWMETHOD); ;}
    break;

  case 265:
#line 1443 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW lvalue_expr '.' %s '(' expr_list ')'\n", (yyvsp[(4) - (7)].str));
#endif
		  (yyval.action) = (yyvsp[(6) - (7)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(6) - (7)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(2) - (7)].action));
		  bufferWriteString((yyval.action), (yyvsp[(4) - (7)].str), strlen((yyvsp[(4) - (7)].str))+1);
		  free((yyvsp[(4) - (7)].str));
		  bufferWriteOp((yyval.action), SWFACTION_NEWMETHOD); ;}
    break;

  case 266:
#line 1455 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW lvalue_expr '[' expr ']' '(' expr_list ')'\n");
#endif
		  (yyval.action) = (yyvsp[(7) - (8)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(7) - (8)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(2) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(4) - (8)].action));
		  bufferWriteOp((yyval.action), SWFACTION_NEWMETHOD); ;}
    break;

  case 267:
#line 1466 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(2) - (3)].exprlist).count);
		  bufferWriteOp((yyval.action), SWFACTION_INITARRAY); ;}
    break;

  case 268:
#line 1471 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferWriteOp((yyval.action), SWFACTION_INITOBJECT); ;}
    break;

  case 269:
#line 1476 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(2) - (3)].exprlist).count);
		  bufferWriteOp((yyval.action), SWFACTION_INITOBJECT); ;}
    break;

  case 272:
#line 1488 "./swf5compiler.y"
    { if((yyvsp[(2) - (2)].lval).obj)
		  {
		    if((yyvsp[(2) - (2)].lval).ident)	// expr . identifier
		    {
		      (yyval.action) = (yyvsp[(2) - (2)].lval).obj;
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(2) - (2)].lval).ident);        /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a */
		      bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).ident);             /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER);
		      bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a.i = a.i+1 */
		      bufferWriteRegister((yyval.action), 0);	      /* a.i+1 */
		    }
		    else	// expr [ expr ]
		    {
		      (yyval.action) = (yyvsp[(2) - (2)].lval).memexpr;			      /* i */
		      bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).obj);		      /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);	/* ($2.memexpr can use reg0) */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);	      /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));		      /* a, i, a[i]+1 */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a[i] = a[i]+1 */
		      bufferWriteRegister((yyval.action), 0);	      /* a[i]+1 */
		    }
		  }
		  else	// identifier
		  {
		    (yyval.action) = newBuffer();
		    bufferWriteBuffer((yyval.action), (yyvsp[(2) - (2)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));
		    bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);
		    bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 273:
#line 1533 "./swf5compiler.y"
    { if((yyvsp[(1) - (2)].lval).obj)
		  {
		    if((yyvsp[(1) - (2)].lval).ident)
		    {
		      (yyval.action) = (yyvsp[(1) - (2)].lval).obj;	                      /* a */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].lval).ident);        /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, a.i */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a.i, a */
		      bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).ident);             /* a.i, a, i */
		      bufferWriteRegister((yyval.action), 0);             /* a.i, a, i, a.i */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));		      /* a.i, a, i, a.i+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER);
		    }
		    else
		    {
		      (yyval.action) = (yyvsp[(1) - (2)].lval).memexpr;
		      bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).obj);               /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);             /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));		      /* a, i, a[i]+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER);
		      bufferWriteRegister((yyval.action), 0);             /* a[i] */
		    }
		  }
		  else
		  {
		    (yyval.action) = newBuffer();
		    bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);
		    bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));
		    bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 274:
#line 1578 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); ;}
    break;

  case 275:
#line 1581 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), -(yyvsp[(2) - (2)].intVal)); ;}
    break;

  case 276:
#line 1585 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), (yyvsp[(1) - (1)].intVal)); ;}
    break;

  case 277:
#line 1589 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteDouble((yyval.action), -(yyvsp[(2) - (2)].doubleVal)); ;}
    break;

  case 278:
#line 1593 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteDouble((yyval.action), (yyvsp[(1) - (1)].doubleVal)); ;}
    break;

  case 279:
#line 1597 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteBoolean((yyval.action), (yyvsp[(1) - (1)].intVal)); ;}
    break;

  case 280:
#line 1601 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteNull((yyval.action)); ;}
    break;

  case 281:
#line 1605 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteUndef((yyval.action)); ;}
    break;

  case 282:
#line 1609 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str)); ;}
    break;

  case 283:
#line 1613 "./swf5compiler.y"
    { if((yyvsp[(1) - (3)].lval).obj)
		  {
		    if((yyvsp[(1) - (3)].lval).ident)
		    {
		      (yyval.action) = (yyvsp[(1) - (3)].lval).obj;			      /* a */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(1) - (3)].lval).ident);	      /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, a.i */
		      bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));		      /* a, a.i, v */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (3)].op));		      /* a, a.i+v */
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).ident);	      /* a, a.i+v, i */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a.i+v */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a.i = a.i+v */
		      bufferWriteRegister((yyval.action), 0);
		    }
		    else
		    {
		      (yyval.action) = (yyvsp[(1) - (3)].lval).memexpr;			      /* i */
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).obj);		      /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);	      /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));		      /* a, i, a[i], v */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (3)].op));		      /* a, i, a[i]+v */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a[i] = a[i]+v */
		      bufferWriteRegister((yyval.action), 0);
		    }
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (3)].lval).ident;
		    bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		    bufferWriteOp((yyval.action), (yyvsp[(2) - (3)].op));
		    bufferWriteSetRegister((yyval.action), 0);
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		    bufferWriteRegister((yyval.action), 0);
		  }
		;}
    break;

  case 285:
#line 1664 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action)); ;}
    break;

  case 286:
#line 1670 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (3)].str), strlen((yyvsp[(1) - (3)].str))+1);
		  free((yyvsp[(1) - (3)].str));
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_DEFINELOCAL); ;}
    break;

  case 287:
#line 1677 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str));
		  bufferWriteOp((yyval.action), SWFACTION_DEFINELOCAL2); ;}
    break;

  case 288:
#line 1685 "./swf5compiler.y"
    { asmBuffer = newBuffer(); ;}
    break;

  case 289:
#line 1687 "./swf5compiler.y"
    { (yyval.action) = asmBuffer; ;}
    break;

  case 290:
#line 1690 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action); ;}
    break;

  case 292:
#line 1695 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action);
		  bufferWriteOp((yyval.action), SWFACTION_POP); ;}
    break;

  case 293:
#line 1699 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action);
		  bufferWriteOp((yyval.action), SWFACTION_POP); ;}
    break;

  case 294:
#line 1703 "./swf5compiler.y"
    { if((yyvsp[(2) - (2)].lval).obj)
		  {
		    if((yyvsp[(2) - (2)].lval).ident)
		    {
		      (yyval.action) = (yyvsp[(2) - (2)].lval).obj;		              /* a */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(2) - (2)].lval).ident);	      /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, a.i */
		      bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));		      /* a, a.i+1 */
		      bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).ident);	      /* a, a.i+1, i */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a.i+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a.i = a.i+1 */
		    }
		    else
		    {
		      /* weird contortions so that $2.memexpr can use reg 0 */
		      (yyval.action) = (yyvsp[(2) - (2)].lval).memexpr;			      /* i */
		      bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).obj);		      /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);	      /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));		      /* a, i, a[i]+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a[i] = a[i]+1 */
		    }
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(2) - (2)].lval).ident;
		    bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 295:
#line 1742 "./swf5compiler.y"
    { if((yyvsp[(1) - (2)].lval).obj)
		  {
		    if((yyvsp[(1) - (2)].lval).ident)
		    {
		      (yyval.action) = (yyvsp[(1) - (2)].lval).obj;			      /* a */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);       /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].lval).ident);	      /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, a.i */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));                  /* a, a.i+1 */
		      bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).ident);             /* a, a.i+1, i */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a.i+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a.i = a.i+1 */
		    }
		    else
		    {
		      /* weird contortions so that $1.memexpr can use reg 0 */
		      (yyval.action) = (yyvsp[(1) - (2)].lval).memexpr;	/* i */
		      bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).obj);		      /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);       /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);             /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));                  /* a, i, a[i]+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a[i] = a[i]+1 */
		    }
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (2)].lval).ident;
		    bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 296:
#line 1781 "./swf5compiler.y"
    { if((yyvsp[(1) - (3)].lval).obj)
		  {
		    (yyval.action) = (yyvsp[(1) - (3)].lval).obj;

		    if((yyvsp[(1) - (3)].lval).ident)
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).ident);
		    else
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).memexpr);

		    bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		    bufferWriteOp((yyval.action), SWFACTION_SETMEMBER);
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (3)].lval).ident;
		    bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 297:
#line 1802 "./swf5compiler.y"
    { if((yyvsp[(1) - (3)].lval).obj)
		  {
		    if((yyvsp[(1) - (3)].lval).ident)
		    {
		      (yyval.action) = (yyvsp[(1) - (3)].lval).obj;			      /* a */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(1) - (3)].lval).ident);	      /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, a.i */
		      bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));		      /* a, a.i, v */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (3)].op));		      /* a, a.i+v */
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).ident);	      /* a, a.i+v, i */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a.i+v */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a.i = a.i+v */
		    }
		    else
		    {
		      (yyval.action) = (yyvsp[(1) - (3)].lval).memexpr;			      /* i */
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).obj);		      /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);	      /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);	      /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_STACKSWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));		      /* a, i, a[i], v */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (3)].op));		      /* a, i, a[i]+v */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a[i] = a[i]+v */
		    }
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (3)].lval).ident;
		    bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		    bufferWriteOp((yyval.action), (yyvsp[(2) - (3)].op));
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 299:
#line 1847 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(1) - (2)].len) + (yyvsp[(2) - (2)].len); ;}
    break;

  case 300:
#line 1852 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_WITH); ;}
    break;

  case 301:
#line 1854 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(2) - (4)].len) + (yyvsp[(3) - (4)].len);
				  bufferPatchLength(asmBuffer, (yyvsp[(3) - (4)].len)); ;}
    break;

  case 302:
#line 1859 "./swf5compiler.y"
    { (yyval.len) = bufferWriteConstantString(asmBuffer, (yyvsp[(1) - (1)].str),
								 strlen((yyvsp[(1) - (1)].str))+1); ;}
    break;

  case 303:
#line 1862 "./swf5compiler.y"
    { (yyval.len) = bufferWriteInt(asmBuffer, (yyvsp[(1) - (1)].intVal)); ;}
    break;

  case 304:
#line 1864 "./swf5compiler.y"
    { (yyval.len) = bufferWriteDouble(asmBuffer, (yyvsp[(1) - (1)].doubleVal)); ;}
    break;

  case 305:
#line 1866 "./swf5compiler.y"
    { bufferWriteU8(asmBuffer, PUSH_BOOLEAN);
				  (yyval.len) = bufferWriteU8(asmBuffer, (yyvsp[(1) - (1)].intVal))+1;
				  bufferPatchPushLength(asmBuffer, 2); ;}
    break;

  case 306:
#line 1870 "./swf5compiler.y"
    { (yyval.len) = bufferWriteU8(asmBuffer, PUSH_NULL);
				  bufferPatchPushLength(asmBuffer, 1); ;}
    break;

  case 307:
#line 1873 "./swf5compiler.y"
    { (yyval.len) = bufferWriteU8(asmBuffer, PUSH_UNDEF);
				  bufferPatchPushLength(asmBuffer, 1); ;}
    break;

  case 308:
#line 1876 "./swf5compiler.y"
    { bufferWriteU8(asmBuffer, PUSH_REGISTER);
				  (yyval.len) = bufferWriteU8(asmBuffer,
						     (char)atoi((yyvsp[(1) - (1)].str)))+1;
				  bufferPatchPushLength(asmBuffer, 2); ;}
    break;

  case 309:
#line 1884 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(1) - (1)].len); ;}
    break;

  case 310:
#line 1885 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(1) - (3)].len) + (yyvsp[(3) - (3)].len); ;}
    break;

  case 311:
#line 1889 "./swf5compiler.y"
    { (yyval.len) = bufferWritePushOp(asmBuffer);
				  (yyval.len) += bufferWriteS16(asmBuffer, 0); ;}
    break;

  case 312:
#line 1891 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(2) - (3)].len) + (yyvsp[(3) - (3)].len);
				  bufferPatchLength(asmBuffer, (yyvsp[(3) - (3)].len)); ;}
    break;

  case 314:
#line 1897 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_STOREREGISTER);
				  (yyval.len) += bufferWriteS16(asmBuffer, 1);
				  (yyval.len) += bufferWriteU8(asmBuffer,
						      (char)atoi((yyvsp[(2) - (2)].str))); ;}
    break;

  case 315:
#line 1903 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_CALLFUNCTION); ;}
    break;

  case 316:
#line 1905 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_RETURN); ;}
    break;

  case 317:
#line 1907 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_CALLMETHOD); ;}
    break;

  case 318:
#line 1909 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_BITWISEAND); ;}
    break;

  case 319:
#line 1911 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_BITWISEOR); ;}
    break;

  case 320:
#line 1913 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_BITWISEXOR); ;}
    break;

  case 321:
#line 1915 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_MODULO); ;}
    break;

  case 322:
#line 1917 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_ADD2); ;}
    break;

  case 323:
#line 1919 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_LESS2); ;}
    break;

  case 324:
#line 1921 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_EQUALS2); ;}
    break;

  case 325:
#line 1923 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INCREMENT); ;}
    break;

  case 326:
#line 1925 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_DECREMENT); ;}
    break;

  case 327:
#line 1927 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_TYPEOF); ;}
    break;

  case 328:
#line 1929 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INSTANCEOF); ;}
    break;

  case 329:
#line 1931 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_ENUMERATE); ;}
    break;

  case 330:
#line 1933 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_DELETE); ;}
    break;

  case 331:
#line 1935 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_NEWOBJECT); ;}
    break;

  case 332:
#line 1937 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INITARRAY); ;}
    break;

  case 333:
#line 1939 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INITOBJECT); ;}
    break;

  case 334:
#line 1941 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_GETMEMBER); ;}
    break;

  case 335:
#line 1943 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SETMEMBER); ;}
    break;

  case 336:
#line 1945 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SHIFTLEFT); ;}
    break;

  case 337:
#line 1947 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SHIFTRIGHT); ;}
    break;

  case 338:
#line 1949 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SHIFTRIGHT2); ;}
    break;

  case 339:
#line 1951 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_DEFINELOCAL2); ;}
    break;

  case 340:
#line 1953 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_EXTENDS); ;}
    break;

  case 341:
#line 1957 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_ADD); ;}
    break;

  case 342:
#line 1958 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SUBTRACT); ;}
    break;

  case 343:
#line 1959 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MULTIPLY); ;}
    break;

  case 344:
#line 1960 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_DIVIDE); ;}
    break;

  case 345:
#line 1961 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_EQUAL); ;}
    break;

  case 346:
#line 1962 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LESSTHAN); ;}
    break;

  case 347:
#line 1963 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALAND); ;}
    break;

  case 348:
#line 1964 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALOR); ;}
    break;

  case 349:
#line 1965 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALNOT); ;}
    break;

  case 350:
#line 1966 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGEQ); ;}
    break;

  case 351:
#line 1967 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGLENGTH); ;}
    break;

  case 352:
#line 1968 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SUBSTRING); ;}
    break;

  case 353:
#line 1969 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_INT); ;}
    break;

  case 354:
#line 1970 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_PUSHDUP); ;}
    break;

  case 355:
#line 1971 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STACKSWAP); ;}
    break;

  case 356:
#line 1972 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_POP); ;}
    break;

  case 357:
#line 1973 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETVARIABLE); ;}
    break;

  case 358:
#line 1974 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETVARIABLE); ;}
    break;

  case 359:
#line 1975 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETPROPERTY); ;}
    break;

  case 360:
#line 1976 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETPROPERTY); ;}
    break;

  case 361:
#line 1977 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_TONUMBER); ;}
    break;

  case 362:
#line 1978 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_TOSTRING); ;}
    break;

  case 363:
#line 1979 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETTARGET2); ;}
    break;

  case 364:
#line 1980 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGCONCAT); ;}
    break;

  case 365:
#line 1981 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_DUPLICATECLIP); ;}
    break;

  case 366:
#line 1982 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_REMOVECLIP); ;}
    break;

  case 367:
#line 1983 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_TRACE); ;}
    break;

  case 368:
#line 1984 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGCOMPARE); ;}
    break;

  case 369:
#line 1985 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_RANDOMNUMBER); ;}
    break;

  case 370:
#line 1986 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBLENGTH); ;}
    break;

  case 371:
#line 1987 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_ORD); ;}
    break;

  case 372:
#line 1988 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_CHR); ;}
    break;

  case 373:
#line 1989 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETTIME); ;}
    break;

  case 374:
#line 1990 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBSUBSTRING); ;}
    break;

  case 375:
#line 1991 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBORD); ;}
    break;

  case 376:
#line 1992 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBCHR); ;}
    break;

  case 377:
#line 1995 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_JUMP);
				  (yyval.len) += bufferWriteS16(asmBuffer, 2);
				  (yyval.len) += bufferBranchTarget(asmBuffer, (yyvsp[(2) - (2)].str)); ;}
    break;

  case 378:
#line 1999 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_IF);
				  (yyval.len) += bufferWriteS16(asmBuffer, 2);
				  (yyval.len) += bufferBranchTarget(asmBuffer, (yyvsp[(2) - (2)].str)); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 6168 "swf5compiler.tab.c"
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


#line 2005 "./swf5compiler.y"



