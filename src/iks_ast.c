#include <stdio.h>
#include <stdlib.h>
#include "iks_grammar.h"
#include "iks_tree.h"
#include "iks_ast.h"
#include "iks_gv.h"

static inline void __iks_ast_node_value_init(iks_ast_node_value_t *iks_ast_node_value) {
    iks_ast_node_value->type = IKS_AST_INDEFINIDO;
    iks_ast_node_value->symbol = NULL;
    iks_ast_node_value->need_coercion = 0;
		iks_ast_node_value->code = new_iks_list();
		iks_ast_node_value->temp.b.t = NULL;
		iks_ast_node_value->temp.b.f = NULL;
		iks_ast_node_value->temp.name = NULL;
		iks_ast_node_value->temp.next = NULL;
		iks_ast_node_value->temp.local = NULL;
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

void iks_ast_node_value_set(iks_ast_node_value_t *iks_ast_node_value, int type, iks_grammar_symbol_t *symbol) {
    iks_ast_node_value->type = type;
    iks_ast_node_value->symbol = symbol;
}

//void iks_ast_node_value_print(iks_ast_node_value_t *iks_ast_node_value) {
//    printf("%s\n",(char *)iks_ast_node_value->value);
//}

void iks_ast_append(iks_tree_t *parent, iks_tree_t *child) {
    iks_tree_append(parent,child);
    //gv_connect(parent,child);
}

void iks_ast_append_value(iks_tree_t *parent, iks_ast_node_value_t *child_value) {
    iks_tree_create_child_with_item(parent,(void*)child_value);
}

void iks_ast_init() {
    ast = new_iks_tree();
}

iks_tree_t *iks_ast_new_node(int type, iks_grammar_symbol_t *symbol) {
  iks_ast_node_value_t *v;
  v = new_iks_ast_node_value();
  iks_ast_node_value_set(v,type,symbol);
  iks_tree_t *t;
  t = new_iks_tree();
  iks_tree_set_item(t,(void*)v);
  if (symbol) {
    gv_declare(type,t,symbol->value);
  }
  else {
  	if(type != IKS_AST_INDEFINIDO) { //no need to draw undefined nodes
    	gv_declare(type,t,NULL);
  	}
  }
  return t;
}

void iks_ast_connect_nodes(iks_tree_t *parent, iks_tree_t *child) {
  iks_ast_append(parent,child);
  gv_connect(parent,child);
}

void ast_set_temp(int t,char *v, iks_tree_t **ast) {
	iks_ast_node_value_t *ast_n = (*ast)->item;
	switch(t) {
		case TEMP_NAME:
			ast_n->temp.name = v;
			break;					
		case TEMP_NEXT:
			ast_n->temp.next = v;
			break;					
		case TEMP_BT:
			ast_n->temp.b.t = v;
			break;					
		case TEMP_BF:
			ast_n->temp.b.f = v;
			break;					
		case TEMP_LOCAL:
			ast_n->temp.local = v;
			break;					
		case TEMP_BEGIN:
			ast_n->temp.begin = v;
			break;					
	}
}

char *ast_get_temp(int t, iks_tree_t **ast) {
	char *c;
	iks_ast_node_value_t *ast_n = (*ast)->item;
	switch(t) {
		case TEMP_NAME:
			c = ast_n->temp.name;
			break;					
		case TEMP_NEXT:
			c = ast_n->temp.next;
			break;					
		case TEMP_BT:
			c = ast_n->temp.b.t;
			break;					
		case TEMP_BF:
			c = ast_n->temp.b.f;
			break;					
	}
	return c;
}

reg_or_label *new_reg_or_label() {
    reg_or_label *rl;
    rl = malloc(sizeof(reg_or_label));
    return rl;
}

void delete_reg_or_label(reg_or_label **rl) {
    free(rl);
}
