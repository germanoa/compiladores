#include <stdio.h>
#include <stdlib.h>
#include "iks_iloc.h"
#include "iks_ast.h"
#include "iks_tree.h"

iks_list_t *label_code_generator(char *l) {
	iks_list_t *code;
	code = new_iks_list();
	iks_list_set_item(code,l);
	return code;
}

void code_if(iks_tree_t **ast) {
	iks_ast_node_value_t *ctrl_flow_n = (*ast)->item;
	iks_ast_node_value_t *logic_expr_n = (*ast)->children->item;
	iks_ast_node_value_t *commands_n = (*ast)->children->next->item;
	
	logic_expr_n->temp.b.t = label_generator();
	logic_expr_n->temp.b.f = ctrl_flow_n->temp.next;
	
	commands_n->temp.next = ctrl_flow_n->temp.next;
	
	iks_list_concat(logic_expr_n->code,label_code_generator(logic_expr_n->temp.b.t));
	iks_list_concat(logic_expr_n->code,commands_n->code);
	ctrl_flow_n->code = logic_expr_n->code;	
}

void code_generator(iks_tree_t **ast) {
	iks_list_t *code;
	code = new_iks_list();

	iks_ast_node_value_t *n;
	n = (*ast)->item;
	switch(n->type) {
		case IKS_AST_PROGRAMA:
		case IKS_AST_FUNCAO:
		case IKS_AST_IF_ELSE:
			code_if(ast);
			break;
		case IKS_AST_DO_WHILE:
		case IKS_AST_WHILE_DO:
		case IKS_AST_INPUT:
		case IKS_AST_OUTPUT:
		case IKS_AST_ATRIBUICAO:
		case IKS_AST_RETURN:
		case IKS_AST_BLOCO:
		case IKS_AST_IDENTIFICADOR:
		case IKS_AST_LITERAL:
		case IKS_AST_ARIM_SOMA:
		case IKS_AST_ARIM_SUBTRACAO:
		case IKS_AST_ARIM_MULTIPLICACAO:
		case IKS_AST_ARIM_DIVISAO:
		case IKS_AST_ARIM_INVERSAO:
		case IKS_AST_LOGICO_E:
		case IKS_AST_LOGICO_OU:
		case IKS_AST_LOGICO_COMP_DIF:
		case IKS_AST_LOGICO_COMP_IGUAL:
		case IKS_AST_LOGICO_COMP_LE:
		case IKS_AST_LOGICO_COMP_GE:
		case IKS_AST_LOGICO_COMP_L:
		case IKS_AST_LOGICO_COMP_G:
		case IKS_AST_LOGICO_COMP_NEGACAO:
		case IKS_AST_VETOR_INDEXADO:
		case IKS_AST_CHAMADA_DE_FUNCAO:
		case IKS_AST_INDEFINIDO:
		default:
			break;
	}
}

char *label_generator() {
	char *teste;
	teste = malloc(2);
	teste = "l0";
	return teste;
}
