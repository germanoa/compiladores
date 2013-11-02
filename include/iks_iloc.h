/*
  iks_iloc.h
*/
#ifndef __IKS_ILOC_H__
#define __IKS_ILOC_H__

#include "iks_list.h"
#include "iks_tree.h"


/*
*
* Definition of maximum amount of characters that
* build the registers name and labels name.
*
*/
#define LABEL_WIDTH 32
#define REGISTER_WIDTH 32
#define MAXMEM  10 //amount of digits for memory

/*
*
* Variables declaration that control the
* amount of registers and labels.
*
*/
int reg_ctrl;
int label_ctrl;


/*
*
* Iloc comands list declaration
* iks_list_t->item has no type defined
* program_iloc->item : iloc_t
*
*/
iks_list_t *program_iloc;


/*
*
* Structure that defines an iloc operation
* Contains a iloc operands list
* iks_list_t->item has no type defined
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
* Constants declaration, in names global space, 
* that represent valid operations
*
*/
typedef enum { 
	op_add, op_sub, op_mult, op_div,
	op_addI, op_subI, op_multI, op_divI, op_rdivI,
	op_and, op_andI, op_or, op_orI, op_xor, op_xorI, 
	op_lshift, op_rshift,
	op_lshiftI, op_rshifI,
	op_load, op_loadAI, op_loadA0, op_loadI,
	op_cload, op_cloadAI, op_cloadA0,
	op_store, op_storeAI, op_storeA0,
	op_cstore, op_cstoreAI, op_cstoreA0,
	op_i2i, op_c2c, op_c2i, op_i2c,
	op_cmp_LT, op_cmp_LE, op_cmp_EQ, op_cmp_GE, op_cmp_GT, op_cmp_NE, op_cbr,
	op_jump, op_jumpI,
 	op_nop
 } opcode_t;


/*
*
* Different data types of operands 
* sharing the same memory space
*
*/
union operands {
	int n;
	char *l;
	char *r;
};


/*
*
* Struct that define an iloc operation
* Contains a source operands list
* and a destination operands list
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
* Objective: generate a new label from global label controller
* Input: none
* Output: pointer to a new string to be used as label name	
******************************************************************************/
char *label_generator();


/******************************************************************************
* Objective: generate a new register from global register controller
* Input: none
* Output: pointer to a new string to be used as register name
******************************************************************************/
char *register_generator();


/******************************************************************************
* Objective: Address value conversion from integer to string
* Input: an integer value
* Output: pointer to corresponding string
******************************************************************************/
char *int_to_char(int i);


/****************************************************************************** 
* Objective: main function for code generator
* Input: ast
* Output: none	
******************************************************************************/
void code_generator(iks_tree_t **ast);


/******************************************************************************
* Objective: insert a label in iloc code list
* Input: list of iloc operations and new label to insert.
* Output: none
******************************************************************************/
void label_insert(iks_list_t *code, char *label);


/******************************************************************************
* Objective: create an iloc code
* Input: label of new iloc code, operands list
* Output: pointer to a new iloc_t
******************************************************************************/
iloc_t *new_iloc(char *label, iloc_oper_t *oper);


/******************************************************************************
* Objective: create an iloc operation
* Input: type of iloc operation, three pointers to names of source operands  
* 	and three pointers to names of destination operands. 
* Output: pointer to a new iloc_oper_t
******************************************************************************/
iloc_oper_t *new_iloc_oper(opcode_t opcode, char *s1, char *s2, char *s3, char *d1, char *d2, char *d3);


/****************************************************************************** 
* Objective: print operands of an iloc operation 
* Input: operands list
* Output: none
******************************************************************************/
void iloc_oper_print(iks_list_t *opers);


/****************************************************************************** 
* Objective: print an iloc code
* Input: code list
* Output: none
******************************************************************************/
void iloc_print(iks_list_t *code);


#endif /* __IKS_ILOC_H__ */
