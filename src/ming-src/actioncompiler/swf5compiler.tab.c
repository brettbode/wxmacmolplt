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
     DELETE2 = 273,
     TARGETPATH = 274,
     RETURN = 275,
     END = 276,
     WITH = 277,
     ASM = 278,
     EVAL = 279,
     RANDOM = 280,
     GETTIMER = 281,
     LENGTH = 282,
     CONCAT = 283,
     SUBSTR = 284,
     TRACE = 285,
     INT = 286,
     ORD = 287,
     CHR = 288,
     GETURL = 289,
     GETURL1 = 290,
     NEXTFRAME = 291,
     PREVFRAME = 292,
     PLAY = 293,
     STOP = 294,
     TOGGLEQUALITY = 295,
     STOPSOUNDS = 296,
     DUP = 297,
     SWAP = 298,
     POP = 299,
     PUSH = 300,
     SETREGISTER = 301,
     CALLFUNCTION = 302,
     CALLMETHOD = 303,
     NEWOBJECT = 304,
     NEWMETHOD = 305,
     AND = 306,
     OR = 307,
     XOR = 308,
     MODULO = 309,
     ADD = 310,
     SWFLESSTHAN = 311,
     EQUALS = 312,
     INC = 313,
     DEC = 314,
     TYPEOF = 315,
     INSTANCEOF = 316,
     ENUMERATE = 317,
     ENUMERATE2 = 318,
     INITOBJECT = 319,
     INITARRAY = 320,
     GETMEMBER = 321,
     SETMEMBER = 322,
     SHIFTLEFT = 323,
     SHIFTRIGHT = 324,
     SHIFTRIGHT2 = 325,
     VAREQUALS = 326,
     OLDADD = 327,
     SUBTRACT = 328,
     MULTIPLY = 329,
     DIVIDE = 330,
     OLDEQUALS = 331,
     OLDLESSTHAN = 332,
     LOGICALAND = 333,
     LOGICALOR = 334,
     NOT = 335,
     STRINGEQ = 336,
     STRINGLENGTH = 337,
     SUBSTRING = 338,
     GETVARIABLE = 339,
     SETVARIABLE = 340,
     SETTARGETEXPRESSION = 341,
     DUPLICATEMOVIECLIP = 342,
     REMOVEMOVIECLIP = 343,
     STRINGLESSTHAN = 344,
     MBLENGTH = 345,
     MBSUBSTRING = 346,
     MBORD = 347,
     MBCHR = 348,
     BRANCHALWAYS = 349,
     BRANCHIFTRUE = 350,
     GETURL2 = 351,
     POST = 352,
     GET = 353,
     CAST = 354,
     LOADVARIABLES = 355,
     LOADMOVIE = 356,
     LOADVARIABLESNUM = 357,
     LOADMOVIENUM = 358,
     CALLFRAME = 359,
     STARTDRAG = 360,
     STOPDRAG = 361,
     GOTOANDSTOP = 362,
     GOTOANDPLAY = 363,
     SETTARGET = 364,
     GETPROPERTY = 365,
     SETPROPERTY = 366,
     TONUMBER = 367,
     TOSTRING = 368,
     TRY = 369,
     THROW = 370,
     CATCH = 371,
     FINALLY = 372,
     THIS = 373,
     EXTENDS = 374,
     IMPLEMENTS = 375,
     FSCOMMAND2 = 376,
     CLASS = 377,
     PUBLIC = 378,
     PRIVATE = 379,
     NULLVAL = 380,
     UNDEFINED = 381,
     INTEGER = 382,
     DOUBLE = 383,
     BOOLEAN = 384,
     REGISTER = 385,
     BROKENSTRING = 386,
     STRING = 387,
     IDENTIFIER = 388,
     EQ = 389,
     EEQ = 390,
     NEE = 391,
     LE = 392,
     GE = 393,
     NE = 394,
     LAN = 395,
     LOR = 396,
     INCR = 397,
     DECR = 398,
     IEQ = 399,
     DEQ = 400,
     MEQ = 401,
     SEQ = 402,
     REQ = 403,
     AEQ = 404,
     OEQ = 405,
     SHL = 406,
     SHR = 407,
     SHR2 = 408,
     SHLEQ = 409,
     SHREQ = 410,
     SHR2EQ = 411,
     _P_X = 412,
     _P_Y = 413,
     _P_XSCALE = 414,
     _P_YSCALE = 415,
     _P_CURRENTFRAME = 416,
     _P_TOTALFRAMES = 417,
     _P_ALPHA = 418,
     _P_VISIBLE = 419,
     _P_WIDTH = 420,
     _P_HEIGHT = 421,
     _P_ROTATION = 422,
     _P_TARGET = 423,
     _P_FRAMESLOADED = 424,
     _P_NAME = 425,
     _P_DROPTARGET = 426,
     _P_URL = 427,
     _P_HIGHQUALITY = 428,
     _P_FOCUSRECT = 429,
     _P_SOUNDBUFTIME = 430,
     _P_QUALITY = 431,
     _P_XMOUSE = 432,
     _P_YMOUSE = 433,
     NOELSE = 434,
     UMINUS = 436,
     POSTFIX = 437
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
#define DELETE2 273
#define TARGETPATH 274
#define RETURN 275
#define END 276
#define WITH 277
#define ASM 278
#define EVAL 279
#define RANDOM 280
#define GETTIMER 281
#define LENGTH 282
#define CONCAT 283
#define SUBSTR 284
#define TRACE 285
#define INT 286
#define ORD 287
#define CHR 288
#define GETURL 289
#define GETURL1 290
#define NEXTFRAME 291
#define PREVFRAME 292
#define PLAY 293
#define STOP 294
#define TOGGLEQUALITY 295
#define STOPSOUNDS 296
#define DUP 297
#define SWAP 298
#define POP 299
#define PUSH 300
#define SETREGISTER 301
#define CALLFUNCTION 302
#define CALLMETHOD 303
#define NEWOBJECT 304
#define NEWMETHOD 305
#define AND 306
#define OR 307
#define XOR 308
#define MODULO 309
#define ADD 310
#define SWFLESSTHAN 311
#define EQUALS 312
#define INC 313
#define DEC 314
#define TYPEOF 315
#define INSTANCEOF 316
#define ENUMERATE 317
#define ENUMERATE2 318
#define INITOBJECT 319
#define INITARRAY 320
#define GETMEMBER 321
#define SETMEMBER 322
#define SHIFTLEFT 323
#define SHIFTRIGHT 324
#define SHIFTRIGHT2 325
#define VAREQUALS 326
#define OLDADD 327
#define SUBTRACT 328
#define MULTIPLY 329
#define DIVIDE 330
#define OLDEQUALS 331
#define OLDLESSTHAN 332
#define LOGICALAND 333
#define LOGICALOR 334
#define NOT 335
#define STRINGEQ 336
#define STRINGLENGTH 337
#define SUBSTRING 338
#define GETVARIABLE 339
#define SETVARIABLE 340
#define SETTARGETEXPRESSION 341
#define DUPLICATEMOVIECLIP 342
#define REMOVEMOVIECLIP 343
#define STRINGLESSTHAN 344
#define MBLENGTH 345
#define MBSUBSTRING 346
#define MBORD 347
#define MBCHR 348
#define BRANCHALWAYS 349
#define BRANCHIFTRUE 350
#define GETURL2 351
#define POST 352
#define GET 353
#define CAST 354
#define LOADVARIABLES 355
#define LOADMOVIE 356
#define LOADVARIABLESNUM 357
#define LOADMOVIENUM 358
#define CALLFRAME 359
#define STARTDRAG 360
#define STOPDRAG 361
#define GOTOANDSTOP 362
#define GOTOANDPLAY 363
#define SETTARGET 364
#define GETPROPERTY 365
#define SETPROPERTY 366
#define TONUMBER 367
#define TOSTRING 368
#define TRY 369
#define THROW 370
#define CATCH 371
#define FINALLY 372
#define THIS 373
#define EXTENDS 374
#define IMPLEMENTS 375
#define FSCOMMAND2 376
#define CLASS 377
#define PUBLIC 378
#define PRIVATE 379
#define NULLVAL 380
#define UNDEFINED 381
#define INTEGER 382
#define DOUBLE 383
#define BOOLEAN 384
#define REGISTER 385
#define BROKENSTRING 386
#define STRING 387
#define IDENTIFIER 388
#define EQ 389
#define EEQ 390
#define NEE 391
#define LE 392
#define GE 393
#define NE 394
#define LAN 395
#define LOR 396
#define INCR 397
#define DECR 398
#define IEQ 399
#define DEQ 400
#define MEQ 401
#define SEQ 402
#define REQ 403
#define AEQ 404
#define OEQ 405
#define SHL 406
#define SHR 407
#define SHR2 408
#define SHLEQ 409
#define SHREQ 410
#define SHR2EQ 411
#define _P_X 412
#define _P_Y 413
#define _P_XSCALE 414
#define _P_YSCALE 415
#define _P_CURRENTFRAME 416
#define _P_TOTALFRAMES 417
#define _P_ALPHA 418
#define _P_VISIBLE 419
#define _P_WIDTH 420
#define _P_HEIGHT 421
#define _P_ROTATION 422
#define _P_TARGET 423
#define _P_FRAMESLOADED 424
#define _P_NAME 425
#define _P_DROPTARGET 426
#define _P_URL 427
#define _P_HIGHQUALITY 428
#define _P_FOCUSRECT 429
#define _P_SOUNDBUFTIME 430
#define _P_QUALITY 431
#define _P_XMOUSE 432
#define _P_YMOUSE 433
#define NOELSE 434
#define UMINUS 436
#define POSTFIX 437




/* Copy the first part of user declarations.  */
#line 5 "./swf5compiler.y"


#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "compile.h"
#include "actiontypes.h"
#include "assembler.h"

#define YYERROR_VERBOSE 1
#define YYPARSE_PARAM buffer
//#define DEBUG 1

#ifdef _MSC_VER 
#define strcasecmp stricmp 
#endif


