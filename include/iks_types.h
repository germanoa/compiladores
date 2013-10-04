/*
  iks_types.h
*/
#ifndef __IKS_TYPES_H
#define __IKS_TYPES_H

#include "comp_stack.h"
#include "comp_tree.h"

#define IKS_NOTYPE 0
#define IKS_INT 1
#define IKS_FLOAT 2
#define IKS_CHAR 3
#define IKS_STRING 4
#define IKS_BOOL 5

#define IKS_DECL_VAR 1
#define IKS_DECL_VECTOR 2
#define IKS_DECL_FUNCTION 3

#define IKS_ERROR_UNDECLARED 1
#define IKS_ERROR_DECLARED 2
#define IKS_ERROR_VARIABLE 3
#define IKS_ERROR_VECTOR 4
#define IKS_ERROR_FUNCTION 5
#define IKS_ERROR_WRONG_TYPE 6
#define IKS_ERROR_STRING_TO_X 7
#define IKS_ERROR_CHAR_TO_X 8
#define IKS_ERROR_MISSING_ARGS 9
#define IKS_ERROR_EXCESS_ARGS 10
#define IKS_ERROR_WRONG_TYPE_ARGS 11
#define IKS_ERROR_WRONG_PAR_INPUT 12
#define IKS_ERROR_WRONG_PAR_OUTPUT 13
#define IKS_ERROR_WRONG_PAR_RETURN 14

#define IKS_ERROR_USE 100
#define IKS_ERROR_DECL 101
#define IKS_ERROR_TYPE 102

#define IKS_COERCION_INT_TO_FLOAT 1
#define IKS_COERCION_INT_TO_BOOL 2
#define IKS_COERCION_FLOAT_TO_INT 3
#define IKS_COERCION_FLOAT_TO_BOOL 4
#define IKS_COERCION_BOOL_TO_INT 5
#define IKS_COERCION_BOOL_TO_FLOAT 6

/* stack of symbol_tables(scope) */
comp_stack_t *scope; 

int verify_coercion(comp_tree_t *id, comp_tree_t *expr);

#endif
