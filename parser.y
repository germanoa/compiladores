/*
PROLOGUE
http://www.gnu.org/software/bison/manual/bison.html#Prologue
*/

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "iks_grammar.h"
	#include "iks_dict.h"
	#include "iks_tree.h"
	#include "iks_stack.h"
	#include "iks_ast.h"
	#include "iks_types.h"
	#include "iks_gv.h"
	#include "iks_iloc.h"
	//#include "iks_hash.h"

	iks_tree_t *ptr_function;
	iks_tree_t *ptr_function_call;
	iks_grammar_symbol_t *function_with_param;
	iks_list_t *args;
	unsigned int dimen_counter;
	
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
	iks_grammar_symbol_t *symbol;
	iks_tree_t *nt;
	iks_list_t *list;
}

%token<symbol> TK_LIT_INT		280
%token<symbol> TK_LIT_FLOAT		281
%token<symbol> TK_LIT_FALSE		282
%token<symbol> TK_LIT_TRUE		283
%token<symbol> TK_LIT_CHAR		284	
%token<symbol> TK_LIT_STRING	285
%token<symbol> TK_IDENTIFICADOR 286

%token TOKEN_ERRO	290

%start p

%type<symbol> decl
%type<list> array_decl_dimen
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
%type<nt> arim_expr 
%type<nt> logic_expr 
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
			ptr_function=NULL;
			ptr_function_call=NULL;
			function_with_param=NULL;
			iks_dict_t *symbol_table_global;
			symbol_table_global = new_iks_dict();
			scope->st = iks_stack_push(scope->st,(void*)symbol_table_global);
			// just the first function is child of ast
			iks_ast_node_value_t *v;
			v = new_iks_ast_node_value();
			iks_ast_node_value_set(v,IKS_AST_PROGRAMA,NULL);
			iks_tree_set_item(ast,(void*)v);
			gv_declare(IKS_AST_PROGRAMA,ast,NULL);
			dimen_counter = 0;
		} prog
	;

prog:
		prog global_decl
		{
		//symbol_table_print((iks_dict_t*)iks_stack_top(scope->st));
		}
	| prog func
		{
			/* 3.A.1 */
			// just the first function is child of ast
			if (iks_list_is_empty(ast->children)) {
				iks_ast_connect_nodes(ast,$2);
			}
			else {
				iks_ast_connect_nodes($1,$2);
			}
			//symbol_table_print((iks_dict_t*)iks_stack_top(scope->st));
			$$ = $2;
		}
	| /* empty */ {}
	;

/* 2.1 */
global_decl:
		decl ';'
	| array_decl ';'
	;

array_decl:
		decl {
			dimen_counter = 0;
		} array_decl_dimen {
			update_vector_symbol($decl,dimen_counter,$array_decl_dimen);
		}
	;

array_decl_dimen:
		array_decl_dimen '[' TK_LIT_INT ']'
		{
			iks_list_t *list = $1;
			iks_grammar_symbol_t *lit = $3;
			int size = atoi(lit->value);
			iks_list_t *dimen = new_iks_list();
			iks_list_set_item(dimen,(void*)&size);
			
			iks_list_append(list, dimen);
			
			dimen_counter++;
			$$ = list;
		}
	| '[' TK_LIT_INT ']'
		{
			iks_grammar_symbol_t *lit = $2;
			int size = atoi(lit->value);
			iks_list_t *dimen = new_iks_list();
			iks_list_set_item(dimen,(void*)&size);
			
			dimen_counter++;
			$$ = dimen;
		}
	;