Buffer bf, bc;
static int classContext = 0;



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
#line 29 "./swf5compiler.y"
{
  Buffer action;
  char *str;
  SWFGetUrl2Method getURLMethod;
  int op;
  int intVal;
  int len;
  double doubleVal;
  ASFunction 		function;
  ASClass		clazz;
  ASClassMember		classMember;
  struct exprlist_s 	exprlist;
  struct switchcase	switchcase;
  struct switchcases	switchcases;
  struct
  {
	Buffer obj, ident, memexpr;
  } lval;
}
/* Line 187 of yacc.c.  */
#line 509 "swf5compiler.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 522 "swf5compiler.tab.c"

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
#define YYLAST   6590

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  207
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  72
/* YYNRULES -- Number of rules.  */
#define YYNRULES  453
/* YYNRULES -- Number of states.  */
#define YYNSTATES  760

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   437

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   195,     2,     2,     2,   194,   187,     2,
     205,   206,   192,   190,   180,   191,   199,   193,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   184,   204,
     185,   181,   186,   183,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   200,     2,   201,   189,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   202,   188,   203,   196,     2,     2,     2,
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
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   182,   197,   198
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     8,    10,    13,    15,    17,
      19,    21,    24,    27,    29,    33,    35,    38,    40,    42,
      44,    46,    48,    50,    52,    55,    57,    59,    62,    65,
      70,    72,    74,    78,    82,    83,    85,    87,    88,    91,
      93,    97,   102,   105,   109,   112,   120,   125,   135,   143,
     146,   150,   152,   156,   164,   170,   171,   173,   175,   183,
     184,   187,   192,   196,   198,   200,   202,   204,   206,   208,
     210,   212,   214,   216,   218,   220,   222,   224,   226,   228,
     230,   232,   234,   236,   238,   240,   242,   244,   246,   248,
     250,   252,   254,   256,   258,   260,   262,   264,   266,   268,
     270,   272,   274,   276,   278,   280,   282,   284,   286,   288,
     290,   292,   294,   296,   298,   300,   302,   304,   306,   308,
     310,   312,   314,   316,   318,   320,   322,   324,   326,   328,
     330,   332,   334,   336,   338,   340,   342,   344,   346,   348,
     350,   352,   354,   356,   358,   360,   362,   364,   366,   368,
     370,   372,   374,   376,   378,   380,   382,   384,   386,   388,
     390,   392,   394,   396,   398,   400,   402,   404,   406,   408,
     410,   412,   414,   415,   418,   423,   424,   426,   434,   437,
     439,   441,   443,   445,   447,   449,   451,   455,   457,   459,
     460,   461,   467,   474,   485,   493,   502,   503,   505,   508,
     511,   512,   515,   518,   521,   523,   528,   533,   541,   549,
     557,   565,   573,   578,   585,   600,   604,   613,   618,   625,
     629,   633,   637,   641,   645,   649,   654,   659,   664,   669,
     674,   679,   684,   689,   698,   703,   708,   713,   717,   722,
     727,   732,   737,   742,   749,   758,   763,   770,   773,   774,
     776,   780,   787,   795,   799,   801,   805,   807,   809,   811,
     813,   815,   817,   819,   821,   823,   825,   827,   829,   831,
     833,   835,   837,   839,   843,   848,   850,   852,   855,   858,
     861,   865,   869,   873,   877,   881,   885,   889,   893,   897,
     901,   905,   909,   913,   917,   921,   925,   929,   933,   937,
     941,   945,   951,   955,   959,   961,   964,   970,   975,   981,
     989,   998,  1002,  1004,  1008,  1010,  1012,  1014,  1017,  1020,
    1024,  1028,  1030,  1032,  1034,  1036,  1038,  1040,  1042,  1046,
    1051,  1054,  1055,  1061,  1064,  1066,  1068,  1070,  1072,  1075,
    1078,  1082,  1086,  1088,  1091,  1092,  1097,  1099,  1101,  1103,
    1105,  1107,  1109,  1111,  1113,  1117,  1118,  1122,  1124,  1127,
    1129,  1131,  1133,  1135,  1137,  1139,  1141,  1143,  1145,  1147,
    1149,  1151,  1153,  1155,  1157,  1159,  1161,  1163,  1165,  1167,
    1169,  1171,  1173,  1175,  1177,  1179,  1181,  1183,  1185,  1187,
    1189,  1191,  1193,  1195,  1197,  1199,  1201,  1203,  1205,  1207,
    1209,  1211,  1213,  1215,  1217,  1219,  1221,  1223,  1225,  1227,
    1229,  1231,  1233,  1235,  1237,  1239,  1241,  1243,  1245,  1247,
    1249,  1251,  1253,  1255,  1257,  1259,  1261,  1263,  1265,  1267,
    1270,  1273,  1275,  1277,  1279,  1281,  1283,  1285,  1287,  1289,
    1291,  1293,  1295,  1297,  1299,  1301,  1303,  1305,  1307,  1309,
    1311,  1313,  1315,  1317
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     208,     0,    -1,    -1,   209,   210,    -1,    -1,   211,    -1,
     210,   211,    -1,   214,    -1,   238,    -1,   219,    -1,   214,
      -1,   212,   214,    -1,   202,   203,    -1,   213,    -1,   202,
     212,   203,    -1,   204,    -1,   269,   204,    -1,   229,    -1,
     246,    -1,   248,    -1,   249,    -1,   232,    -1,   227,    -1,
     226,    -1,   225,   204,    -1,   224,    -1,   216,    -1,   215,
     216,    -1,   220,   238,    -1,   220,    15,   222,   204,    -1,
     122,    -1,   213,    -1,   202,   215,   203,    -1,   217,   235,
     218,    -1,    -1,   123,    -1,   124,    -1,    -1,   184,   235,
      -1,   223,    -1,   222,   180,   223,    -1,   235,   221,   181,
     266,    -1,   235,   221,    -1,   115,   264,   204,    -1,   114,
     214,    -1,   114,   214,   116,   205,   235,   206,   214,    -1,
     114,   214,   117,   214,    -1,   114,   214,   116,   205,   235,
     206,   214,   117,   214,    -1,    22,   205,   263,   206,   202,
     212,   203,    -1,    20,   204,    -1,    20,   264,   204,    -1,
     269,    -1,   228,   180,   269,    -1,    12,   205,   263,   206,
     214,     6,   214,    -1,    12,   205,   263,   206,   214,    -1,
      -1,   263,    -1,     7,    -1,   231,   205,   263,   206,   202,
     233,   203,    -1,    -1,   233,   234,    -1,     8,   263,   184,
     212,    -1,     9,   184,   212,    -1,   133,    -1,    19,    -1,
      25,    -1,    26,    -1,    27,    -1,    28,    -1,    29,    -1,
      30,    -1,    31,    -1,    32,    -1,    33,    -1,    34,    -1,
      35,    -1,    36,    -1,    37,    -1,    38,    -1,    39,    -1,
      40,    -1,    41,    -1,    42,    -1,    43,    -1,    44,    -1,
      45,    -1,    46,    -1,    47,    -1,    48,    -1,    51,    -1,
      52,    -1,    53,    -1,    54,    -1,    55,    -1,    56,    -1,
      57,    -1,    58,    -1,    59,    -1,    60,    -1,    63,    -1,
      62,    -1,    64,    -1,    65,    -1,    66,    -1,    67,    -1,
      68,    -1,    69,    -1,    70,    -1,    71,    -1,    72,    -1,
      73,    -1,    74,    -1,    75,    -1,    76,    -1,    77,    -1,
      78,    -1,    79,    -1,    80,    -1,    81,    -1,    82,    -1,
      83,    -1,    84,    -1,    85,    -1,    86,    -1,    87,    -1,
      88,    -1,   105,    -1,   106,    -1,    89,    -1,    90,    -1,
      91,    -1,    92,    -1,    93,    -1,    94,    -1,    95,    -1,
      96,    -1,    97,    -1,    98,    -1,   100,    -1,   102,    -1,
     101,    -1,   103,    -1,   107,    -1,   108,    -1,   109,    -1,
     104,    -1,   110,    -1,   111,    -1,    99,    -1,   118,    -1,
     157,    -1,   158,    -1,   159,    -1,   160,    -1,   161,    -1,
     162,    -1,   163,    -1,   164,    -1,   165,    -1,   166,    -1,
     167,    -1,   168,    -1,   169,    -1,   170,    -1,   171,    -1,
     172,    -1,   173,    -1,   174,    -1,   175,    -1,   176,    -1,
     177,    -1,   178,    -1,    -1,   235,   221,    -1,   236,   180,
     235,   221,    -1,    -1,   235,    -1,     5,   237,   205,   236,
     206,   221,   214,    -1,    11,   240,    -1,   262,    -1,   253,
      -1,   256,    -1,   262,    -1,   253,    -1,   252,    -1,   256,
      -1,   205,   241,   206,    -1,    13,    -1,    14,    -1,    -1,
      -1,   242,   205,   263,   206,   214,    -1,   243,   214,    13,
     205,   263,   206,    -1,    10,   205,   247,   204,   230,   204,
     247,   206,   244,   214,    -1,    10,   205,   235,   239,   206,
     245,   214,    -1,    10,   205,    15,   235,   239,   206,   245,
     214,    -1,    -1,   228,    -1,     4,   204,    -1,     3,   204,
      -1,    -1,   180,    98,    -1,   180,    97,    -1,   180,   132,
      -1,   263,    -1,    30,   205,   264,   206,    -1,    34,   205,
     263,   206,    -1,    34,   205,   263,   180,   263,   250,   206,
      -1,   100,   205,   263,   180,   263,   250,   206,    -1,   102,
     205,   263,   180,   251,   250,   206,    -1,   101,   205,   263,
     180,   263,   250,   206,    -1,   103,   205,   263,   180,   251,
     250,   206,    -1,   104,   205,   263,   206,    -1,   105,   205,
     263,   180,   263,   206,    -1,   105,   205,   263,   180,   263,
     180,   263,   180,   263,   180,   263,   180,   263,   206,    -1,
     106,   205,   206,    -1,    87,   205,   263,   180,   263,   180,
     263,   206,    -1,    88,   205,   263,   206,    -1,    35,   205,
     132,   180,   132,   206,    -1,    36,   205,   206,    -1,    37,
     205,   206,    -1,    38,   205,   206,    -1,    39,   205,   206,
      -1,    41,   205,   206,    -1,    40,   205,   206,    -1,   108,
     205,   127,   206,    -1,   107,   205,   127,   206,    -1,   108,
     205,   132,   206,    -1,   107,   205,   132,   206,    -1,   108,
     205,   263,   206,    -1,   107,   205,   263,   206,    -1,   109,
     205,   132,   206,    -1,   109,   205,   263,   206,    -1,   111,
     205,   263,   180,   278,   180,   263,   206,    -1,   133,   205,
     255,   206,    -1,    19,   205,   133,   206,    -1,    24,   205,
     263,   206,    -1,    26,   205,   206,    -1,    25,   205,   263,
     206,    -1,    27,   205,   264,   206,    -1,    31,   205,   263,
     206,    -1,    32,   205,   263,   206,    -1,    33,   205,   263,
     206,    -1,    28,   205,   263,   180,   263,   206,    -1,    29,
     205,   263,   180,   263,   180,   263,   206,    -1,    60,   205,
     264,   206,    -1,   110,   205,   263,   180,   278,   206,    -1,
      17,   241,    -1,    -1,   264,    -1,   255,   180,   264,    -1,
     261,   199,   235,   205,   255,   206,    -1,   261,   200,   263,
     201,   205,   255,   206,    -1,   235,   184,   264,    -1,   257,
      -1,   258,   180,   257,    -1,   144,    -1,   147,    -1,   146,
      -1,   145,    -1,   148,    -1,   149,    -1,   150,    -1,   182,
      -1,   154,    -1,   155,    -1,   156,    -1,   142,    -1,   143,
      -1,   262,    -1,   253,    -1,   256,    -1,   235,    -1,   261,
     199,   235,    -1,   261,   200,   263,   201,    -1,   265,    -1,
     266,    -1,   191,   263,    -1,   196,   263,    -1,   195,   263,
      -1,   263,   140,   263,    -1,   263,   141,   263,    -1,   263,
     192,   263,    -1,   263,   193,   263,    -1,   263,   194,   263,
      -1,   263,   190,   263,    -1,   263,   191,   263,    -1,   263,
     187,   263,    -1,   263,   188,   263,    -1,   263,   189,   263,
      -1,   263,   185,   263,    -1,   263,   186,   263,    -1,   263,
     137,   263,    -1,   263,   138,   263,    -1,   263,   134,   263,
      -1,   263,   135,   263,    -1,   263,   139,   263,    -1,   263,
     136,   263,    -1,   263,   151,   263,    -1,   263,   152,   263,
      -1,   263,   153,   263,    -1,   263,   183,   263,   184,   263,
      -1,   262,   181,   264,    -1,   263,    61,   261,    -1,   263,
      -1,    16,   235,    -1,    16,   133,   205,   255,   206,    -1,
      16,   261,   199,   133,    -1,    16,   261,   200,   263,   201,
      -1,    16,   261,   199,   133,   205,   255,   206,    -1,    16,
     261,   200,   263,   201,   205,   255,   206,    -1,   200,   255,
     201,    -1,   213,    -1,   202,   258,   203,    -1,   238,    -1,
     261,    -1,   254,    -1,   260,   262,    -1,   262,   260,    -1,
     205,   263,   206,    -1,   262,   259,   263,    -1,   129,    -1,
     125,    -1,   126,    -1,   132,    -1,   127,    -1,   128,    -1,
     268,    -1,   267,   180,   268,    -1,   235,   221,   181,   264,
      -1,   235,   221,    -1,    -1,    23,   202,   270,   271,   203,
      -1,    15,   267,    -1,   252,    -1,   254,    -1,   253,    -1,
     256,    -1,   260,   262,    -1,   262,   260,    -1,   262,   181,
     264,    -1,   262,   259,   263,    -1,   276,    -1,   271,   276,
      -1,    -1,    22,   273,   271,    21,    -1,   132,    -1,   127,
      -1,   128,    -1,   129,    -1,   125,    -1,   126,    -1,   130,
      -1,   274,    -1,   275,   180,   274,    -1,    -1,    45,   277,
     275,    -1,   272,    -1,    46,   130,    -1,    47,    -1,    20,
      -1,    48,    -1,    49,    -1,    50,    -1,    51,    -1,    52,
      -1,    53,    -1,    54,    -1,    55,    -1,    56,    -1,    57,
      -1,    58,    -1,    59,    -1,    60,    -1,    61,    -1,    62,
      -1,    63,    -1,    17,    -1,    18,    -1,    16,    -1,    65,
      -1,    64,    -1,    66,    -1,    67,    -1,    68,    -1,    69,
      -1,    70,    -1,    15,    -1,   119,    -1,    19,    -1,   120,
      -1,   121,    -1,    99,    -1,    72,    -1,    73,    -1,    74,
      -1,    75,    -1,    76,    -1,    77,    -1,    78,    -1,    79,
      -1,    80,    -1,    81,    -1,    82,    -1,    83,    -1,    31,
      -1,    42,    -1,    43,    -1,    44,    -1,    84,    -1,    85,
      -1,   110,    -1,   111,    -1,   112,    -1,   113,    -1,    86,
      -1,    28,    -1,    87,    -1,    88,    -1,    30,    -1,    89,
      -1,    25,    -1,    90,    -1,    32,    -1,    33,    -1,    26,
      -1,    91,    -1,    92,    -1,    93,    -1,    94,   132,    -1,
      95,   132,    -1,   132,    -1,   157,    -1,   158,    -1,   159,
      -1,   160,    -1,   161,    -1,   162,    -1,   163,    -1,   164,
      -1,   165,    -1,   166,    -1,   167,    -1,   168,    -1,   169,
      -1,   170,    -1,   171,    -1,   172,    -1,   173,    -1,   174,
      -1,   175,    -1,   176,    -1,   177,    -1,   178,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   183,   183,   183,   191,   195,   196,   200,   202,   210,
     215,   218,   224,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   245,   246,   254,   255,
     259,   271,   272,   276,   283,   285,   286,   289,   291,   294,
     296,   303,   308,   316,   320,   330,   344,   355,   373,   384,
     397,   412,   413,   417,   428,   438,   439,   443,   448,   462,
     465,   473,   485,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,   542,   543,   544,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   590,
     593,   594,   595,   596,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     613,   614,   619,   622,   628,   636,   637,   641,   652,   657,
     674,   675,   680,   695,   696,   697,   698,   702,   707,   713,
     718,   722,   735,   748,   787,   824,   862,   863,   869,   884,
     909,   911,   913,   915,   924,   933,   937,   949,   960,   967,
     974,   981,   988,   994,  1001,  1012,  1017,  1025,  1029,  1039,
    1043,  1047,  1051,  1055,  1059,  1063,  1070,  1077,  1085,  1093,
    1099,  1105,  1112,  1116,  1127,  1138,  1145,  1149,  1153,  1157,
    1161,  1165,  1169,  1173,  1178,  1184,  1192,  1202,  1214,  1217,
    1222,  1231,  1239,  1248,  1256,  1260,  1266,  1267,  1268,  1269,
    1270,  1271,  1272,  1273,  1274,  1275,  1276,  1280,  1281,  1287,
    1304,  1305,  1310,  1317,  1324,  1333,  1335,  1337,  1342,  1347,
    1351,  1361,  1370,  1375,  1380,  1385,  1390,  1395,  1400,  1405,
    1410,  1415,  1420,  1426,  1431,  1435,  1439,  1444,  1449,  1453,
    1457,  1461,  1471,  1498,  1506,  1508,  1519,  1530,  1542,  1554,
    1566,  1577,  1582,  1587,  1595,  1608,  1610,  1612,  1657,  1702,
    1706,  1755,  1758,  1762,  1766,  1770,  1773,  1779,  1781,  1787,
    1794,  1803,  1802,  1807,  1810,  1812,  1816,  1820,  1824,  1863,
    1902,  1923,  1968,  1969,  1974,  1973,  1981,  1984,  1986,  1988,
    1992,  1995,  1998,  2006,  2007,  2011,  2011,  2016,  2018,  2025,
    2027,  2029,  2031,  2033,  2035,  2037,  2039,  2041,  2043,  2045,
    2047,  2049,  2051,  2053,  2055,  2057,  2059,  2061,  2063,  2065,
    2067,  2069,  2071,  2073,  2075,  2077,  2079,  2081,  2083,  2085,
    2087,  2089,  2091,  2095,  2096,  2097,  2098,  2099,  2100,  2101,
    2102,  2103,  2104,  2105,  2106,  2107,  2108,  2109,  2110,  2111,
    2112,  2113,  2114,  2115,  2116,  2117,  2118,  2119,  2120,  2121,
    2122,  2123,  2124,  2125,  2126,  2127,  2128,  2129,  2130,  2133,
    2137,  2144,  2145,  2146,  2147,  2148,  2149,  2150,  2151,  2152,
    2153,  2154,  2155,  2156,  2157,  2158,  2159,  2160,  2161,  2162,
    2163,  2164,  2165,  2166
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BREAK", "CONTINUE", "FUNCTION", "ELSE",
  "SWITCH", "CASE", "DEFAULT", "FOR", "IN", "IF", "WHILE", "DO", "VAR",
  "NEW", "DELETE", "DELETE2", "TARGETPATH", "RETURN", "END", "WITH", "ASM",
  "EVAL", "RANDOM", "GETTIMER", "LENGTH", "CONCAT", "SUBSTR", "TRACE",
  "INT", "ORD", "CHR", "GETURL", "GETURL1", "NEXTFRAME", "PREVFRAME",
  "PLAY", "STOP", "TOGGLEQUALITY", "STOPSOUNDS", "DUP", "SWAP", "POP",
  "PUSH", "SETREGISTER", "CALLFUNCTION", "CALLMETHOD", "NEWOBJECT",
  "NEWMETHOD", "AND", "OR", "XOR", "MODULO", "ADD", "LESSTHAN", "EQUALS",
  "INC", "DEC", "TYPEOF", "INSTANCEOF", "ENUMERATE", "ENUMERATE2",
  "INITOBJECT", "INITARRAY", "GETMEMBER", "SETMEMBER", "SHIFTLEFT",
  "SHIFTRIGHT", "SHIFTRIGHT2", "VAREQUALS", "OLDADD", "SUBTRACT",
  "MULTIPLY", "DIVIDE", "OLDEQUALS", "OLDLESSTHAN", "LOGICALAND",
  "LOGICALOR", "NOT", "STRINGEQ", "STRINGLENGTH", "SUBSTRING",
  "GETVARIABLE", "SETVARIABLE", "SETTARGETEXPRESSION",
  "DUPLICATEMOVIECLIP", "REMOVEMOVIECLIP", "STRINGLESSTHAN", "MBLENGTH",
  "MBSUBSTRING", "MBORD", "MBCHR", "BRANCHALWAYS", "BRANCHIFTRUE",
  "GETURL2", "POST", "GET", "CAST", "LOADVARIABLES", "LOADMOVIE",
  "LOADVARIABLESNUM", "LOADMOVIENUM", "CALLFRAME", "STARTDRAG", "STOPDRAG",
  "GOTOANDSTOP", "GOTOANDPLAY", "SETTARGET", "GETPROPERTY", "SETPROPERTY",
  "TONUMBER", "TOSTRING", "TRY", "THROW", "CATCH", "FINALLY", "THIS",
  "EXTENDS", "IMPLEMENTS", "FSCOMMAND2", "CLASS", "PUBLIC", "PRIVATE",
  "NULLVAL", "UNDEFINED", "INTEGER", "DOUBLE", "BOOLEAN", "REGISTER",
  "BROKENSTRING", "STRING", "IDENTIFIER", "\"==\"", "\"===\"", "\"!==\"",
  "\"<=\"", "\">=\"", "\"!=\"", "\"&&\"", "\"||\"", "\"++\"", "\"--\"",
  "\"+=\"", "\"/=\"", "\"*=\"", "\"-=\"", "\"%=\"", "\"&=\"", "\"|=\"",
  "\"<<\"", "\">>\"", "\">>>\"", "\"<<=\"", "\">>=\"", "\">>>=\"", "_P_X",
  "_P_Y", "_P_XSCALE", "_P_YSCALE", "_P_CURRENTFRAME", "_P_TOTALFRAMES",
  "_P_ALPHA", "_P_VISIBLE", "_P_WIDTH", "_P_HEIGHT", "_P_ROTATION",
  "_P_TARGET", "_P_FRAMESLOADED", "_P_NAME", "_P_DROPTARGET", "_P_URL",
  "_P_HIGHQUALITY", "_P_FOCUSRECT", "_P_SOUNDBUFTIME", "_P_QUALITY",
  "_P_XMOUSE", "_P_YMOUSE", "NOELSE", "','", "'='", "\"^=\"", "'?'", "':'",
  "'<'", "'>'", "'&'", "'|'", "'^'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "'!'", "'~'", "UMINUS", "POSTFIX", "'.'", "'['", "']'", "'{'", "'}'",
  "';'", "'('", "')'", "$accept", "program", "@1", "code", "anycode",
  "stmts", "emptybraces", "stmt", "class_stmts", "class_stmt",
  "class_init", "class_body", "class_decl", "access_attr", "type_attr",
  "class_vars", "class_var", "throw_stmt", "try_catch_stmt", "with_stmt",
  "return_stmt", "assign_stmts", "if_stmt", "expr_opt", "switch_init",
  "switch_stmt", "switch_cases", "switch_case", "identifier",
  "formals_list", "function_identifier", "function_decl", "inpart",
  "obj_ref", "obj_ref_for_delete_only", "while_init", "do_init",
  "for_init", "for_in_init", "iter_stmt", "assign_stmts_opt", "cont_stmt",
  "break_stmt", "urlmethod", "level", "void_function_call",
  "function_call", "delete_call", "expr_list", "method_call", "objexpr",
  "objexpr_list", "assignop", "incdecop", "lvalue_expr", "lvalue", "expr",
  "expr_or_obj", "primary", "primary_constant", "init_vars", "init_var",
  "assign_stmt", "@2", "opcode_list", "with", "@3", "push_item",
  "push_list", "opcode", "@4", "property", 0
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
      44,    61,   435,    63,    58,    60,    62,    38,   124,    94,
      43,    45,    42,    47,    37,    33,   126,   436,   437,    46,
      91,    93,   123,   125,    59,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   207,   209,   208,   208,   210,   210,   211,   211,   211,
     212,   212,   213,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   215,   215,   216,   216,
     217,   218,   218,   219,   220,   220,   220,   221,   221,   222,
     222,   223,   223,   224,   225,   225,   225,   225,   226,   227,
     227,   228,   228,   229,   229,   230,   230,   231,   232,   233,
     233,   234,   234,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   236,   236,   236,   237,   237,   238,   239,   240,
     240,   240,   241,   241,   241,   241,   241,   242,   243,   244,
     245,   246,   246,   246,   246,   246,   247,   247,   248,   249,
     250,   250,   250,   250,   251,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   254,   255,   255,
     255,   256,   256,   257,   258,   258,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   260,   260,   261,
     261,   261,   262,   262,   262,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   264,   264,   264,   264,   264,   264,
     264,   264,   264,   264,   265,   265,   265,   265,   265,   265,
     265,   266,   266,   266,   266,   266,   266,   267,   267,   268,
     268,   270,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   271,   271,   273,   272,   274,   274,   274,   274,
     274,   274,   274,   275,   275,   277,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     1,     2,     1,     1,     1,
       1,     2,     2,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     2,     2,     4,
       1,     1,     3,     3,     0,     1,     1,     0,     2,     1,
       3,     4,     2,     3,     2,     7,     4,     9,     7,     2,
       3,     1,     3,     7,     5,     0,     1,     1,     7,     0,
       2,     4,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     2,     4,     0,     1,     7,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     0,
       0,     5,     6,    10,     7,     8,     0,     1,     2,     2,
       0,     2,     2,     2,     1,     4,     4,     7,     7,     7,
       7,     7,     4,     6,    14,     3,     8,     4,     6,     3,
       3,     3,     3,     3,     3,     4,     4,     4,     4,     4,
       4,     4,     4,     8,     4,     4,     4,     3,     4,     4,
       4,     4,     4,     6,     8,     4,     6,     2,     0,     1,
       3,     6,     7,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     4,     1,     1,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     3,     3,     1,     2,     5,     4,     5,     7,
       8,     3,     1,     3,     1,     1,     1,     2,     2,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       2,     0,     5,     2,     1,     1,     1,     1,     2,     2,
       3,     3,     1,     2,     0,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     1,     0,     0,   175,    57,     0,     0,
     187,   188,     0,     0,    64,     0,     0,     0,     0,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,   100,    99,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     148,   138,   140,   139,   141,   145,   126,   127,   142,   143,
     144,   146,   147,     0,     0,   149,    30,    63,   267,   268,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,     0,    15,     3,     5,    13,     7,     0,     9,
      25,     0,    23,    22,    17,     0,    21,   272,     8,     0,
       0,    18,    19,    20,   334,   336,   335,   337,     0,     0,
     269,     0,   199,   198,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    98,   124,   125,   138,   140,   139,   141,   145,
     126,   127,   142,   143,   144,   146,   147,    63,   176,     0,
     196,     0,    37,   333,   327,     0,   247,   184,   183,   185,
     182,     0,     0,   322,   323,   325,   326,   321,   324,     0,
       0,     0,   248,     0,    49,     0,   312,   314,   270,   316,
     271,     0,   315,   269,   304,     0,   275,   276,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,     0,   248,    12,     0,    10,     6,
       0,    24,     0,     0,     0,   338,     0,     0,   256,   259,
     258,   257,   260,   261,   262,   264,   265,   266,     0,   263,
       0,   339,    16,   172,     0,   197,   272,     0,    51,     0,
       0,   330,     0,     0,     0,    63,   305,     0,   269,   277,
     279,   278,     0,   249,     0,   254,     0,     0,   317,     0,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     219,   220,   221,   222,   224,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   215,   325,   324,     0,   325,
     324,     0,   324,     0,     0,     0,     0,     0,    43,     0,
      14,    11,    34,    31,    33,     0,     0,     0,   273,     0,
     340,   341,    37,     0,    37,     0,     0,     0,    55,     0,
      38,     0,   328,   186,   235,   248,     0,     0,     0,   311,
       0,     0,   313,   319,   302,   320,   303,   294,   295,   297,
     292,   293,   296,   280,   281,   298,   299,   300,     0,   290,
     291,   287,   288,   289,   285,   286,   282,   283,   284,     0,
     387,   379,   377,   378,   389,   360,   344,   421,   425,   416,
     419,   405,   423,   424,   406,   407,   408,   355,     0,   359,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   381,   380,   382,   383,
     384,   385,   386,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   409,   410,   415,   417,   418,
     420,   422,   426,   427,   428,     0,     0,   392,   411,   412,
     413,   414,   388,   390,   391,     0,   357,   342,   236,   238,
     239,     0,     0,   205,   240,   241,   242,     0,   206,     0,
     245,     0,   217,     0,     0,     0,     0,   212,     0,   226,
     228,   230,   225,   227,   229,   231,   232,     0,     0,     0,
      46,   234,    35,    36,    34,    26,     0,     0,     0,     0,
     248,   274,   173,     0,    37,     0,    52,   178,   270,   271,
     269,   190,     0,    56,    54,   329,     0,   307,     0,   250,
     253,   255,     0,     0,     0,     0,   358,   429,   430,   332,
     343,     0,     0,   200,     0,     0,   200,   200,   200,   204,
     200,     0,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,     0,    32,    27,
       0,    28,    59,   191,     0,     0,   248,    37,     0,   190,
       0,   196,     0,   306,   248,   308,   301,     0,     0,   350,
     351,   347,   348,   349,   352,   346,   353,   356,   243,     0,
       0,     0,   218,     0,     0,     0,     0,     0,     0,   213,
     246,     0,     0,     0,    39,    37,     0,   192,   251,     0,
     174,   177,     0,   194,     0,    53,     0,   248,    48,   345,
       0,     0,   202,   201,   203,   207,     0,   208,   210,   209,
     211,     0,     0,    45,     0,    29,    42,     0,     0,    58,
      60,   252,   195,   189,   309,     0,   354,   244,   216,     0,
     233,     0,    40,     0,     0,     0,     0,   310,     0,    47,
      41,     0,    62,   193,     0,    61,     0,     0,     0,   214
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   134,   135,   277,   136,   278,   574,   575,
     138,   404,   139,   576,   311,   693,   694,   140,   141,   142,
     143,   305,   144,   592,   145,   146,   696,   730,   147,   413,
     199,   227,   417,   587,   206,   149,   150,   746,   660,   151,
     307,   152,   153,   681,   618,   154,   228,   229,   322,   230,
     325,   326,   300,   231,   232,   233,   234,   323,   236,   237,
     203,   204,   161,   357,   535,   536,   604,   676,   677,   537,
     605,   645
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -589
static const yytype_int16 yypact[] =
{
      35,    37,  1419,  -589,  -178,  -163,  4490,  -589,  -119,  -117,
    -589,  -589,  4490,  3484,  -109,  2392,   -94,  -100,   -90,   -81,
     -78,   -69,   -61,   -59,   -55,   -46,   -41,   -38,   -35,   -28,
     -27,   -19,   -10,    -8,     7,     9,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,    10,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,    16,    17,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,    19,    22,    23,    25,    28,    30,    32,    33,    38,
      39,    40,    41,  2211,  2574,  -589,  -589,    42,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  1605,  -589,  1419,  -589,  -589,  -589,  4490,  -589,
    -589,   -24,  -589,  -589,  -589,    45,  -589,  -589,  -589,    48,
    2211,  -589,  -589,  -589,  -589,  -169,  -589,  -154,  4170,  -101,
     -65,     3,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,    50,
    3846,  2756,    56,    52,  -589,  3484,  -589,  -589,  -169,  -154,
     -77,   123,  4330,  -589,  -589,  -589,  -589,  -589,  -589,  2756,
    2756,  2756,  2574,  3665,  -589,  2756,  -589,  -589,  -589,  -589,
    -589,  4170,  -101,   686,  6182,    58,  -589,  -589,  2756,  -589,
    2756,  2756,    59,  2574,  2756,  2756,  2574,  2756,  2756,  2756,
    2756,   132,    63,    64,    65,    66,    70,    72,  2574,  2756,
    2756,  2756,  2756,  2756,  2756,  2756,  2756,    74,  2938,  3120,
    3302,  2756,  2756,    18,    69,  2574,  -589,  1807,  -589,  -589,
     -20,  -589,  2756,  2756,   254,   -77,  4490,  2756,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  2574,  -589,
    2756,  -589,  -589,  4490,  4490,    88,   270,    80,  -589,   432,
    4490,   104,  4490,    81,    83,    86,   -45,   -25,  -589,   225,
     225,   225,  -151,  -589,   108,  -589,  -170,   846,   -77,  2574,
    2756,  -589,  4170,  2756,  2756,  2756,  2756,  2756,  2756,  2756,
    2756,  2756,  2756,  2756,  2756,  2756,  2756,  2756,  2756,  2756,
    2756,  2756,  2756,  2756,  2756,  -589,  1098,  6469,  1180,  1202,
    -589,    87,  5453,  5473,    90,  4701,  4723,  4784,   213,   117,
    -589,  -589,  -589,  -589,  -589,  -589,    92,  5534,  4806,  5554,
    5615,  5635,  5696,  4867,  5716,  -589,    94,    95,  4889,    96,
      99,  4950,   103,  4972,  5777,  5797,   107,  2211,  -589,  -172,
    -589,  -589,  -106,  -589,  -589,  5033,  5055,   109,   110,  5365,
    -589,  6182,    56,  -158,     1,  4010,  4170,   112,  2756,  2211,
    -589,  2574,  -589,  -589,  -589,  2574,  4650,  2756,  2574,  -589,
    2574,  4490,  -589,  -589,  -589,  6182,  -101,   457,   457,   457,
     180,   180,   457,  1108,  1108,   -52,   -52,   -52,  5858,   180,
     180,    67,    67,    67,   -21,   -21,   225,   225,   225,   111,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,   190,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,   189,   192,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  1062,  -589,  -589,  -589,  -589,
    -589,  2756,  2756,  -589,  -589,  -589,  -589,  2756,  -589,   195,
    -589,  2756,  -589,  2756,  2756,  2756,  2756,  -589,  2756,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -105,  -105,  4490,
    -589,  -589,  -589,  -589,  -103,  -589,    27,   120,  2211,  2756,
    2574,   124,  -589,  4490,    56,   122,  -589,  -589,   128,   129,
     133,  -589,   134,  6182,   334,  -589,   -93,  -156,  5385,  -589,
    -589,  -589,  2756,  2211,  6469,   -22,  -589,  -589,  -589,  -589,
    -589,  5116,  5878,  5939,   136,  5959,  5939,  5939,   164,  6182,
     164,   320,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,   140,   175,   150,  -589,  -589,
    4490,  -589,  -589,  -589,  5138,   -88,  2574,    56,  2211,  -589,
    2211,  4010,  2211,    -7,  2574,   -74,  6182,  2009,  6362,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,   177,  -589,  2756,
     -23,   152,  -589,  2756,   154,   155,   156,   157,  2756,  -589,
    -589,  2756,  2211,  -166,  -589,    56,    -3,  -589,  -589,   -87,
    -589,  -589,  2211,  -589,   169,  -589,   -86,  2574,  -589,  -589,
     -22,  5199,  -589,  -589,  -589,  -589,  5221,  -589,  -589,  -589,
    -589,  6020,  5282,   259,  4490,  -589,   196,  2756,   194,  -589,
    -589,  -589,  -589,  -589,  -589,   -85,  -589,  -589,  -589,  2756,
    -589,  2211,  -589,    62,  6040,  2211,  2211,     2,  6101,  -589,
    -589,  2211,  2211,  -589,  2756,  2211,  6121,  2756,  5304,  -589
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -589,  -589,  -589,  -589,   245,  -588,    61,   160,  -589,  -194,
    -589,  -589,  -589,  -589,  -405,  -589,  -342,  -589,  -589,  -589,
    -589,  -589,  -589,  -589,  -589,  -589,  -589,  -589,    13,  -589,
    -589,    -1,   -31,  -589,   181,  -589,  -589,  -589,  -274,  -589,
    -273,  -589,  -589,  -412,  -167,    -9,    26,   145,  -272,    34,
     -44,  -589,   158,    49,    -2,    11,   673,     8,  -589,  -349,
    -589,    85,  -198,  -589,  -195,  -589,  -589,  -300,  -589,  -519,
    -589,  -157
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -275
static const yytype_int16 yytable[] =
{
     159,   148,   308,   399,   207,   727,   728,   582,   428,   332,
     431,   159,   416,   160,   724,   667,   610,   572,   573,   198,
     572,   573,   583,   235,   210,   202,   162,   622,   155,   428,
    -270,  -270,     6,   432,   571,    -4,   157,     3,   725,   208,
     332,   163,   650,   -63,   -63,  -271,  -271,   209,   584,   664,
     429,   158,   623,   624,   625,   626,   627,   628,   629,   630,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   644,   712,   713,   226,   108,   109,   288,
     289,   290,   291,   292,   293,   294,   200,   428,   201,   295,
     296,   297,   428,   428,   428,   428,   211,   276,   286,   287,
     648,   159,   239,   669,   670,   671,   672,   673,   674,   714,
     675,   238,   274,   663,   160,   240,   298,   299,   698,   731,
     734,   747,  -269,  -269,   241,  -274,  -274,   242,   332,   155,
     159,   707,   159,   148,   396,   397,   243,   157,   350,   351,
     352,   353,   354,   160,   244,   160,   245,   156,   159,   610,
     246,   280,   158,   596,  -272,  -272,   159,   752,   155,   247,
     155,   160,   137,   755,   248,   226,   157,   249,   157,   285,
     250,   352,   353,   354,   426,   427,   155,   251,   252,   658,
     281,   158,   402,   158,   157,   310,   253,   213,   214,   215,
     216,   217,  -234,  -234,   218,   254,   207,   255,   159,   158,
     729,  -252,  -252,   159,   684,   685,   686,   302,   687,   301,
     317,   160,   256,   306,   257,   258,   210,   586,   341,   342,
     343,   259,   260,   318,   261,   316,   155,   262,   263,   159,
     264,   208,   312,   265,   157,   266,   324,   267,   268,   209,
     310,   332,   328,   269,   270,   271,   272,   275,   156,   158,
     282,   361,   700,   283,   364,   303,   314,   350,   351,   352,
     353,   354,   355,   273,   369,   360,   376,   407,   415,   370,
     371,   372,   373,   398,   332,   159,   374,   156,   375,   156,
     385,   416,   331,   226,   418,   421,   332,   423,   160,   424,
     726,   425,   430,   540,   137,   156,   543,   549,   550,   408,
     559,   560,   562,   155,   226,   563,   410,   226,   655,   565,
     284,   157,   569,   603,   579,   580,   412,   414,   591,   226,
     606,   607,   652,   420,   608,   202,   158,   614,   659,   656,
     436,   341,   342,   343,  -180,  -181,   226,   434,   661,  -179,
     662,   403,   682,   318,   680,   156,   690,   333,   334,   335,
     336,   337,   338,   339,   340,   691,   692,   710,   715,   226,
     717,   718,   719,   720,   341,   342,   343,   347,   348,   349,
     350,   351,   352,   353,   354,   733,   741,   743,   745,   279,
     649,   332,   742,   585,   699,   702,   313,   601,   704,   620,
     226,   330,   706,   547,   750,   159,   344,   422,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   160,   668,
     736,   646,     0,   159,   159,     0,     0,   159,     0,   548,
       0,     0,   156,   155,     0,     0,   160,   590,     0,   595,
     160,   157,     0,     0,     0,   735,   599,   401,   600,   408,
       0,   155,   588,     0,   324,   155,   158,     0,     0,   157,
     589,     0,     0,   157,   333,   334,   335,   336,   337,   338,
     339,   340,     0,   308,   158,     0,     0,     0,   158,     0,
       0,   341,   342,   343,     0,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,   226,     0,     0,   226,
       0,   226,     0,   332,     0,     0,     0,     0,     0,     0,
     688,     0,     0,   344,     0,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,     0,     0,     0,   332,     0,
       0,     0,     0,     0,     0,     0,   689,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   156,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   570,     0,     0,
     156,     0,     0,     0,   156,     0,   333,   334,   335,   336,
     337,   338,   339,   340,     0,   651,   159,     0,     0,   594,
       0,     0,   647,   341,   342,   343,     0,     0,     0,   160,
       0,     0,     0,     0,   336,   337,   657,     0,     0,     0,
       0,   159,     0,     0,   155,     0,     0,     0,   341,   342,
     343,     0,   157,     0,   160,   344,     0,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   158,     0,   155,
       0,     0,     0,     0,     0,     0,     0,   157,   419,     0,
       0,   226,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   158,     0,     0,     0,   159,     0,   159,   159,
     159,     0,     0,   695,     0,   159,     0,     0,     0,   160,
       0,   160,   160,   160,     0,     0,     0,     0,   160,     0,
       0,     0,     0,     0,   155,     0,   155,   155,   155,     0,
     159,     0,   157,   155,   157,   157,   157,     0,     0,     0,
     159,   157,     0,   160,     0,     0,     0,   158,     0,   158,
     158,   158,     0,   160,     0,     0,   158,   226,   155,     0,
       0,     0,     0,   156,     0,   226,   157,     0,   155,     0,
       0,     0,     0,     0,     0,     0,   157,   695,   653,   159,
       0,   158,     0,   159,   159,     0,     0,     0,   156,   159,
     159,   158,   160,   159,     0,     0,   160,   160,     0,     0,
       0,     0,   160,   160,     0,     0,   160,   155,   226,     0,
       0,   155,   155,     0,     0,   157,     0,   155,   155,   157,
     157,   155,     0,     0,     0,   157,   157,     0,     0,   157,
     158,     0,     0,     0,   158,   158,     0,     0,     0,     0,
     158,   158,     0,   156,   158,   156,   156,   156,     0,     0,
       0,     0,   156,     0,     0,     0,     0,     0,   701,     0,
     703,     0,   705,     0,     0,     0,     0,   401,   108,   109,
     288,   289,   290,   291,   292,   293,   294,   156,     0,     0,
     295,   296,   297,     0,     0,     0,     0,   156,     0,     0,
       0,     0,   723,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   732,     0,     0,     0,     0,   329,   299,     0,
       0,     0,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   156,     0,     0,     0,
     156,   156,   319,   320,   321,     0,   156,   156,   327,     0,
     156,   749,     0,     0,     0,     0,   753,   332,     0,     0,
       0,   356,   401,   358,   359,   401,     0,   362,   363,     0,
     365,   366,   367,   368,     0,     0,     0,     0,     0,     0,
       0,     0,   377,   378,   379,   380,   381,   382,   383,   384,
       0,   388,   391,   393,   394,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   405,   406,     0,     0,     0,
     409,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   411,     0,     0,     0,     0,     0,     0,
     333,   334,   335,   336,   337,   338,   339,   340,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   341,   342,   343,
       0,     0,     0,   435,     0,     0,   437,   438,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,     0,   344,
       0,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   460,   461,   462,
     463,   464,   465,     0,   466,     0,     0,   467,   468,     0,
     469,   593,   470,   471,   472,   473,     0,     0,     0,     0,
     598,     0,     0,     0,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,     0,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   525,   526,     0,   332,
       0,   527,     0,     0,     0,     0,     0,     0,     0,   332,
       0,     0,   528,   529,   530,   531,     0,     0,     0,     0,
       0,   532,   533,   534,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   611,   612,     0,     0,     0,     0,
     613,     0,     0,     0,   615,     0,   616,   617,   619,   619,
       0,   621,   333,   334,   335,   336,   337,   338,   339,   340,
       0,   332,   333,   334,   335,   336,   337,   338,     0,   341,
     342,   343,   654,     0,     0,     0,     0,     0,     0,   341,
     342,   343,     0,   332,     0,   609,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   666,     0,     0,     0,     0,
       0,   344,     0,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,     0,   459,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   333,   334,   335,   336,   337,   338,
     339,   340,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   341,   342,   343,     0,     0,   333,   334,   335,   336,
     337,   338,   339,   340,     0,     0,     0,     0,     0,     0,
       0,     0,   711,   341,   342,   343,   716,     0,     0,     0,
       0,   721,     0,   344,   722,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,   538,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,     0,     0,     0,
     744,     0,     0,     0,     0,     0,     0,     0,   539,     0,
       0,     0,   748,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     0,     7,   756,     0,     8,
     758,     9,    10,    11,    12,     0,    13,     0,    14,    15,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,     0,     0,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
       0,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,     0,     0,   103,   104,     0,     0,   105,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,   108,   109,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       0,     0,     7,     0,     0,     8,     0,     9,    10,    11,
      12,   132,    13,   133,    14,    15,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,     0,     0,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,     0,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,     0,     0,   103,
     104,     0,     0,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   107,     0,
       0,     0,     0,     0,     0,     0,     0,   108,   109,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   132,   276,   133,
       4,     5,     0,     0,     7,     0,     0,     8,     0,     9,
      10,    11,    12,     0,    13,     0,    14,    15,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,     0,     0,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,     0,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,     0,
       0,   103,   104,     0,     0,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     107,     0,     0,     0,     0,     0,     0,     0,     0,   108,
     109,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   132,
     400,   133,     4,     5,     0,     0,     7,     0,     0,     8,
       0,     9,    10,    11,    12,     0,    13,     0,    14,    15,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,     0,     0,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
       0,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,     0,     0,   103,   104,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,   108,   109,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   132,   708,   133,     4,     5,     0,     0,     7,     0,
       0,     8,     0,     9,    10,    11,    12,     0,    13,     0,
      14,    15,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
       0,     0,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,     0,     0,   103,   104,     0,     0,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   107,     0,     0,     0,     0,     0,
       0,     0,     0,   108,   109,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       0,     0,     0,     0,     0,     0,     0,     6,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,    13,
       0,    14,     0,   132,     0,   133,    18,    19,    20,    21,
      22,    23,   170,    25,    26,    27,   174,   175,   176,   177,
     178,   179,   180,   181,    36,    37,    38,    39,    40,    41,
      42,     0,     0,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     0,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,   183,
     184,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   101,   196,     0,     0,     0,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,   213,   214,   215,
     216,   217,     0,     0,   218,   107,     0,     0,     0,     0,
       0,     0,     0,     0,   108,   109,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       0,     0,     0,   219,     0,     0,     0,   220,   221,     0,
     212,    13,   222,    14,   223,     0,   224,   225,    18,    19,
      20,    21,    22,    23,   170,    25,    26,    27,   174,   175,
     176,   177,   178,   179,   180,   181,    36,    37,    38,    39,
      40,    41,    42,     0,     0,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     0,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,   183,   184,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   101,   196,     0,     0,     0,     0,
       0,     0,   105,     0,     0,     0,     0,     0,     0,   213,
     214,   215,   216,   217,     0,     0,   218,   107,     0,     0,
       0,     0,     0,     0,     0,     0,   108,   109,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     6,     0,     0,     0,   219,     0,     0,     0,   220,
     221,     0,     0,    13,   222,    14,   223,     0,     0,   225,
      18,    19,    20,    21,    22,    23,   170,    25,    26,    27,
     174,   175,   176,   177,   178,   179,   180,   181,    36,    37,
      38,    39,    40,    41,    42,     0,     0,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,     0,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,   183,   184,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   101,   196,     0,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,   213,   214,   215,   216,   217,     0,     0,   218,   107,
       0,     0,     0,     0,     0,     0,     0,     0,   108,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     0,     0,     0,   219,     0,     0,
       0,   220,   221,     0,     0,    13,     0,    14,     0,     0,
       0,   225,    18,    19,    20,    21,    22,    23,   170,    25,
      26,    27,   174,   175,   176,   177,   178,   179,   180,   181,
      36,    37,    38,    39,    40,    41,    42,     0,     0,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,     0,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,   183,   184,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   101,   196,
       0,     0,     0,     0,     0,     0,   105,     0,     0,     0,
       0,     0,     0,   213,   214,   386,   216,   217,     0,     0,
     387,   107,     0,     0,     0,     0,     0,     0,     0,     0,
     108,   109,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     6,     0,     0,     0,   219,
       0,     0,     0,   220,   221,     0,     0,    13,     0,    14,
       0,     0,     0,   225,    18,    19,    20,    21,    22,    23,
     170,    25,    26,    27,   174,   175,   176,   177,   178,   179,
     180,   181,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     0,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   183,   184,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     101,   196,     0,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,   213,   214,   389,   216,   217,
       0,     0,   390,   107,     0,     0,     0,     0,     0,     0,
       0,     0,   108,   109,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     6,     0,     0,
       0,   219,     0,     0,     0,   220,   221,     0,     0,    13,
       0,    14,     0,     0,     0,   225,    18,    19,    20,    21,
      22,    23,   170,    25,    26,    27,   174,   175,   176,   177,
     178,   179,   180,   181,    36,    37,    38,    39,    40,    41,
      42,     0,     0,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     0,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,   183,
     184,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   101,   196,     0,     0,     0,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,   213,   214,   215,
     216,   217,     0,     0,   392,   107,     0,     0,     0,     0,
       0,     0,     0,     0,   108,   109,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   219,     0,     0,     0,   220,   221,     0,
       0,     0,     0,    14,     0,     0,     0,   225,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,     0,     0,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,     0,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,     0,     0,     0,     0,
       0,     0,   105,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   164,     0,     0,     0,     0,   205,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,    36,    37,    38,
      39,    40,    41,    42,     0,     0,    43,    44,    45,    46,
      47,    48,    49,    50,    51,   182,     0,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,   183,   184,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,     0,     0,     0,
       0,     0,     0,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,     0,    13,     0,    14,     0,     0,   276,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     0,     0,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,     0,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,     0,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
       0,     0,     0,     0,     0,     0,     0,     0,   108,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,    12,     0,    13,     0,    14,
       0,     0,     0,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     0,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,     0,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   107,     0,     0,     0,     0,     0,     0,
       0,     0,   108,   109,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,    14,
       0,     0,     0,     0,    18,    19,    20,    21,    22,    23,
     170,    25,    26,    27,   174,   175,   176,   177,   178,   179,
     180,   181,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     0,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   183,   184,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     101,   196,     0,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,    14,
       0,     0,     0,     0,    18,    19,    20,    21,    22,    23,
     170,    25,    26,    27,   174,   175,   176,   177,   178,   179,
     180,   181,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     0,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   183,   184,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     101,   196,     0,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   164,
       0,     0,     0,     0,     0,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
     182,     0,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   183,   184,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,     0,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   197,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   164,
       0,     0,     0,     0,     0,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
     182,     0,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,   183,   184,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   332,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   597,   332,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
       0,     0,     0,     0,     0,   333,   334,   335,   336,   337,
     338,   339,   340,     0,     0,   332,     0,     0,     0,     0,
       0,     0,   341,   342,   343,     0,     0,   333,   334,   335,
     336,   337,   338,   339,   340,     0,     0,   332,     0,     0,
       0,     0,     0,     0,   341,   342,   343,     0,     0,     0,
       0,     0,     0,     0,   344,     0,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,   544,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   333,   334,
     335,   336,   337,   338,   339,   340,     0,     0,   332,   545,
       0,     0,     0,     0,     0,   341,   342,   343,     0,     0,
     333,   334,   335,   336,   337,   338,   339,   340,     0,     0,
     332,     0,     0,     0,     0,     0,     0,   341,   342,   343,
       0,     0,     0,     0,     0,     0,     0,   344,     0,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   344,
     546,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   333,   334,   335,   336,   337,   338,   339,   340,     0,
       0,   332,   552,     0,     0,     0,     0,     0,   341,   342,
     343,     0,     0,   333,   334,   335,   336,   337,   338,   339,
     340,     0,     0,   332,     0,     0,     0,     0,     0,     0,
     341,   342,   343,     0,     0,     0,     0,     0,     0,     0,
     344,     0,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,   557,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   333,   334,   335,   336,   337,   338,
     339,   340,     0,     0,   332,   561,     0,     0,     0,     0,
       0,   341,   342,   343,     0,     0,   333,   334,   335,   336,
     337,   338,   339,   340,     0,     0,   332,     0,     0,     0,
       0,     0,     0,   341,   342,   343,     0,     0,     0,     0,
       0,     0,     0,   344,     0,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,   564,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   333,   334,   335,
     336,   337,   338,   339,   340,     0,     0,   332,   566,     0,
       0,     0,     0,     0,   341,   342,   343,     0,     0,   333,
     334,   335,   336,   337,   338,   339,   340,     0,     0,   332,
       0,     0,     0,     0,     0,     0,   341,   342,   343,     0,
       0,     0,     0,     0,     0,     0,   344,     0,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   344,   577,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     333,   334,   335,   336,   337,   338,   339,   340,     0,     0,
     332,   578,     0,     0,     0,     0,     0,   341,   342,   343,
       0,     0,   333,   334,   335,   336,   337,   338,   339,   340,
       0,     0,   332,     0,     0,     0,     0,     0,     0,   341,
     342,   343,     0,     0,     0,     0,     0,     0,     0,   344,
       0,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,   678,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   333,   334,   335,   336,   337,   338,   339,
     340,     0,     0,   332,   697,     0,     0,     0,     0,     0,
     341,   342,   343,     0,     0,   333,   334,   335,   336,   337,
     338,   339,   340,     0,     0,   332,     0,     0,     0,     0,
       0,     0,   341,   342,   343,     0,     0,     0,     0,     0,
       0,     0,   344,     0,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   344,   737,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   333,   334,   335,   336,
     337,   338,   339,   340,     0,     0,   332,   738,     0,     0,
       0,     0,     0,   341,   342,   343,     0,     0,   333,   334,
     335,   336,   337,   338,   339,   340,   332,     0,     0,     0,
       0,     0,     0,     0,     0,   341,   342,   343,     0,     0,
       0,     0,     0,     0,     0,   344,     0,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,   740,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   333,
     334,   335,   336,   337,   338,   339,   340,     0,     0,     0,
     759,     0,     0,     0,   332,     0,   341,   342,   343,   333,
     334,   335,   336,   337,   338,   339,   340,     0,     0,     0,
       0,     0,     0,     0,   332,     0,   341,   342,   343,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   344,     0,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
       0,     0,     0,     0,     0,     0,   581,     0,   344,     0,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
       0,     0,     0,     0,     0,     0,   665,   333,   334,   335,
     336,   337,   338,   339,   340,   332,     0,     0,     0,     0,
       0,     0,     0,     0,   341,   342,   343,   333,   334,   335,
     336,   337,   338,   339,   340,   332,     0,     0,     0,     0,
       0,     0,     0,     0,   341,   342,   343,     0,     0,     0,
       0,     0,     0,   541,     0,     0,   344,     0,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,     0,     0,
       0,     0,     0,   542,     0,     0,   344,     0,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   333,   334,
     335,   336,   337,   338,   339,   340,   332,     0,     0,     0,
       0,     0,     0,     0,     0,   341,   342,   343,   333,   334,
     335,   336,   337,   338,   339,   340,   332,     0,     0,     0,
       0,     0,     0,     0,     0,   341,   342,   343,     0,     0,
       0,     0,     0,     0,   551,     0,     0,   344,     0,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,     0,
       0,     0,     0,     0,   553,     0,     0,   344,     0,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   333,
     334,   335,   336,   337,   338,   339,   340,   332,     0,     0,
       0,     0,     0,     0,     0,     0,   341,   342,   343,   333,
     334,   335,   336,   337,   338,   339,   340,   332,     0,     0,
       0,     0,     0,     0,     0,     0,   341,   342,   343,     0,
       0,     0,     0,     0,     0,   554,     0,     0,   344,     0,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
       0,     0,     0,     0,     0,   555,     0,     0,   344,     0,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     333,   334,   335,   336,   337,   338,   339,   340,   332,     0,
       0,     0,     0,     0,     0,     0,     0,   341,   342,   343,
     333,   334,   335,   336,   337,   338,   339,   340,   332,     0,
       0,     0,     0,     0,     0,     0,     0,   341,   342,   343,
       0,     0,     0,     0,     0,     0,   556,     0,     0,   344,
       0,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,     0,     0,     0,     0,     0,   558,     0,     0,   344,
       0,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   333,   334,   335,   336,   337,   338,   339,   340,   332,
       0,     0,     0,     0,     0,     0,     0,     0,   341,   342,
     343,   333,   334,   335,   336,   337,   338,   339,   340,   332,
       0,     0,     0,     0,     0,     0,     0,     0,   341,   342,
     343,     0,     0,     0,     0,     0,     0,   567,     0,     0,
     344,     0,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,     0,     0,     0,     0,     0,   568,     0,     0,
     344,     0,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   333,   334,   335,   336,   337,   338,   339,   340,
     332,     0,     0,     0,     0,     0,     0,     0,     0,   341,
     342,   343,   333,   334,   335,   336,   337,   338,   339,   340,
     332,     0,     0,     0,     0,     0,     0,     0,     0,   341,
     342,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,   602,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,     0,     0,     0,     0,     0,   679,     0,
       0,   344,     0,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   333,   334,   335,   336,   337,   338,   339,
     340,   332,     0,     0,     0,     0,     0,     0,     0,     0,
     341,   342,   343,   333,   334,   335,   336,   337,   338,   339,
     340,   332,     0,     0,     0,     0,     0,     0,     0,     0,
     341,   342,   343,     0,     0,     0,     0,     0,     0,   680,
       0,     0,   344,     0,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,     0,     0,     0,     0,     0,   683,
       0,     0,   344,     0,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   333,   334,   335,   336,   337,   338,
     339,   340,   332,     0,     0,     0,     0,     0,     0,     0,
       0,   341,   342,   343,   333,   334,   335,   336,   337,   338,
     339,   340,   332,     0,     0,     0,     0,     0,     0,     0,
       0,   341,   342,   343,     0,     0,     0,     0,     0,     0,
     739,     0,     0,   344,     0,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,     0,     0,     0,     0,     0,
       0,     0,     0,   344,   751,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   333,   334,   335,   336,   337,
     338,   339,   340,   332,     0,     0,     0,     0,     0,     0,
       0,     0,   341,   342,   343,   333,   334,   335,   336,   337,
     338,   339,   340,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   341,   342,   343,     0,     0,     0,     0,     0,
       0,   754,     0,     0,   344,     0,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,     0,     0,     0,     0,
       0,   757,     0,     0,   344,     0,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   333,   334,   335,   336,
     337,   338,   339,   340,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,   342,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   344,     0,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   460,   461,   462,
     463,   464,   465,   709,   466,     0,     0,   467,   468,     0,
     469,     0,   470,   471,   472,   473,     0,     0,     0,     0,
       0,     0,     0,     0,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,     0,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   525,   526,     0,     0,
       0,   527,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,   529,   530,   531,     0,     0,     0,     0,
       0,   532,   533,   534,   460,   461,   462,   463,   464,   465,
       0,   466,     0,     0,   467,   468,     0,   469,     0,   470,
     471,   472,   473,     0,     0,     0,     0,     0,     0,     0,
       0,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
       0,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,   521,
     522,   523,   524,   525,   526,     0,     0,     0,   527,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
     529,   530,   531,     0,     0,     0,     0,     0,   532,   533,
     534
};

static const yytype_int16 yycheck[] =
{
       2,     2,   200,   275,    13,     8,     9,   412,   180,    61,
     180,    13,    11,     2,   180,   603,   535,   123,   124,     6,
     123,   124,   180,    15,    13,    12,   204,   132,     2,   180,
     199,   200,     5,   203,   206,     0,     2,     0,   204,    13,
      61,   204,    15,   199,   200,   199,   200,    13,   206,   205,
     201,     2,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    97,    98,    15,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   205,   180,   205,   154,
     155,   156,   180,   180,   180,   180,   205,   203,   199,   200,
     203,   103,   202,   125,   126,   127,   128,   129,   130,   132,
     132,   205,   104,   206,   103,   205,   181,   182,   206,   206,
     206,   206,   199,   200,   205,   199,   200,   205,    61,   103,
     132,   205,   134,   134,   116,   117,   205,   103,   190,   191,
     192,   193,   194,   132,   205,   134,   205,     2,   150,   668,
     205,   138,   103,   425,   199,   200,   158,   745,   132,   205,
     134,   150,     2,   751,   205,   104,   132,   205,   134,   158,
     205,   192,   193,   194,   199,   200,   150,   205,   205,   584,
     204,   132,   202,   134,   150,   184,   205,   125,   126,   127,
     128,   129,   199,   200,   132,   205,   205,   205,   200,   150,
     203,   199,   200,   205,   616,   617,   618,   204,   620,   160,
     212,   200,   205,   200,   205,   205,   205,   415,   151,   152,
     153,   205,   205,   212,   205,   212,   200,   205,   205,   231,
     205,   205,   180,   205,   200,   205,   223,   205,   205,   205,
     184,    61,   231,   205,   205,   205,   205,   205,   103,   200,
     205,   243,   657,   205,   246,   205,   133,   190,   191,   192,
     193,   194,   204,   103,   132,   206,   258,    13,   180,   206,
     206,   206,   206,   204,    61,   277,   206,   132,   206,   134,
     206,    11,   233,   222,   204,   181,    61,   206,   277,   206,
     695,   205,   184,   206,   134,   150,   206,   180,   206,   286,
     206,   206,   206,   277,   243,   206,   298,   246,   580,   206,
     150,   277,   205,   202,   205,   205,   303,   304,   206,   258,
     130,   132,   202,   310,   132,   312,   277,   132,   206,   205,
     332,   151,   152,   153,   206,   206,   275,   329,   204,   206,
       6,   280,   206,   332,   180,   200,   206,   134,   135,   136,
     137,   138,   139,   140,   141,   180,   206,   180,   206,   298,
     206,   206,   206,   206,   151,   152,   153,   187,   188,   189,
     190,   191,   192,   193,   194,   206,   117,   181,   184,   134,
     574,    61,   724,   414,   656,   659,   205,   431,   661,   556,
     329,   233,   664,   180,   743,   397,   183,   312,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   397,   604,
     710,   568,    -1,   415,   416,    -1,    -1,   419,    -1,   206,
      -1,    -1,   277,   397,    -1,    -1,   415,   416,    -1,   421,
     419,   397,    -1,    -1,    -1,   707,   428,   277,   430,   426,
      -1,   415,   416,    -1,   431,   419,   397,    -1,    -1,   415,
     416,    -1,    -1,   419,   134,   135,   136,   137,   138,   139,
     140,   141,    -1,   661,   415,    -1,    -1,    -1,   419,    -1,
      -1,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   421,    -1,    -1,    -1,   425,    -1,    -1,   428,
      -1,   430,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   397,    -1,    -1,
     415,    -1,    -1,    -1,   419,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,   576,   578,    -1,    -1,   419,
      -1,    -1,   569,   151,   152,   153,    -1,    -1,    -1,   578,
      -1,    -1,    -1,    -1,   137,   138,   583,    -1,    -1,    -1,
      -1,   603,    -1,    -1,   578,    -1,    -1,    -1,   151,   152,
     153,    -1,   578,    -1,   603,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   578,    -1,   603,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   603,   206,    -1,
      -1,   580,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   603,    -1,    -1,    -1,   658,    -1,   660,   661,
     662,    -1,    -1,   650,    -1,   667,    -1,    -1,    -1,   658,
      -1,   660,   661,   662,    -1,    -1,    -1,    -1,   667,    -1,
      -1,    -1,    -1,    -1,   658,    -1,   660,   661,   662,    -1,
     692,    -1,   658,   667,   660,   661,   662,    -1,    -1,    -1,
     702,   667,    -1,   692,    -1,    -1,    -1,   658,    -1,   660,
     661,   662,    -1,   702,    -1,    -1,   667,   656,   692,    -1,
      -1,    -1,    -1,   578,    -1,   664,   692,    -1,   702,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   702,   724,   578,   741,
      -1,   692,    -1,   745,   746,    -1,    -1,    -1,   603,   751,
     752,   702,   741,   755,    -1,    -1,   745,   746,    -1,    -1,
      -1,    -1,   751,   752,    -1,    -1,   755,   741,   707,    -1,
      -1,   745,   746,    -1,    -1,   741,    -1,   751,   752,   745,
     746,   755,    -1,    -1,    -1,   751,   752,    -1,    -1,   755,
     741,    -1,    -1,    -1,   745,   746,    -1,    -1,    -1,    -1,
     751,   752,    -1,   658,   755,   660,   661,   662,    -1,    -1,
      -1,    -1,   667,    -1,    -1,    -1,    -1,    -1,   658,    -1,
     660,    -1,   662,    -1,    -1,    -1,    -1,   667,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   692,    -1,    -1,
     154,   155,   156,    -1,    -1,    -1,    -1,   702,    -1,    -1,
      -1,    -1,   692,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   702,    -1,    -1,    -1,    -1,   181,   182,    -1,
      -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   741,    -1,    -1,    -1,
     745,   746,   219,   220,   221,    -1,   751,   752,   225,    -1,
     755,   741,    -1,    -1,    -1,    -1,   746,    61,    -1,    -1,
      -1,   238,   752,   240,   241,   755,    -1,   244,   245,    -1,
     247,   248,   249,   250,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   259,   260,   261,   262,   263,   264,   265,   266,
      -1,   268,   269,   270,   271,   272,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   282,   283,    -1,    -1,    -1,
     287,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   300,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
      -1,    -1,    -1,   330,    -1,    -1,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,    -1,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    20,    -1,    22,    -1,    -1,    25,    26,    -1,
      28,   418,    30,    31,    32,    33,    -1,    -1,    -1,    -1,
     427,    -1,    -1,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    61,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   541,   542,    -1,    -1,    -1,    -1,
     547,    -1,    -1,    -1,   551,    -1,   553,   554,   555,   556,
      -1,   558,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,    61,   134,   135,   136,   137,   138,   139,    -1,   151,
     152,   153,   579,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,    -1,    61,    -1,   203,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   602,    -1,    -1,    -1,    -1,
      -1,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,    -1,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   679,   151,   152,   153,   683,    -1,    -1,    -1,
      -1,   688,    -1,   183,   691,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   206,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    -1,    -1,    -1,
     727,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,   739,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,   754,    -1,    10,
     757,    12,    13,    14,    15,    -1,    17,    -1,    19,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,    -1,   114,   115,    -1,    -1,   118,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
      -1,    -1,     7,    -1,    -1,    10,    -1,    12,    13,    14,
      15,   202,    17,   204,    19,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,    -1,   114,
     115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,   203,   204,
       3,     4,    -1,    -1,     7,    -1,    -1,    10,    -1,    12,
      13,    14,    15,    -1,    17,    -1,    19,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    -1,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
      -1,   114,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   202,
     203,   204,     3,     4,    -1,    -1,     7,    -1,    -1,    10,
      -1,    12,    13,    14,    15,    -1,    17,    -1,    19,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,    -1,   114,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   202,   203,   204,     3,     4,    -1,    -1,     7,    -1,
      -1,    10,    -1,    12,    13,    14,    15,    -1,    17,    -1,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,    -1,   114,   115,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    17,
      -1,    19,    -1,   202,    -1,   204,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    -1,    -1,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,
      -1,    -1,    -1,   191,    -1,    -1,    -1,   195,   196,    -1,
      16,    17,   200,    19,   202,    -1,   204,   205,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    -1,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     5,    -1,    -1,    -1,   191,    -1,    -1,    -1,   195,
     196,    -1,    -1,    17,   200,    19,   202,    -1,    -1,   205,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    -1,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     5,    -1,    -1,    -1,   191,    -1,    -1,
      -1,   195,   196,    -1,    -1,    17,    -1,    19,    -1,    -1,
      -1,   205,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     5,    -1,    -1,    -1,   191,
      -1,    -1,    -1,   195,   196,    -1,    -1,    17,    -1,    19,
      -1,    -1,    -1,   205,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,    -1,    -1,
      -1,   191,    -1,    -1,    -1,   195,   196,    -1,    -1,    17,
      -1,    19,    -1,    -1,    -1,   205,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    -1,    -1,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,    -1,    -1,   195,   196,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,   205,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    -1,    -1,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   205,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    17,    -1,    19,    -1,    -1,   203,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    -1,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    15,    -1,    17,    -1,    19,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    19,
      -1,    -1,    -1,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    19,
      -1,    -1,    -1,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    19,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    61,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,    -1,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,   206,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   134,   135,
     136,   137,   138,   139,   140,   141,    -1,    -1,    61,   206,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     206,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    61,   206,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,    -1,    -1,   134,   135,   136,   137,   138,   139,   140,
     141,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   206,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   134,   135,   136,   137,   138,   139,
     140,   141,    -1,    -1,    61,   206,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,    -1,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   206,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   134,   135,   136,
     137,   138,   139,   140,   141,    -1,    -1,    61,   206,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,   134,
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   206,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,    -1,
      61,   206,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
      -1,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,   206,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   134,   135,   136,   137,   138,   139,   140,
     141,    -1,    -1,    61,   206,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,    -1,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   206,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,    -1,    61,   206,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,    -1,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   206,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   134,
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,    -1,
     206,    -1,    -1,    -1,    61,    -1,   151,   152,   153,   134,
     135,   136,   137,   138,   139,   140,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
      -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,   183,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
      -1,    -1,    -1,    -1,    -1,    -1,   201,   134,   135,   136,
     137,   138,   139,   140,   141,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,   134,   135,   136,
     137,   138,   139,   140,   141,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,   183,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,    -1,    -1,
      -1,    -1,    -1,   180,    -1,    -1,   183,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   134,   135,
     136,   137,   138,   139,   140,   141,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,   134,   135,
     136,   137,   138,   139,   140,   141,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,   183,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   134,
     135,   136,   137,   138,   139,   140,   141,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,   134,
     135,   136,   137,   138,   139,   140,   141,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,   183,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,   183,    -1,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     134,   135,   136,   137,   138,   139,   140,   141,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
     134,   135,   136,   137,   138,   139,   140,   141,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,   183,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   134,   135,   136,   137,   138,   139,   140,   141,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,   134,   135,   136,   137,   138,   139,   140,   141,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
     183,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   134,   135,   136,   137,   138,   139,   140,   141,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,   134,   135,   136,   137,   138,   139,   140,   141,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,
     152,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      -1,   183,    -1,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   134,   135,   136,   137,   138,   139,   140,
     141,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,   134,   135,   136,   137,   138,   139,   140,
     141,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,   183,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,   183,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   134,   135,   136,   137,   138,   139,
     140,   141,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,   134,   135,   136,   137,   138,   139,
     140,   141,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,   183,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   134,   135,   136,   137,   138,
     139,   140,   141,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,   183,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,   183,    -1,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   151,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    25,    26,    -1,
      28,    -1,    30,    31,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    -1,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,    15,    16,    17,    18,    19,    20,
      -1,    22,    -1,    -1,    25,    26,    -1,    28,    -1,    30,
      31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    -1,    -1,    -1,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   208,   209,     0,     3,     4,     5,     7,    10,    12,
      13,    14,    15,    17,    19,    20,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   114,   115,   118,   122,   133,   142,   143,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   202,   204,   210,   211,   213,   214,   217,   219,
     224,   225,   226,   227,   229,   231,   232,   235,   238,   242,
     243,   246,   248,   249,   252,   253,   254,   256,   260,   261,
     262,   269,   204,   204,    19,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    60,    87,    88,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   133,   235,   237,
     205,   205,   235,   267,   268,   205,   241,   252,   253,   256,
     262,   205,    16,   125,   126,   127,   128,   129,   132,   191,
     195,   196,   200,   202,   204,   205,   213,   238,   253,   254,
     256,   260,   261,   262,   263,   264,   265,   266,   205,   202,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   214,   264,   205,   203,   212,   214,   211,
     235,   204,   205,   205,   214,   262,   199,   200,   144,   145,
     146,   147,   148,   149,   150,   154,   155,   156,   181,   182,
     259,   260,   204,   205,    15,   228,   235,   247,   269,   263,
     184,   221,   180,   241,   133,   133,   235,   261,   262,   263,
     263,   263,   255,   264,   235,   257,   258,   263,   262,   181,
     259,   260,    61,   134,   135,   136,   137,   138,   139,   140,
     141,   151,   152,   153,   183,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   204,   263,   270,   263,   263,
     206,   264,   263,   263,   264,   263,   263,   263,   263,   132,
     206,   206,   206,   206,   206,   206,   264,   263,   263,   263,
     263,   263,   263,   263,   263,   206,   127,   132,   263,   127,
     132,   263,   132,   263,   263,   263,   116,   117,   204,   255,
     203,   214,   202,   213,   218,   263,   263,    13,   235,   263,
     264,   263,   235,   236,   235,   180,    11,   239,   204,   206,
     235,   181,   268,   206,   206,   205,   199,   200,   180,   201,
     184,   180,   203,   206,   264,   263,   261,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     263,   263,   263,   263,   263,   263,   263,   263,   263,   206,
      15,    16,    17,    18,    19,    20,    22,    25,    26,    28,
      30,    31,    32,    33,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    99,   110,   111,
     112,   113,   119,   120,   121,   271,   272,   276,   206,   206,
     206,   180,   180,   206,   206,   206,   206,   180,   206,   180,
     206,   180,   206,   180,   180,   180,   180,   206,   180,   206,
     206,   206,   206,   206,   206,   206,   206,   180,   180,   205,
     214,   206,   123,   124,   215,   216,   220,   206,   206,   205,
     205,   201,   221,   180,   206,   239,   269,   240,   253,   256,
     262,   206,   230,   263,   214,   264,   255,   133,   263,   264,
     264,   257,   184,   202,   273,   277,   130,   132,   132,   203,
     276,   263,   263,   263,   132,   263,   263,   263,   251,   263,
     251,   263,   132,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   278,   278,   235,   203,   216,
      15,   238,   202,   214,   263,   255,   205,   235,   221,   206,
     245,   204,     6,   206,   205,   201,   263,   212,   271,   125,
     126,   127,   128,   129,   130,   132,   274,   275,   206,   180,
     180,   250,   206,   180,   250,   250,   250,   250,   180,   206,
     206,   180,   206,   222,   223,   235,   233,   206,   206,   255,
     221,   214,   245,   214,   247,   214,   255,   205,   203,    21,
     180,   263,    97,    98,   132,   206,   263,   206,   206,   206,
     206,   263,   263,   214,   180,   204,   221,     8,     9,   203,
     234,   206,   214,   206,   206,   255,   274,   206,   206,   180,
     206,   117,   223,   181,   263,   184,   244,   206,   263,   214,
     266,   184,   212,   214,   180,   212,   263,   180,   263,   206
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
#line 183 "./swf5compiler.y"
    { bf = newBuffer();
		bc = newBuffer();
	}
    break;

  case 3:
#line 186 "./swf5compiler.y"
    { Buffer b = newBuffer();
		  bufferWriteConstants(b);
		  bufferConcat(b, bf);
		  bufferConcat(b, bc);
		  *((Buffer *)buffer) = b; }
    break;

  case 4:
#line 191 "./swf5compiler.y"
    { Buffer b = newBuffer(); *((Buffer *)buffer) = b; }
    break;

  case 7:
#line 201 "./swf5compiler.y"
    { bufferConcat(bc, (yyvsp[(1) - (1)].action)); }
    break;

  case 8:
#line 203 "./swf5compiler.y"
    { 
		  if(swfVersion > 6)
			bufferWriteFunction(bf, (yyvsp[(1) - (1)].function), 2); 
		  else
			bufferWriteFunction(bf, (yyvsp[(1) - (1)].function), 1);
		}
    break;

  case 9:
#line 211 "./swf5compiler.y"
    { bufferWriteClass(bf, (yyvsp[(1) - (1)].clazz)); }
    break;

  case 10:
#line 216 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action); }
    break;

  case 11:
#line 219 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (2)].action);
		  bufferConcat((yyval.action), (yyvsp[(2) - (2)].action)); }
    break;

  case 12:
#line 224 "./swf5compiler.y"
    { }
    break;

  case 13:
#line 228 "./swf5compiler.y"
    { (yyval.action) = NULL; }
    break;

  case 14:
#line 229 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); }
    break;

  case 15:
#line 230 "./swf5compiler.y"
    { (yyval.action) = NULL; }
    break;

  case 16:
#line 231 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (2)].action); }
    break;

  case 27:
#line 247 "./swf5compiler.y"
    { 	
		(yyval.classMember) = (yyvsp[(1) - (2)].classMember);
		ASClassMember_append((yyvsp[(1) - (2)].classMember), (yyvsp[(2) - (2)].classMember));			
	}
    break;

  case 28:
#line 254 "./swf5compiler.y"
    { (yyval.classMember) = newASClassMember_function((yyvsp[(2) - (2)].function)); }
    break;

  case 29:
#line 255 "./swf5compiler.y"
    { (yyval.classMember) = (yyvsp[(3) - (4)].classMember); }
    break;

  case 30:
#line 260 "./swf5compiler.y"
    {
		if(classContext)
		{
			swf5error("Nested classes are not allowed\n");
			YYABORT;
		}
		classContext = 1;
	}
    break;

  case 31:
#line 271 "./swf5compiler.y"
    {(yyval.classMember) = NULL; }
    break;

  case 32:
#line 272 "./swf5compiler.y"
    { (yyval.classMember) = (yyvsp[(2) - (3)].classMember); }
    break;

  case 33:
#line 277 "./swf5compiler.y"
    { 
		(yyval.clazz) = newASClass((yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].classMember));
		classContext = 0;
	}
    break;

  case 40:
