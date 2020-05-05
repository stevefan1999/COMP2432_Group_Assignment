%require "3.0"
%locations
%defines
%no-lines
%verbose

// definitions
%define api.pure true

%define parse.trace true
%define parse.error verbose

%param {void *yyscanner}
%code {}
%code requires {
#define YY_NO_UNISTD_H
}
%code provides {
typedef struct node {
	void *yyscanner;
} node;

}

%{

#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>

#define YYDEBUG 0
#include "input.parse.h"
#include "input.lex.h"

#if defined (alloca) || defined (_ALLOCA_H)
# define YYSTACK_ALLOC alloca
#elif defined (__GNUC__)
# define YYSTACK_ALLOC __builtin_alloca
#else
# undef YYSTACK_ALLOC
#endif

#define YYMALLOC(size) yyalloc(size, yyscanner)
#define YYFREE(ptr) yyfree(ptr, yyscanner)

/*
 * little cheat sheet:
 * yytext, yyleng, yyval
 */

#if YYDEBUG
int yydebug = 1;
#endif

void yyerror(YYLTYPE* yylloc, void* yyscanner, char const* msg);

%}

// syntax union
%union {
	int integer;
	struct {
		int year;
		int month;
		int day;
	} date;
	const char *identifier;
	struct node *node;
}

// numbers
%token<integer> T_INTEGER "integer"

// date
%token<date> T_DATE "date"

// identifier
%token<identifier> T_IDENTIFIER "identifier"

%token
	T_NEWLINE
	T_PIPE
	T_GT
	ADD_PERIOD
	ADD_ORDER
	ADD_BATCH
	RUN_PLS
	PRINT_REPORT
	EXIT_PLS

%type<integer> integer;
%type<date> date;
%type<identifier> identifier;

%type<node>
	add_period
	add_order
	add_batch
	run_pls
	print_report
;

%start line

%%

integer: T_INTEGER { $$ = $1; };
date: T_DATE { $$ = $1; };
identifier: T_IDENTIFIER { $$ = $1; };

line: 
	  T_NEWLINE { YYACCEPT; }
  | add_period T_NEWLINE { YYACCEPT; }
	| add_order T_NEWLINE { YYACCEPT; }
	| add_batch T_NEWLINE { YYACCEPT; }
	| run_pls T_NEWLINE { YYACCEPT; }
	| EXIT_PLS T_NEWLINE { YYACCEPT; }
;

add_period:
	ADD_PERIOD date date {
		// TODO
	}
;

add_order:
	ADD_ORDER identifier date integer identifier {
		// TODO
	}
;

add_batch:
	ADD_BATCH identifier {
		// TODO
	}
;

run_pls:
	RUN_PLS identifier T_PIPE print_report {
		// TODO
	}
;

print_report:
	PRINT_REPORT T_GT identifier {
		// TODO
	}
;

%%

void yyerror(YYLTYPE* yylloc, void* yyscanner, char const* msg) {
  printf("parse error: %s\n", msg);
  // printf("%i %i\n", yylloc->first_line, yylloc->first_column);
  fflush(stdout);
}
