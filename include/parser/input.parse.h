/* A Bison parser, made by GNU Bison 3.6.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_ROOT_VS_COMP2432_GROUP_ASSIGNMENT_E2B135E0_6779_43C0_AC64_7C9BFCEC07B9_OUT_BUILD_LINUX_GCC_DEBUG_INPUT_PARSE_H_INCLUDED
# define YY_YY_ROOT_VS_COMP2432_GROUP_ASSIGNMENT_E2B135E0_6779_43C0_AC64_7C9BFCEC07B9_OUT_BUILD_LINUX_GCC_DEBUG_INPUT_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */

#define YY_NO_UNISTD_H
#include "context.h"



/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_INTEGER = 258,               /* "integer"  */
    T_DATE = 259,                  /* "date"  */
    T_IDENTIFIER = 260,            /* "identifier"  */
    T_NEWLINE = 261,               /* T_NEWLINE  */
    T_PIPE = 262,                  /* T_PIPE  */
    T_GT = 263,                    /* T_GT  */
    ADD_PERIOD = 264,              /* ADD_PERIOD  */
    ADD_ORDER = 265,               /* ADD_ORDER  */
    ADD_BATCH = 266,               /* ADD_BATCH  */
    RUN_PLS = 267,                 /* RUN_PLS  */
    PRINT_REPORT = 268,            /* PRINT_REPORT  */
    EXIT_PLS = 269                 /* EXIT_PLS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

	int integer;
	pls_date date;
	struct {
		const char *begin;
		const char *end;
	} identifier;
	const char *report_name;


};
typedef union YYSTYPE YYSTYPE;
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



int yyparse (void *yyscanner);
/* "%code provides" blocks.  */

typedef struct node {
	void *yyscanner;
} node;



#endif /* !YY_YY_ROOT_VS_COMP2432_GROUP_ASSIGNMENT_E2B135E0_6779_43C0_AC64_7C9BFCEC07B9_OUT_BUILD_LINUX_GCC_DEBUG_INPUT_PARSE_H_INCLUDED  */
