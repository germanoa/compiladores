#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comp_grammar.h"
#include "comp_dict.h"
#include "iks_types.h"
#include "iks_ast.h"

static inline void __comp_grammar_symbol_init(comp_grammar_symbol_t *grammar_symbol) {
    grammar_symbol->token_type = IKS_SIMBOLO_INDEFINIDO;
    grammar_symbol->code_line_number = 0;
    grammar_symbol->value = NULL;
    grammar_symbol->decl_type = 0;
    grammar_symbol->iks_type = IKS_NOTYPE;
    grammar_symbol->iks_size = 0;
    grammar_symbol->symbol_table = NULL;
    grammar_symbol->params = new_comp_list();
    grammar_symbol->addr_offset = 0;
    grammar_symbol->base = 0;
    grammar_symbol->num_dimen = 0;
    grammar_symbol->dimens = new_comp_list();
    
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

void symbol_table_delete(comp_dict_t *dict) {
    if (!comp_dict_is_empty(dict)) {
        comp_dict_t *temp;
        temp = dict->next;
        do {
            temp = temp->next;
						if (temp->prev->item) {
							printf("apaga!!!!!!!!!!!!!!\n");
							comp_grammar_symbol_delete((comp_grammar_symbol_t*)temp->prev->item);					
						}    
            free(temp->prev);
            temp->prev = NULL;
        } while(temp != dict);
    }   
		if (dict->item) {
			iks_ast_node_value_delete((iks_ast_node_value_t*)dict->item);
		}    
    free(dict);
    dict = NULL;
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
          printf("symbol: %s\n\ttoken_type: %d\n\tline: %d\n\tidentifier: %s\n\tsymbol_table: %X\n\tdecl_type: %d\n\tiks_size: %d\n\t iks_type: %d\n", \
            comp_dict_item_key_get(temp->item),\
            s->token_type,\
            s->code_line_number,\
            s->value,\
            s->symbol_table,\
            s->decl_type,\
            s->iks_size,\
            s->iks_type);
        }} 
        temp = temp->next;
    } while(temp != symbol_table);
    printf("\n");
}

comp_grammar_symbol_t *search_symbol_global(comp_grammar_symbol_t *symbol, comp_stack_t *scope) {
    comp_grammar_symbol_t *ret=NULL;
    comp_stack_t *it_scope;
    it_scope = scope;
    do {
      comp_dict_t *symbol_table;
      symbol_table = comp_stack_top(it_scope);
      //printf("global looking for %s at: %X\n",symbol->value,symbol_table);
      ret = search_symbol_local(symbol,symbol_table); 
      it_scope = it_scope->below;
    } while ((ret==NULL) && (it_scope != it_scope->below)); 
    if (ret==NULL) { //look at global
      ret = search_symbol_local(symbol,comp_stack_top(it_scope)); 
    }
    return ret;
}

comp_grammar_symbol_t *search_symbol_local(comp_grammar_symbol_t *symbol, comp_dict_t *symbol_table) {
    comp_grammar_symbol_t *ret =NULL;
    //printf("local looking at: %X\n",symbol_table);
    if (!comp_dict_is_empty(symbol_table)) {
      comp_dict_t *temp;
      temp = symbol_table;
      do {
          if (temp->item) {
          if (temp->item->value) {
            comp_grammar_symbol_t *s;
            s = temp->item->value;
            int diff = strcmp(symbol->value,s->value);
            //printf("%s == %s :%d\n",symbol->value,s->value,diff);
            if (!diff) {
              ret = s;
              break;
            }
          }}
          temp = temp->next;    
      } while(temp != symbol_table);
    }
    return ret;
}


int exist_symbol_local(comp_grammar_symbol_t *symbol, comp_dict_t *symbol_table) {
    int ret = 0;
    //printf("local looking at: %X\n",symbol_table);
    if (!comp_dict_is_empty(symbol_table)) {
      comp_dict_t *temp;
      temp = symbol_table;
      do {
          if (temp->item) {
          if (temp->item->value) {
            comp_grammar_symbol_t *s;
            s = temp->item->value;
            int diff = strcmp(symbol->value,s->value);
            //printf("%s == %s :%d\n",symbol->value,s->value,diff);
            if (!diff) {
              ret = 1;
              break;
            }
          }}
          temp = temp->next;    
      } while(temp != symbol_table);
    }
    return ret;
}