#line 297 "./swf5compiler.y"
    { (yyval.classMember) = (yyvsp[(1) - (3)].classMember);
		  ASClassMember_append((yyvsp[(1) - (3)].classMember), (yyvsp[(3) - (3)].classMember)); 
		}
    break;

  case 41:
#line 304 "./swf5compiler.y"
    { 
		  ASVariable v = newASVariable((yyvsp[(1) - (4)].str), (yyvsp[(4) - (4)].action)); 
		  (yyval.classMember) = newASClassMember_variable(v);
		}
    break;

  case 42:
#line 309 "./swf5compiler.y"
    { 
			ASVariable v = newASVariable((yyvsp[(1) - (2)].str), NULL);
			(yyval.classMember) = newASClassMember_variable(v);
		}
    break;

  case 43:
#line 316 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); bufferWriteOp((yyval.action), SWFACTION_THROW); }
    break;

  case 44:
#line 320 "./swf5compiler.y"
    { 	(yyval.action) = newBuffer();
									bufferWriteOp((yyval.action), SWFACTION_TRY);
									bufferWriteS16((yyval.action), 8);                /* TRY tag length */
									bufferWriteU8((yyval.action), 0);                 /* flags */
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (2)].action))); /* try block length */
									bufferWriteS16((yyval.action), 0);                /* catch block length */
									bufferWriteS16((yyval.action), 0);                /* finally block length */
									bufferWriteU8((yyval.action), 0);                 /* catch name - empty string */
									bufferConcat((yyval.action), (yyvsp[(2) - (2)].action));                 /* append TRY body */
								 }
    break;

  case 45:
