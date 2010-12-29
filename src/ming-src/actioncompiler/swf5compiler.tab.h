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
/* Line 1489 of yacc.c.  */
#line 431 "swf5compiler.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE swf5lval;

