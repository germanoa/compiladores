#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iks_iloc.h"
#include "iks_ast.h"
#include "iks_tree.h"
#include "parser.h"

void code_programa(iks_tree_t **ast) {

  //TODO: global_decl
  
}

void code_funcao(iks_tree_t **ast) {
	iks_ast_node_value_t *F = (*ast)->item;
	iks_tree_t *St = (*ast)->children->item;
	if (St) {
		iks_ast_node_value_t *S = St->item;
		F->code = iks_list_concat(F->code,S->code);
	}
}

void code_literal(iks_tree_t **ast) {
	iks_ast_node_value_t *S = (*ast)->item;

  iloc_t *iloc;
  switch(S->symbol->token_type) {
    case TK_LIT_TRUE:
      //gera(goto B.t)
      iloc = new_iloc(NULL, new_iloc_oper(jumpI,NULL,NULL,NULL,S->temp.b.t,NULL,NULL));
      iks_list_append(S->code,(void*)iloc);
      break;
    case TK_LIT_FALSE:
      //gera(goto B.f)
      iloc = new_iloc(NULL, new_iloc_oper(jumpI,NULL,NULL,NULL,S->temp.b.f,NULL,NULL));
      iks_list_append(S->code,(void*)iloc);
      break;
    default:
      printf("ops at code_literal\n");
  }
}
  
void code_or(iks_tree_t **ast) {
	iks_ast_node_value_t *or_n = (*ast)->item;
	iks_tree_t *e1 = (*ast)->children->item;
	iks_ast_node_value_t *e1_n = e1->item;
	iks_tree_t *e2 = (*ast)->children->next->item;
	iks_ast_node_value_t *e2_n = e2->item;

	
	e1_n->temp.b.t = or_n->temp.b.t;
	e1_n->temp.b.f = label_generator();

	e2_n->temp.b.t = or_n->temp.b.t;
	e2_n->temp.b.f = or_n->temp.b.f;

  label_insert(e2_n->code,e1_n->temp.b.f);
	e1_n->code = iks_list_concat(e1_n->code,e2_n->code);	

	or_n->code = e1_n->code;
}

void code_if(iks_tree_t **ast){
	iks_ast_node_value_t *S = (*ast)->item;
	iks_tree_t *Bt = (*ast)->children->item;
	iks_ast_node_value_t *B = Bt->item;
	iks_tree_t *S1t = (*ast)->children->next->item;
	iks_ast_node_value_t *S1 = S1t->item;

	// X = gera(B.t) || S1.code
  label_insert(S1->code,B->temp.b.t);
	// S.code = B.code || X
	S->code = iks_list_concat(B->code,S1->code);

	// adicionamos gera(B.f) por compatibilidade com if_else
	// pois B.f teria que ser S.next mas teve q ser rot()
  //S.code = B.code || gera(B.t) || S1.code || gera(B.f)
  iloc_t *B_f = new_iloc(NULL, new_iloc_oper(nop,NULL,NULL,NULL,NULL,NULL,NULL));
	iks_list_t *gambi = new_iks_list();
  iks_list_append(gambi,(void*)B_f);
  label_insert(gambi,B->temp.b.f);
	S->code = iks_list_concat(S->code,gambi);

}

void code_if_else(iks_tree_t **ast) {
	iks_ast_node_value_t *S = (*ast)->item;
	iks_tree_t *Bt = (*ast)->children->item;
	iks_ast_node_value_t *B = Bt->item;
	iks_tree_t *S1t = (*ast)->children->next->item;
	iks_ast_node_value_t *S1 = S1t->item;
	iks_tree_t *S2t = (*ast)->children->next->next->item;
	iks_ast_node_value_t *S2 = S2t->item;

	// X = gera(B.t) || S1.code
  label_insert(S1->code,B->temp.b.t);
	// S.code = B.code || X
	S->code = iks_list_concat(B->code,S1->code);
  // S.code = S.code || gera(goto S.next)
	// aqui vamos gerar e usar S2.next ao inves de S.next, cfe. explicado
	// no parser em ctrl_flow2
	S2->temp.next = label_generator();
  iloc_t *goto_S_next = new_iloc(NULL, new_iloc_oper(jumpI,NULL,NULL,NULL,S2->temp.next,NULL,NULL));
  iks_list_append(S->code,(void*)goto_S_next);
	// Y = gera(B.f) || S2.code
  label_insert(S2->code,B->temp.b.f);
	// S.code = S.code || Y
	S->code = iks_list_concat(S->code,S2->code);
	// S.code = S.code || gera(goto(S2.next)
  iloc_t *goto_S2_next = new_iloc(NULL, new_iloc_oper(nop,NULL,NULL,NULL,NULL,NULL,NULL));
	iks_list_t *gambi = new_iks_list();
  iks_list_append(gambi,(void*)goto_S2_next);
  label_insert(gambi,S2->temp.next);
	S->code = iks_list_concat(S->code,gambi);

}