#line 330 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
									bufferWriteOp((yyval.action), SWFACTION_TRY);
									bufferWriteS16((yyval.action), 8+strlen((yyvsp[(5) - (7)].str)));       /* TRY tag length */
									bufferWriteU8((yyval.action), 1);                   /* flags */
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (7)].action))+5); /* try block length + JUMP length */
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (7)].action)));   /* catch block length */
									bufferWriteS16((yyval.action), 0);                  /* finally block length */
									bufferWriteHardString((yyval.action), (yyvsp[(5) - (7)].str), strlen((yyvsp[(5) - (7)].str))+1); /* catch name */
									bufferConcat((yyval.action), (yyvsp[(2) - (7)].action));                   /* append TRY body */
									bufferWriteOp((yyval.action), SWFACTION_JUMP);      /* jump after catch */
									bufferWriteS16((yyval.action), 2);                  /* ... */
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (7)].action)));   /* ... */
									bufferConcat((yyval.action), (yyvsp[(7) - (7)].action));                   /* append CATCH body */
								}
    break;

  case 46:
#line 344 "./swf5compiler.y"
    {	(yyval.action) = newBuffer();
									bufferWriteOp((yyval.action), SWFACTION_TRY);
									bufferWriteS16((yyval.action), 8);                /* TRY tag length */
									bufferWriteU8((yyval.action), 2);                 /* flags */
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (4)].action))); /* try block length */
									bufferWriteS16((yyval.action), 0);                /* catch block length */
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(4) - (4)].action))); /* finally block length */
									bufferWriteU8((yyval.action), 0);                 /* catch name - empty string */
									bufferConcat((yyval.action), (yyvsp[(2) - (4)].action));                 /* append TRY body */
									bufferConcat((yyval.action), (yyvsp[(4) - (4)].action));                 /* append FINALLY body */
								 }
    break;

  case 47:
