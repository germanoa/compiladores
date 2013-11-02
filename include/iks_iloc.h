/*
  iks_iloc.h
*/
#ifndef __IKS_ILOC_H__
#define __IKS_ILOC_H__

#include "iks_list.h"
#include "iks_tree.h"

/*
*
* Definição da quantidade máxima de caracteres
* que compõem o nome de registradores e de rótulos
*
*/
#define LABEL_WIDTH 32
#define REGISTER_WIDTH 32

/*
*
* Declaração de variáveis que controlam a quantidade
* de registradores e de rótulos criados
*
*/
int reg_ctrl;
int label_ctrl;

/*
*
* Declaração de lista de comandos iloc
* iks_list_t->item não tem tipo definido
* program_iloc->item : iloc_t
*
*/
iks_list_t *program_iloc;

/*
*
* Estrutura que define uma operação em iloc
* Contém uma lista de operandos iloc
* opers->item: iloc_oper_t
*
*/
typedef struct iloc_t iloc_t;
struct iloc_t {
	char *label;
	iks_list_t *opers;
};


/*
*
* Declaração de constantes no espaço global de nomes
* que representam as operações válidas em iloc
*
*/
typedef enum { 
	add, sub, mult, _div,
	addI, subI, multI, divI, rdivI,
	and, andI, or, orI, xor, xorI, 
	lshift, rshift,
	lshiftI, rshifI,
	load, loadAI, loadA0, loadI,
	cload, cloadAI, cloadA0,
	store, storeAI, storeA0,
	cstore, cstoreAI, cstoreA0,
	i2i, c2c, c2i, i2c,
	cmp_LT, cmp_LE, cmp_EQ, cmp_GE, cmp_GT, cmp_NE, cbr,
	jump, jumpI,
  	nop
 } opcode_t;

/*
*
* Operandos compartilhando mesmo espaço físico de memória
*
*/
union operands {
	int n;
	char *l;
	char *r;
};

/*
*
* Estrutura que define uma operação em iloc
* Contém uma lista de operandos fonte
* e uma lista de operandos destino
* src_operands->item: operands
* dst_operands->item: operands
*
*/
typedef struct iloc_oper_t iloc_oper_t;
struct iloc_oper_t {
	opcode_t opcode;
	iks_list_t *src_operands; 
	iks_list_t *dst_operands;
};


/******************************************************************************
* Objective: 
* Input:
* Output:	
******************************************************************************/
char *int_to_char(int i);


/******************************************************************************
* Objective: generate a new label from global label controller
* Input: empty
* Output: pointer to a new string to be used as label name	
******************************************************************************/
char *label_generator();


/******************************************************************************
* Objective: generate a new register from global register controller
* Input: empty
* Output: pointer to a new string to be used as register name
******************************************************************************/
char *register_generator();


/******************************************************************************
* Objective: verify if label is valid
* Input:
* Output:	
******************************************************************************/
int label_is_valid(char *label);


/******************************************************************************
* Objective: verify if register is valid
* Input:
* Output:	
******************************************************************************/
int register_is_valid(char *label);


/****************************************************************************** 
* Objective: iloc code generator
* Input:
* Output:		
******************************************************************************/
void code_generator(iks_tree_t **ast);


/******************************************************************************
* Objective: iloc label insert
* Input:
* Output:	
******************************************************************************/
void label_insert(iks_list_t *code, char *label);


/******************************************************************************
* Objective: create a iloc instruction
* Input:
* Output:	
******************************************************************************/
iloc_t *new_iloc(char *label, iloc_oper_t *oper);


/******************************************************************************
* Objective: create a iloc operation
* Input:
* Output:
******************************************************************************/
iloc_oper_t *new_iloc_oper(opcode_t opcode, char *s1, char *s2, char *s3, char *d1, char *d2, char *d3);


/****************************************************************************** 
* Objective: print a iloc operation
* Input:
* Output:
******************************************************************************/
void iloc_oper_print(iks_list_t *opers);


/****************************************************************************** 
* Objective: print a iloc code line
* Input:
* Output:
******************************************************************************/
void iloc_print(iks_list_t *code);


#endif /* __IKS_ILOC_H__ */
