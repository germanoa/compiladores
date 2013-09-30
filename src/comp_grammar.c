#include <stdio.h>
#include <stdlib.h>
#include "comp_grammar.h"
#include "comp_dict.h"
#include "iks_types.h"

static inline void __comp_grammar_symbol_init(comp_grammar_symbol_t *grammar_symbol) {
    grammar_symbol->type = IKS_SIMBOLO_INDEFINIDO;
    grammar_symbol->code_line_number = 0;
    grammar_symbol->value = NULL;
    grammar_symbol->type = IKS_NOTYPE;
}

comp_grammar_symbol_t *new_comp_grammar_symbol() {
    comp_grammar_symbol_t *grammar_symbol;
    grammar_symbol = malloc(sizeof(comp_grammar_symbol_t));
    __comp_grammar_symbol_init(grammar_symbol);
    return grammar_symbol;
}

void comp_grammar_symbol_delete(comp_grammar_symbol_t *grammar_symbol) {
    free(grammar_symbol->value);
    grammar_symbol->value = NULL;
    free(grammar_symbol);
    grammar_symbol = NULL;
}

void comp_grammar_symbol_set(comp_grammar_symbol_t *grammar_symbol, int type, int code_line_number, char *value) {
    grammar_symbol->type = type;
    grammar_symbol->code_line_number = code_line_number;
    grammar_symbol->value = value;
}

void comp_grammar_symbol_print(comp_grammar_symbol_t *grammar_symbol) {
    printf("%s\n",grammar_symbol->value);
}

void symbol_table_append(char *identifier, comp_grammar_symbol_t *symbol) {
    comp_dict_t *new_entry;
    new_entry = new_comp_dict();
    new_entry->item = new_comp_dict_item();
    comp_dict_item_set(new_entry->item,identifier,(void *)symbol);
    comp_dict_append(symbol_table,new_entry);
}

void symbol_table_init() {
    symbol_table = new_comp_dict();
    comp_grammar_symbol_t *symbol;
    symbol = new_comp_grammar_symbol();
    symbol_table->item = new_comp_dict_item();
    comp_dict_item_set(symbol_table->item,"root",(void *)symbol);
}

void symbol_table_print() {
    comp_dict_t *temp;
    temp = symbol_table;
    int i=0;
    do {
        comp_grammar_symbol_t *s;
        s = temp->item->value;
        printf("symbol: %s\n\ttype: %d\n\tline: %d\n\tidentifier: %s\n\tscope: %X\n\tdecl_type: %d\n", \
          comp_dict_item_key_get(temp->item),\
          s->type,\
          s->code_line_number,\
          s->value,\
          s->scope,\
          s->decl_type);
        temp = temp->next;    
    } while(temp != symbol_table);
    printf("\n");
}

int exist_symbol(comp_grammar_symbol_t *symbol, int force_type) {
    int ret = 0;
    comp_dict_t *temp;
    temp = symbol_table;
    do {
        comp_grammar_symbol_t *s;
        s = temp->item->value;
        if (s->value) {
          int diff = strcmp(symbol->value,s->value);
          if (force_type) {
            if ( (!diff) && (symbol->scope==s->scope) && (symbol->decl_type==s->decl_type) ) {
              ret = 1;
              break;
            }
          }
          else {
            if ( (!diff) && (symbol->scope==s->scope) ){
              ret = 1;
              break;
            }
          }
        }
        temp = temp->next;    
    } while(temp != symbol_table);
    return ret;
}