#line 355 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
									bufferWriteOp((yyval.action), SWFACTION_TRY);
									bufferWriteS16((yyval.action), 8+strlen((yyvsp[(5) - (9)].str)));        /* TRY tag length */
									bufferWriteU8((yyval.action), 3);                    /* flags */
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(2) - (9)].action))+5);  /* try block length + JUMP length */
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (9)].action)));    /* catch block length */
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(9) - (9)].action)));    /* finally block length */
									bufferWriteHardString((yyval.action), (yyvsp[(5) - (9)].str), strlen((yyvsp[(5) - (9)].str))+1); /* catch name */
									bufferConcat((yyval.action), (yyvsp[(2) - (9)].action));                    /* append TRY body */
									bufferWriteOp((yyval.action), SWFACTION_JUMP);       /* jump after catch */
									bufferWriteS16((yyval.action), 2); 
									bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (9)].action)));
									bufferConcat((yyval.action), (yyvsp[(7) - (9)].action));                    /* append CATCH body */
									bufferConcat((yyval.action), (yyvsp[(9) - (9)].action));                    /* append FINALLY body */
								}
    break;

  case 48:
#line 374 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferWriteOp((yyval.action), SWFACTION_WITH);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(6) - (7)].action)));
		  bufferConcat((yyval.action), (yyvsp[(6) - (7)].action)); }
    break;

  case 49:
