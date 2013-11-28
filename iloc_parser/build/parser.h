/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
     add = 0,
     sub = 1,
     mult = 2,
     _div = 3,
     inv = 4,
     addI = 5,
     subI = 6,
     multI = 7,
     divI = 8,
     rdivI = 9,
     and = 10,
     andI = 11,
     or = 12,
     orI = 13,
     xor = 14,
     xorI = 15,
     lshift = 16,
     rshift = 17,
     lshiftI = 18,
     rshifI = 19,
     load = 20,
     loadAI = 21,
     loadA0 = 22,
     loadI = 23,
     cload = 24,
     cloadAI = 25,
     cloadA0 = 26,
     store = 27,
     storeAI = 28,
     storeA0 = 29,
     cstore = 30,
     cstoreAI = 31,
     cstoreA0 = 32,
     i2i = 33,
     c2c = 34,
     c2i = 35,
     i2c = 36,
     cmp_LT = 37,
     cmp_LE = 38,
     cmp_EQ = 39,
     cmp_GE = 40,
     cmp_GT = 41,
     cmp_NE = 42,
     cbr = 43,
     jump = 44,
     jumpI = 45,
     nop = 46,
     tbl = 47,
     r = 48,
     l = 49,
     comma = 50,
     to = 51
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE ilocparserlval;