void code_while_do(iks_tree_t **ast) {
	iks_ast_node_value_t *S = (*ast)->item;
	iks_tree_t *Bt = (*ast)->children->item;
	iks_ast_node_value_t *B = Bt->item;
	iks_tree_t *S1t = (*ast)->children->next->item;
	iks_ast_node_value_t *S1 = S1t->item;

	// S.code = X = gera(S.begin) || B.code
  label_insert(B->code,S->temp.begin);
	S->code = iks_list_concat(S->code,B->code);

	// S.code = Y = X || gera(B.t) || S1.code
  label_insert(S1->code,B->temp.b.t);
	S->code = iks_list_concat(S->code,S1->code);

	// S.code = Y || gera(goto(S.begin)
  iloc_t *goto_S_begin = new_iloc(NULL, new_iloc_oper(jumpI,NULL,NULL,NULL,S->temp.begin,NULL,NULL));
  iks_list_append(S->code,(void*)goto_S_begin);
}

void code_do_while(iks_tree_t **ast) {
	iks_ast_node_value_t *S = (*ast)->item;
	iks_tree_t *S1t = (*ast)->children->item;
	iks_ast_node_value_t *S1 = S1t->item;
	iks_tree_t *Bt = (*ast)->children->next->item;
	iks_ast_node_value_t *B = Bt->item;

	//printf("a\n");
	//iloc_print(S1->code);
	//printf("a\n");
	//printf("b\n");
	//iloc_print(B->code);
	//printf("b\n");

	// S.code = X = gera(S.begin) || S1.code || B.code
  label_insert(S1->code,S->temp.begin);
	S->code = iks_list_concat(S->code,S1->code);
	S->code = iks_list_concat(S->code,B->code);


	////S.code = X || gera(B.f)
  //iloc_t *B_f = new_iloc(NULL, new_iloc_oper(nop,NULL,NULL,NULL,NULL,NULL,NULL));
	//iks_list_t *gambi = new_iks_list();
  //iks_list_append(gambi,(void*)B_f);
  //label_insert(gambi,B->temp.b.f);
	//S->code = iks_list_concat(S->code,gambi);
}

void code_generator(iks_tree_t **ast) {
	iks_list_t *code;
	code = new_iks_list();

	iks_ast_node_value_t *n;
	n = (*ast)->item;
	switch(n->type) {
		case IKS_AST_PROGRAMA:
      code_programa(ast);
      break;
		case IKS_AST_FUNCAO:
			code_funcao(ast);
			break;
		case IKS_AST_IF:
			code_if(ast);
			break;
		case IKS_AST_IF_ELSE:
			code_if_else(ast);
			break;
		case IKS_AST_DO_WHILE:
			code_do_while(ast);
			break;
		case IKS_AST_WHILE_DO:
			code_while_do(ast);
			break;
		case IKS_AST_INPUT:
		case IKS_AST_OUTPUT:
		case IKS_AST_ATRIBUICAO:
		case IKS_AST_RETURN:
		case IKS_AST_BLOCO:
		case IKS_AST_IDENTIFICADOR:
		case IKS_AST_LITERAL:
			code_literal(ast);
			break;
		case IKS_AST_ARIM_SOMA:
		case IKS_AST_ARIM_SUBTRACAO:
		case IKS_AST_ARIM_MULTIPLICACAO:
		case IKS_AST_ARIM_DIVISAO:
		case IKS_AST_ARIM_INVERSAO:
		case IKS_AST_LOGICO_E:
		case IKS_AST_LOGICO_OU:
			code_or(ast);
			break;
		case IKS_AST_LOGICO_COMP_DIF:
		case IKS_AST_LOGICO_COMP_IGUAL:
		case IKS_AST_LOGICO_COMP_LE:
		case IKS_AST_LOGICO_COMP_GE:
		case IKS_AST_LOGICO_COMP_L:
		case IKS_AST_LOGICO_COMP_G:
//		case IKS_AST_LOGICO_COMP_NEGACAO:
		case IKS_AST_VETOR_INDEXADO:
		case IKS_AST_CHAMADA_DE_FUNCAO:
		case IKS_AST_INDEFINIDO:
		default:
			break;
	}
}