#line 385 "./swf5compiler.y"
    { int tmp = chkctx(CTX_FUNCTION);
		  if(tmp < 0) 
		  {
			swf5error("return outside function");
			YYABORT;
		  }
		  (yyval.action) = newBuffer();
		  while(--tmp >= 0)
			bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferWriteUndef((yyval.action));
		  bufferWriteOp((yyval.action), SWFACTION_RETURN); }
    break;

  case 50:
#line 398 "./swf5compiler.y"
    { int tmp = chkctx(CTX_FUNCTION);
		  if(tmp < 0)
		  {
			swf5error("return outside function");
			YYABORT;
		  }
		  (yyval.action) = newBuffer();
		  while(--tmp >= 0)
			bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferConcat((yyval.action), (yyvsp[(2) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_RETURN); }
    break;

  case 52:
#line 413 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action)); }
    break;

  case 53:
#line 418 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferWriteOp((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (7)].action))+5);
		  bufferConcatSimple((yyval.action), (yyvsp[(7) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (7)].action)));
		  bufferConcatSimple((yyval.action), (yyvsp[(5) - (7)].action)); }
    break;

  case 54:
#line 429 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (5)].action);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT);
		  bufferWriteOp((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (5)].action)));
		  bufferConcatSimple((yyval.action), (yyvsp[(5) - (5)].action)); }
    break;

  case 55:
#line 438 "./swf5compiler.y"
    { (yyval.action) = NULL; }
    break;

  case 56:
#line 439 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action); }
    break;

  case 57:
#line 444 "./swf5compiler.y"
    { addctx(CTX_SWITCH); }
    break;

  case 58:
#line 450 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferResolveSwitch((yyval.action), &(yyvsp[(6) - (7)].switchcases));
		  bufferResolveJumps((yyval.action));
		  bufferWriteOp((yyval.action), SWFACTION_POP);
		  delctx(CTX_SWITCH);
 /* FIXME: continue in switch continues surrounding loop, if any */
	}
    break;

  case 59:
#line 462 "./swf5compiler.y"
    { (yyval.switchcases).count = 0;
		  (yyval.switchcases).list = 0; }
    break;

  case 60:
#line 466 "./swf5compiler.y"
    { (yyval.switchcases) = (yyvsp[(1) - (2)].switchcases);
		  (yyval.switchcases).list = (struct switchcase*) realloc((yyval.switchcases).list, ((yyval.switchcases).count+1) * sizeof(struct switchcase));
		  (yyval.switchcases).list[(yyval.switchcases).count] = (yyvsp[(2) - (2)].switchcase);
		  (yyval.switchcases).count++; }
    break;

  case 61:
#line 474 "./swf5compiler.y"
    { (yyval.switchcase).cond = (yyvsp[(2) - (4)].action);
		  (yyval.switchcase).action = (yyvsp[(4) - (4)].action);
		  if(chkctx(CTX_BREAK) == CTX_BREAK)
		  {
			delctx(CTX_BREAK);
		  	(yyval.switchcase).isbreak = 1;
		  }
		  else
			(yyval.switchcase).isbreak = 0; 
		}
    break;

  case 62:
#line 486 "./swf5compiler.y"
    { (yyval.switchcase).cond = NULL;
		  (yyval.switchcase).action = (yyvsp[(3) - (3)].action);
		  if(chkctx(CTX_BREAK) == CTX_BREAK)
	          {
			delctx(CTX_BREAK);
		  	(yyval.switchcase).isbreak = 1;
		  }
		  else
			(yyval.switchcase).isbreak = 0;
		}
    break;

  case 64:
#line 504 "./swf5compiler.y"
    { (yyval.str) = strdup("targetPath"); }
    break;

  case 65:
#line 505 "./swf5compiler.y"
    { (yyval.str) = strdup("random"); }
    break;

  case 66:
#line 506 "./swf5compiler.y"
    { (yyval.str) = strdup("getTimer"); }
    break;

  case 67:
#line 507 "./swf5compiler.y"
    { (yyval.str) = strdup("length"); }
    break;

  case 68:
#line 508 "./swf5compiler.y"
    { (yyval.str) = strdup("concat"); }
    break;

  case 69:
#line 509 "./swf5compiler.y"
    { (yyval.str) = strdup("substr"); }
    break;

  case 70:
#line 510 "./swf5compiler.y"
    { (yyval.str) = strdup("trace"); }
    break;

  case 71:
#line 511 "./swf5compiler.y"
    { (yyval.str) = strdup("int"); }
    break;

  case 72:
#line 512 "./swf5compiler.y"
    { (yyval.str) = strdup("ord"); }
    break;

  case 73:
#line 513 "./swf5compiler.y"
    { (yyval.str) = strdup("chr"); }
    break;

  case 74:
#line 514 "./swf5compiler.y"
    { (yyval.str) = strdup("getURL"); }
    break;

  case 75:
#line 515 "./swf5compiler.y"
    { (yyval.str) = strdup("getURL1"); }
    break;

  case 76:
#line 516 "./swf5compiler.y"
    { (yyval.str) = strdup("nextFrame"); }
    break;

  case 77:
#line 517 "./swf5compiler.y"
    { (yyval.str) = strdup("prevFrame"); }
    break;

  case 78:
#line 518 "./swf5compiler.y"
    { (yyval.str) = strdup("play"); }
    break;

  case 79:
#line 519 "./swf5compiler.y"
    { (yyval.str) = strdup("stop"); }
    break;

  case 80:
#line 520 "./swf5compiler.y"
    { (yyval.str) = strdup("toggleQuality"); }
    break;

  case 81:
#line 521 "./swf5compiler.y"
    { (yyval.str) = strdup("stopSounds"); }
    break;

  case 82:
#line 522 "./swf5compiler.y"
    { (yyval.str) = strdup("dup"); }
    break;

  case 83:
#line 523 "./swf5compiler.y"
    { (yyval.str) = strdup("swap"); }
    break;

  case 84:
#line 524 "./swf5compiler.y"
    { (yyval.str) = strdup("pop"); }
    break;

  case 85:
#line 525 "./swf5compiler.y"
    { (yyval.str) = strdup("push"); }
    break;

  case 86:
#line 526 "./swf5compiler.y"
    { (yyval.str) = strdup("setRegister"); }
    break;

  case 87:
#line 527 "./swf5compiler.y"
    { (yyval.str) = strdup("callFunction"); }
    break;

  case 88:
#line 528 "./swf5compiler.y"
    { (yyval.str) = strdup("callMethod"); }
    break;

  case 89:
#line 529 "./swf5compiler.y"
    { (yyval.str) = strdup("and"); }
    break;

  case 90:
#line 530 "./swf5compiler.y"
    { (yyval.str) = strdup("or"); }
    break;

  case 91:
#line 531 "./swf5compiler.y"
    { (yyval.str) = strdup("xor"); }
    break;

  case 92:
#line 532 "./swf5compiler.y"
    { (yyval.str) = strdup("modulo"); }
    break;

  case 93:
#line 533 "./swf5compiler.y"
    { (yyval.str) = strdup("add"); }
    break;

  case 94:
#line 534 "./swf5compiler.y"
    { (yyval.str) = strdup("lessThan"); }
    break;

  case 95:
#line 535 "./swf5compiler.y"
    { (yyval.str) = strdup("equals"); }
    break;

  case 96:
#line 536 "./swf5compiler.y"
    { (yyval.str) = strdup("inc"); }
    break;

  case 97:
#line 537 "./swf5compiler.y"
    { (yyval.str) = strdup("dec"); }
    break;

  case 98:
#line 538 "./swf5compiler.y"
    { (yyval.str) = strdup("typeof"); }
    break;

  case 99:
#line 539 "./swf5compiler.y"
    { (yyval.str) = strdup("enumerate2"); }
    break;

  case 100:
#line 540 "./swf5compiler.y"
    { (yyval.str) = strdup("enumerate"); }
    break;

  case 101:
#line 541 "./swf5compiler.y"
    { (yyval.str) = strdup("initobject"); }
    break;

  case 102:
#line 542 "./swf5compiler.y"
    { (yyval.str) = strdup("initarray"); }
    break;

  case 103:
#line 543 "./swf5compiler.y"
    { (yyval.str) = strdup("getmember"); }
    break;

  case 104:
#line 544 "./swf5compiler.y"
    { (yyval.str) = strdup("setmember"); }
    break;

  case 105:
#line 545 "./swf5compiler.y"
    { (yyval.str) = strdup("shiftleft"); }
    break;

  case 106:
#line 546 "./swf5compiler.y"
    { (yyval.str) = strdup("shiftright"); }
    break;

  case 107:
#line 547 "./swf5compiler.y"
    { (yyval.str) = strdup("shiftright2"); }
    break;

  case 108:
#line 548 "./swf5compiler.y"
    { (yyval.str) = strdup("varequals"); }
    break;

  case 109:
#line 549 "./swf5compiler.y"
    { (yyval.str) = strdup("oldAdd"); }
    break;

  case 110:
#line 550 "./swf5compiler.y"
    { (yyval.str) = strdup("subtract"); }
    break;

  case 111:
#line 551 "./swf5compiler.y"
    { (yyval.str) = strdup("multiply"); }
    break;

  case 112:
#line 552 "./swf5compiler.y"
    { (yyval.str) = strdup("divide"); }
    break;

  case 113:
#line 553 "./swf5compiler.y"
    { (yyval.str) = strdup("oldequals"); }
    break;

  case 114:
#line 554 "./swf5compiler.y"
    { (yyval.str) = strdup("oldlessthan"); }
    break;

  case 115:
#line 555 "./swf5compiler.y"
    { (yyval.str) = strdup("logicaland"); }
    break;

  case 116:
#line 556 "./swf5compiler.y"
    { (yyval.str) = strdup("logicalor"); }
    break;

  case 117:
#line 557 "./swf5compiler.y"
    { (yyval.str) = strdup("not"); }
    break;

  case 118:
#line 558 "./swf5compiler.y"
    { (yyval.str) = strdup("stringeq"); }
    break;

  case 119:
#line 559 "./swf5compiler.y"
    { (yyval.str) = strdup("stringlength"); }
    break;

  case 120:
#line 560 "./swf5compiler.y"
    { (yyval.str) = strdup("substring"); }
    break;

  case 121:
#line 561 "./swf5compiler.y"
    { (yyval.str) = strdup("getvariable"); }
    break;

  case 122:
#line 562 "./swf5compiler.y"
    { (yyval.str) = strdup("setvariable"); }
    break;

  case 123:
#line 563 "./swf5compiler.y"
    { (yyval.str) = strdup("settargetexpression"); }
    break;

  case 124:
#line 564 "./swf5compiler.y"
    { (yyval.str) = strdup("duplicateMovieClip"); }
    break;

  case 125:
#line 565 "./swf5compiler.y"
    { (yyval.str) = strdup("removeMovieClip"); }
    break;

  case 126:
#line 566 "./swf5compiler.y"
    { (yyval.str) = strdup("startDrag"); }
    break;

  case 127:
#line 567 "./swf5compiler.y"
    { (yyval.str) = strdup("stopDrag"); }
    break;

  case 128:
#line 568 "./swf5compiler.y"
    { (yyval.str) = strdup("stringlessthan"); }
    break;

  case 129:
#line 569 "./swf5compiler.y"
    { (yyval.str) = strdup("mblength"); }
    break;

  case 130:
#line 570 "./swf5compiler.y"
    { (yyval.str) = strdup("mbsubstring"); }
    break;

  case 131:
#line 571 "./swf5compiler.y"
    { (yyval.str) = strdup("mbord"); }
    break;

  case 132:
#line 572 "./swf5compiler.y"
    { (yyval.str) = strdup("mbchr"); }
    break;

  case 133:
#line 573 "./swf5compiler.y"
    { (yyval.str) = strdup("branchalways"); }
    break;

  case 134:
#line 574 "./swf5compiler.y"
    { (yyval.str) = strdup("branchiftrue"); }
    break;

  case 135:
#line 575 "./swf5compiler.y"
    { (yyval.str) = strdup("getURL2"); }
    break;

  case 136:
#line 576 "./swf5compiler.y"
    { (yyval.str) = strdup("post"); }
    break;

  case 137:
#line 577 "./swf5compiler.y"
    { (yyval.str) = strdup("get"); }
    break;

  case 138:
#line 578 "./swf5compiler.y"
    { (yyval.str) = strdup("loadVariables"); }
    break;

  case 139:
#line 579 "./swf5compiler.y"
    { (yyval.str) = strdup("loadVariablesNum"); }
    break;

  case 140:
#line 580 "./swf5compiler.y"
    { (yyval.str) = strdup("loadMovie"); }
    break;

  case 141:
#line 581 "./swf5compiler.y"
    { (yyval.str) = strdup("loadMovieNum"); }
    break;

  case 142:
#line 582 "./swf5compiler.y"
    { (yyval.str) = strdup("gotoAndStop"); }
    break;

  case 143:
#line 583 "./swf5compiler.y"
    { (yyval.str) = strdup("gotoAndPlay"); }
    break;

  case 144:
#line 584 "./swf5compiler.y"
    { (yyval.str) = strdup("setTarget"); }
    break;

  case 145:
#line 585 "./swf5compiler.y"
    { (yyval.str) = strdup("call"); }
    break;

  case 146:
#line 586 "./swf5compiler.y"
    { (yyval.str) = strdup("getProperty"); }
    break;

  case 147:
#line 587 "./swf5compiler.y"
    { (yyval.str) = strdup("setProperty"); }
    break;

  case 148:
#line 588 "./swf5compiler.y"
    { (yyval.str) = strdup("cast"); }
    break;

  case 149:
#line 590 "./swf5compiler.y"
    { (yyval.str) = strdup("this"); }
    break;

  case 150:
#line 593 "./swf5compiler.y"
    { (yyval.str) = strdup("_x"); }
    break;

  case 151:
#line 594 "./swf5compiler.y"
    { (yyval.str) = strdup("_y"); }
    break;

  case 152:
#line 595 "./swf5compiler.y"
    { (yyval.str) = strdup("_xscale"); }
    break;

  case 153:
#line 596 "./swf5compiler.y"
    { (yyval.str) = strdup("_yscale"); }
    break;

  case 154:
#line 597 "./swf5compiler.y"
    { (yyval.str) = strdup("_currentframe"); }
    break;

  case 155:
#line 598 "./swf5compiler.y"
    { (yyval.str) = strdup("_totalframes"); }
    break;

  case 156:
#line 599 "./swf5compiler.y"
    { (yyval.str) = strdup("_alpha"); }
    break;

  case 157:
#line 600 "./swf5compiler.y"
    { (yyval.str) = strdup("_visible"); }
    break;

  case 158:
#line 601 "./swf5compiler.y"
    { (yyval.str) = strdup("_width"); }
    break;

  case 159:
#line 602 "./swf5compiler.y"
    { (yyval.str) = strdup("_height"); }
    break;

  case 160:
#line 603 "./swf5compiler.y"
    { (yyval.str) = strdup("_rotation"); }
    break;

  case 161:
#line 604 "./swf5compiler.y"
    { (yyval.str) = strdup("_target"); }
    break;

  case 162:
#line 605 "./swf5compiler.y"
    { (yyval.str) = strdup("_framesloaded"); }
    break;

  case 163:
#line 606 "./swf5compiler.y"
    { (yyval.str) = strdup("_name"); }
    break;

  case 164:
#line 607 "./swf5compiler.y"
    { (yyval.str) = strdup("_droptarget"); }
    break;

  case 165:
#line 608 "./swf5compiler.y"
    { (yyval.str) = strdup("_url"); }
    break;

  case 166:
#line 609 "./swf5compiler.y"
    { (yyval.str) = strdup("_highquality"); }
    break;

  case 167:
#line 610 "./swf5compiler.y"
    { (yyval.str) = strdup("_focusrect"); }
    break;

  case 168:
#line 611 "./swf5compiler.y"
    { (yyval.str) = strdup("_soundbuftime"); }
    break;

  case 169:
#line 612 "./swf5compiler.y"
    { (yyval.str) = strdup("_quality"); }
    break;

  case 170:
#line 613 "./swf5compiler.y"
    { (yyval.str) = strdup("_xmouse"); }
    break;

  case 171:
#line 614 "./swf5compiler.y"
    { (yyval.str) = strdup("_ymouse"); }
    break;

  case 172:
#line 619 "./swf5compiler.y"
    { (yyval.exprlist).buffer = newBuffer();
		  (yyval.exprlist).count = 0; }
    break;

  case 173:
#line 623 "./swf5compiler.y"
    { (yyval.exprlist).buffer = newBuffer();
		  bufferWriteHardString((yyval.exprlist).buffer, (yyvsp[(1) - (2)].str), strlen((yyvsp[(1) - (2)].str))+1);
		  (yyval.exprlist).count = 1;
		  free((yyvsp[(1) - (2)].str)); }
    break;

  case 174:
