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
#include "comp_stack.h"
#include "iks_ast.h"
#include "iks_types.h"
#include "gv.h"
//#include "hash_table.h"

comp_tree_t *ptr_funcao;

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
    int type;
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

%start p

%type<symbol> decl
%type<nt> prog
%type<nt> func
%type<nt> command_block 
%type<nt> command_block_f
%type<nt> command_seq 
%type<nt> command 
%type<nt> commands 
%type<nt> ctrl_flow 
%type<nt> output_list 
%type<nt> expr 
%type<nt> func_param_list
%type<nt> param_list 
%type<nt> terminal_value
%type<nt> id
%type<nt> idv
%type<nt> func_call

%type<type> type

%right TK_PR_THEN TK_PR_ELSE
%right '='
%left '<'
%left '>'
%left TK_OC_LE
%left TK_OC_GE
%left TK_OC_EQ
%left TK_OC_NE
%left TK_OC_OR
%left TK_OC_AND
%left '+' '-'
%left '*' '/'
%left '!'
%left INVERSAO

%%

/*
GRAMMAR RULES
http://www.gnu.org/software/bison/manual/bison.html#Rules
*/

/* 2 */

p:
    {    
            //
            comp_dict_t *symbol_table_global;
            symbol_table_global = new_comp_dict();
            scope = comp_stack_push(scope,(void*)symbol_table_global);
            // just the first function is child of ast
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_PROGRAMA,NULL);
            comp_tree_set_item(ast,(void*)v);
  	        gv_declare(IKS_AST_PROGRAMA,ast,NULL);
    }
    prog
    ;