char *label_generator(){
  char temp_label[LABEL_WIDTH];
  char *prefix, *new_label;
  
  prefix = malloc (sizeof (char) * LABEL_WIDTH);
  new_label = malloc (sizeof (char) * LABEL_WIDTH);

  strcpy (prefix, "l");
  sprintf(temp_label, "%d", label_ctrl); 
  label_ctrl++;
  
  strcpy (new_label, temp_label);

  new_label = strcat(prefix, new_label);

  return new_label;
}

char *register_generator(){
  char temp_register[REGISTER_WIDTH];
  char *prefix, *new_register;
  
  prefix = malloc (sizeof (char) * REGISTER_WIDTH);
  new_register = malloc (sizeof (char) * REGISTER_WIDTH);

  strcpy (prefix, "r");
  sprintf(temp_register, "%d", reg_ctrl); 
  reg_ctrl++;

  strcpy (new_register, temp_register);

  new_register = strcat(prefix, new_register);

  return new_register;
}

void label_insert(iks_list_t *code, char *label) {
  iloc_t *iloc = code->item;
  if(!iloc) {
    iloc_t *iloc = new_iloc(label, new_iloc_oper(nop,NULL,NULL,NULL,NULL,NULL,NULL));
    iks_list_append(code,(void*)iloc);
  }
  else {
    if(!iloc->label) {
      iloc->label = label;
    }
    else {
      iloc_t *iloc = new_iloc(label, new_iloc_oper(nop,NULL,NULL,NULL,NULL,NULL,NULL));
      iks_list_append(code,(void*)iloc);
    }
  }
}

iloc_t *new_iloc(char *label, iloc_oper_t *oper) {
  iloc_t *iloc;
  iloc = malloc(sizeof(iloc_t));
  iloc->label = label;
  iloc->opers = new_iks_list();
  iks_list_append(iloc->opers,(void*)oper);
  return iloc;
}

iloc_oper_t *new_iloc_oper(opcode_t opcode, char *s1, char *s2, char *s3, char *d1, char *d2, char *d3) {
  iloc_oper_t *oper;
  oper = malloc(sizeof(iloc_oper_t));
  oper->opcode = opcode;
  oper->src_operands = new_iks_list();
  oper->dst_operands = new_iks_list();
  if (s1) {
    iks_list_append(oper->src_operands,(void*)s1);
    if (s2) {
      iks_list_append(oper->src_operands,(void*)s2);
    }
      if (s3) {
        iks_list_append(oper->src_operands,(void*)s3);
      }
  }
  if (d1) {
    iks_list_append(oper->dst_operands,(void*)d1);
    if (d2) {
      iks_list_append(oper->dst_operands,(void*)d2);
    }
      if (d3) {
        iks_list_append(oper->dst_operands,(void*)d3);
      }
  }
  return oper;
}

void iloc_oper_print(iks_list_t *opers) {
  iks_list_t *it;
  it = opers;
  do {
    iloc_oper_t *oper = it->item;

    if (!oper) break;
    switch(oper->opcode) {
      case nop:
        break;
      case jumpI:
        printf("jumpI -> %s",oper->dst_operands->item);
        break;

    }
    printf("\n");

    it = it->next;    
  } while(it != opers);
}

void iloc_print(iks_list_t *code) {
  iks_list_t *it;
  it = code;
  do {
    iloc_t *iloc = it->item;

    if (!iloc) break;
    if (iloc->label) {
      printf ("%s: ",iloc->label);
    }

    iloc_oper_print(iloc->opers);

    it = it->next;    
  } while(it != code);
}
