#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iks_iloc.h"
#include "iks_ast.h"
#include "iks_types.h"
#include "iks_tree.h"
#include "parser.h"


/******************************************************************************
* Objective: 
* Input:
* Output:	
******************************************************************************/
void code_programa(iks_tree_t **ast) {
	iks_ast_node_value_t *P = (*ast)->item;
	iks_tree_t *Ft = (*ast)->children->item;

	if(Ft) { //because program can be empty
		iks_ast_node_value_t *F = Ft->item;
		code_generator(&Ft);
		P->code = F->code;
	}
}

/******************************************************************************
* Objective: generate code for a function
* Input: pointer of pointer of iks_tree
* Output:	none	
******************************************************************************/
void code_funcao(iks_tree_t **ast) {
	iks_ast_node_value_t *F = (*ast)->item;
	iks_tree_t *St = (*ast)->children->item;
	
	if (St) {
		iks_ast_node_value_t *S = St->item;
		S->temp.next = label_generator();
		code_generator(&St);
		F->code = iks_list_concat(F->code, S->code);
	}
}

/******************************************************************************
* Objective: generate code for jump operation
* Input: pointer of pointer of iks_tree ast
* Output:	none	
******************************************************************************/
void code_id_lits(iks_tree_t **ast) {
	iks_ast_node_value_t *E = (*ast)->item;

	//registrador que receberah conteudo de id na memoria
	E->temp.name = register_generator();

	//registrador que receberah endereco na memoria deste id
	char *reg_temp = register_generator();

	char *addr = int_to_char(E->symbol->addr_offset);

	iloc_t *load,*loadAI;

	loadAI = new_iloc(NULL, new_iloc_oper(op_loadAI,
																				"rarp",
																				addr,
																				NULL,
																				reg_temp,
																				NULL,
																				NULL));

	// carregando conteudo da memoria para registrador E->temp.name
	switch(E->iks_type) {
		case IKS_INT:
			load = new_iloc(NULL, new_iloc_oper(op_load,
																					reg_temp,
																					NULL,
																					NULL,
																					E->temp.name,
																					NULL,
																					NULL));
			break;
		
		case IKS_CHAR:
			load = new_iloc(NULL, new_iloc_oper(op_cload,
																					reg_temp,
																					NULL,
																					NULL,
																					E->temp.name,
																					NULL,
																					NULL));
			break;	
	}
	
	iks_list_append(E->code, loadAI);
	iks_list_append(E->code, load);

	//iloc_print(E->code);
}

/******************************************************************************
* Objective: generate code for jump operation
* Input: pointer of pointer of iks_tree ast
* Output:	none	
******************************************************************************/
void code_literal(iks_tree_t **ast) {
	iks_ast_node_value_t *S = (*ast)->item;

  iloc_t *iloc;
  
  switch(S->symbol->token_type) {
    case TK_LIT_TRUE:
      //gera(goto B.t)
      iloc = new_iloc(NULL, new_iloc_oper(op_jumpI,
																					NULL,
																					NULL,
																					NULL,
																					S->temp.b.t,
																					NULL,
																					NULL));
      iks_list_append(S->code, (void*)iloc);
      break;
    case TK_LIT_FALSE:
      //gera(goto B.f)
      iloc = new_iloc(NULL, new_iloc_oper(op_jumpI,
																					NULL,
																					NULL,
																					NULL,
																					S->temp.b.f,
																					NULL,
																					NULL));
      iks_list_append(S->code, (void*)iloc);
      break;
    case TK_LIT_INT:
    	S->temp.name = register_generator();
    	iloc = new_iloc(NULL, new_iloc_oper(op_loadI,
																					S->symbol->value,
																					NULL,
																					NULL,
																					S->temp.name,
																					NULL,
																					NULL));
      iks_list_append(S->code, (void*)iloc);
    	break;
    case TK_LIT_FLOAT:
    	printf("code_literal para TK_LIT_FLOAT não implementado\n");
    	break;
    case TK_LIT_CHAR:
    	S->temp.name = register_generator();
    	iloc = new_iloc(NULL, new_iloc_oper(op_loadI,
																					S->symbol->value,
																					NULL,
																					NULL,
																					S->temp.name,
																					NULL,
																					NULL));
      iks_list_append(S->code, (void*)iloc);
    	break;
    case TK_LIT_STRING:
			code_id_lits(ast);
			break;
    default:
      fprintf(stderr,"error at code_literal: token type: %d\n",S->symbol->token_type);
  }

	//iloc_print(S->code);
}