decl:
		type ':' TK_IDENTIFICADOR
		{
			if (!decl_symbol($3,$1,IKS_DECL_VAR,iks_stack_top(scope->st),function_with_param)) {
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
		type ':' TK_IDENTIFICADOR {
			if (!decl_symbol($3,$1,IKS_DECL_FUNCTION,iks_stack_top(scope->st),function_with_param)) {
				return(IKS_ERROR_DECLARED);
			}
			
			iks_dict_t *symbol_table_local;
			symbol_table_local = new_iks_dict();
			
			/* 3.A.2 */
			iks_tree_t *funcao = iks_ast_new_node(IKS_AST_FUNCAO,$3);
			iks_ast_node_value_t *funcaon = funcao->item;
			funcaon->iks_type = $1;
			scope->st = iks_stack_push(scope->st,(void*)symbol_table_local);
			ptr_function=funcao;
			function_with_param=$3; //begin params decl
		} '(' func_param_decl_list ')' {
			function_with_param=NULL; // end params decl
		} decl_list {
			//symbol_table_print((iks_dict_t*)iks_stack_top(scope->st));
		} command_block_f {
			if ($command_block_f) {
				iks_ast_connect_nodes(ptr_function,$command_block_f);
			}
			
      //iks_dict_t *st = (iks_dict_t*) iks_stack_top(scope->st);
      //iks_dict_delete(st);
			symbol_table_delete(scope->st->item);
			scope->st = iks_stack_pop(scope->st);
			$$ = ptr_function;
			ptr_function = NULL; //evita aceitar um return fora de uma função
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
			iks_tree_t *bloco = iks_ast_new_node(IKS_AST_BLOCO,NULL);
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

			iks_grammar_symbol_t *ids,*exprs;
			ids = idn->symbol;
			exprs = exprn->symbol;

			if(!symbol_is_decl_type(ids,IKS_DECL_VAR)) {
				return iks_error(ids,IKS_ERROR_USE);
			}

			if(ids->iks_type == IKS_STRING) { //strings set size dinamically
				// CHECAR SE O VETOR DE CHAR TEM SÓ 1 DIMENSÃO
				//update_vector_symbol(...);
			}

			iks_tree_t *e;
			e=$3;
			if(exprn->type==IKS_AST_CHAMADA_DE_FUNCAO) {
				iks_list_t *expr_child;
				expr_child =  $3->children;
				e = expr_child->next->item;
				iks_ast_node_value_t *en;
				en = (iks_ast_node_value_t*)e->item;
				exprs = en->symbol;
			}            
			if (exprs) {
				if(ids->iks_type!=exprs->iks_type) {
					int coercion=verify_coercion($1,e);
					if (coercion) { //if coercion is invalid
						return coercion;
					} else { //attribute id type to attribution
						
					}
				}
			}

			iks_tree_t *atribuicao = iks_ast_new_node(IKS_AST_ATRIBUICAO,NULL);
			iks_ast_node_value_t *atrn = atribuicao->item;
			atrn->iks_type = ids->iks_type;
			iks_ast_connect_nodes(atribuicao,$1);
			iks_ast_connect_nodes(atribuicao,$3);
			$$ = atribuicao;
		}
	| idv '=' expr
		{
			/* 3.A.8 */
			iks_list_t *id_child;
			id_child =  $1->children;
			iks_tree_t *id_tree;
			id_tree = id_child->next->item;
			iks_ast_node_value_t *idn,*exprn;
			idn = (iks_ast_node_value_t*)id_tree->item;
			exprn = (iks_ast_node_value_t*)$expr->item;
			iks_grammar_symbol_t *ids,*exprs;
			ids = idn->symbol;
			exprs = exprn->symbol;
			if(!symbol_is_decl_type(ids,IKS_DECL_VECTOR)) {
				return iks_error(ids,IKS_ERROR_USE);
			}

			iks_tree_t *e;
			e=$3;
			if(exprn->type==IKS_AST_CHAMADA_DE_FUNCAO) {
				iks_list_t *expr_child;
				expr_child =  $3->children;
				e = expr_child->next->item;
				iks_ast_node_value_t *en;
				en = (iks_ast_node_value_t*)e->item;
				exprs = en->symbol;
			}
			if (exprs) {
				if(ids->iks_type!=exprs->iks_type) {
					int coercion=verify_coercion(id_tree,e);
					if (coercion) { //if coercion is invalid
						return coercion;
					} else { //attribute id type to attribution
						
					}
				}
			}

			iks_tree_t *atribuicao = iks_ast_new_node(IKS_AST_ATRIBUICAO,NULL);
			iks_ast_node_value_t *atrn = atribuicao->item;
			iks_ast_node_value_t *idvn = $1->item;
			atrn->iks_type = idvn->iks_type;
			iks_ast_connect_nodes(atribuicao,$1);
			iks_ast_connect_nodes(atribuicao,$3);
			$$ = atribuicao;

		}
	| TK_PR_OUTPUT output_list
		{
			/* 3.A.7 */
			iks_tree_t *output = iks_ast_new_node(IKS_AST_OUTPUT,NULL);
			iks_ast_connect_nodes(output,$2);
			$$ = output;
		}
	| TK_PR_INPUT expr
		{
			/* 3.A.6 */
			iks_ast_node_value_t *exprn;
			exprn = $2->item;
			iks_grammar_symbol_t *exprs;
			exprs = exprn->symbol;

			if(exprn->type != IKS_AST_IDENTIFICADOR) {
				return iks_error(exprs,IKS_ERROR_WRONG_PAR_INPUT);
			} else {
				iks_tree_t *input = iks_ast_new_node(IKS_AST_INPUT,NULL);
				iks_ast_connect_nodes(input,$2);
				$$ = input;
			}
		}
	| TK_PR_RETURN expr 
		{
			iks_ast_node_value_t *exprn;
			exprn = $2->item;
			iks_grammar_symbol_t *exprs;
			exprs = exprn->symbol;

			iks_ast_node_value_t *fn;
			fn = ptr_function->item;
			iks_grammar_symbol_t *fs;
			fs = fn->symbol;
			
			if(exprn->iks_type != fn->iks_type) {
				int coercion = verify_coercion(ptr_function, $2);
				if(coercion) {
					fprintf(stderr, "return: tipo %d deveria ser %d\n", exprn->iks_type, fn->iks_type);
					return iks_error(exprs,IKS_ERROR_WRONG_PAR_RETURN);
				}
			}

			/* 3.A.9 */
			iks_tree_t *ret = iks_ast_new_node(IKS_AST_RETURN,NULL);
			iks_ast_connect_nodes(ret,$2);
			$$ = ret;
		}
	;

id:
		TK_IDENTIFICADOR
		{
			iks_grammar_symbol_t *s;
			s = search_symbol_global($1,scope->st);
			if (s) {
				iks_tree_t *identificador = iks_ast_new_node(IKS_AST_IDENTIFICADOR,s);
				iks_ast_node_value_t *idn = identificador->item;
				idn->iks_type = s->iks_type;
				$$ = identificador;
			} else {
				fprintf(stderr,"identificador não declarado\n");
				return(IKS_ERROR_UNDECLARED);
			}
		}
	;

idv:
		id '[' expr ']'
		{
			iks_ast_node_value_t *idn;
			idn = $id->item; 
			iks_grammar_symbol_t *ids;
			ids = idn->symbol;
			
			if(symbol_is_decl_type(ids,IKS_DECL_VECTOR)) {
				iks_tree_t *vet = iks_ast_new_node(IKS_AST_VETOR_INDEXADO,NULL);
				
				iks_tree_t *int_tree = iks_ast_new_node(IKS_AST_INDEFINIDO,NULL); //temporary tree so infer_type can infer between IKS_INT and expr
				iks_ast_node_value_t *int_treen = int_tree->item;
				int_treen->iks_type = IKS_INT; //index of vector should be an integer
				
				int type = infer_type(int_tree, $expr); //checks if the type of expr can be inferred to int
				if(type > 5) //coercion error
					return type;
				iks_ast_node_value_t *vetn = vet->item;
				vetn->iks_type = idn->iks_type; //type of this ast node is that of the id
				
				iks_ast_node_value_delete(int_treen); //removing temporary tree
				iks_tree_delete(int_tree);
				
				iks_ast_connect_nodes(vet,$id);
				iks_ast_connect_nodes(vet,$expr);
				$$ = vet;
			} else { //using wrong identificador. id is not a vector
				return iks_error(ids,IKS_ERROR_USE);
			}
		}
	;

output_list:
		expr
		{
			iks_ast_node_value_t *exprn;
			exprn = $1->item;
			iks_grammar_symbol_t *exprs;
			exprs = exprn->symbol;

			switch(exprn->type)
			{
				case IKS_AST_LITERAL:
					if(exprs != NULL) {
						if(exprs->iks_type != IKS_STRING) {
							fprintf(stderr,"line %d: '%s' não é literal string\n",exprs->code_line_number, exprs->value);
							return(IKS_ERROR_WRONG_PAR_OUTPUT);
						}
					} else fprintf(stderr,"literal em parametro de output sem simbolo\n");
					break;

				case IKS_AST_ARIM_SOMA:
				case IKS_AST_ARIM_SUBTRACAO:
				case IKS_AST_ARIM_MULTIPLICACAO:
				case IKS_AST_ARIM_DIVISAO:
				case IKS_AST_ARIM_INVERSAO:
					break;

				default:
					if (exprs) {
						fprintf(stderr,"line %d: '%s' não é expressao artimetica ou string\n",exprs->code_line_number, exprs->value);
						fprintf(stderr,"tipo do no: %d\n", exprn->type);
					} else {
						fprintf(stderr,"parametro de output não é expressao artimetica ou string\n");
						fprintf(stderr,"tipo do no: %d\n", exprn->type);
					}
					return(IKS_ERROR_WRONG_PAR_OUTPUT);
					break;
			}
		}
	| expr ',' output_list
		{
			iks_ast_node_value_t *exprn;
			exprn = $1->item;
			iks_grammar_symbol_t *exprs;
			exprs = exprn->symbol;

			switch(exprn->type)
			{
				case IKS_AST_LITERAL:
					if(exprs != NULL) {
						if(exprs->iks_type != IKS_STRING) {
							fprintf(stderr,"line %d: '%s' não é literal string\n",exprs->code_line_number, exprs->value);
							return(IKS_ERROR_WRONG_PAR_OUTPUT);
						}
					} else fprintf(stderr,"literal em parametro de output sem simbolo\n");
					break;

				case IKS_AST_ARIM_SOMA:
				case IKS_AST_ARIM_SUBTRACAO:
				case IKS_AST_ARIM_MULTIPLICACAO:
				case IKS_AST_ARIM_DIVISAO:
				case IKS_AST_ARIM_INVERSAO:
					break;

				default:
					if (exprs) {
						fprintf(stderr,"line %d: '%s' não é expressao artimetica ou string\n",exprs->code_line_number, exprs->value);
						fprintf(stderr,"tipo do no: %d\n", exprn->type);
					} else {
						fprintf(stderr,"parametro de output não é expressao artimetica ou string\n");
						fprintf(stderr,"tipo do no: %d\n", exprn->type);
					}
					return(IKS_ERROR_WRONG_PAR_OUTPUT);
					break;
			}

			iks_ast_connect_nodes($1,$3);
		}
	;

/* 2.5 */
expr:
		id
		{
			iks_ast_node_value_t *n;
			n = $1->item; 
			iks_grammar_symbol_t *s;
			s = n->symbol;
			if(!symbol_is_decl_type(s,IKS_DECL_VAR)) {
				return iks_error(s,IKS_ERROR_USE);
			}
		}
	| idv
		{
			$$ = $idv;
		}
	| terminal_value
	| '(' expr ')'
		{
			$$ = $2;
		}
	| func_call
	| arim_expr
	| logic_expr
 ;

arim_expr: 	
	expr '+' expr
		{
			/* 3.A.12 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_SOMA,NULL);
			iks_ast_node_value_t *oon = oo->item;
			iks_ast_node_value_t *n1 = $1->item;
			iks_ast_node_value_t *n2 = $3->item;
			int type = infer_type($1, $3);
			if(type > 5) //erro de coerção
				return type;
			oon->iks_type = type;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| expr '-' expr
		{
			/* 3.A.12 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_SUBTRACAO,NULL);
			iks_ast_node_value_t *oon = oo->item;
			iks_ast_node_value_t *n1 = $1->item;
			iks_ast_node_value_t *n2 = $3->item;
			int type = infer_type($1, $3);
			if(type > 5) //erro de coerção
				return type;
			oon->iks_type = type;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| expr '*' expr
		{
			/* 3.A.12 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_MULTIPLICACAO,NULL);
			iks_ast_node_value_t *oon = oo->item;
			iks_ast_node_value_t *n1 = $1->item;
			iks_ast_node_value_t *n2 = $3->item;
			int type = infer_type($1, $3);
			if(type > 5) //erro de coerção
				return type;
			oon->iks_type = type;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| expr '/' expr
		{
			/* 3.A.12 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_DIVISAO,NULL);
			iks_ast_node_value_t *oon = oo->item;
			iks_ast_node_value_t *n1 = $1->item;
			iks_ast_node_value_t *n2 = $3->item;
			int type = infer_type($1, $3);
			if(type > 5) //erro de coerção
				return type;
			oon->iks_type = type;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| '-' expr %prec INVERSAO
		{
			/* 3.A.15 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_ARIM_INVERSAO,NULL);
			iks_ast_node_value_t *oon = oo->item;
			iks_ast_node_value_t *exprn = $2->item;
			oon->iks_type = exprn->iks_type;
			iks_ast_connect_nodes(oo,$2);
			$$ = oo;
		}
  ;

logic_expr:
	expr '<' expr
		{
			/* 3.A.14 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_L,NULL);
			iks_ast_node_value_t *oon = oo->item;
			iks_ast_node_value_t *n1 = $1->item;
			iks_ast_node_value_t *n2 = $3->item;
			int type = infer_type($1, $3);
			if(type > 5) //erro de coerção
				return type;
			oon->iks_type = type;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| expr '>' expr
		{
			/* 3.A.14 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_G,NULL);
			iks_ast_node_value_t *oon = oo->item;
			iks_ast_node_value_t *n1 = $1->item;
			iks_ast_node_value_t *n2 = $3->item;
			int type = infer_type($1, $3);
			if(type > 5) //erro de coerção
				return type;
			oon->iks_type = type;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| '!' expr
		{
			/* 3.A.15 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_NEGACAO,NULL);
			iks_ast_node_value_t *oon = oo->item;
			oon->iks_type = IKS_BOOL;
			iks_ast_connect_nodes(oo,$2);
			$$ = oo;
		}
	| expr TK_OC_LE expr
		{
			/* 3.A.14 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_LE,NULL);
			iks_ast_node_value_t *oon = oo->item;
			oon->iks_type = IKS_BOOL;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| expr TK_OC_GE expr
		{
			/* 3.A.14 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_GE,NULL);
			iks_ast_node_value_t *oon = oo->item;
			oon->iks_type = IKS_BOOL;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| expr TK_OC_EQ expr
		{
			/* 3.A.14 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_IGUAL,NULL);
			iks_ast_node_value_t *oon = oo->item;
			oon->iks_type = IKS_BOOL;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| expr TK_OC_NE expr
		{
			/* 3.A.14 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_COMP_DIF,NULL);
			iks_ast_node_value_t *oon = oo->item;
			oon->iks_type = IKS_BOOL;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| expr TK_OC_AND expr
		{
			/* 3.A.14 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_E,NULL);
			iks_ast_node_value_t *oon = oo->item;
			oon->iks_type = IKS_BOOL;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	| expr TK_OC_OR expr
		{
			/* 3.A.14 */
			iks_tree_t *oo = iks_ast_new_node(IKS_AST_LOGICO_OU,NULL);
			iks_ast_node_value_t *oon = oo->item;
			oon->iks_type = IKS_BOOL;
			iks_ast_connect_nodes(oo,$1);
			iks_ast_connect_nodes(oo,$3);
			$$ = oo;
		}
	;

func_call:
		id {
			iks_ast_node_value_t *n;
			n = $1->item; 
			iks_grammar_symbol_t *s;
			s = n->symbol;
			/* 3.A.17 */
			if(symbol_is_decl_type(s,IKS_DECL_FUNCTION)) {
				iks_tree_t *x = iks_ast_new_node(IKS_AST_CHAMADA_DE_FUNCAO,NULL);
				iks_ast_node_value_t *xn = x->item;
				xn->iks_type = n->iks_type;
				iks_ast_connect_nodes(x,$1);
				ptr_function_call=x;
			} else {
				return iks_error(s,IKS_ERROR_USE);
			}
			args = new_iks_list();
			function_with_param=s;
		} '(' func_param_list ')' {
			// not so good, better if analyze during parser?
			int arg_analyze = verify_function_args(function_with_param,args);
			function_with_param=NULL;
			if (arg_analyze==0) {
				if ($func_param_list) { //if no params, so NULL
					iks_ast_connect_nodes(ptr_function_call,$func_param_list);
				}
				$$ = ptr_function_call;
			} else {
				return arg_analyze; //arg error
			}
			//iks_list_delete(args);
		}
	;

terminal_value:
	/* 3.A.11 */
		TK_LIT_INT
		{
			$1->iks_type=IKS_INT;
			iks_tree_t *lit = iks_ast_new_node(IKS_AST_LITERAL,$1);
			iks_ast_node_value_t *litn = lit->item;
			litn->iks_type = $1->iks_type;
			$$ = lit;
		}
	| TK_LIT_FLOAT
		{
			$1->iks_type=IKS_FLOAT;
			iks_tree_t *lit = iks_ast_new_node(IKS_AST_LITERAL,$1);
			iks_ast_node_value_t *litn = lit->item;
			litn->iks_type = $1->iks_type;
			$$ = lit;
		}
	| TK_LIT_FALSE
		{
			$1->iks_type=IKS_BOOL;
			iks_ast_node_value_t *v1;
			v1 = new_iks_ast_node_value();
			iks_ast_node_value_set(v1,IKS_AST_LITERAL,$1);
			iks_tree_t *lit;
			lit = new_iks_tree();
			iks_tree_set_item(lit,(void*)v1);
			iks_ast_node_value_t *litn = lit->item;
			litn->iks_type = $1->iks_type;
			gv_declare(IKS_AST_LITERAL,lit,"false");
			$$ = lit;
		}
	| TK_LIT_TRUE
		{
			$1->iks_type=IKS_BOOL;
			iks_ast_node_value_t *v1;
			v1 = new_iks_ast_node_value();
			iks_ast_node_value_set(v1,IKS_AST_LITERAL,$1);
			iks_tree_t *lit;
			lit = new_iks_tree();
			iks_tree_set_item(lit,(void*)v1);
			iks_ast_node_value_t *litn = lit->item;
			litn->iks_type = $1->iks_type;
			gv_declare(IKS_AST_LITERAL,lit,"true");
			$$ = lit;
		}
	| TK_LIT_CHAR
		{
			$1->iks_type=IKS_CHAR;
			iks_tree_t *lit = iks_ast_new_node(IKS_AST_LITERAL,$1);
			iks_ast_node_value_t *litn = lit->item;
			litn->iks_type = $1->iks_type;
			$$ = lit;
		}
	| TK_LIT_STRING
		{
			$1->iks_type=IKS_STRING;
			iks_tree_t *lit = iks_ast_new_node(IKS_AST_LITERAL,$1);
			iks_ast_node_value_t *litn = lit->item;
			litn->iks_type = $1->iks_type;
			$$ = lit;
		}
	;

func_param_list:
		param_list
	| { $$ = NULL; } /* empty */
	;

param_list:
		expr
		{
			iks_ast_node_value_t *n;
			n = $1->item; 
			iks_grammar_symbol_t *s;
			s = n->symbol;
			iks_list_t *l; 
			l = new_iks_list();
			iks_list_set_item(l,(void*)s);
			iks_list_insert(args,l);
		}
	| expr ',' param_list
		{
			if ($3) { //because can command_seq <- command <- empty
				iks_ast_node_value_t *n;
				n = $1->item; 
				iks_grammar_symbol_t *s;
				s = n->symbol;
				iks_list_t *l; 
				l = new_iks_list();
				iks_list_set_item(l,(void*)s);     
				iks_list_insert(args,l);
				iks_ast_connect_nodes($1,$3);
			}
		}
	;

/* 2.6 */
ctrl_flow:
		TK_PR_IF '(' logic_expr ')' TK_PR_THEN commands
		{
			/* 3.A.3 */
			$<nt>0 = iks_ast_new_node(IKS_AST_IF_ELSE,NULL);
			iks_ast_connect_nodes($<nt>0,$3);
			iks_ast_connect_nodes($<nt>0,$6);
			code_generator(&($<nt>0));
			$$ = $<nt>0;
		}
	| TK_PR_IF '(' logic_expr ')' TK_PR_THEN commands TK_PR_ELSE commands
		{
			/* 3.A.3 */
			iks_tree_t *if_else = iks_ast_new_node(IKS_AST_IF_ELSE,NULL);
			iks_ast_connect_nodes(if_else,$3);
			iks_ast_connect_nodes(if_else,$6);
			iks_ast_connect_nodes(if_else,$8);
			$$ = if_else;
		}
	| TK_PR_WHILE '(' logic_expr ')' TK_PR_DO commands
		{
			/* 3.A.5 */
			iks_tree_t *while_do = iks_ast_new_node(IKS_AST_WHILE_DO,NULL);
			iks_ast_connect_nodes(while_do,$3);
			iks_ast_connect_nodes(while_do,$6);
			$$ = while_do;
		}
	| TK_PR_DO commands TK_PR_WHILE '(' logic_expr ')' 
		{
			/* 3.A.4 */
			iks_tree_t *do_while = iks_ast_new_node(IKS_AST_DO_WHILE,NULL);
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