prog:
	  prog global_decl
        {
            //symbol_table_print((comp_dict_t*)comp_stack_top(scope));
        }
	| prog func
        {
            /* 3.A.1 */
            // just the first function is child of ast
            if (comp_list_is_empty(ast->children)) {
                iks_ast_connect_nodes(ast,$2);
            }
            else {
              iks_ast_connect_nodes($1,$2);
            }
            //symbol_table_print((comp_dict_t*)comp_stack_top(scope));
            $$ = $2;
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
      {
          update_decl_symbol($1,IKS_DECL_VECTOR,$3);
      }
	;

decl:
      type ':' TK_IDENTIFICADOR
        {
          if (!decl_symbol($3,$1,IKS_DECL_VAR,comp_stack_top(scope))) {
            return(IKS_ERROR_DECLARED);
          }
          $$ = $3;
        }  
    ;

type:
	  TK_PR_INT
      {
        $$ = IKS_INT;
      }
	| TK_PR_FLOAT
      {
        $$ = IKS_FLOAT;
      }
	| TK_PR_BOOL
      {
        $$ = IKS_BOOL;
      }
	| TK_PR_CHAR
      {
        $$ = IKS_CHAR;
      }
	| TK_PR_STRING
      {
        $$ = IKS_STRING;
      }
	;

/* 2.2 */
func:
	  type ':' TK_IDENTIFICADOR
        {
          if (!decl_symbol($3,$1,IKS_DECL_FUNCTION,comp_stack_top(scope))) {
            return(IKS_ERROR_DECLARED);
          }
          comp_dict_t *symbol_table_local;
          symbol_table_local = new_comp_dict();
          /* 3.A.2 */
          comp_tree_t *funcao = iks_ast_new_node(IKS_AST_FUNCAO,$3);
          scope = comp_stack_push(scope,(void*)symbol_table_local);
          ptr_funcao=funcao;
        }
	  '(' func_param_decl_list ')' decl_list
      {
            //symbol_table_print((comp_dict_t*)comp_stack_top(scope));
      }
      command_block_f
      {
            if ($command_block_f) {
                iks_ast_connect_nodes(ptr_funcao,$command_block_f);
            }
            scope = comp_stack_pop(scope);
            $$ = ptr_funcao;
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
command_block_f://two command_block because cmd_blk from function isnt in ast
	  '{' command_seq '}'
        {
            $$ = $2;
        }
	;

command_block:
	  '{' command_seq '}'
        {
            comp_tree_t *bloco = iks_ast_new_node(IKS_AST_BLOCO,NULL);
            if ($2) { //because can command_seq <- command <- empty
                iks_ast_connect_nodes(bloco,$2);
            }
            $$ = bloco;
        }
	;

command_seq:
	  command ';' command_seq
        {
            /* 3.A.10 */
            if ($3) { //because can command_seq <- command <- empty
              iks_ast_connect_nodes($1,$3);
            }
        }
	| command
	;

/* 2.4 */

command:
    commands
	| /* empty */
        {
            $$ = NULL;
        }
    ;

commands:
      func_call
	| command_block
    | ctrl_flow
	| id '=' expr
        {
            /* 3.A.8 */
            iks_ast_node_value_t *idn,*exprn;
            idn = $1->item;
            exprn = $3->item;
            
            comp_grammar_symbol_t *ids,*exprs;
            ids = idn->symbol;
            exprs = exprn->symbol;
            
            if(ids->iks_type == IKS_STRING) { //strings set size dinamically
              update_decl_symbol(ids,IKS_STRING,exprs);
            }
            
            comp_tree_t *atribuicao = iks_ast_new_node(IKS_AST_ATRIBUICAO,NULL);
            iks_ast_connect_nodes(atribuicao,$1);
            iks_ast_connect_nodes(atribuicao,$3);
            $$ = atribuicao;
            
            if (exprs) {
              if(ids->iks_type!=exprs->iks_type) {
                int coercion=verify_coercion($1,$3);
                if (coercion) { //if coercion is invalid
                  return coercion;
                } else { //attribute id type to attribution
          		  
              	}
              }
            }
        }
	| idv '=' expr
        {
            /* 3.A.8 */
            comp_tree_t *atribuicao = iks_ast_new_node(IKS_AST_ATRIBUICAO,NULL);
            iks_ast_connect_nodes(atribuicao,$1);
            iks_ast_connect_nodes(atribuicao,$3);
            $$ = atribuicao;
        }
	| TK_PR_OUTPUT output_list
        {
            /* 3.A.7 */
            comp_tree_t *output = iks_ast_new_node(IKS_AST_OUTPUT,NULL);
            iks_ast_connect_nodes(output,$2);
            $$ = output;
        }
    | TK_PR_INPUT id
        {
            /* 3.A.6 */
            comp_tree_t *input = iks_ast_new_node(IKS_AST_INPUT,NULL);
            iks_ast_connect_nodes(input,$2);
            $$ = input;
        }
    | TK_PR_RETURN expr 
        {
            /* 3.A.9 */
            comp_tree_t *ret = iks_ast_new_node(IKS_AST_RETURN,NULL);
            iks_ast_connect_nodes(ret,$2);
            $$ = ret;
        }
	;

id:
    TK_IDENTIFICADOR
    {
      comp_grammar_symbol_t *s = $1;
      s = search_symbol_global($1,scope);
      if (s) {
        comp_tree_t *identificador = iks_ast_new_node(IKS_AST_IDENTIFICADOR,s);
        $$ = identificador;
      }
      else {
        fprintf(stderr,"line %d: identificador '%s' não declarado\n",$1->code_line_number,$1->value);
        return(IKS_ERROR_UNDECLARED);
      }
    }
    ;
idv:
    id '[' expr ']'
    {
            // []
            comp_tree_t *vets = iks_ast_new_node(IKS_AST_VETOR_INDEXADO,NULL);
            iks_ast_connect_nodes(vets,$1);
            iks_ast_connect_nodes(vets,$3);
            $$ = vets;
    }
    ;

output_list:
	  expr
	| expr ',' output_list
        {
            iks_ast_connect_nodes($1,$3);
        }
	;

/* 2.5 */
expr:
	  id
      {
            iks_ast_node_value_t *n;
            n = $1->item; 
            comp_grammar_symbol_t *s;
            s = n->symbol;
            if(!symbol_is_decl_type(s,IKS_DECL_VAR)) {
              return iks_error(s,IKS_ERROR_USE);
            }
        }
	| id '[' expr ']'
        {
            // []
            iks_ast_node_value_t *n;
            n = $1->item; 
            comp_grammar_symbol_t *s;
            s = n->symbol;
            if(symbol_is_decl_type(s,IKS_DECL_VECTOR)) {
              comp_tree_t *vets = iks_ast_new_node(IKS_AST_VETOR_INDEXADO,NULL);
              iks_ast_connect_nodes(vets,$1);
              iks_ast_connect_nodes(vets,$3);
              $$ = vets;
            }
            else {
              return iks_error(s,IKS_ERROR_USE);
            }
        }
	| terminal_value
	| '(' expr ')'
        {
            $$ = $2;
        }
	| expr '+' expr
        {
            /* 3.A.12 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_SOMA,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr '-' expr
        {
            /* 3.A.12 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_SUBTRACAO,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr '*' expr
        {
            /* 3.A.12 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_MULTIPLICACAO,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr '/' expr
        {
            /* 3.A.12 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_DIVISAO,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr '<' expr
        {
            /* 3.A.14 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_L,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr '>' expr
        {
            /* 3.A.14 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_G,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| '!' expr
        {
            /* 3.A.15 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_NEGACAO,NULL);
            iks_ast_connect_nodes(oo,$2);
            $$ = oo;
        }
	| '-' expr %prec INVERSAO
        {
            /* 3.A.15 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_INVERSAO,NULL);
            iks_ast_connect_nodes(oo,$2);
            $$ = oo;
        }
	| expr TK_OC_LE expr
        {
            /* 3.A.14 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_LE,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_GE expr
        {
            /* 3.A.14 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_GE,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_EQ expr
        {
            /* 3.A.14 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_IGUAL,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_NE expr
        {
            /* 3.A.14 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_DIF,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_AND expr
        {
            /* 3.A.14 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_E,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_OR expr
        {
            /* 3.A.14 */
            comp_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_OU,NULL);
            iks_ast_connect_nodes(oo,$1);
            iks_ast_connect_nodes(oo,$3);
            $$ = oo;
        }
    | func_call
	;

func_call:
	id '(' func_param_list ')'
        {
            iks_ast_node_value_t *n;
            n = $1->item; 
            comp_grammar_symbol_t *s;
            s = n->symbol;
            /* 3.A.17 */
            if(symbol_is_decl_type(s,IKS_DECL_FUNCTION)) {
              comp_tree_t *x = iks_ast_new_node(IKS_AST_CHAMADA_DE_FUNCAO,NULL);
              iks_ast_connect_nodes(x,$1);
              if ($3) { //if no params, so NULL
                iks_ast_connect_nodes(x,$3);
              }
              $$ = x;
            }
            else {
              return iks_error(s,IKS_ERROR_USE);
            }
        }
    ;

terminal_value:
        /* 3.A.11 */
	  TK_LIT_INT
        {
            $1->iks_type=IKS_INT;
            comp_tree_t *lit = iks_ast_new_node(IKS_AST_LITERAL,$1);
            $$ = lit;
        }
	| TK_LIT_FLOAT
        {
            $1->iks_type=IKS_FLOAT;
            comp_tree_t *lit = iks_ast_new_node(IKS_AST_LITERAL,$1);
            $$ = lit;
        }
	| TK_LIT_FALSE
        {
            $1->iks_type=IKS_BOOL;
            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v1,IKS_AST_LITERAL,$1);
            comp_tree_t *lit;
            lit = new_comp_tree();
            comp_tree_set_item(lit,(void*)v1);
	          gv_declare(IKS_AST_LITERAL,lit,"false");
            $$ = lit;
        }
	| TK_LIT_TRUE
        {
            $1->iks_type=IKS_BOOL;
            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v1,IKS_AST_LITERAL,$1);
            comp_tree_t *lit;
            lit = new_comp_tree();
            comp_tree_set_item(lit,(void*)v1);
	          gv_declare(IKS_AST_LITERAL,lit,"true");
            $$ = lit;
        }
	| TK_LIT_CHAR
        {
            $1->iks_type=IKS_CHAR;
            comp_tree_t *lit = iks_ast_new_node(IKS_AST_LITERAL,$1);
            $$ = lit;
        }
	| TK_LIT_STRING
        {
            $1->iks_type=IKS_STRING;
            comp_tree_t *lit = iks_ast_new_node(IKS_AST_LITERAL,$1);
            $$ = lit;
        }
	;
	