int decl_symbol(comp_grammar_symbol_t *s,int iks_type, int decl_type, void *symbol_table, comp_grammar_symbol_t *function_with_param) {
  int ret=1;
  s->iks_type = iks_type;
  
  switch (iks_type) {
    case IKS_INT:
      s->iks_size=4;
      break;
    case IKS_FLOAT:
      s->iks_size=8;
      break;
    case IKS_BOOL:
      s->iks_size=1;
      break;
    case IKS_CHAR:
      s->iks_size=1;
      break;
    case IKS_STRING:
      s->iks_size=1;
      break;
  }
  
  s->decl_type = decl_type;
  s->symbol_table = (comp_dict_t*)symbol_table;
  
  if (!exist_symbol_local(s,s->symbol_table)) {
    symbol_table_append(s->value,s,s->symbol_table);
    if (function_with_param) {
      comp_list_t *l;
      l = new_comp_list();
      comp_list_set_item(l,(void*)s);      
      comp_list_append(function_with_param->params,l);
    }
  } else {
    ret=0;
    fprintf(stderr,"line %d: identificador '%s' já declarado\n",s->code_line_number,s->value);
  }
  
  return ret;
}

int update_vector_symbol(comp_grammar_symbol_t *s,unsigned int dimen_counter,comp_list_t *dimens) {
  //s->iks_size = s->iks_size * atoi(lit->value);
  
  if(s != NULL && dimens != NULL) {
  	s->decl_type = IKS_DECL_VECTOR;
		s->num_dimen = dimen_counter;
		s->dimens = dimens;
		
		comp_list_t *d = dimens;
		int *size = NULL;
		do {
			size = d->item;
			if(*size < 0)
				return iks_error(s, IKS_ERROR_DECL);
		
			d = d->next;
		} while(d != dimens);
  }
  else fprintf(stderr,"erro interno: parâmetros inválidos para update_vector_symbol.\n");
  
  return 0;
}

int symbol_is_decl_type(comp_grammar_symbol_t *s,int decl_type) {
  int ret=1;
  if (!(s->decl_type==decl_type)) {
    ret=0;
  }
  return ret;
}


int iks_error(comp_grammar_symbol_t *s, int error_type) {
  int ret=0;
  switch(error_type) {
    case IKS_ERROR_USE:
      if (s->decl_type==IKS_DECL_VAR) {
        fprintf(stderr,"line %d: identificador '%s' deve ser usado como variavel\n",s->code_line_number,s->value);      
        ret=IKS_ERROR_VARIABLE;
      }
      else if (s->decl_type==IKS_DECL_VECTOR) {
        fprintf(stderr,"line %d: identificador '%s' deve ser usado como vetor\n",s->code_line_number,s->value);      
        ret=IKS_ERROR_VECTOR;
      }
      else if (s->decl_type==IKS_DECL_FUNCTION) {
        fprintf(stderr,"line %d: identificador '%s' deve ser usado como funcao\n",s->code_line_number,s->value);      
        ret=IKS_ERROR_FUNCTION;
      }
      else {
        fprintf(stderr,"line %d: identificador '%s' ???????????\n",s->code_line_number,s->value);      
        ret=99999;
      }
      break;
    
    case IKS_ERROR_WRONG_PAR_RETURN:
      fprintf(stderr,"parametro nao e compativel com expressao de retorno.\n");
      ret=IKS_ERROR_WRONG_PAR_RETURN;
      break;
    
    case IKS_ERROR_WRONG_PAR_INPUT:
    	if(s != NULL)
    		fprintf(stderr,"line %d: '%s' deve ser identificador\n",s->code_line_number,s->value);
  		else fprintf(stderr,"parâmetro para input deve ser identificador\n");
  		
  		ret = IKS_ERROR_WRONG_PAR_INPUT;
  		break;
		
		case IKS_ERROR_DECL:
			if(s != NULL) {
				if(s->decl_type == IKS_DECL_VECTOR)
					fprintf(stderr,"line %d: declaração de vetor %s inválida. dimensões negativas?\n", s->code_line_number,s->value);
				else fprintf(stderr,"line %d: declaração de %s inválida.\n", s->code_line_number,s->value);
			}
			else fprintf(stderr,"declaração inválida.\n");
			
			ret = IKS_ERROR_DECL;
			break;
		
		default:
			fprintf(stderr,"código de erro %d indesperado.\n", error_type);
			break;
  }
  return ret;
}
