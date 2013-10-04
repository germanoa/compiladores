#ifndef COMP_GRAMMAR_H
#define COMP_GRAMMAR_H

#include "comp_dict.h"
#include "comp_tree.h"
#include "comp_stack.h"

#define IKS_SIMBOLO_INDEFINIDO 0
#define IKS_SIMBOLO_LITERAL_INT 1
#define IKS_SIMBOLO_LITERAL_FLOAT 2
#define IKS_SIMBOLO_LITERAL_CHAR 3
#define IKS_SIMBOLO_LITERAL_STRING 4
#define IKS_SIMBOLO_LITERAL_BOOL 5
#define IKS_SIMBOLO_IDENTIFICADOR 6

#define RS_SUCESSO 0
#define RS_ERRO 1

//comp_dict_t *symbol_table;

/**
 * Symbol struct
 */
typedef struct comp_grammar_symbol_t comp_grammar_symbol_t;
struct comp_grammar_symbol_t {
    int token_type;
    int code_line_number;
    char *value;    
    int decl_type;
    int iks_type;
    comp_dict_t *symbol_table;
    //comp_tree_t *scope;
};

/**
 * initialize grammar_symbol with prev,next with its same address
 */
static inline void __comp_grammar_symbol_init(comp_grammar_symbol_t *grammar_symbol);
/**
 * create a grammar_symbol, alloc mem and return address
 */
comp_grammar_symbol_t *new_comp_grammar_symbol();
/**
 * free address memory used by grammar_symbol
 */
void comp_grammar_symbol_delete(comp_grammar_symbol_t *grammar_symbol);

/**
 * set grammar_symbol
 */
void comp_grammar_symbol_set(comp_grammar_symbol_t *grammar_symbol, int token_type, int code_line_number, char *value);

/**
 * print grammar_symbol
 */
void comp_grammar_symbol_print(comp_grammar_symbol_t *grammar_symbol);


/**
 * append a symbol to the symbol_table
 */
void symbol_table_append(char *identifier, comp_grammar_symbol_t *symbol, comp_dict_t *symbol_table);

/**
 * initialize the symbol_table
 */
void symbol_table_init();

/**
 * print the symbol_table
 */
void symbol_table_print();

/**
 * verify if symbol already exists at symbol_table and scope
 * return 1 if exists, 0 if not
 */
comp_grammar_symbol_t *search_symbol_global(comp_grammar_symbol_t *symbol, comp_stack_t *scope);
comp_grammar_symbol_t *search_symbol_local(comp_grammar_symbol_t *symbol, comp_dict_t *symbol_table);
int exist_symbol_local(comp_grammar_symbol_t *symbol, comp_dict_t *symbol_table);

/**
 * add symbol to a symbol_table
 */
int decl_symbol(int type, comp_grammar_symbol_t *s,int decl_type, void *symbol_table);


/**
 * handling errors
 */
int iks_error(comp_grammar_symbol_t *s, int error_type);


#endif /* COMP_GRAMMAR_H */
