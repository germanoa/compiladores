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
    comp_grammar_symbol_t *symbol;
    comp_tree_t *nt;
}
%token<symbol> TK_LIT_INT	280
%token<symbol> TK_LIT_FLOAT	281
%token<symbol> TK_LIT_FALSE	282
%token<symbol> TK_LIT_TRUE	283
%token<symbol> TK_LIT_CHAR	284	
%token<symbol> TK_LIT_STRING	285
%token<symbol> TK_IDENTIFICADOR 286

%token TOKEN_ERRO	290

%start prog

/* declaracoes nao sao nodos da arvore? */
%type<nt> prog
%type<nt> func
%type<nt> command_block 
%type<nt> command_seq 
%type<nt> command 
%type<nt> ctrl_flow 
%type<nt> output_list 
%type<nt> expr 
%type<nt> func_param_list
%type<nt> param_list 

%%

/*
GRAMMAR RULES
http://www.gnu.org/software/bison/manual/bison.html#Rules
*/

/* 2 */
prog:
	  prog global_decl
	| prog func
        {
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_PROGRAMA,NULL);
            comp_tree_set_item(ast,(void*)v);
            iks_ast_append(ast,$func);
            $$ = ast;
        }
	| /* empty */
	;

/* 2.1 */
global_decl:
	  decl ';'
	| array_decl ';'
	;

array_decl:
	  decl '[' TK_LIT_INT ']'
	;

decl:
      type ':' TK_IDENTIFICADOR  
    ;

/*
type_and_id:
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
	  type ':' TK_IDENTIFICADOR '(' func_param_decl_list ')' decl_list command_block
        {
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_FUNCAO,yylval.symbol);
            comp_tree_set_item($func,(void*)v);
            iks_ast_append($func,$command_block);
            $$ = $func;
        }
	;

func_param_decl_list:
	  param_decl_list
	| /* empty */
	;

param_decl_list:
	  decl ',' param_decl_list
	| decl
	;

decl_list: // pode ser vazia?
	  decl ';' decl_list
    | /* empty */
	;

/* 2.3 */
command_block:
	  '{' command_seq '}'
        {
            $$ = $2;
        }
	;

command_seq:
	  command ';' command_seq
        {
            iks_ast_append($command,$3);
        }
	| command
	| /* empty */
	;

/* 2.4 */
command:
	  command_block
    | ctrl_flow
	| TK_IDENTIFICADOR '=' expr
        {
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_ATRIBUICAO,NULL);
            comp_tree_t *atribuicao;
            atribuicao = new_comp_tree();
            comp_tree_set_item(atribuicao,(void*)v);

            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_IDENTIFICADOR,$1);
            comp_tree_t *identificador;
            identificador = new_comp_tree();
            comp_tree_set_item(identificador,(void*)v1);

            iks_ast_append(atribuicao,identificador);
            iks_ast_append(atribuicao,$3);
            $$ = atribuicao;
                    
        }
    | TK_IDENTIFICADOR '[' expr ']' '=' expr
	| TK_PR_OUTPUT output_list
    | TK_PR_INPUT TK_IDENTIFICADOR
    | TK_PR_RETURN expr 
    | /* empty */
	;

output_list:
	  expr
	| expr ',' output_list
	;

/* 2.5 */
expr:
	  TK_IDENTIFICADOR
	| TK_IDENTIFICADOR '[' expr ']'
	| terminal_value
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
	| TK_IDENTIFICADOR '(' func_param_list ')'
	;

terminal_value:
	  TK_LIT_INT
	| '-' TK_LIT_INT
	| TK_LIT_FLOAT
	| '-' TK_LIT_FLOAT
	| TK_LIT_FALSE
	| TK_LIT_TRUE
	| TK_LIT_CHAR
	| TK_LIT_STRING
	;
	
func_param_list:
	  param_list
	| /* empty */
	;

param_list:
	  expr
	| expr ',' param_list
	;

/* 2.6 */
ctrl_flow:
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