#line 629 "./swf5compiler.y"
    { (yyval.exprlist) = (yyvsp[(1) - (4)].exprlist);
		  bufferWriteHardString((yyval.exprlist).buffer, (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  ++(yyval.exprlist).count;
		  free((yyvsp[(3) - (4)].str)); }
    break;

  case 175:
#line 636 "./swf5compiler.y"
    { addctx(CTX_FUNCTION); (yyval.str) = NULL; }
    break;

  case 176:
#line 637 "./swf5compiler.y"
    { addctx(CTX_FUNCTION); (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 177:
#line 642 "./swf5compiler.y"
    {
		(yyval.function) = newASFunction();
		(yyval.function)->name = (yyvsp[(2) - (7)].str);
		(yyval.function)->params = (yyvsp[(4) - (7)].exprlist);
		(yyval.function)->code = (yyvsp[(7) - (7)].action);	
		delctx(CTX_FUNCTION);	
	}
    break;

  case 178:
#line 653 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action); }
    break;

  case 179:
#line 658 "./swf5compiler.y"
    { if((yyvsp[(1) - (1)].lval).obj)
		  {
		    (yyval.action) = (yyvsp[(1) - (1)].lval).obj;
		    (yyval.action)->hasObject = 1;
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
		}
    break;

  case 182:
#line 681 "./swf5compiler.y"
    { if((yyvsp[(1) - (1)].lval).obj)
		  {
		    (yyval.action) = (yyvsp[(1) - (1)].lval).obj;
		    (yyval.action)->hasObject = 1; 
		    if((yyvsp[(1) - (1)].lval).ident)
		      bufferConcat((yyval.action), (yyvsp[(1) - (1)].lval).ident);
		    else
		      bufferConcat((yyval.action), (yyvsp[(1) - (1)].lval).memexpr);
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (1)].lval).ident;
		  }
		}
    break;

  case 186:
#line 698 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); }
    break;

  case 187:
#line 703 "./swf5compiler.y"
    { addctx(CTX_LOOP); }
    break;

  case 188:
#line 708 "./swf5compiler.y"
    { addctx(CTX_LOOP); }
    break;

  case 189:
#line 713 "./swf5compiler.y"
    { addctx(CTX_LOOP); }
    break;

  case 190:
#line 718 "./swf5compiler.y"
    { addctx(CTX_FOR_IN); }
    break;

  case 191:
#line 723 "./swf5compiler.y"
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
		  delctx(CTX_LOOP); }
    break;

  case 192:
#line 736 "./swf5compiler.y"
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
		  delctx(CTX_LOOP); }
    break;

  case 193:
#line 749 "./swf5compiler.y"
    {
		  int continue_len;
		  if((yyvsp[(3) - (10)].action))
		    (yyval.action) = (yyvsp[(3) - (10)].action);
		  else
		    (yyval.action) = newBuffer();

		  continue_len = bufferLength ((yyvsp[(7) - (10)].action));
		  if((yyvsp[(10) - (10)].action))
		    bufferConcatSimple((yyvsp[(10) - (10)].action), (yyvsp[(7) - (10)].action));
		  else if ((yyvsp[(7) - (10)].action))
		    (yyvsp[(10) - (10)].action) = (yyvsp[(7) - (10)].action);
		  else 
		    (yyvsp[(10) - (10)].action) = newBuffer();
		  if((yyvsp[(5) - (10)].action))
		  {
                    bufferWriteOp((yyvsp[(5) - (10)].action), SWFACTION_LOGICALNOT);
                    bufferWriteOp((yyvsp[(5) - (10)].action), SWFACTION_IF);
                    bufferWriteS16((yyvsp[(5) - (10)].action), 2);
                    bufferWriteS16((yyvsp[(5) - (10)].action), bufferLength((yyvsp[(10) - (10)].action))+5);
		    bufferConcat((yyvsp[(5) - (10)].action), (yyvsp[(10) - (10)].action));
                  }
		  else
		    (yyvsp[(5) - (10)].action) = (yyvsp[(10) - (10)].action);

                  bufferWriteOp((yyvsp[(5) - (10)].action), SWFACTION_JUMP);
                  bufferWriteS16((yyvsp[(5) - (10)].action), 2);
                  bufferWriteS16((yyvsp[(5) - (10)].action), -(bufferLength((yyvsp[(5) - (10)].action))+2));
		  /* need to jump to last part of for stmt in continue case */
		  if (continue_len)
		    bufferResolveJumpsFull((yyvsp[(5) - (10)].action), (yyvsp[(5) - (10)].action)->pos, (yyvsp[(5) - (10)].action)->pos - continue_len - 5);
		  else
		    bufferResolveJumps((yyvsp[(5) - (10)].action));

                  bufferConcat((yyval.action), (yyvsp[(5) - (10)].action));
		  delctx(CTX_LOOP);
                }
    break;

  case 194:
#line 788 "./swf5compiler.y"
    { Buffer b2, b3;
		  int tmp;

		  (yyval.action) = (yyvsp[(4) - (7)].action);
		  if((yyvsp[(4) - (7)].action)->hasObject)
			bufferWriteOp((yyval.action), SWFACTION_ENUMERATE2);
		  else
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
		  bufferWriteOp(b3, SWFACTION_JUMP);
		  bufferWriteS16(b3, 2);
		  bufferWriteS16(b3, -tmp);
		  bufferConcat(b2, b3);
		  bufferResolveJumps(b2);
		  bufferConcat((yyval.action), b2);
		  delctx(CTX_FOR_IN);
		  free((yyvsp[(3) - (7)].str)); }
    break;

  case 195:
#line 825 "./swf5compiler.y"
    { Buffer b2, b3;
		  int tmp;

		  (yyval.action) = (yyvsp[(5) - (8)].action);
		  if((yyvsp[(5) - (8)].action)->hasObject)
			bufferWriteOp((yyval.action), SWFACTION_ENUMERATE2);        
		  else
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
		  bufferWriteOp(b3, SWFACTION_JUMP);
		  bufferWriteS16(b3, 2);
		  bufferWriteS16(b3, -tmp);
		  bufferConcat(b2, b3);
		  bufferResolveJumps(b2);
		  bufferConcat((yyval.action), b2);
		  delctx(CTX_FOR_IN);
		  free((yyvsp[(4) - (8)].str)); }
    break;

  case 196:
#line 862 "./swf5compiler.y"
    { (yyval.action) = NULL; }
    break;

  case 198:
#line 870 "./swf5compiler.y"
    { 
		  if(chkctx(CTX_CONTINUE) < 0)
		  {
			swf5error("continue outside loop");
			YYABORT;
		  }
		  (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_JUMP);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), MAGIC_CONTINUE_NUMBER); }
    break;

  case 199:
#line 885 "./swf5compiler.y"
    { int context = chkctx(CTX_BREAK);
		  (yyval.action) = newBuffer();
		  if(context == CTX_FOR_IN || context == CTX_LOOP)
		  {
		  	if(context == CTX_FOR_IN)	/* break out of a for .. in */
				bufferWriteOp((yyval.action), SWFACTION_POP);
		  	bufferWriteOp((yyval.action), SWFACTION_JUMP);
		  	bufferWriteS16((yyval.action), 2);
		  	bufferWriteS16((yyval.action), MAGIC_BREAK_NUMBER); 
		  }
		  else if(context == CTX_SWITCH)
		  {
			addctx(CTX_BREAK);	
		  }
		  else
		  {
			swf5error("break outside switch / loop");
			YYABORT;
		  }
		}
    break;

  case 200:
#line 909 "./swf5compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_NOSEND; }
    break;

  case 201:
#line 911 "./swf5compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_GET; }
    break;

  case 202:
#line 913 "./swf5compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_POST; }
    break;

  case 203:
#line 915 "./swf5compiler.y"
    { if(strcasecmp((yyvsp[(2) - (2)].str), "GET") == 0)
				    (yyval.getURLMethod) = GETURL_METHOD_GET;
				  else if(strcasecmp((yyvsp[(2) - (2)].str), "POST") == 0)
				    (yyval.getURLMethod) = GETURL_METHOD_POST;
				  else (yyval.getURLMethod) = GETURL_METHOD_NOSEND;
				}
    break;

  case 204:
#line 925 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), "_level", 7);
		  bufferConcat((yyval.action), (yyvsp[(1) - (1)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STRINGCONCAT); }
    break;

  case 205:
#line 934 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_TRACE); }
    break;

  case 206:
#line 938 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("void_function_call: GETURL '(' expr ')'\n");
#endif
		  (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_PUSH);
		  bufferWriteS16((yyval.action), 2); bufferWriteS16((yyval.action), 0); // two 0 bytes	
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0); }
    break;

  case 207:
#line 950 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("void_function_call: GETURL '(' expr ',' expr urlmethod ')'\n");
#endif
		  (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), (yyvsp[(6) - (7)].getURLMethod)); }
    break;

  case 208:
#line 961 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0xc0+(yyvsp[(6) - (7)].getURLMethod)); }
    break;

  case 209:
#line 968 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0x80+(yyvsp[(6) - (7)].getURLMethod)); }
    break;

  case 210:
#line 975 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0x40+(yyvsp[(6) - (7)].getURLMethod)); }
    break;

  case 211:
#line 982 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), (yyvsp[(6) - (7)].getURLMethod)); }
    break;

  case 212:
#line 989 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_CALLFRAME);
		  bufferWriteS16((yyval.action), 0); }
    break;

  case 213:
#line 995 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), "0", 2); /* no constraint */
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (6)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STARTDRAG); }
    break;

  case 214:
#line 1002 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferConcat((yyval.action), (yyvsp[(7) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(11) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(9) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(13) - (14)].action));
		  bufferWriteString((yyval.action), "1", 2); /* has constraint */
		  bufferConcat((yyval.action), (yyvsp[(5) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (14)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STARTDRAG); }
    break;

  case 215:
#line 1013 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_ENDDRAG); }
    break;

  case 216:
#line 1018 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (8)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(7) - (8)].action));
		  bufferWriteInt((yyval.action), 16384); /* magic number */
		  bufferWriteOp((yyval.action), SWFACTION_ADD);
		  bufferWriteOp((yyval.action), SWFACTION_DUPLICATECLIP); }
    break;

  case 217:
#line 1026 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_REMOVECLIP); }
    break;

  case 218:
#line 1030 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GETURL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (6)].str)) + strlen((yyvsp[(5) - (6)].str)) + 2);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (6)].str), strlen((yyvsp[(3) - (6)].str)));
		  bufferWriteU8((yyval.action), 0);
		  bufferWriteHardString((yyval.action), (yyvsp[(5) - (6)].str), strlen((yyvsp[(5) - (6)].str)));
		  bufferWriteU8((yyval.action), 0); }
    break;

  case 219:
#line 1040 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_NEXTFRAME); }
    break;

  case 220:
#line 1044 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_PREVFRAME); }
    break;

  case 221:
#line 1048 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_PLAY); }
    break;

  case 222:
#line 1052 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_STOP); }
    break;

  case 223:
#line 1056 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_STOPSOUNDS); }
    break;

  case 224:
#line 1060 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_TOGGLEQUALITY); }
    break;

  case 225:
#line 1064 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOFRAME);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), (yyvsp[(3) - (4)].intVal)-1); /* GOTOFRAME target is 0-based */
		  bufferWriteOp((yyval.action), SWFACTION_PLAY); }
    break;

  case 226:
#line 1071 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOFRAME);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), (yyvsp[(3) - (4)].intVal)-1);  /* GOTOFRAME target is 0-based */
		  bufferWriteOp((yyval.action), SWFACTION_STOP); }
    break;

  case 227:
#line 1078 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOLABEL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); 
		  bufferWriteOp((yyval.action), SWFACTION_PLAY); }
    break;

  case 228:
#line 1086 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOLABEL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); 
		  bufferWriteOp((yyval.action), SWFACTION_STOP); }
    break;

  case 229:
#line 1094 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_GOTOFRAME2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 1); }
    break;

  case 230:
#line 1100 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_GOTOFRAME2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0); }
    break;

  case 231:
#line 1106 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_SETTARGET);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); }
    break;

  case 232:
#line 1113 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_SETTARGET2); }
    break;

  case 233:
#line 1117 "./swf5compiler.y"
    {
			(yyval.action) = (yyvsp[(3) - (8)].action);
			bufferWriteFloat((yyval.action), (yyvsp[(5) - (8)].intVal));
			bufferConcat((yyval.action), (yyvsp[(7) - (8)].action));
			bufferWriteOp((yyval.action), SWFACTION_SETPROPERTY);	
		}
    break;

  case 234:
#line 1128 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("function_call: %s '(' expr_list ')'\n", (yyvsp[(1) - (4)].str));
#endif
		  (yyval.action) = (yyvsp[(3) - (4)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(3) - (4)].exprlist).count);
		  bufferWriteString((yyval.action), (yyvsp[(1) - (4)].str), strlen((yyvsp[(1) - (4)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_CALLFUNCTION);
		  free((yyvsp[(1) - (4)].str)); }
    break;

  case 235:
#line 1139 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str));
		  bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE); 
		  bufferWriteOp((yyval.action), SWFACTION_TARGETPATH); }
    break;

  case 236:
#line 1146 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE); }
    break;

  case 237:
#line 1150 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GETTIME); }
    break;

  case 238:
#line 1154 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_RANDOMNUMBER); }
    break;

  case 239:
#line 1158 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_STRINGLENGTH); }
    break;

  case 240:
#line 1162 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_INT); }
    break;

  case 241:
#line 1166 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_ORD); }
    break;

  case 242:
#line 1170 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_CHR); }
    break;

  case 243:
#line 1174 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (6)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STRINGCONCAT); }
    break;

  case 244:
#line 1179 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (8)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(7) - (8)].action));
		  bufferWriteOp((yyval.action), SWFACTION_SUBSTRING); }
    break;

  case 245:
#line 1185 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("function_call: TYPEOF '(' expr_or_obj ')'\n");
#endif
		  (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_TYPEOF); }
    break;

  case 246:
#line 1193 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferConcat((yyval.action), (yyvsp[(3) - (6)].action));
		  bufferWriteFloat((yyval.action), (yyvsp[(5) - (6)].intVal));
		  bufferWriteOp((yyval.action), SWFACTION_GETPROPERTY);
		}
    break;

  case 247:
#line 1203 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  if((yyvsp[(2) - (2)].action)->hasObject)
		    bufferWriteOp((yyval.action), SWFACTION_DELETE);
		  else 
		    bufferWriteOp((yyval.action), SWFACTION_DELETE2);
		}
    break;

  case 248:
#line 1214 "./swf5compiler.y"
    { (yyval.exprlist).buffer = newBuffer();
		  (yyval.exprlist).count = 0; }
    break;

  case 249:
#line 1218 "./swf5compiler.y"
    { (yyval.exprlist).buffer = (yyvsp[(1) - (1)].action);
		  (yyval.exprlist).count = 1; }
    break;

  case 250:
#line 1223 "./swf5compiler.y"
    { Buffer tmp = newBuffer();
		  bufferConcat(tmp, (yyvsp[(3) - (3)].action));
		  bufferConcat(tmp, (yyval.exprlist).buffer);
		  (yyval.exprlist).buffer = tmp;
		  ++(yyval.exprlist).count;  }
    break;

  case 251:
#line 1232 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(5) - (6)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(5) - (6)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(1) - (6)].action));
		  bufferWriteString((yyval.action), (yyvsp[(3) - (6)].str), strlen((yyvsp[(3) - (6)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_CALLMETHOD);
		  free((yyvsp[(3) - (6)].str)); }
    break;

  case 252:
#line 1240 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(6) - (7)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(6) - (7)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(1) - (7)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_CALLMETHOD); }
    break;

  case 253:
#line 1249 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (3)].str), strlen((yyvsp[(1) - (3)].str))+1);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  free((yyvsp[(1) - (3)].str)); }
    break;

  case 254:
#line 1257 "./swf5compiler.y"
    { (yyval.exprlist).buffer = (yyvsp[(1) - (1)].action);
		  (yyval.exprlist).count = 1; }
    break;

  case 255:
#line 1261 "./swf5compiler.y"
    { bufferConcat((yyval.exprlist).buffer, (yyvsp[(3) - (3)].action));
		  ++(yyval.exprlist).count;  }
    break;

  case 256:
#line 1266 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_ADD2; }
    break;

  case 257:
#line 1267 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_SUBTRACT; }
    break;

  case 258:
#line 1268 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_MULTIPLY; }
    break;

  case 259:
#line 1269 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_DIVIDE; }
    break;

  case 260:
#line 1270 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_MODULO; }
    break;

  case 261:
#line 1271 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_BITWISEAND; }
    break;

  case 262:
#line 1272 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_BITWISEOR; }
    break;

  case 263:
#line 1273 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_BITWISEXOR; }
    break;

  case 264:
#line 1274 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_SHIFTLEFT; }
    break;

  case 265:
#line 1275 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_SHIFTRIGHT; }
    break;

  case 266:
#line 1276 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_SHIFTRIGHT2; }
    break;

  case 267:
#line 1280 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_INCREMENT; }
    break;

  case 268:
#line 1281 "./swf5compiler.y"
    { (yyval.op) = SWFACTION_DECREMENT; }
    break;

  case 269:
#line 1288 "./swf5compiler.y"
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
		}
    break;

  case 272:
#line 1311 "./swf5compiler.y"
    { (yyval.lval).ident = newBuffer();
		  bufferWriteString((yyval.lval).ident, (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str));
		  (yyval.lval).obj = 0;
		  (yyval.lval).memexpr = 0; }
    break;

  case 273:
#line 1318 "./swf5compiler.y"
    { (yyval.lval).obj = (yyvsp[(1) - (3)].action);
		  (yyval.lval).ident = newBuffer();
		  bufferWriteString((yyval.lval).ident, (yyvsp[(3) - (3)].str), strlen((yyvsp[(3) - (3)].str))+1);
		  free((yyvsp[(3) - (3)].str));
		  (yyval.lval).memexpr = 0; }
    break;

  case 274:
#line 1325 "./swf5compiler.y"
    { (yyval.lval).obj = (yyvsp[(1) - (4)].action);
		  (yyval.lval).memexpr = (yyvsp[(3) - (4)].action);
		  (yyval.lval).ident = 0; }
    break;

  case 277:
#line 1338 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteInt((yyvsp[(2) - (2)].action), -1);
		  bufferWriteOp((yyvsp[(2) - (2)].action), SWFACTION_MULTIPLY); }
    break;

  case 278:
#line 1343 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteInt((yyvsp[(2) - (2)].action), 0xffffffff);
		  bufferWriteOp((yyvsp[(2) - (2)].action), SWFACTION_BITWISEXOR); }
    break;

  case 279:
#line 1348 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteOp((yyvsp[(2) - (2)].action), SWFACTION_LOGICALNOT); }
    break;

  case 280:
#line 1352 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT);
		  bufferWriteOp((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(3) - (3)].action))+1);
		  bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferConcatSimple((yyval.action), (yyvsp[(3) - (3)].action)); }
    break;

  case 281:
#line 1362 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferWriteOp((yyval.action), SWFACTION_PUSHDUP);
		  bufferWriteOp((yyval.action), SWFACTION_IF);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(3) - (3)].action))+1);
		  bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferConcatSimple((yyval.action), (yyvsp[(3) - (3)].action)); }
    break;

  case 282:
