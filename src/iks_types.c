#include <stdio.h>
#include <stdlib.h>
//#include "iks_grammar.h"
#include "iks_tree.h"
#include "iks_stack.h"
#include "iks_ast.h"
#include "iks_types.h"

static inline void __scope_init(scope_t *scope) {
	//scope->st = new_iks_stack();
	scope->st = new_iks_dict();
	scope->next_addr = 0;
}

scope_t *new_scope() {
	scope_t *scope;
	scope = malloc(sizeof(scope_t));
  __scope_init(scope);
	return scope;
}

int verify_coercion(iks_tree_t *id, iks_tree_t *expr) {
  int ret=0;
  iks_ast_node_value_t *idn,*exprn;
  idn = id->item;
  exprn = expr->item;
  iks_grammar_symbol_t *ids,*exprs;
  ids = idn->symbol;
  exprs = exprn->symbol;
	
	if(idn->iks_type != exprn->iks_type) {
		if((idn->iks_type == IKS_INT)&&(exprn->iks_type == IKS_BOOL)) {
		  idn->need_coercion=IKS_COERCION_INT_TO_BOOL;
		} else if((idn->iks_type == IKS_INT)&&(exprn->iks_type == IKS_FLOAT)) {
		  //printf("coercion int to float\n");
		  idn->need_coercion=IKS_COERCION_INT_TO_FLOAT;
		} else if((idn->iks_type == IKS_FLOAT)&&(exprn->iks_type == IKS_BOOL)) {
		  //printf("coercion float to bool\n");
		  idn->need_coercion=IKS_COERCION_FLOAT_TO_BOOL;
		} else if((idn->iks_type == IKS_FLOAT)&&(exprn->iks_type == IKS_INT)) {
		  //printf("coercion float to int\n");
		  idn->need_coercion=IKS_COERCION_FLOAT_TO_INT;
		} else if((idn->iks_type == IKS_BOOL)&&(exprn->iks_type == IKS_INT)) {
		  //printf("coercion bool to int\n");
		  idn->need_coercion=IKS_COERCION_BOOL_TO_INT;
		} else if((idn->iks_type == IKS_BOOL)&&(exprn->iks_type == IKS_FLOAT)) {
		  //printf("coercion bool to float\n");
		  idn->need_coercion=IKS_COERCION_BOOL_TO_FLOAT;
		} else if((exprn->iks_type == IKS_CHAR)) {
			if(exprs)
		  	fprintf(stderr,"%d: identificador '%s' conversao impossivel do tipo char\n",exprs->code_line_number,exprs->value);
			else fprintf(stderr,"conversao impossivel do tipo char\n");
		  ret=IKS_ERROR_CHAR_TO_X;
		} else if((idn->iks_type != IKS_STRING)&&(exprn->iks_type == IKS_STRING)) {
			if(exprs)
		  	fprintf(stderr,"%d: identificador '%s' conversao impossivel do tipo string\n",exprs->code_line_number,exprs->value);
			else fprintf(stderr,"conversao impossivel do tipo string\n");
		  ret=IKS_ERROR_STRING_TO_X;
		} else {
			if(exprs && ids)
				fprintf(stderr,"identificador '%s' e '%s' de tipos incompativeis\n",ids->value,exprs->value);
			else fprintf(stderr,"identificadores de tipos incompativeis\n");
		  ret=IKS_ERROR_WRONG_TYPE;
		}
	}
  
  return ret;
}

//int verify_function_args(iks_grammar_symbol_t *s, iks_list_t *args) {
//  int ret=0;
//  iks_grammar_symbol_t *s1,*s2;
//  iks_list_t *l1,*l2;
//  int sl1,sl2,diff;
//  l1 = s->params->next;
//  l2 = args->next;
//  sl1 = iks_list_size(s->params->next);
//  sl2 = iks_list_size(args->next);
//  diff = sl1-sl2;
//  if (diff!=0) {
//    if (sl1>sl2) {
//      fprintf(stderr,"faltam %d argumentos em '%s'\n",diff,s->value);
//      ret=IKS_ERROR_MISSING_ARGS;
//    }
//    else {
//      fprintf(stderr,"sobram %d argumentos em '%s'\n",diff*-1,s->value);
//      ret=IKS_ERROR_EXCESS_ARGS;
//    }
//  }
//  else if (sl1!=0){
//    do {
//       s1 = l1->item;
//       s2 = l2->item;
//       if (s1->iks_type!=s2->iks_type) {
//        fprintf(stderr,"tipos incompativeis entre '%s' e '%s'\n",s1->value,s2->value);
//        ret=IKS_ERROR_WRONG_TYPE_ARGS;
//        break;
//       }
//       l1 = l1->next;
//       l2 = l2->next;
//    } while(l1 != s->params);
//  }
//  return ret;
//}
//
//int symbol_is_iks_type(iks_grammar_symbol_t *s,int iks_type) {
//  int ret=1;
//  //printf("%d vs %d\n",s->iks_type,iks_type);
//  if (!(s->iks_type==iks_type)) {
//    ret=0;
//  }
//  return ret;
//}

int infer_type(iks_tree_t *tree1, iks_tree_t *tree2) {
	iks_ast_node_value_t *tree1n = tree1->item;
	iks_ast_node_value_t *tree2n = tree2->item;
	
	if(tree1n->iks_type == tree2n->iks_type)
		return tree1n->iks_type;
		
	if(tree1n->iks_type == IKS_FLOAT) {
		int coercion = verify_coercion(tree1, tree2);
		if(coercion)
			return coercion;
		
		return IKS_FLOAT;
	} else if(tree2n->iks_type == IKS_FLOAT) {
		int coercion = verify_coercion(tree2, tree1);
		if(coercion)
			return coercion;
		
		return IKS_FLOAT;
	}
	
	if(tree1n->iks_type == IKS_INT) {
		int coercion = verify_coercion(tree1, tree2);
		if(coercion)
			return coercion;
		
		return IKS_INT;
	} else if(tree2n->iks_type == IKS_INT) {
		int coercion = verify_coercion(tree2, tree1);
		if(coercion)
			return coercion;
		
		return IKS_INT;
	}
}

