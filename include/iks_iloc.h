/*
  iks_iloc.h
*/
#ifndef __IKS_ILOC_H__
#define __IKS_ILOC_H__

#include "iks_list.h"
#include "iks_tree.h"
#include "iks_ast.h"


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
* Current function, used to set return value
* and to get ra size.
*
*/
iks_ast_node_value_t *curr_function;

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
enum opcode { 
	op_add=0, op_sub=1, op_mult=2, op_div=3, op_inv=4,
	op_addI=5, op_subI=6, op_multI=7, op_divI=8, op_rdivI=9,
	op_and=10, op_andI=11, op_or=12, op_orI=13, op_xor=14, op_xorI=15, 
	op_lshift=16, op_rshift=17,
	op_lshiftI=18, op_rshifI=19,
	op_load=20, op_loadAI=21, op_loadA0=22, op_loadI=23,
	op_cload=24, op_cloadAI=25, op_cloadA0=26,
	op_store=27, op_storeAI=28, op_storeA0=29,
	op_cstore=30, op_cstoreAI=31, op_cstoreA0=32,
	op_i2i=33, op_c2c=34, op_c2i=35, op_i2c=36,
	op_cmp_LT=37, op_cmp_LE=38, op_cmp_EQ=39, op_cmp_GE=40, op_cmp_GT=41, op_cmp_NE=42, op_cbr=43,
	op_jump=44, op_jumpI=45,
 	op_nop=46,
	op_tbl=47,
  op_inc=48, op_dec=49
 };
typedef enum opcode opcode_t;

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
* Objective: append a label in iloc code list
* Input: list of iloc operations and new label to insert.
* Output: none
******************************************************************************/
void label_append(iks_list_t *code, char *label);

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

/****************************************************************************** 
* Objective: get coercion code
* Input: ast
* Output: none
******************************************************************************/
iks_list_t *get_coercion_code(iks_ast_node_value_t *n);

/*
*
* Different data types of return
*
*/
union ret_value {
	int i;
	char c;
};

#endif /* __IKS_ILOC_H__ */