#line 1371 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_MULTIPLY); }
    break;

  case 283:
#line 1376 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_DIVIDE); }
    break;

  case 284:
#line 1381 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_MODULO); }
    break;

  case 285:
#line 1386 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_ADD2); }
    break;

  case 286:
#line 1391 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_SUBTRACT); }
    break;

  case 287:
#line 1396 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BITWISEAND); }
    break;

  case 288:
#line 1401 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BITWISEOR); }
    break;

  case 289:
#line 1406 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BITWISEXOR); }
    break;

  case 290:
#line 1411 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_LESS2); }
    break;

  case 291:
#line 1416 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(1) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_LESS2); }
    break;

  case 292:
#line 1421 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(1) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_LESS2);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT); }
    break;

  case 293:
#line 1427 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LESS2);
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); }
    break;

  case 294:
#line 1432 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_EQUALS2); }
    break;

  case 295:
#line 1436 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_STRICTEQUALS); }
    break;

  case 296:
#line 1440 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_EQUALS2);
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); }
    break;

  case 297:
#line 1445 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_STRICTEQUALS); 
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); }
    break;

  case 298:
#line 1450 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_SHIFTLEFT); }
    break;

  case 299:
#line 1454 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_SHIFTRIGHT); }
    break;

  case 300:
#line 1458 "./swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_SHIFTRIGHT2); }
    break;

  case 301:
#line 1462 "./swf5compiler.y"
    { bufferWriteOp((yyvsp[(1) - (5)].action), SWFACTION_IF);
		  bufferWriteS16((yyvsp[(1) - (5)].action), 2);
		  bufferWriteS16((yyvsp[(1) - (5)].action), bufferLength((yyvsp[(5) - (5)].action))+5);
		  bufferConcatSimple((yyvsp[(1) - (5)].action), (yyvsp[(5) - (5)].action));
		  bufferWriteOp((yyvsp[(1) - (5)].action), SWFACTION_JUMP);
		  bufferWriteS16((yyvsp[(1) - (5)].action), 2);
		  bufferWriteS16((yyvsp[(1) - (5)].action), bufferLength((yyvsp[(3) - (5)].action)));
		  bufferConcatSimple((yyvsp[(1) - (5)].action), (yyvsp[(3) - (5)].action)); }
    break;

  case 302:
#line 1472 "./swf5compiler.y"
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
		}
    break;

  case 303:
#line 1499 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_INSTANCEOF); }
    break;

  case 305:
#line 1509 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW %s\n", (yyvsp[(2) - (2)].str));
#endif
		  (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferWriteString((yyval.action), (yyvsp[(2) - (2)].str), strlen((yyvsp[(2) - (2)].str))+1);
		  free((yyvsp[(2) - (2)].str));
		  bufferWriteOp((yyval.action), SWFACTION_NEWOBJECT); }
    break;

  case 306:
#line 1520 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW %s '(' expr_list ')'\n", (yyvsp[(2) - (5)].str));
#endif
		  (yyval.action) = (yyvsp[(4) - (5)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(4) - (5)].exprlist).count);
		  bufferWriteString((yyval.action), (yyvsp[(2) - (5)].str), strlen((yyvsp[(2) - (5)].str))+1);
		  free((yyvsp[(2) - (5)].str));
		  bufferWriteOp((yyval.action), SWFACTION_NEWOBJECT); }
    break;

  case 307:
#line 1531 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW lvalue_expr '.' %s\n", (yyvsp[(4) - (4)].str));
#endif
		  (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferConcat((yyval.action), (yyvsp[(2) - (4)].action));
		  bufferWriteString((yyval.action), (yyvsp[(4) - (4)].str), strlen((yyvsp[(4) - (4)].str))+1);
		  free((yyvsp[(4) - (4)].str));
		  bufferWriteOp((yyval.action), SWFACTION_NEWMETHOD); }
    break;

  case 308:
#line 1543 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW lvalue_expr '[' expr ']'\n");
#endif
		  (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferConcat((yyval.action), (yyvsp[(2) - (5)].action));
		  bufferConcat((yyval.action), (yyvsp[(4) - (5)].action));
		  bufferWriteOp((yyval.action), SWFACTION_NEWMETHOD); }
    break;

  case 309:
#line 1555 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW lvalue_expr '.' %s '(' expr_list ')'\n", (yyvsp[(4) - (7)].str));
#endif
		  (yyval.action) = (yyvsp[(6) - (7)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(6) - (7)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(2) - (7)].action));
		  bufferWriteString((yyval.action), (yyvsp[(4) - (7)].str), strlen((yyvsp[(4) - (7)].str))+1);
		  free((yyvsp[(4) - (7)].str));
		  bufferWriteOp((yyval.action), SWFACTION_NEWMETHOD); }
    break;

  case 310:
#line 1567 "./swf5compiler.y"
    {
#ifdef DEBUG
		  printf("NEW lvalue_expr '[' expr ']' '(' expr_list ')'\n");
#endif
		  (yyval.action) = (yyvsp[(7) - (8)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(7) - (8)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(2) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(4) - (8)].action));
		  bufferWriteOp((yyval.action), SWFACTION_NEWMETHOD); }
    break;

  case 311:
#line 1578 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(2) - (3)].exprlist).count);
		  bufferWriteOp((yyval.action), SWFACTION_INITARRAY); }
    break;

  case 312:
#line 1583 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferWriteOp((yyval.action), SWFACTION_INITOBJECT); }
    break;

  case 313:
#line 1588 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(2) - (3)].exprlist).count);
		  bufferWriteOp((yyval.action), SWFACTION_INITOBJECT); }
    break;

  case 314:
#line 1596 "./swf5compiler.y"
    {
			if((yyvsp[(1) - (1)].function)->name != NULL)
			{
				swf5error("anonymous decl only. identifier not allowed");
				YYABORT;
			}
			(yyval.action) = newBuffer();
			if(swfVersion > 6)
				bufferWriteFunction((yyval.action), (yyvsp[(1) - (1)].function), 2);
			else
				bufferWriteFunction((yyval.action), (yyvsp[(1) - (1)].function), 1);
		}
    break;

  case 317:
#line 1613 "./swf5compiler.y"
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
		}
    break;

  case 318:
#line 1658 "./swf5compiler.y"
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
		}
    break;

  case 319:
#line 1703 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); }
    break;

  case 320:
#line 1707 "./swf5compiler.y"
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
		}
    break;

  case 321:
#line 1756 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteBoolean((yyval.action), (yyvsp[(1) - (1)].intVal)); }
    break;

  case 322:
#line 1759 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteNull((yyval.action)); }
    break;

  case 323:
#line 1763 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteUndef((yyval.action)); }
    break;

  case 324:
#line 1767 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str)); }
    break;

  case 325:
#line 1771 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), (yyvsp[(1) - (1)].intVal)); }
    break;

  case 326:
#line 1774 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteDouble((yyval.action), (yyvsp[(1) - (1)].doubleVal)); }
    break;

  case 328:
#line 1782 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action)); }
    break;

  case 329:
#line 1788 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (4)].str), strlen((yyvsp[(1) - (4)].str))+1);
		  free((yyvsp[(1) - (4)].str));
		  bufferConcat((yyval.action), (yyvsp[(4) - (4)].action));
		  bufferWriteOp((yyval.action), SWFACTION_DEFINELOCAL); }
    break;

  case 330:
#line 1795 "./swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (2)].str), strlen((yyvsp[(1) - (2)].str))+1);
		  free((yyvsp[(1) - (2)].str));
		  bufferWriteOp((yyval.action), SWFACTION_DEFINELOCAL2); }
    break;

  case 331:
#line 1803 "./swf5compiler.y"
    { asmBuffer = newBuffer(); }
    break;

  case 332:
#line 1805 "./swf5compiler.y"
    { (yyval.action) = asmBuffer; }
    break;

  case 333:
#line 1808 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action); }
    break;

  case 335:
#line 1813 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action);
		  bufferWriteOp((yyval.action), SWFACTION_POP); }
    break;

  case 336:
#line 1817 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action);
		  bufferWriteOp((yyval.action), SWFACTION_POP); }
    break;

  case 337:
#line 1821 "./swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action);
		  bufferWriteOp((yyval.action), SWFACTION_POP); }
    break;

  case 338:
#line 1825 "./swf5compiler.y"
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
		}
    break;

  case 339:
#line 1864 "./swf5compiler.y"
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
		}
    break;

  case 340:
#line 1903 "./swf5compiler.y"
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
		}
    break;

  case 341:
#line 1924 "./swf5compiler.y"
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
		}
    break;

  case 343:
#line 1969 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(1) - (2)].len) + (yyvsp[(2) - (2)].len); }
    break;

  case 344:
#line 1974 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_WITH); }
    break;

  case 345:
#line 1976 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(2) - (4)].len) + (yyvsp[(3) - (4)].len);
				  bufferPatchLength(asmBuffer, (yyvsp[(3) - (4)].len)); }
    break;

  case 346:
#line 1981 "./swf5compiler.y"
    { (yyval.len) = bufferWriteConstantString(asmBuffer, (yyvsp[(1) - (1)].str),
								 strlen((yyvsp[(1) - (1)].str))+1); }
    break;

  case 347:
#line 1984 "./swf5compiler.y"
    { (yyval.len) = bufferWriteInt(asmBuffer, (yyvsp[(1) - (1)].intVal)); }
    break;

  case 348:
#line 1986 "./swf5compiler.y"
    { (yyval.len) = bufferWriteDouble(asmBuffer, (yyvsp[(1) - (1)].doubleVal)); }
    break;

  case 349:
#line 1988 "./swf5compiler.y"
    { bufferWriteU8(asmBuffer, PUSH_BOOLEAN);
				  (yyval.len) = bufferWriteU8(asmBuffer, (yyvsp[(1) - (1)].intVal))+1;
				  bufferPatchPushLength(asmBuffer, 2); }
    break;

  case 350:
#line 1992 "./swf5compiler.y"
    { (yyval.len) = bufferWriteU8(asmBuffer, PUSH_NULL);
				  bufferPatchPushLength(asmBuffer, 1); }
    break;

  case 351:
#line 1995 "./swf5compiler.y"
    { (yyval.len) = bufferWriteU8(asmBuffer, PUSH_UNDEF);
				  bufferPatchPushLength(asmBuffer, 1); }
    break;

  case 352:
#line 1998 "./swf5compiler.y"
    { bufferWriteU8(asmBuffer, PUSH_REGISTER);
				  (yyval.len) = bufferWriteU8(asmBuffer,
						     (char)atoi((yyvsp[(1) - (1)].str)))+1;
				  bufferPatchPushLength(asmBuffer, 2); }
    break;

  case 353:
#line 2006 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(1) - (1)].len); }
    break;

  case 354:
#line 2007 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(1) - (3)].len) + (yyvsp[(3) - (3)].len); }
    break;

  case 355:
#line 2011 "./swf5compiler.y"
    { (yyval.len) = bufferWritePushOp(asmBuffer);
				  (yyval.len) += bufferWriteS16(asmBuffer, 0); }
    break;

  case 356:
#line 2013 "./swf5compiler.y"
    { (yyval.len) = (yyvsp[(2) - (3)].len) + (yyvsp[(3) - (3)].len);
				  bufferPatchLength(asmBuffer, (yyvsp[(3) - (3)].len)); }
    break;

  case 358:
#line 2019 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_STOREREGISTER);
				  (yyval.len) += bufferWriteS16(asmBuffer, 1);
				  (yyval.len) += bufferWriteU8(asmBuffer,
						      (char)atoi((yyvsp[(2) - (2)].str))); }
    break;

  case 359:
#line 2025 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_CALLFUNCTION); }
    break;

  case 360:
#line 2027 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_RETURN); }
    break;

  case 361:
#line 2029 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_CALLMETHOD); }
    break;

  case 362:
#line 2031 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_NEWOBJECT); }
    break;

  case 363:
#line 2033 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_NEWMETHOD); }
    break;

  case 364:
#line 2035 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_BITWISEAND); }
    break;

  case 365:
#line 2037 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_BITWISEOR); }
    break;

  case 366:
#line 2039 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_BITWISEXOR); }
    break;

  case 367:
#line 2041 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_MODULO); }
    break;

  case 368:
#line 2043 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_ADD2); }
    break;

  case 369:
#line 2045 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_LESS2); }
    break;

  case 370:
#line 2047 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_EQUALS2); }
    break;

  case 371:
#line 2049 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INCREMENT); }
    break;

  case 372:
#line 2051 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_DECREMENT); }
    break;

  case 373:
#line 2053 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_TYPEOF); }
    break;

  case 374:
#line 2055 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INSTANCEOF); }
    break;

  case 375:
#line 2057 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_ENUMERATE); }
    break;

  case 376:
#line 2059 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_ENUMERATE2); }
    break;

  case 377:
#line 2061 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_DELETE); }
    break;

  case 378:
#line 2063 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_DELETE2); }
    break;

  case 379:
#line 2065 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_NEWOBJECT); }
    break;

  case 380:
#line 2067 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INITARRAY); }
    break;

  case 381:
#line 2069 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INITOBJECT); }
    break;

  case 382:
#line 2071 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_GETMEMBER); }
    break;

  case 383:
#line 2073 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SETMEMBER); }
    break;

  case 384:
#line 2075 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SHIFTLEFT); }
    break;

  case 385:
#line 2077 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SHIFTRIGHT); }
    break;

  case 386:
#line 2079 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SHIFTRIGHT2); }
    break;

  case 387:
#line 2081 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_DEFINELOCAL2); }
    break;

  case 388:
#line 2083 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_EXTENDS); }
    break;

  case 389:
#line 2085 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_TARGETPATH); }
    break;

  case 390:
#line 2087 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_IMPLEMENTSOP); }
    break;

  case 391:
#line 2089 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_FSCOMMAND2); }
    break;

  case 392:
#line 2091 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_CASTOP);	}
    break;

  case 393:
#line 2095 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_ADD); }
    break;

  case 394:
#line 2096 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SUBTRACT); }
    break;

  case 395:
#line 2097 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MULTIPLY); }
    break;

  case 396:
#line 2098 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_DIVIDE); }
    break;

  case 397:
#line 2099 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_EQUAL); }
    break;

  case 398:
#line 2100 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LESSTHAN); }
    break;

  case 399:
#line 2101 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALAND); }
    break;

  case 400:
#line 2102 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALOR); }
    break;

  case 401:
#line 2103 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALNOT); }
    break;

  case 402:
#line 2104 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGEQ); }
    break;

  case 403:
#line 2105 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGLENGTH); }
    break;

  case 404:
#line 2106 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SUBSTRING); }
    break;

  case 405:
#line 2107 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_INT); }
    break;

  case 406:
#line 2108 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_PUSHDUP); }
    break;

  case 407:
#line 2109 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STACKSWAP); }
    break;

  case 408:
#line 2110 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_POP); }
    break;

  case 409:
#line 2111 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETVARIABLE); }
    break;

  case 410:
#line 2112 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETVARIABLE); }
    break;

  case 411:
#line 2113 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETPROPERTY); }
    break;

  case 412:
#line 2114 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETPROPERTY); }
    break;

  case 413:
#line 2115 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_TONUMBER); }
    break;

  case 414:
#line 2116 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_TOSTRING); }
    break;

  case 415:
#line 2117 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETTARGET2); }
    break;

  case 416:
#line 2118 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGCONCAT); }
    break;

  case 417:
#line 2119 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_DUPLICATECLIP); }
    break;

  case 418:
#line 2120 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_REMOVECLIP); }
    break;

  case 419:
#line 2121 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_TRACE); }
    break;

  case 420:
#line 2122 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGCOMPARE); }
    break;

  case 421:
#line 2123 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_RANDOMNUMBER); }
    break;

  case 422:
#line 2124 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBLENGTH); }
    break;

  case 423:
#line 2125 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_ORD); }
    break;

  case 424:
#line 2126 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_CHR); }
    break;

  case 425:
#line 2127 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETTIME); }
    break;

  case 426:
#line 2128 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBSUBSTRING); }
    break;

  case 427:
#line 2129 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBORD); }
    break;

  case 428:
#line 2130 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBCHR); }
    break;

  case 429:
#line 2133 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_JUMP);
				  (yyval.len) += bufferWriteS16(asmBuffer, 2);
				  (yyval.len) += bufferBranchTarget(asmBuffer, (yyvsp[(2) - (2)].str)); }
    break;

  case 430:
#line 2137 "./swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_IF);
				  (yyval.len) += bufferWriteS16(asmBuffer, 2);
				  (yyval.len) += bufferBranchTarget(asmBuffer, (yyvsp[(2) - (2)].str)); }
    break;

  case 431:
#line 2144 "./swf5compiler.y"
    { (yyval.intVal) = lookupProperty((yyvsp[(1) - (1)].str)); }
    break;

  case 432:
#line 2145 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_X; }
    break;

  case 433:
#line 2146 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_Y; }
    break;

  case 434:
#line 2147 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_XSCALE; }
    break;

  case 435:
#line 2148 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_YSCALE; }
    break;

  case 436:
#line 2149 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_CURRENTFRAME; }
    break;

  case 437:
#line 2150 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_TOTALFRAMES; }
    break;

  case 438:
#line 2151 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_ALPHA; }
    break;

  case 439:
#line 2152 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_VISIBLE; }
    break;

  case 440:
#line 2153 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_WIDTH; }
    break;

  case 441:
#line 2154 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_HEIGHT; }
    break;

  case 442:
#line 2155 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_ROTATION; }
    break;

  case 443:
#line 2156 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_TARGET; }
    break;

  case 444:
#line 2157 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_FRAMESLOADED; }
    break;

  case 445:
#line 2158 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_NAME; }
    break;

  case 446:
#line 2159 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_DROPTARGET; }
    break;

  case 447:
#line 2160 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_URL; }
    break;

  case 448:
#line 2161 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_HIGHQUALITY; }
    break;

  case 449:
#line 2162 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_FOCUSRECT; }
    break;

  case 450:
#line 2163 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_SOUNDBUFTIME; }
    break;

  case 451:
#line 2164 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_QUALITY; }
    break;

  case 452:
#line 2165 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_XMOUSE; }
    break;

  case 453:
#line 2166 "./swf5compiler.y"
    { (yyval.intVal) = PROPERTY_YMOUSE; }
    break;


/* Line 1267 of yacc.c.  */
#line 6736 "swf5compiler.tab.c"
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


#line 2168 "./swf5compiler.y"



