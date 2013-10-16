#include <stdio.h>
#include <stdlib.h>
#include "comp_grammar.h"
#include "comp_tree.h"
#include "iks_ast.h"
#include "iks_types.h"
#include "gv.h"

static inline void __iks_ast_node_value_init(iks_ast_node_value_t *iks_ast_node_value) {
    iks_ast_node_value->type = IKS_AST_INDEFINIDO;
    iks_ast_node_value->symbol = NULL;
    iks_ast_node_value->need_coercion = 0;
}

iks_ast_node_value_t *new_iks_ast_node_value() {
    iks_ast_node_value_t *iks_ast_node_value;
    iks_ast_node_value = malloc(sizeof(iks_ast_node_value_t));
    __iks_ast_node_value_init(iks_ast_node_value);
    return iks_ast_node_value;
}

void iks_ast_node_value_delete(iks_ast_node_value_t *iks_ast_node_value) {
   // CONTINUAR DAQUI
   // if(iks_ast_node_value->symbol) {
   //   comp_grammar_symbol_t *s = iks_ast_node_value->symbol;
   //   //printf("deleting symbol at ast: %X %X %X\n",*s,s,&s);
   //   comp_grammar_symbol_delete(&s);
   // }
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
    //gv_connect(parent,child);
}

void iks_ast_delete(comp_tree_t *ast) {
  //printf ("deleting ast\n");
  if(ast) {
    if(ast->item) {
      iks_ast_node_value_delete(ast->item);
    }
    if(ast->children) {
      comp_list_t *childList = ast->children;
      do { // runs through every child
        childList = childList->next;
        iks_ast_delete(childList->item);
      } while(childList != ast->children);
    }
    comp_list_delete(ast->children);
    free(ast);
    ast = NULL;
  }
}


void iks_ast_append_value(comp_tree_t *parent, iks_ast_node_value_t *child_value) {
    comp_tree_create_child_with_item(parent,(void*)child_value);
}

void iks_ast_init() {
    ast = new_comp_tree();
}

comp_tree_t *iks_ast_new_node(int type, comp_grammar_symbol_t *symbol, comp_tree_t *t) {
  iks_ast_node_value_t *v;
  v = new_iks_ast_node_value();
  iks_ast_node_value_set(v,type,symbol);
	if (!t) {
  	t = new_comp_tree(); //any case != IKS_AST_PROGRAMA
	}
  comp_tree_set_item(t,(void*)v);
  if (symbol) {
    gv_declare(type,t,symbol->value);
  }
  else {
    gv_declare(type,t,NULL);
  }
  return t;
}

void iks_ast_connect_nodes(comp_tree_t *parent, comp_tree_t *child) {
  iks_ast_append(parent,child);
  gv_connect(parent,child);
}