/******************************************************************************
* Objective: Generate code to sum arithmetic operation
* Input: pointer of pointer of iks_tree ast
* Output:	none	
******************************************************************************/
void code_arit_sum(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *arit_sum = new_iks_list();


	iloc_t *art_sum = new_iloc(NULL, new_iloc_oper(op_add,	
																									E1->temp.name,
																									E2->temp.name,
																									NULL,
																									B->temp.name,
																									NULL,
																									NULL));	

	iks_list_append(arit_sum,art_sum);	
 
	B->code = iks_list_concat(B->code,arit_sum);	

	//iloc_print(B->code);
}


/******************************************************************************
* Objective: Generate code to subtraction arithmetic operation
* Input: pointer of pointer of iks_tree ast
* Output:	none	
******************************************************************************/
void code_arit_sub(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *arit_sub = new_iks_list();

	iloc_t *art_sub = new_iloc(NULL, new_iloc_oper(op_sub,	
																								E1->temp.name,
																								E2->temp.name,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iks_list_append(arit_sub,art_sub);	
 
	B->code = iks_list_concat(B->code,arit_sub);

	//iloc_print(B->code);
}


/******************************************************************************
* Objective: Generate code to multiplication arithmetic operation
* Input: pointer of pointer of iks_tree
* Output:	none	
******************************************************************************/
void code_arit_mul(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *arit_mul = new_iks_list();

	iloc_t *art_mul = new_iloc(NULL, new_iloc_oper(op_mult,	
																								E1->temp.name,
																								E2->temp.name,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iks_list_append(arit_mul, art_mul);	

	B->code = iks_list_concat(B->code,arit_mul);

	//iloc_print(B->code);	
}


/******************************************************************************
* Objective: Generate code to division arithmetic operation
* Input: pointer of pointer of iks_tree ast
* Output:	none	
******************************************************************************/
void code_arit_div(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *arit_div = new_iks_list();

	iloc_t *art_div = new_iloc(NULL, new_iloc_oper(op_div,	
																								E1->temp.name,
																								E2->temp.name,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iks_list_append(arit_div, art_div);	
 
	B->code = iks_list_concat(B->code,arit_div);	

	//iloc_print(B->code);
}


/******************************************************************************
* Objective: Generate code to inversion arithmetic operation
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_log_inv(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->temp.name = register_generator();

	iks_list_t *arit_inv = new_iks_list();

	iloc_t *art_inv = new_iloc(NULL, new_iloc_oper(op_inv,	
																								E1->temp.name,
																								NULL,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iks_list_append(arit_inv, art_inv);	
 
	B->code = iks_list_concat(B->code,arit_inv);	

	//iloc_print(B->code);
}


/******************************************************************************
* Objective: Generate code to and logic operation
* Input: pointer of pointer of iks_tree ast
* Output:	none	
******************************************************************************/
void code_log_and(iks_tree_t **ast) {

}


/******************************************************************************
* Objective: Generate code to or logic operation
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_log_or(iks_tree_t **ast) {
	iks_ast_node_value_t *B = (*ast)->item;

	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	E1->temp.b.t = B->temp.b.t;
	E1->temp.b.f = label_generator();
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	E2->temp.b.t = B->temp.b.t;
	E2->temp.b.f = B->temp.b.f;
	code_generator(&E2t);

	label_insert(E2->code,E1->temp.b.f);
	B->code = iks_list_concat(E1->code,E2->code);
}


/******************************************************************************
* Objective: Generate code to equal comparison type
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_comp_eq(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *comp_eq = new_iks_list();

	iloc_t *cmp_eq = new_iloc(NULL, new_iloc_oper(op_cmp_EQ,	
																								E1->temp.name,
																								E2->temp.name,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iloc_t *cbr = new_iloc(NULL, new_iloc_oper(op_cbr,
																							B->temp.name,
																							NULL,
																							NULL,
																							B->temp.b.t,
																							B->temp.b.f,
																							NULL));	
	iks_list_append(comp_eq,cmp_eq);	
	iks_list_append(comp_eq,cbr);	

	B->code = iks_list_concat(B->code,comp_eq);

	//iloc_print(B->code);

}


/******************************************************************************
* Objective: Generate code to not equal comparison type
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/			
void code_comp_ne(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *comp_ne = new_iks_list();

	iloc_t *cmp_ne = new_iloc(NULL, new_iloc_oper(op_cmp_NE,	
																								E1->temp.name,
																								E2->temp.name,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iloc_t *cbr = new_iloc(NULL, new_iloc_oper(op_cbr,
																							B->temp.name,
																							NULL,
																							NULL,
																							B->temp.b.t,
																							B->temp.b.f,
																							NULL));	
	iks_list_append(comp_ne,cmp_ne);	
	iks_list_append(comp_ne,cbr);	

	B->code = iks_list_concat(B->code,comp_ne);

	//iloc_print(B->code);
}


/******************************************************************************
* Objective: Generate code to less or equal comparison
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_comp_le(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *comp_le = new_iks_list();

	iloc_t *cmp_le = new_iloc(NULL, new_iloc_oper(op_cmp_LE,	
																								E1->temp.name,
																								E2->temp.name,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iloc_t *cbr = new_iloc(NULL, new_iloc_oper(op_cbr,
																							B->temp.name,
																							NULL,
																							NULL,
																							B->temp.b.t,
																							B->temp.b.f,
																							NULL));	
	iks_list_append(comp_le,cmp_le);	
	iks_list_append(comp_le,cbr);	

	B->code = iks_list_concat(B->code,comp_le);

	//iloc_print(B->code);
}		


/******************************************************************************
* Objective: Generate code to greater or equal comparison
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_comp_ge(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *comp_ge = new_iks_list();

	iloc_t *cmp_ge = new_iloc(NULL, new_iloc_oper(op_cmp_GE,	
																								E1->temp.name,
																								E2->temp.name,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iloc_t *cbr = new_iloc(NULL, new_iloc_oper(op_cbr,
																							B->temp.name,
																							NULL,
																							NULL,
																							B->temp.b.t,
																							B->temp.b.f,
																							NULL));	
	iks_list_append(comp_ge,cmp_ge);	
	iks_list_append(comp_ge,cbr);	

	B->code = iks_list_concat(B->code,comp_ge);

	//iloc_print(B->code);
}	


/******************************************************************************
* Objective: generate the code for less comparison
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_comp_lt(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *comp_lt = new_iks_list();

	iloc_t *cmp_lt = new_iloc(NULL, new_iloc_oper(op_cmp_LT,	
																								E1->temp.name,
																								E2->temp.name,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iloc_t *cbr = new_iloc(NULL, new_iloc_oper(op_cbr,
																							B->temp.name,
																							NULL,
																							NULL,
																							B->temp.b.t,
																							B->temp.b.f,
																							NULL));	
	iks_list_append(comp_lt,cmp_lt);	
	iks_list_append(comp_lt,cbr);

	B->code = iks_list_concat(B->code,comp_lt);

	//iloc_print(B->code);
}


/******************************************************************************
* Objective: generate the code for greater comparison
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_comp_gt(iks_tree_t **ast) {
	
	iks_tree_t *E1t = (*ast)->children->item;
	iks_ast_node_value_t *E1 = E1t->item;
	code_generator(&E1t);

	iks_tree_t *E2t = (*ast)->children->next->item;
	iks_ast_node_value_t *E2 = E2t->item;
	code_generator(&E2t);

	iks_ast_node_value_t *B = (*ast)->item;
	B->code = iks_list_concat(E1->code,E2->code);
	B->temp.name = register_generator();

	iks_list_t *comp_gt = new_iks_list();

	iloc_t *cmp_gt = new_iloc(NULL, new_iloc_oper(op_cmp_GT,	
																								E1->temp.name,
																								E2->temp.name,
																								NULL,
																								B->temp.name,
																								NULL,
																								NULL));	

	iloc_t *cbr = new_iloc(NULL, new_iloc_oper(op_cbr,
																							B->temp.name,
																							NULL,
																							NULL,
																							B->temp.b.t,
																							B->temp.b.f,
																							NULL));	
	iks_list_append(comp_gt,cmp_gt);	
	iks_list_append(comp_gt,cbr);	

	B->code = iks_list_concat(B->code,comp_gt);

	//iloc_print(B->code);
}

/******************************************************************************
* Objective: generate the code for if operation
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_if(iks_tree_t **ast){
	iks_ast_node_value_t *S = (*ast)->item;

	iks_tree_t *Bt = (*ast)->children->item;
	iks_ast_node_value_t *B = Bt->item;
	B->temp.b.t = label_generator();
	B->temp.b.f = S->temp.next;
	code_generator(&Bt);

	iks_tree_t *S1t = (*ast)->children->next->item;
	iks_ast_node_value_t *S1 = S1t->item;
	S1->temp.next = S->temp.next;
	code_generator(&S1t);

	// X = gera(B.t) || S1.code
	label_insert(S1->code,B->temp.b.t);
	
	// S.code = B.code || X
	S->code = iks_list_concat(B->code,S1->code);

	//iloc_print(S->code);
}

/******************************************************************************
* Objective: generate the code for if else operation
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_if_else(iks_tree_t **ast) {
	iks_ast_node_value_t *S = (*ast)->item;

	iks_tree_t *Bt = (*ast)->children->item;
	iks_ast_node_value_t *B = Bt->item;
	B->temp.b.t = label_generator();
	B->temp.b.f = label_generator();
	code_generator(&Bt);

	iks_tree_t *S1t = (*ast)->children->next->item;
	iks_ast_node_value_t *S1 = S1t->item;
	S1->temp.next = S->temp.next;
	code_generator(&S1t);

	iks_tree_t *S2t = (*ast)->children->next->next->item;
	iks_ast_node_value_t *S2 = S2t->item;
	S2->temp.next = S->temp.next;
	code_generator(&S2t);

	// X = gera(B.t) || S1.code
	label_insert(S1->code,B->temp.b.t);
	
	// S.code = B.code || X
	S->code = iks_list_concat(B->code,S1->code);

	// S.code = S.code || gera(goto(S.next)
  iloc_t *goto_S_next = new_iloc(NULL, new_iloc_oper(op_jumpI,
																											NULL,
																											NULL,
																											NULL,
																											S->temp.next,
																											NULL,
																											NULL));
  iks_list_append(S->code,(void*)goto_S_next);

	// Y = gera(B.f) || S2.code
	label_insert(S2->code,B->temp.b.f);

	// S.code = S.code || Y
	S->code = iks_list_concat(S->code,S2->code);

	//iloc_print(S->code);
}


/******************************************************************************
* Objective: generate the code for do operation
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
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
  iloc_t *goto_S_begin = new_iloc(NULL, new_iloc_oper(op_jumpI,
																											NULL,
																											NULL,
																											NULL,
																											S->temp.begin,
																											NULL,
																											NULL));
  iks_list_append(S->code,(void*)goto_S_begin);

	//iloc_print(S->code);
}

/******************************************************************************
* Objective: generate the code for do while operation
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_do_while(iks_tree_t **ast) {
	iks_ast_node_value_t *S = (*ast)->item;
	iks_tree_t *S1t = (*ast)->children->item;
	iks_ast_node_value_t *S1 = S1t->item;
	iks_tree_t *Bt = (*ast)->children->next->item;
	iks_ast_node_value_t *B = Bt->item;

	// S.code = X = gera(S.begin) || S1.code || B.code
	label_insert(S1->code,S->temp.begin);
	S->code = iks_list_concat(S->code,S1->code);
	S->code = iks_list_concat(S->code,B->code);

	////S.code = X || gera(B.f)
	//iloc_t *B_f = new_iloc(NULL, new_iloc_oper(op_nop,NULL,NULL,NULL,NULL,NULL,NULL));
	//iks_list_t *gambi = new_iks_list();
	//iks_list_append(gambi,(void*)B_f);
	//label_insert(gambi,B->temp.b.f);
	//S->code = iks_list_concat(S->code,gambi);

	//iloc_print(S->code);
}


/******************************************************************************
* Objective: generate the code for attribution operation
* Input: pointer of pointer of iks_tree ast
* Output:	none
******************************************************************************/
void code_attr(iks_tree_t **ast) {
	iks_ast_node_value_t *S = (*ast)->item;

	iks_tree_t *Et = (*ast)->children->next->item;
	iks_ast_node_value_t *E = Et->item;
	code_generator(&Et);

	iks_tree_t *IDt = (*ast)->children->item;
	iks_ast_node_value_t *ID = IDt->item;
	//code_generator(&IDt); //no need to call this since we're simply writing in ID, not using its value

	S->temp.name = register_generator();

	iloc_t *attr;
	opcode_t op;
	char *addr = int_to_char(E->symbol->addr_offset);
	
	op = op_storeAI;
	attr = new_iloc(NULL, new_iloc_oper(op,
																			E->temp.name,
																			NULL,
																			NULL,
																			"rarp",
																			addr,
																			NULL));

	iks_list_t *attr_code = new_iks_list();
	iks_list_append(attr_code,attr);
	S->code = iks_list_concat(E->code,ID->code);
	S->code = iks_list_concat(S->code,attr_code);

	//iloc_print(S->code);

}


/****************************************************************************** 
* Objective: main function for code generator
* Input: pointer of pointer of ast node
* Output:	none
******************************************************************************/
void code_generator(iks_tree_t **ast) {
	iks_list_t *code;
	code = new_iks_list();

	iks_ast_node_value_t *n;
	n = (*ast)->item;
	
	switch(n->type) {
		case IKS_AST_PROGRAMA:
			//printf("\nIKS_AST_PROGRAMA", n->type);
			code_programa(ast);
			break;
		case IKS_AST_FUNCAO:
			//printf("\nIKS_AST_FUNCAO", n->type);
			code_funcao(ast);
			break;
		case IKS_AST_IF:
			//printf("\nIKS_AST_IF", n->type);
			code_if(ast);
			break;
		case IKS_AST_IF_ELSE:
			//printf("\nIKS_AST_IF_ELSE", n->type);
			code_if_else(ast);
			break;
		case IKS_AST_DO_WHILE:
			//printf("\nIKS_AST_DO_WHILE", n->type);
			code_do_while(ast);
			break;
		case IKS_AST_WHILE_DO:
			//printf("\nIKS_AST_WHILE_DO", n->type);
			code_while_do(ast);
			break;
		case IKS_AST_INPUT:
			/* in progress */
			//printf("\nIKS_AST_INPUT", n->type);
			break;
		case IKS_AST_OUTPUT:
			/* in progress */
			//printf("\nIKS_AST_OUTPUT", n->type);
			break;
		case IKS_AST_ATRIBUICAO:
			//printf("\nIKS_AST_ATRIBUICAO", n->type);
			code_attr(ast);
			break;
		case IKS_AST_RETURN:
			/* in progress */
			//printf("\nIKS_AST_RETURN", n->type);
			break;
		case IKS_AST_BLOCO:
			/* in progress */
			//printf("\nIKS_AST_BLOCO", n->type);
			break;
		case IKS_AST_IDENTIFICADOR:
			//printf("\nIKS_AST_IDENTIFICADOR", n->type);
			code_id_lits(ast);
			break;
		case IKS_AST_LITERAL:
			//printf("\nIKS_AST_LITERAL", n->type);
			code_literal(ast);
			break;
		case IKS_AST_ARIM_SOMA:
			//printf("\nIKS_AST_ARIM_SOMA", n->type);
			code_arit_sum(ast);
			break;
		case IKS_AST_ARIM_SUBTRACAO:
			//printf("\nIKS_AST_ARIM_SUBTRACAO", n->type);
			code_arit_sub(ast);
			break;
		case IKS_AST_ARIM_MULTIPLICACAO:
			//printf("\nIKS_AST_ARIM_MULTIPLICACAO", n->type);
			code_arit_mul(ast);
			break;
		case IKS_AST_ARIM_DIVISAO:
			//printf("\nIKS_AST_ARIM_DIVISAO", n->type);
			code_arit_div(ast);
			break;
		case IKS_AST_ARIM_INVERSAO:
			//printf("\nIKS_AST_ARIM_INVERSAO", n->type);
			code_log_inv(ast);
			break;
		case IKS_AST_LOGICO_E:
			//printf("\nIKS_AST_LOGICO_E", n->type);
			code_log_and(ast);
			break;
		case IKS_AST_LOGICO_OU:
			//printf("\nIKS_AST_LOGICO_OU", n->type);
			code_log_or(ast);
			break;
		case IKS_AST_LOGICO_COMP_IGUAL:
			//printf("\nIKS_AST_LOGICO_COMP_IGUAL", n->type);
			code_comp_eq(ast);
			break;
		case IKS_AST_LOGICO_COMP_DIF:
			//printf("\nIKS_AST_LOGICO_COMP_DIF", n->type);
			code_comp_ne(ast);
			break;
		case IKS_AST_LOGICO_COMP_LE:
			//printf("\nIKS_AST_LOGICO_COMP_LE", n->type);
			code_comp_le(ast);		
			break;
		case IKS_AST_LOGICO_COMP_GE:
			//printf("\nIKS_AST_LOGICO_COMP_GE", n->type);
			code_comp_ge(ast);	
			break;	
		case IKS_AST_LOGICO_COMP_L:
			//printf("\nIKS_AST_LOGICO_COMP_L", n->type);
			code_comp_lt(ast);		
			break;
		case IKS_AST_LOGICO_COMP_G:
			//printf("\nIKS_AST_LOGICO_COMP_G", n->type);
			code_comp_gt(ast);
			break;
		//case IKS_AST_LOGICO_COMP_NEGACAO:
		case IKS_AST_VETOR_INDEXADO:
			//printf("\nIKS_AST_VETOR_INDEXADO", n->type);			
			break;
		case IKS_AST_CHAMADA_DE_FUNCAO:
			/* in progress */
			//printf("\nIKS_AST_CHAMADA_DE_FUNCAO", n->type);	
			break;
		case IKS_AST_INDEFINIDO:
			/* in progress */
			//printf("\nIKS_AST_INDEFINIDO", n->type);	
		default:
			fprintf(stderr,"error at code_generator\n");
			break;
	}
}

/******************************************************************************
* Objective: generate a new label from global label controller
* Input: none
* Output: pointer to a new string to be used as label name	
******************************************************************************/
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

/******************************************************************************
* Objective: Address value conversion from integer to string
* Input: an integer value
* Output: pointer to corresponding string
******************************************************************************/
char *int_to_char(int i) {
	char temp[MAXMEM];
	char *t;

	//horrivel, pensar como melhorar isso
	t = malloc (sizeof(char) * MAXMEM);
	sprintf(t, "%d", i);
	return t;
}

/******************************************************************************
* Objective: generate a new register from global register controller
* Input: none
* Output: pointer to a new string to be used as register name
******************************************************************************/
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

/******************************************************************************
* Objective: insert a label in iloc code list
* Input: list of iloc operations and new label to insert.
* Output: none
******************************************************************************/
void label_insert(iks_list_t *code, char *label) {
	iloc_t *iloc = code->item;

	if(!iloc) {
		iloc_t *iloc = new_iloc(label, new_iloc_oper(op_nop,
																									NULL,
																									NULL,
																									NULL,
																									NULL,
																									NULL,
																									NULL));
		iks_list_append(code,(void*)iloc);
	}
	else {
		if(!iloc->label) {
			iloc->label = label;
		}
		else {
			iloc_t *iloc = new_iloc(label, new_iloc_oper(op_nop,
																										NULL,
																										NULL,
																										NULL,
																										NULL,
																										NULL,
																										NULL));
			iks_list_append(code,(void*)iloc);
		}
	}
}

/******************************************************************************
* Objective: create an iloc code
* Input: label of new iloc code, operands list
* Output: pointer to a new iloc_t
******************************************************************************/
iloc_t *new_iloc(char *label, iloc_oper_t *oper) {
	iloc_t *iloc;
	
	iloc = malloc(sizeof(iloc_t));
	iloc->label = label;
	iloc->opers = new_iks_list();
	iks_list_append(iloc->opers,(void*)oper);

	return iloc;
}

/******************************************************************************
* Objective: create an iloc operation
* Input: type of iloc operation, three pointers to names of source operands  
* 	and three pointers to names of destination operands. 
* Output: pointer to a new iloc_oper_t
******************************************************************************/
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


/****************************************************************************** 
* Objective: print operands of an iloc operation 
* Input: operands list
* Output: none
******************************************************************************/
void iloc_oper_print(iks_list_t *opers) {
	iks_list_t *it;

	it = opers;

	do {
		iloc_oper_t *oper = it->item;

		if (!oper) break;
		printf("\t");
		//printf("\n%i\n", oper->opcode);
		switch(oper->opcode) {
			case op_nop:
				break;
			case op_add:
				printf("add %s, %s => %s",	(char*)oper->src_operands->item,
																		(char*)oper->src_operands->next->item,
																		(char*)oper->dst_operands->item);
				break;
			case op_sub:
				printf("sub %s, %s => %s",	(char*)oper->src_operands->item,
																		(char*)oper->src_operands->next->item,
																		(char*)oper->dst_operands->item);
				break;
			case op_mult:
				printf("mult %s, %s => %s",	(char*)oper->src_operands->item,
																		(char*)oper->src_operands->next->item,
																		(char*)oper->dst_operands->item);
				break;
			case op_div:
				printf("div %s, %s => %s",	(char*)oper->src_operands->item,
																		(char*)oper->src_operands->next->item,
																		(char*)oper->dst_operands->item);
				break;
			case op_jumpI:
				printf("jumpI -> %s",(char*)oper->dst_operands->item);
				break;
			case op_cmp_LT:
				printf("cmp_LT %s, %s -> %s",	(char*)oper->src_operands->item,
																			(char*)oper->src_operands->next->item,
																			(char*)oper->dst_operands->item);
				break;
			case op_cmp_LE:
				printf("cmp_LE %s, %s -> %s",	(char*)oper->src_operands->item,
																			(char*)oper->src_operands->next->item,
																			(char*)oper->dst_operands->item);
				break;
			case op_cmp_EQ:
				printf("cmp_EQ %s, %s -> %s",	(char*)oper->src_operands->item,
																			(char*)oper->src_operands->next->item,
																			(char*)oper->dst_operands->item);
				break;
			case op_cmp_GE:
				printf("cmp_GE %s, %s -> %s",	(char*)oper->src_operands->item,
																			(char*)oper->src_operands->next->item,
																			(char*)oper->dst_operands->item);
				break;
			case op_cmp_GT:
				printf("cmp_GT %s, %s -> %s",	(char*)oper->src_operands->item,
																			(char*)oper->src_operands->next->item,
																			(char*)oper->dst_operands->item);
				break;
			case op_cmp_NE:
				printf("cmp_NE %s, %s -> %s",	(char*)oper->src_operands->item,
																			(char*)oper->src_operands->next->item,
																			(char*)oper->dst_operands->item);
				break;
			case op_cbr:
				printf("cbr %s -> %s, %s",(char*)oper->src_operands->item,
																	(char*)oper->dst_operands->item,
																	(char*)oper->dst_operands->next->item);
				break;
      case op_loadAI:
        printf("loadAI %s, %s => %s",(char*)oper->src_operands->item,
																(char*)oper->src_operands->next->item,
        												(char*)oper->dst_operands->item);
        break;
      case op_loadI:
        printf("loadI %s => %s",(char*)oper->src_operands->item,
        												(char*)oper->dst_operands->item);
        break;
      case op_load:
        printf("load %s => %s",	(char*)oper->src_operands->item,
        												(char*)oper->dst_operands->item);
        break;
      case op_cload:
        printf("cload %s => %s",(char*)oper->src_operands->item,
        												(char*)oper->dst_operands->item);
        break;
      case op_i2i:
        printf("i2i %s => %s",(char*)oper->src_operands->item,
        											(char*)oper->dst_operands->item);
        break;
      case op_i2c:
        printf("i2c %s => %s",(char*)oper->src_operands->item,
        											(char*)oper->dst_operands->item);
        break;
      case op_c2i:
        printf("c2i %s => %s",(char*)oper->src_operands->item,
        											(char*)oper->dst_operands->item);
        break;
      case op_c2c:
        printf("c2c %s => %s",(char*)oper->src_operands->item,
    													(char*)oper->dst_operands->item);
        break;
      case op_storeAI:
      	printf("storeAI %s => %s, %s",(char*)oper->src_operands->item,
																			(char*)oper->dst_operands->item,
																			(char*)oper->dst_operands->next->item);
				break;
			default:
				fprintf(stderr,"error at iloc_oper_print: op code: %d\n",oper->opcode);
    }
    
    printf("\n");

    it = it->next;    
  } while(it != opers);
}


/****************************************************************************** 
* Objective: print an iloc code
* Input: code list
* Output: none
******************************************************************************/
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
