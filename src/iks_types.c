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
     // printf("coercion int to bool\n");
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
      fprintf(stderr,"identificador '%s' e '%s' de tipos incompativeis\n",ids->value,ids->iks_type,exprs->value,exprs->iks_type);
        ret=IKS_ERROR_WRONG_TYPE;
    }
  }
  return ret;
}

