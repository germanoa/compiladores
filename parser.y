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
#include "gv.h"
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

%start p

/* declaracoes nao sao nodos da arvore? */
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
	| prog func
        {
            /* 3.A.1 */
            // just the first function is child of ast
            if (comp_list_is_empty(ast->children)) {
                iks_ast_append(ast,$2);
                gv_connect(ast, $2);
            }
            else {
                iks_ast_append($1,$2);
		        gv_connect($1,$2);
            }
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
	  type ':' TK_IDENTIFICADOR '(' func_param_decl_list ')' decl_list command_block_f
        {
            /* 3.A.2 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_FUNCAO,$3);
            comp_tree_t *funcao;
            funcao = new_comp_tree();
            comp_tree_set_item(funcao,(void*)v);
            gv_declare(IKS_AST_FUNCAO,funcao,$3->value); 
            if ($8) {
                iks_ast_append(funcao,$8);
                gv_connect(funcao,$8);
            }
            $$ = funcao;
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
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_BLOCO,NULL);
            comp_tree_t *bloco;
            bloco = new_comp_tree();
            comp_tree_set_item(bloco,(void*)v);
            gv_declare(IKS_AST_BLOCO,bloco,NULL);

            if ($2) { //because can command_seq <- command <- empty
                iks_ast_append(bloco,$2);
		        gv_connect(bloco,$2);
            }
            $$ = bloco;
        }
	;

command_seq:
	  command ';' command_seq
        {
            /* 3.A.10 */
            if ($3) { //because can command_seq <- command <- empty
                iks_ast_append($1,$3);
		        gv_connect($1,$3);
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
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_ATRIBUICAO,NULL);
            comp_tree_t *atribuicao;
            atribuicao = new_comp_tree();
            comp_tree_set_item(atribuicao,(void*)v);
            gv_declare(IKS_AST_ATRIBUICAO,atribuicao,NULL);

            iks_ast_append(atribuicao,$1);
	        gv_connect(atribuicao,$1);
            iks_ast_append(atribuicao,$3);
	        gv_connect(atribuicao,$3);
            $$ = atribuicao;
        }
	| idv '=' expr
        {
            /* 3.A.8 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_ATRIBUICAO,NULL);
            comp_tree_t *atribuicao;
            atribuicao = new_comp_tree();
            comp_tree_set_item(atribuicao,(void*)v);
            gv_declare(IKS_AST_ATRIBUICAO,atribuicao,NULL);

            iks_ast_append(atribuicao,$1);
	        gv_connect(atribuicao,$1);
            iks_ast_append(atribuicao,$3);
	        gv_connect(atribuicao,$3);
            $$ = atribuicao;
        }
	| TK_PR_OUTPUT output_list
        {
            /* 3.A.7 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_OUTPUT,NULL);
            comp_tree_t *output;
            output = new_comp_tree();
            comp_tree_set_item(output,(void*)v);
	        gv_declare(IKS_AST_OUTPUT,output,NULL);

            iks_ast_append(output,$2);
	    gv_connect(output,$2);
            $$ = output;
        }
    | TK_PR_INPUT TK_IDENTIFICADOR
        {
            /* 3.A.6 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_INPUT,NULL);
            comp_tree_t *input;
            input = new_comp_tree();
            comp_tree_set_item(input,(void*)v);
	        gv_declare(IKS_AST_INPUT,input,NULL);

            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v1,IKS_AST_IDENTIFICADOR,$2);
            comp_tree_t *identificador;
            identificador = new_comp_tree();
            comp_tree_set_item(identificador,(void*)v1);
            gv_declare(IKS_AST_IDENTIFICADOR,identificador,$2->value);

            iks_ast_append(input,identificador);
	        gv_connect(input,identificador);
            $$ = input;
        }
    | TK_PR_RETURN expr 
        {
            /* 3.A.9 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_RETURN,NULL);
            comp_tree_t *ret;
            ret = new_comp_tree();
            comp_tree_set_item(ret,(void*)v);
	        gv_declare(IKS_AST_RETURN,ret,NULL);

            iks_ast_append(ret,$2);
	        gv_connect(ret,$2);
            $$ = ret;
        }
	;

id:
    TK_IDENTIFICADOR
    {
            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v1,IKS_AST_IDENTIFICADOR,$1);
            comp_tree_t *identificador;
            identificador = new_comp_tree();
            comp_tree_set_item(identificador,(void*)v1);
	        gv_declare(IKS_AST_IDENTIFICADOR,identificador,$1->value);
            $$ = identificador;
    }
    ;
idv:
    id '[' expr ']'
    {
            // []
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_VETOR_INDEXADO,NULL);
            comp_tree_t *vets;
            vets = new_comp_tree();
            comp_tree_set_item(vets,(void*)v);
	        gv_declare(IKS_AST_VETOR_INDEXADO,vets,NULL);

            iks_ast_append(vets,$1);
	        gv_connect(vets,$1);
            iks_ast_append(vets,$3);
	        gv_connect(vets,$3);
            $$ = vets;
    }
    ;

output_list:
	  expr
	| expr ',' output_list
        {
            iks_ast_append($1,$3);
	        gv_connect($1,$3);
        }
	;

/* 2.5 */
expr:
	  TK_IDENTIFICADOR
        {
            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v1,IKS_AST_LITERAL,$1);
            comp_tree_t *lit;
            lit = new_comp_tree();
            comp_tree_set_item(lit,(void*)v1);
	        gv_declare(IKS_AST_LITERAL,lit,$1->value);
            $$ = lit;
        }
	| id '[' expr ']'
        {
            // []
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_VETOR_INDEXADO,NULL);
            comp_tree_t *vets;
            vets = new_comp_tree();
            comp_tree_set_item(vets,(void*)v);
	        gv_declare(IKS_AST_VETOR_INDEXADO,vets,NULL);

            iks_ast_append(vets,$1);
	        gv_connect(vets,$1);
            iks_ast_append(vets,$3);
	        gv_connect(vets,$3);

            $$ = vets;
        }
	| terminal_value
	| '(' expr ')'
        {
            $$ = $2;
        }
	| expr '+' expr
        {
            /* 3.A.12 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_ARIM_SOMA,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_ARIM_SOMA,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr '-' expr
        {
            /* 3.A.12 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_ARIM_SUBTRACAO,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_ARIM_SUBTRACAO,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr '*' expr
        {
            /* 3.A.12 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_ARIM_MULTIPLICACAO,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_ARIM_MULTIPLICACAO,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr '/' expr
        {
            /* 3.A.12 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_ARIM_DIVISAO,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_ARIM_DIVISAO,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr '<' expr
        {
            /* 3.A.14 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_LOGICO_COMP_L,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_LOGICO_COMP_L,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr '>' expr
        {
            /* 3.A.14 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_LOGICO_COMP_G,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_LOGICO_COMP_G,oo,NULL);

            iks_ast_append(oo,$1);
	        gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	        gv_connect(oo,$3);
            $$ = oo;
        }
	| '!' expr
        {
            /* 3.A.15 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_LOGICO_COMP_NEGACAO,NULL);
            comp_tree_t *inv;
            inv = new_comp_tree();
            comp_tree_set_item(inv,(void*)v);
	        gv_declare(IKS_AST_LOGICO_COMP_NEGACAO,inv,NULL);

            iks_ast_append(inv,$2);
	        gv_connect(inv,$2);
            $$ = inv;
        }
	| '-' expr %prec INVERSAO
        {
            /* 3.A.15 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_ARIM_INVERSAO,NULL);
            comp_tree_t *inv;
            inv = new_comp_tree();
            comp_tree_set_item(inv,(void*)v);
	        gv_declare(IKS_AST_ARIM_INVERSAO,inv,NULL);

            iks_ast_append(inv,$2);
	        gv_connect(inv,$2);
            $$ = inv;
        }
	| expr TK_OC_LE expr
        {
            /* 3.A.14 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_LOGICO_COMP_LE,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_LOGICO_COMP_LE,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_GE expr
        {
            /* 3.A.14 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_LOGICO_COMP_GE,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_LOGICO_COMP_GE,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_EQ expr
        {
            /* 3.A.14 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_LOGICO_COMP_IGUAL,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_LOGICO_COMP_IGUAL,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_NE expr
        {
            /* 3.A.14 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_LOGICO_COMP_DIF,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_LOGICO_COMP_DIF,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_AND expr
        {
            /* 3.A.14 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_LOGICO_E,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_LOGICO_E,oo,NULL);

            iks_ast_append(oo,$1);
	    gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	    gv_connect(oo,$3);
            $$ = oo;
        }
	| expr TK_OC_OR expr
        {
            /* 3.A.14 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_LOGICO_OU,NULL);
            comp_tree_t *oo;
            oo = new_comp_tree();
            comp_tree_set_item(oo,(void*)v);
	        gv_declare(IKS_AST_LOGICO_OU,oo,NULL);

            iks_ast_append(oo,$1);
	        gv_connect(oo,$1);
            iks_ast_append(oo,$3);
	        gv_connect(oo,$3);
            $$ = oo;
        }
    | func_call
	;

func_call:
	id '(' func_param_list ')'
        {
            /* 3.A.17 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_CHAMADA_DE_FUNCAO,NULL);
            comp_tree_t *x;
            x = new_comp_tree();
            comp_tree_set_item(x,(void*)v);
	        gv_declare(IKS_AST_CHAMADA_DE_FUNCAO,x,NULL);

            iks_ast_append(x,$1);
	        gv_connect(x,$1);

            if ($3) { //if no params, so NULL
                iks_ast_append(x,$3);
	            gv_connect(x,$3);
            }
            $$ = x;
        }

terminal_value:
        /* 3.A.11 */
	  TK_LIT_INT
        {
            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v1,IKS_AST_LITERAL,$1);
            comp_tree_t *lit;
            lit = new_comp_tree();
            comp_tree_set_item(lit,(void*)v1);
	        gv_declare(IKS_AST_LITERAL,lit,$1->value);
            $$ = lit;
        }
	//| '-' TK_LIT_INT %prec INVERSAO
    //    {
    //        /* 3.A.13 */
    //        iks_ast_node_value_t *v;
    //        v = new_iks_ast_node_value();
    //        comp_tree_t *inv;
    //        inv = new_comp_tree();
    //        comp_tree_set_item(inv,(void*)v);
	//        gv_declare(IKS_AST_ARIM_INVERSAO,inv,NULL);

    //        iks_ast_node_value_t *v1;
    //        v1 = new_iks_ast_node_value();
    //        iks_ast_node_value_set(v1,IKS_AST_LITERAL,$2);
    //        comp_tree_t *lit;
    //        lit = new_comp_tree();
    //        comp_tree_set_item(lit,(void*)v1);
	//        gv_declare(IKS_AST_LITERAL,lit,$2->value);
    //        
    //        iks_ast_append(inv,lit);
	//    gv_connect(inv,lit);
    //        $$ = inv;
    //    }
	| TK_LIT_FLOAT
        {
            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v1,IKS_AST_LITERAL,$1);
            comp_tree_t *lit;
            lit = new_comp_tree();
            comp_tree_set_item(lit,(void*)v1);
	        gv_declare(IKS_AST_LITERAL,lit,$1->value);
            $$ = lit;
        }
	//| '-' TK_LIT_FLOAT %prec INVERSAO
    //    {
    //        /* 3.A.13 */
    //        iks_ast_node_value_t *v;
    //        v = new_iks_ast_node_value();
    //        iks_ast_node_value_set(v,IKS_AST_ARIM_INVERSAO,NULL);
    //        comp_tree_t *inv;
    //        inv = new_comp_tree();
    //        comp_tree_set_item(inv,(void*)v);
	//        gv_declare(IKS_AST_ARIM_INVERSAO,inv,NULL);

    //        iks_ast_node_value_t *v1;
    //        v1 = new_iks_ast_node_value();
    //        iks_ast_node_value_set(v1,IKS_AST_LITERAL,$2);
    //        comp_tree_t *lit;
    //        lit = new_comp_tree();
    //        comp_tree_set_item(lit,(void*)v1);
	//        gv_declare(IKS_AST_LITERAL,lit,$2->value);
    //        
    //        iks_ast_append(inv,lit);
	//    gv_connect(inv,lit);
    //        $$ = inv;
    //    }
	| TK_LIT_FALSE
        {
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
            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v1,IKS_AST_LITERAL,$1);
            comp_tree_t *lit;
            lit = new_comp_tree();
            comp_tree_set_item(lit,(void*)v1);
	        gv_declare(IKS_AST_LITERAL,lit,$1->value);
            $$ = lit;
        }
	| TK_LIT_STRING
        {
            iks_ast_node_value_t *v1;
            v1 = new_iks_ast_node_value();
            iks_ast_node_value_set(v1,IKS_AST_LITERAL,$1);
            comp_tree_t *lit;
            lit = new_comp_tree();
            comp_tree_set_item(lit,(void*)v1);
	        gv_declare(IKS_AST_LITERAL,lit,$1->value);
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
                iks_ast_append($1,$3);
	            gv_connect($1,$3);
            }
        }
	;

