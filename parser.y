/*
PROLOGUE
http://www.gnu.org/software/bison/manual/bison.html#Prologue
*/

%{
#include <stdio.h>
#include "comp_grammar.h" /* symbol_table is there.*/
%}

/*
DECLARATIONS
*/

/* Declaração dos tokens da gramática da Linguagem K */
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%%

/*
GRAMMAR RULES
http://www.gnu.org/software/bison/manual/bison.html#Rules
*/


/* declaracao de variaveis 
decl:
      TK_PR_INT ':' TK_IDENTIFICADOR  { $1 $2; }
    | TK_PR_FLOAT ':' TK_IDENTIFICADOR  { $1 $2; }
    | TK_PR_BOOL ':' TK_IDENTIFICADOR  { $1 $2; }
    | TK_PR_CHAR ':' TK_IDENTIFICADOR  { $1 $2; }
    | TK_PR_STRING ':' TK_IDENTIFICADOR  { $1 $2; }
    | TK_PR_INT ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']'{ $1 $2[$3]; }
    | TK_PR_FLOAT ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']'{ $1 $2[$3]; }
    | TK_PR_BOOL ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']'{ $1 $2[$3]; }
    | TK_PR_CHAR ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']'{ $1 $2[$3]; }
    | TK_PR_STRING ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']'{ $1 $2[$3]; }
*/
s:

%%

