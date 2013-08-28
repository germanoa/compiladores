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

%start program

%%

/*
GRAMMAR RULES
http://www.gnu.org/software/bison/manual/bison.html#Rules
*/

/* 2 */
program:
	  program global.decl ';'
	| program func
	| /* empty */
	;

/* 2.1 */
global.decl:
	  decl
	| array.decl
	;

array.decl:
	  decl '[' TK_LIT_INT ']'  //{ $1 $2[$3]; }
	;

decl:
      type ':' TK_IDENTIFICADOR //{ $1 $2; }
    ;

/*
type.and.id:
	  type ':' TK_IDENTIFICADOR
	;
*/

type:
	  TK_PR_INT
	| TK_PR_FLOAT
	| TK_PR_BOOL
	| TK_PR_CHAR
	| TK_PR_STRING
	;

/* 2.2 */
func:
	  type ':' TK_IDENTIFICADOR '(' func.param.list ')' decl.list command.block
	;

func.param.list:
	  param.list
	| /* empty */
	;

param.list:
	  type ':' TK_IDENTIFICADOR ',' param.list
	| type ':' TK_IDENTIFICADOR
	;

decl.list: //pode ser vazia?
	  decl ';' decl.list
	| decl ';'
	;

/* 2.3 */
command.block:
	  '{' command.seq '}'
	;

command.seq:
	  command ';' command.seq
	| command
	| /* empty */
	;

/* 2.4 */
command:
	  command.block
	| 
	;

%%