/* 2.6 */
ctrl_flow:
      TK_PR_IF '(' expr ')' TK_PR_THEN commands
        {
            /* 3.A.3 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_IF_ELSE,NULL);
            comp_tree_t *if_else;
            if_else = new_comp_tree();
            comp_tree_set_item(if_else,(void*)v);
	        gv_declare(IKS_AST_IF_ELSE,if_else,NULL);

            iks_ast_append(if_else,$3);
	    gv_connect(if_else,$3);
            iks_ast_append(if_else,$6);
	    gv_connect(if_else,$6);
            $$ = if_else;
        }
	| TK_PR_IF '(' expr ')' TK_PR_THEN commands TK_PR_ELSE commands
        {
            /* 3.A.3 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_IF_ELSE,NULL);
            comp_tree_t *if_else;
            if_else = new_comp_tree();
            comp_tree_set_item(if_else,(void*)v);
	        gv_declare(IKS_AST_IF_ELSE,if_else,NULL);

            iks_ast_append(if_else,$3);
	        gv_connect(if_else,$3);
            iks_ast_append(if_else,$6);
	        gv_connect(if_else,$6);
            iks_ast_append(if_else,$8);
	        gv_connect(if_else,$8);
            $$ = if_else;
        }
	| TK_PR_WHILE '(' expr ')' TK_PR_DO commands
        {
            /* 3.A.5 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_WHILE_DO,NULL);
            comp_tree_t *while_do;
            while_do = new_comp_tree();
            comp_tree_set_item(while_do,(void*)v);
	        gv_declare(IKS_AST_WHILE_DO,while_do,NULL);

            iks_ast_append(while_do,$3);
	        gv_connect(while_do,$3);
            iks_ast_append(while_do,$6);
	        gv_connect(while_do,$6);
            $$ = while_do;
        }
    | TK_PR_DO commands TK_PR_WHILE '(' expr ')' 
        {
            /* 3.A.4 */
            iks_ast_node_value_t *v;
            v = new_iks_ast_node_value();
            iks_ast_node_value_set(v,IKS_AST_DO_WHILE,NULL);
            comp_tree_t *do_while;
            do_while = new_comp_tree();
            comp_tree_set_item(do_while,(void*)v);
	        gv_declare(IKS_AST_DO_WHILE,do_while,NULL);

            iks_ast_append(do_while,$2);
	    gv_connect(do_while,$2);
            iks_ast_append(do_while,$5);
	    gv_connect(do_while,$5);
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
