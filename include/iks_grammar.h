#ifndef __iks_GRAMMAR_H__
#define __iks_GRAMMAR_H__

#include "iks_dict.h"
#include "iks_tree.h"
#include "iks_stack.h"
#include "iks_types.h"

#define IKS_SIMBOLO_INDEFINIDO 0
#define IKS_SIMBOLO_LITERAL_INT 1
#define IKS_SIMBOLO_LITERAL_FLOAT 2
#define IKS_SIMBOLO_LITERAL_CHAR 3
#define IKS_SIMBOLO_LITERAL_STRING 4
#define IKS_SIMBOLO_LITERAL_BOOL 5
#define IKS_SIMBOLO_IDENTIFICADOR 6

#define RS_SUCESSO 0
#define RS_ERRO 1

//iks_dict_t *symbol_table;

/**
 * Symbol struct
 */
typedef struct iks_grammar_symbol_t iks_grammar_symbol_t;
struct iks_grammar_symbol_t {
    int token_type;
    int code_line_number;
    char *value;    
    int decl_type;
    int iks_type;
    int iks_size;
    iks_dict_t *symbol_table;
    int scope_type;
    iks_list_t *params; //used only with functions
		int addr_offset; //offset of base addr from scope
		unsigned int num_dimen; //number of dimensions the vector has
		iks_list_t *dimens; //list of dimension sizes
};

/**
 * initialize grammar_symbol with prev,next with its same address
 */
static inline void __iks_grammar_symbol_init(iks_grammar_symbol_t *grammar_symbol);
/**
 * create a grammar_symbol, alloc mem and return address
 */
iks_grammar_symbol_t *new_iks_grammar_symbol();
/**
 * free address memory used by grammar_symbol
 */
void iks_grammar_symbol_delete(iks_grammar_symbol_t *grammar_symbol);

/**
 * set grammar_symbol
 */
void iks_grammar_symbol_set(iks_grammar_symbol_t *grammar_symbol, int token_type, int code_line_number, char *value);

/**
 * print grammar_symbol
 */
void iks_grammar_symbol_print(iks_grammar_symbol_t *grammar_symbol);


/**
 * append a symbol to the symbol_table
 */
void symbol_table_append(char *identifier, iks_grammar_symbol_t *symbol, iks_dict_t *symbol_table);

/**
 * initialize the symbol_table
 */
void symbol_table_init();

/**
 * initialize the symbol_table
 */
void symbol_table_delete(iks_dict_t *dict);

/**
 * print the symbol_table
 */
void symbol_table_print();

/**
 * verify if symbol already exists at symbol_table and scope
 * return 1 if exists, 0 if not
 */
iks_grammar_symbol_t *search_symbol_global(iks_grammar_symbol_t *symbol, iks_stack_t *scope);
iks_grammar_symbol_t *search_symbol_local(iks_grammar_symbol_t *symbol, iks_dict_t *symbol_table);
int exist_symbol_local(iks_grammar_symbol_t *symbol, iks_dict_t *symbol_table);

/**
 * add symbol to a symbol_table
 */
//int decl_symbol(iks_grammar_symbol_t *s, int iks_type, int decl_type, void *symbol_table, iks_grammar_symbol_t *function_with_param);
int decl_symbol(iks_grammar_symbol_t *s, int iks_type, int decl_type, scope_t *scope, iks_grammar_symbol_t *function_with_param);

/**
 * update a vector symbol at symbol_table
 */
int update_vector_symbol(iks_grammar_symbol_t *s,unsigned int dimen_counter,iks_list_t *dimens);

/**
 * verify if symbol is of decl_type
 */
int symbol_is_decl_type(iks_grammar_symbol_t *s,int decl_type);

/**
 * handling errors
 */
int iks_error(iks_grammar_symbol_t *s, int error_type);

int verify_function_args(iks_grammar_symbol_t *s, iks_list_t *args);

int symbol_is_iks_type(iks_grammar_symbol_t *s,int iks_type);



#endif /* __IKS_GRAMMAR_H */
