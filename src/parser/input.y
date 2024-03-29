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
#include "context.h"

}
%code provides {
typedef struct node {
	void *yyscanner;
} node;

}

%{

#include <stdio.h>
#include <stdlib.h>

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
	pls_date date;
	struct {
		const char *begin;
		const char *end;
	} identifier;
	const char *report_name;
}

// numbers
%token<integer> T_INTEGER "integer"

// date
%token<date> T_DATE "date"

// identifier
%token<identifier> T_IDENTIFIER "identifier"

%token YYEOF 0 "end of file"

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
%type<identifier> identifier print_report;

%start line

%%

integer: T_INTEGER { $$ = $1; };
date: T_DATE { $$ = $1; };
identifier: T_IDENTIFIER { $$ = $1; };

line: 
	  T_NEWLINE { YYACCEPT; }
	|	YYEOF { YYABORT; }
  | operations T_NEWLINE { YYACCEPT; }
	| operations YYEOF { YYABORT; }
;

operations: 
	| add_period
	| add_order
	| add_batch
	| run_pls
	| exit_pls
;

add_period:
	ADD_PERIOD date[start] date[end] {
		pls_period p = {.start = $start, .end = $end};
		pls_context_add_period(yyget_extra(yyscanner), p);
	}
;

add_order:
	ADD_ORDER identifier[order] date[due] integer[quantity] identifier[name] {
		pls_order o = {.due = $due, .quantity = $quantity};
		memcpy(o.order_no, $order.begin, (size_t)($order.end - $order.begin));
		memcpy(o.name, $name.begin, (size_t)($name.end - $name.begin));
		pls_context_add_order(yyget_extra(yyscanner), o);
	}
;

add_batch:
	ADD_BATCH identifier[file] {
		char buf[1024] = {};
		memcpy(buf, $file.begin, (size_t)($file.end - $file.begin));
		pls_context_add_batch(yyget_extra(yyscanner), buf);
	}
;

run_pls:
	RUN_PLS identifier[algo] T_PIPE print_report[iden] {
		char buf[1024] = {};
		memcpy(buf, $iden.begin, (size_t)($iden.end - $iden.begin));
		pls_context_run_pls(yyget_extra(yyscanner), $algo.begin, buf);
	}
;

print_report:
	PRINT_REPORT T_GT identifier[iden] {
		$$ = $iden;
	}
;

exit_pls:
	EXIT_PLS {
		pls_context_exit_pls(yyget_extra(yyscanner));
	}
;

%%

void yyerror(YYLTYPE* yylloc, void* yyscanner, char const* msg) {
  printf("parse error: %s\n", msg);
  // printf("%i %i\n", yylloc->first_line, yylloc->first_column);
  // fflush(stdout);
}
