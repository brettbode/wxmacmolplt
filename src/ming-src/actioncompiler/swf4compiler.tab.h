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
/* Line 1489 of yacc.c.  */
#line 277 "swf4compiler.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE swf4lval;

