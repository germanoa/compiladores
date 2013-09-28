/*
  iks_ast.h

  Este arquivo contém as constantes para os tipos dos nós da AST.
*/
#ifndef __IKS_AST_H
#define __IKS_AST_H
#define IKS_AST_PROGRAMA             0
#define IKS_AST_FUNCAO               1
#define IKS_AST_IF_ELSE              2
#define IKS_AST_DO_WHILE             3
#define IKS_AST_WHILE_DO             4
#define IKS_AST_INPUT                5
#define IKS_AST_OUTPUT               6
#define IKS_AST_ATRIBUICAO           7
#define IKS_AST_RETURN               8
#define IKS_AST_BLOCO                9
#define IKS_AST_IDENTIFICADOR       10
#define IKS_AST_LITERAL             11
#define IKS_AST_ARIM_SOMA           12
#define IKS_AST_ARIM_SUBTRACAO      13
#define IKS_AST_ARIM_MULTIPLICACAO  14
#define IKS_AST_ARIM_DIVISAO        15
#define IKS_AST_ARIM_INVERSAO       16 // - (operador unário -)
#define IKS_AST_LOGICO_E            17 // &&
#define IKS_AST_LOGICO_OU           18 // ||
#define IKS_AST_LOGICO_COMP_DIF     19 // !=
#define IKS_AST_LOGICO_COMP_IGUAL   20 // ==
#define IKS_AST_LOGICO_COMP_LE      21 // <=
#define IKS_AST_LOGICO_COMP_GE      22 // >=
#define IKS_AST_LOGICO_COMP_L       23 // <
#define IKS_AST_LOGICO_COMP_G       24 // >
#define IKS_AST_LOGICO_COMP_NEGACAO 25 // !
#define IKS_AST_VETOR_INDEXADO      26 // para var[exp] quando o índice exp é acessado no vetor var
#define IKS_AST_CHAMADA_DE_FUNCAO   27
#define IKS_AST_INDEFINIDO          -1

#include "comp_tree.h"
#include "comp_grammar.h"

comp_tree_t *ast;

/**
 * AST node struct
 */
typedef struct iks_ast_node_value_t iks_ast_node_value_t;
struct iks_ast_node_value_t {
    int type;
    comp_grammar_symbol_t *symbol;
};

/**
 * initialize ast_node
 */
static inline void __iks_ast_node_value_init(iks_ast_node_value_t *iks_ast_node_value);
/**
 * create a iks_ast_node_value, alloc mem and return address
 */
iks_ast_node_value_t *new_iks_ast_node_value();
/**
 * free address memory used by iks_ast_node_value
 */
void iks_ast_node_value_delete(iks_ast_node_value_t *iks_ast_node_value);

/**
 * set iks_ast_node_value
 */
void iks_ast_node_value_set(iks_ast_node_value_t *iks_ast_node_value, int type, comp_grammar_symbol_t *symbol);

/**
 * print iks_ast_node_value
 */
void iks_ast_node_value_print(iks_ast_node_value_t *iks_ast_node_value);

/**
 * append a ast_node to another ast_node
 */
void iks_ast_append(comp_tree_t *parent, comp_tree_t *child);

/**
 * append a ast_node_value as child(create one) to another ast_node
 */
void iks_ast_append_value(comp_tree_t *parent, iks_ast_node_value_t *child_value);

/**
 * initialize the ast
 */
void iks_ast_init();

/**
 * collection of procedures to make a ast node
 */
comp_tree_t *iks_ast_new_node(int type, comp_grammar_symbol_t *symbol);

/**
 * collection of procedures to connect two ast nodes
 */
void iks_ast_connect_nodes(comp_tree_t *parent, comp_tree_t *child);

#endif
