#include <stdio.h>
#include <stdlib.h>
#include "comp_grammar.h"
#include "comp_dict.h"
#include "iks_types.h"

static inline void __comp_grammar_symbol_init(comp_grammar_symbol_t *grammar_symbol) {
    grammar_symbol->token_type = IKS_SIMBOLO_INDEFINIDO;
    grammar_symbol->code_line_number = 0;
    grammar_symbol->value = NULL;
    grammar_symbol->iks_type = IKS_NOTYPE;
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

void comp_grammar_symbol_set(comp_grammar_symbol_t *grammar_symbol, int token_type, int code_line_number, char *value) {
    grammar_symbol->token_type = token_type;
    grammar_symbol->code_line_number = code_line_number;
    grammar_symbol->value = value;
}

void comp_grammar_symbol_print(comp_grammar_symbol_t *grammar_symbol) {
    printf("%s\n",grammar_symbol->value);
}

void symbol_table_append(char *identifier, comp_grammar_symbol_t *symbol, comp_dict_t *symbol_table) {
    comp_dict_t *new_entry;
    new_entry = new_comp_dict();
    if (comp_dict_is_empty(symbol_table)) {
      symbol_table->item = new_comp_dict_item();
      comp_dict_item_set(symbol_table->item,identifier,(void *)symbol);
      symbol_table->next=new_entry;
      symbol_table->prev=new_entry;
      new_entry->next=symbol_table;
      new_entry->prev=symbol_table;
      //new_entry->item = new_comp_dict_item();
      //comp_grammar_symbol_t *symbol;
      //symbol = new_comp_grammar_symbol();
      //comp_dict_item_set(new_entry->item,NULL,(void *)symbol);
    }
    else {
      new_entry->item = new_comp_dict_item();
      comp_dict_item_set(new_entry->item,identifier,(void *)symbol);
      comp_dict_append(symbol_table,new_entry);
    }
}

void symbol_table_init() {
    //symbol_table = new_comp_dict();
    //comp_grammar_symbol_t *symbol;
    //symbol = new_comp_grammar_symbol();
    //symbol_table->item = new_comp_dict_item();
    //comp_dict_item_set(symbol_table->item,"empty",(void *)symbol);
}

void symbol_table_print(comp_dict_t *symbol_table) {
    //printf("imprimindo: %X\n",symbol_table);
    comp_dict_t *temp;
    temp = symbol_table;
    int i=0;
    do {
        if (temp->item) {
        if (temp->item->value) {
          comp_grammar_symbol_t *s;
          s = temp->item->value;
          printf("symbol: %s\n\ttoken_type: %d\n\tline: %d\n\tidentifier: %s\n\tsymbol_table: %X\n\tdecl_type: %d\n\tiks_type: %d\n", \
            comp_dict_item_key_get(temp->item),\
            s->token_type,\
            s->code_line_number,\
            s->value,\
            s->symbol_table,\
            s->decl_type,\
            s->iks_type);
        }} 
        temp = temp->next;
    } while(temp != symbol_table);
    printf("\n");
}

int exist_symbol_global(comp_grammar_symbol_t *symbol, int force_type,comp_stack_t *scope) {
    int ret = 0;

    comp_stack_t *it_scope;
    it_scope = scope;
    do {
      printf("%X e %X\n",scope,it_scope);
      comp_dict_t *symbol_table;
      symbol_table = comp_stack_top(it_scope);
      printf("global looking for %s at: %X\n",symbol->value,symbol_table);
      if (!comp_dict_is_empty(symbol_table)) {
        comp_dict_t *temp;
        temp = symbol_table;
        do {
            if (temp->item) {
            if (temp->item->value) {
              comp_grammar_symbol_t *s;
              s = temp->item->value;
              int diff = strcmp(symbol->value,s->value);
              if (force_type) {
                if ( (!diff) && (symbol->symbol_table==s->symbol_table) && (symbol->iks_type==s->iks_type) ) {
                  ret = 1;
                  break;
                }
              }
              else {
                if ( (!diff) && (symbol->symbol_table==s->symbol_table) ){
                  ret = 1;
                  break;
                }
              }
            }}
            temp = temp->next;    
        } while(temp != symbol_table);
      }
      it_scope = it_scope->below;
    } while (it_scope != it_scope->below); 
    return ret;
}

int exist_symbol_local(comp_grammar_symbol_t *symbol, int force_type,comp_dict_t *symbol_table) {
    int ret = 0;
    printf("looking at: %X\n",symbol_table);
    if (!comp_dict_is_empty(symbol_table)) {
      comp_dict_t *temp;
      temp = symbol_table;
      do {
          if (temp->item) {
          if (temp->item->value) {
            comp_grammar_symbol_t *s;
            s = temp->item->value;
            int diff = strcmp(symbol->value,s->value);
            if (force_type) {
              if ( (!diff) && (symbol->symbol_table==s->symbol_table) && (symbol->iks_type==s->iks_type) ) {
                ret = 1;
                break;
              }
            }
            else {
              if ( (!diff) && (symbol->symbol_table==s->symbol_table) ){
                ret = 1;
                break;
              }
            }
          }}
          temp = temp->next;    
      } while(temp != symbol_table);
    }
    return ret;
}


int decl_symbol(int type, comp_grammar_symbol_t *s,int decl_type, void *symbol_table) {
  int ret=1;
  s->iks_type = type; 
  s->decl_type = decl_type;
  s->symbol_table = (comp_dict_t*)symbol_table;
  if (!exist_symbol_local(s,0,s->symbol_table)) {
    symbol_table_append(s->value,s,s->symbol_table);
  }
  else {
    ret=0;
    fprintf(stderr,"line %d: identificador '%s' já declarado\n",s->code_line_number,s->value);
  }
  return ret;
}

int symbol_is_decl_type(comp_grammar_symbol_t *s,int decl_type) {
  int ret=1;
  if (!(s->decl_type==decl_type)) {
    ret=0;
  }
  return ret;
}
