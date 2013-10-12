#include <stdio.h>
#include <stdlib.h>
#include "comp_grammar.h"
#include "comp_tree.h"
#include "iks_ast.h"
#include "iks_types.h"

int verify_coercion(comp_tree_t *id, comp_tree_t *expr) {
  int ret=0;
  iks_ast_node_value_t *idn,*exprn;
  idn = id->item;
  exprn = expr->item;
  comp_grammar_symbol_t *ids,*exprs;
  ids = idn->symbol;
  exprs = exprn->symbol;
  if (exprs) { //gambi because call function has no symbol
    if((ids->iks_type == IKS_INT)&&(exprs->iks_type == IKS_BOOL)) {
      //printf("coercion int to bool\n");
      idn->need_coercion=IKS_COERCION_INT_TO_BOOL;
    }
    else if((ids->iks_type == IKS_INT)&&(exprs->iks_type == IKS_FLOAT)) {
      //printf("coercion int to float\n");
      idn->need_coercion=IKS_COERCION_INT_TO_FLOAT;
    }
    else if((ids->iks_type == IKS_FLOAT)&&(exprs->iks_type == IKS_BOOL)) {
      //printf("coercion float to bool\n");
      idn->need_coercion=IKS_COERCION_FLOAT_TO_BOOL;
    }
    else if((ids->iks_type == IKS_FLOAT)&&(exprs->iks_type == IKS_INT)) {
      //printf("coercion float to int\n");
      idn->need_coercion=IKS_COERCION_FLOAT_TO_INT;
    }
    else if((ids->iks_type == IKS_BOOL)&&(exprs->iks_type == IKS_INT)) {
      //printf("coercion bool to int\n");
      idn->need_coercion=IKS_COERCION_BOOL_TO_INT;
    }
    else if((ids->iks_type == IKS_BOOL)&&(exprs->iks_type == IKS_FLOAT)) {
      //printf("coercion bool to float\n");
      idn->need_coercion=IKS_COERCION_BOOL_TO_FLOAT;
    }
    else if((exprs->iks_type == IKS_CHAR)) {
      fprintf(stderr,"%d: identificador '%s' conversao impossivel do tipo char\n",exprs->code_line_number,exprs->value);
        ret=IKS_ERROR_CHAR_TO_X;
    }
    else if((ids->iks_type != IKS_STRING)&&(exprs->iks_type == IKS_STRING)) {
      fprintf(stderr,"%d: identificador '%s' conversao impossivel do tipo string\n",exprs->code_line_number,exprs->value);
        ret=IKS_ERROR_STRING_TO_X;
    }
    //else if ((ids->iks_type==IKS_INT) ||\
    //          (ids->iks_type==IKS_FLOAT) ||\
    //          (ids->iks_type==IKS_BOOL)) {
    else {
      fprintf(stderr,"identificador '%s' e '%s' de tipos incompativeis\n",ids->value,exprs->value);
        ret=IKS_ERROR_WRONG_TYPE;
    }
  }
  else {
    printf("expr is null at coercion\n");
  }
  return ret;
}

int verify_function_args(comp_grammar_symbol_t *s, comp_list_t *args) {
  int ret=0;
  comp_grammar_symbol_t *s1,*s2;
  comp_list_t *l1,*l2;
  int sl1,sl2,diff;
  l1 = s->params->next;
  l2 = args->next;
  sl1 = comp_list_size(s->params->next);
  sl2 = comp_list_size(args->next);
  diff = sl1-sl2;
  if (diff!=0) {
    if (sl1>sl2) {
      fprintf(stderr,"faltam %d argumentos em '%s'\n",diff,s->value);
      ret=IKS_ERROR_MISSING_ARGS;
    }
    else {
      fprintf(stderr,"sobram %d argumentos em '%s'\n",diff*-1,s->value);
      ret=IKS_ERROR_EXCESS_ARGS;
    }
  }
  else if (sl1!=0){
    do {
       s1 = l1->item;
       s2 = l2->item;
       if (s1->iks_type!=s2->iks_type) {
        fprintf(stderr,"tipos incompativeis entre '%s' e '%s'\n",s1->value,s2->value);
        ret=IKS_ERROR_EXCESS_ARGS;
        break;
       }
       l1 = l1->next;
       l2 = l2->next;
    } while(l1 != s->params);
  }
  return ret;
}

int symbol_is_iks_type(comp_grammar_symbol_t *s,int iks_type) {
  int ret=1;
  //printf("%d vs %d\n",s->iks_type,iks_type);
  if (!(s->iks_type==iks_type)) {
    ret=0;
  }
  return ret;
}

int infer_type(int iks_type1, int iks_type2) {
	if(iks_type1 == iks_type2)
		return iks_type1;
	if(iks_type1 == IKS_FLOAT || iks_type2 == IKS_FLOAT)
		return IKS_FLOAT;
	
	return IKS_INT;
}

