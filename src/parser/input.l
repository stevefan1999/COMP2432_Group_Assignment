%option outfile="input.lex.c"
%option header-file="input.lex.h"
%option extra-type="void *"
%option noyywrap noyyalloc noyyfree noyyrealloc nodefault
%option bison-bridge bison-locations
%option warn 8bit reentrant ecs
%option yylineno
%option noline

%{
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.parse.h"

#define YY_USER_ACTION do { \
    yylloc->first_line = yylloc->last_line = yylineno; \
    yylloc->first_column = yycolumn; \
    yylloc->last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng; \
  } while (0);

/*
 * little cheat sheet:
 * yytext, yyleng, yyval, yylloc, yyscanner
 */

extern int isatty(int fd);
%}

INTEGER [0-9]+
IDENTIFIER [_a-zA-Z][\._a-zA-Z0-9]*
DATE [0-9]{4}-[0-9]{2}-[0-9]{2}

%%

"addPERIOD" return ADD_PERIOD;
"addORDER" return ADD_ORDER;
"addBATCH" return ADD_BATCH;
"runPLS" return RUN_PLS;
"printREPORT" return PRINT_REPORT;
"exitPLS" return EXIT_PLS;

{INTEGER} {
  yylval->integer = atoi(yytext);
  return T_INTEGER;
}

{IDENTIFIER} {
  size_t len = strlen(yytext);
  char *mem = memcpy(yyalloc(len + 1, yyscanner), yytext, len);
  mem[len] = '\0';
  yylval->identifier = mem;
  return T_IDENTIFIER;
}

{DATE} {
  int i = 0;
  for (const char *s = yytext; s != NULL; s = strstr(s + 1, "-")) {
    if (i == 0) { yylval->date.year = atoi(s); }
    else if (i == 1) { yylval->date.month = atoi(s + 1); }
    else if (i == 2) { yylval->date.day = atoi(s + 1); }
    i++;
  }
  return T_DATE;
}



\r?\n return T_NEWLINE;
[ |\t] {;}
'|' return T_PIPE;
'>' return T_GT;


%%

#if 1

// CONTROL ALL THE MEMORY
void *yyalloc(size_t bytes, void *scanner);

void *yyrealloc(void *ptr, size_t bytes, void *scanner);

void yyfree(void *ptr, void *scanner);

#endif