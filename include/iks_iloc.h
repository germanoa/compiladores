/*
  iks_iloc.h
*/
#ifndef __IKS_ILOC_H__
#define __IKS_ILOC_H__

#include "iks_list.h"
#include "iks_tree.h"


iks_list_t *program_iloc; //list->item: iloc_t

typedef struct iloc_t iloc_t;
struct iloc_t {
	char *label;
	iks_list_t *oper; //list->item: iloc_oper_t
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
 } opcode_t;

union operands {
	int n;
	char *l;
	char *r;
};

typedef struct iloc_oper_t iloc_oper_t;
struct iloc_oper_t {
	opcode_t opcode;
	iks_list_t *src_opers; //list->item: operands
	iks_list_t *dst_opers; //list->item: operands
};


/* 
 * Register struct management
 */
typedef struct iloc_reg_t iloc_reg_t;
struct iloc_reg_t{
	char *id;
	void *content;
};

typedef struct iloc_reg_node_t iloc_reg_node_t;
struct iloc_reg_node_t{
	iloc_reg_t reg;
	iloc_reg_node_t *left;
	iloc_reg_node_t *right;
};

typedef struct iloc_reg_tree_t iloc_reg_tree_t;
struct iloc_reg_tree_t{
	iloc_reg_node_t *root;
	int reg_ctrl;
};

/* 
 * Label struct management 
 */
typedef struct iloc_label_t iloc_label_t;
struct iloc_label_t{
	char *id;
	void *content;
	int call_ctrl;
};

typedef struct iloc_label_node_t iloc_label_node_t;
struct iloc_label_node_t{
	iloc_label_t label;
	iloc_label_node_t *left;
	iloc_label_node_t *right;
};

typedef struct iloc_label_node_t iloc_label_tree_t;
struct iloc_tree_label_t{
	iloc_label_node_t *root;
};


/*
 * generate a new label
 */
char* label_generator();

/*
 * generate a new register
 */
char* register_generator();

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

#endif /* __IKS_ILOC_H__ */
