/*
PROLOGUE
http://www.gnu.org/software/bison/manual/bison.html#Prologue
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include "comp_grammar.h"
#include "comp_dict.h"
#include "comp_tree.h"
#include "iks_ast.h"
//#include "hash_table.h"

%}

/*
DECLARATIONS
*/

/* Declaração dos tokens da gramática da Linguagem K */
%token TK_PR_INT	256		
%token TK_PR_FLOAT	257
%token TK_PR_BOOL	258
%token TK_PR_CHAR	259
%token TK_PR_STRING	260
%token TK_PR_IF		261
%token TK_PR_THEN	262
%token TK_PR_ELSE	263
%token TK_PR_WHILE	264
%token TK_PR_DO		265
%token TK_PR_INPUT	267
%token TK_PR_OUTPUT	268
%token TK_PR_RETURN	269

%token TK_OC_LE		270
%token TK_OC_GE		271
%token TK_OC_EQ		272	
%token TK_OC_NE		273
%token TK_OC_AND	274
%token TK_OC_OR		275

%union {
    int lit_int;
    float lit_float;
    int lit_bool;
    char lit_char;
    char* lit_string;
    char* id;
    comp_grammar_symbol_t *symbol;
    comp_tree_t *nt;
}
%token<lit_int> TK_LIT_INT	280
%token<lit_float> TK_LIT_FLOAT	281
%token<lit_bool> TK_LIT_FALSE	282
%token<lit_bool> TK_LIT_TRUE	283
%token<lit_char> TK_LIT_CHAR	284	
%token<lit_string> TK_LIT_STRING	285
%token<id> TK_IDENTIFICADOR 286

%token TOKEN_ERRO	290

%start prog

/* declaracoes nao sao nodos da arvore? */
//%type<nt> prog
%type<nt> func
%type<nt> command.block 
%type<nt> command.seq 
%type<nt> command 
%type<nt> ctrl.flow 
%type<nt> output.list 
%type<nt> expr 
%type<nt> func.param.list
%type<nt> param.list 

%%

/*
GRAMMAR RULES
http://www.gnu.org/software/bison/manual/bison.html#Rules
*/

/* 2 */
prog:
	  prog global.decl
	| prog func
        {
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_PROGRAMA,NULL);
            //prog is root, so setting, not appending
            comp_tree_set_item(ast,(void*)v);
            iks_ast_append(ast,$func);
        }
	| /* empty */
	;

/* 2.1 */
global.decl:
	  decl ';'
	| array.decl ';'
	;

array.decl:
	  decl '[' TK_LIT_INT ']'
	;

decl:
      type ':' TK_IDENTIFICADOR  
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
	  type ':' TK_IDENTIFICADOR '(' func.param.decl.list ')' decl.list command.block
        {
            //comp_dict_t *d;
            //d = comp_dict_find(symbol_table,id);
            //if (d == NULL) {
            //    printf("putz\n");
            //}
            
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            //iks_ast_node_value_set(v,IKS_AST_FUNCAO,d->item->value);
            iks_ast_node_value_set(v,IKS_AST_FUNCAO,yylval.symbol);
           // if (comp_list_is_empty(ast->children)) {
           //     iks_ast_append_value(ast,v);
           // }
           // else {
           //     iks_ast_append(ast->children->prev->item,v);
           // }
        }
	;

func.param.decl.list:
	  param.decl.list
	| /* empty */
	;

param.decl.list:
	  decl ',' param.decl.list
	| decl
	;

decl.list: // pode ser vazia?
	  decl ';' decl.list
    | /* empty */
	;

/* 2.3 */
command.block:
	  '{' command.seq '}'
        {
            $$ = NULL;
        }
	;

command.seq:
	  command ';' command.seq
	| command
	| /* empty */
	;

/* 2.4 */
command:
	  command.block
    | ctrl.flow
	| TK_IDENTIFICADOR '=' expr { }
    | TK_IDENTIFICADOR '[' expr ']' '=' expr
	| TK_PR_OUTPUT output.list
    | TK_PR_INPUT TK_IDENTIFICADOR
    | TK_PR_RETURN expr 
    | /* empty */
	;

output.list:
	  expr
	| expr ',' output.list
	;

/* 2.5 */
expr:
	  TK_IDENTIFICADOR
	| TK_IDENTIFICADOR '[' expr ']'
	| terminal.value
	| '(' expr ')'
	| expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| expr '<' expr
	| expr '>' expr
	| '!' expr
	| expr TK_OC_LE expr
	| expr TK_OC_GE expr
	| expr TK_OC_EQ expr
	| expr TK_OC_NE expr
	| expr TK_OC_AND expr
	| expr TK_OC_OR expr
	| '*' TK_IDENTIFICADOR
	| '&' TK_IDENTIFICADOR
	| TK_IDENTIFICADOR '(' func.param.list ')'
	;

terminal.value:
	  TK_LIT_INT
	| '-' TK_LIT_INT
	| TK_LIT_FLOAT
	| '-' TK_LIT_FLOAT
	| TK_LIT_FALSE
	| TK_LIT_TRUE
	| TK_LIT_CHAR
	| TK_LIT_STRING
	;
	
func.param.list:
	  param.list
	| /* empty */
	;

param.list:
	  expr
	| expr ',' param.list
	;

/* 2.6 */
ctrl.flow:
      TK_PR_IF '(' expr ')' TK_PR_THEN command
	| TK_PR_IF '(' expr ')' TK_PR_THEN command TK_PR_ELSE command
	| TK_PR_WHILE '(' expr ')' TK_PR_DO command
    | TK_PR_DO command TK_PR_WHILE '(' expr ')' 
	;

%%

/*
void yyerror(char* str)
{
	fflush(stderr);
	fprintf(stderr, "ERRO: \"%s\"\t Linha: %d token: %s\n", str, yy_line_number_get(), yy_last_token_identifier_get());
	exit(RS_ERRO);
}
*/	
