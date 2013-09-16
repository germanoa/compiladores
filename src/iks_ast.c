#include <stdio.h>
#include <stdlib.h>
#include "comp_grammar.h"
#include "comp_tree.h"
#include "iks_ast.h"

static inline void __iks_ast_node_value_init(iks_ast_node_value_t *iks_ast_node_value) {
    iks_ast_node_value->type = IKS_AST_INDEFINIDO;
    iks_ast_node_value->symbol = NULL;
}

iks_ast_node_value_t *new_iks_ast_node_value() {
    iks_ast_node_value_t *iks_ast_node_value;
    iks_ast_node_value = malloc(sizeof(iks_ast_node_value_t));
    __iks_ast_node_value_init(iks_ast_node_value);
    return iks_ast_node_value;
}

void iks_ast_node_value_delete(iks_ast_node_value_t *iks_ast_node_value) {
    free(iks_ast_node_value->symbol);
    iks_ast_node_value->symbol = NULL;
    free(iks_ast_node_value);
    iks_ast_node_value = NULL;
}

void iks_ast_node_value_set(iks_ast_node_value_t *iks_ast_node_value, int type, comp_grammar_symbol_t *symbol) {
    iks_ast_node_value->type = type;
    iks_ast_node_value->symbol = symbol;
}

//void iks_ast_node_value_print(iks_ast_node_value_t *iks_ast_node_value) {
//    printf("%s\n",(char *)iks_ast_node_value->value);
//}

void iks_ast_append(comp_tree_t *parent, comp_tree_t *child) {
    comp_tree_append(parent,child);
}

void iks_ast_append_value(comp_tree_t *parent, iks_ast_node_value_t *child_value) {
    comp_tree_create_child_with_item(parent,(void*)child_value);
}

void iks_ast_init() {
    ast = new_comp_tree();
}

//void iks_ast_print() {
//    comp_dict_print(iks_ast);
//}

