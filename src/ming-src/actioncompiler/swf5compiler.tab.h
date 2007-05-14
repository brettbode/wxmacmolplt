/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
#define LESSTHAN 308
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
/* Line 1489 of yacc.c.  */
#line 365 "swf5compiler.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE swf5lval;