func_param_list:
	  param_list
	| { $$ = NULL; } /* empty */
	;

param_list:
	  expr
	| expr ',' param_list
        {    
            if ($3) { //because can command_seq <- command <- empty
              iks_ast_connect_nodes($1,$3);
            }
        }
	;

/* 2.6 */
ctrl_flow:
      TK_PR_IF '(' expr ')' TK_PR_THEN commands
        {
            /* 3.A.3 */
            comp_tree_t *if_else = iks_ast_new_node(IKS_AST_IF_ELSE,NULL);
            iks_ast_connect_nodes(if_else,$3);
            iks_ast_connect_nodes(if_else,$6);
            $$ = if_else;
        }
	| TK_PR_IF '(' expr ')' TK_PR_THEN commands TK_PR_ELSE commands
        {
            /* 3.A.3 */
            comp_tree_t *if_else = iks_ast_new_node(IKS_AST_IF_ELSE,NULL);
            iks_ast_connect_nodes(if_else,$3);
            iks_ast_connect_nodes(if_else,$6);
            iks_ast_connect_nodes(if_else,$8);
            $$ = if_else;
        }
	| TK_PR_WHILE '(' expr ')' TK_PR_DO commands
        {
            /* 3.A.5 */
            comp_tree_t *while_do = iks_ast_new_node(IKS_AST_WHILE_DO,NULL);
            iks_ast_connect_nodes(while_do,$3);
            iks_ast_connect_nodes(while_do,$6);
            $$ = while_do;
        }
    | TK_PR_DO commands TK_PR_WHILE '(' expr ')' 
        {
            /* 3.A.4 */
            comp_tree_t *do_while = iks_ast_new_node(IKS_AST_DO_WHILE,NULL);
            iks_ast_connect_nodes(do_while,$2);
            iks_ast_connect_nodes(do_while,$5);
            $$ = do_while;
        }
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
