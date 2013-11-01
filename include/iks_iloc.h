/*
  iks_iloc.h
*/
#ifndef __IKS_ILOC_H__
#define __IKS_ILOC_H__

#include "iks_list.h"
#include "iks_tree.h"

#define LABEL_WIDTH 32
#define REGISTER_WIDTH 32

int reg_ctrl;
int label_ctrl;

iks_list_t *program_iloc; //list->item: iloc_t

typedef struct iloc_t iloc_t;
struct iloc_t {
	char *label;
	iks_list_t *opers; //list->item: iloc_oper_t
};

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

union operands {
	int n;
	char *l;
	char *r;
};

typedef struct iloc_oper_t iloc_oper_t;
struct iloc_oper_t {
	opcode_t opcode;
	iks_list_t *src_operands; //list->item: operands
	iks_list_t *dst_operands; //list->item: operands
};

/*
 * generate a new register
 */
char *int_to_char(int i);

/*
 * generate a new label
 */
char *label_generator();

/*
 * generate a new register
 */
char *register_generator();

/*
 * verify if label is valid
 */
int label_is_valid(char *label);

/*
 * verify if register is valid
 */
int register_is_valid(char *label);

/*
 * iloc code generator
 */
void code_generator(iks_tree_t **ast);


/*
 * iloc label insert
 */
void label_insert(iks_list_t *code, char *label);

/*
 * create a iloc instruction
 */
iloc_t *new_iloc(char *label, iloc_oper_t *oper);

/*
 * create a iloc operation
 */
iloc_oper_t *new_iloc_oper(opcode_t opcode, char *s1, char *s2, char *s3, char *d1, char *d2, char *d3);

void iloc_oper_print(iks_list_t *opers);

void iloc_print(iks_list_t *code);

#endif /* __IKS_ILOC_H__ */
