#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iks_grammar.h"
#include "iks_dict.h"
#include "iks_types.h"
#include "iks_ast.h"

static inline void __iks_grammar_symbol_init(iks_grammar_symbol_t *grammar_symbol) {
    grammar_symbol->token_type = IKS_SIMBOLO_INDEFINIDO;
    grammar_symbol->code_line_number = 0;
    grammar_symbol->value = NULL;
    grammar_symbol->decl_type = 0;
    grammar_symbol->iks_type = IKS_NOTYPE;
    grammar_symbol->iks_size = 0;
    grammar_symbol->symbol_table = NULL;
    grammar_symbol->params = new_iks_list();
    grammar_symbol->addr_offset = 0;
    grammar_symbol->num_dimen = 0;
    grammar_symbol->dimens = new_iks_list();
    
}

iks_grammar_symbol_t *new_iks_grammar_symbol() {
    iks_grammar_symbol_t *grammar_symbol;
    grammar_symbol = malloc(sizeof(iks_grammar_symbol_t));
    __iks_grammar_symbol_init(grammar_symbol);
    return grammar_symbol;
}

void iks_grammar_symbol_delete(iks_grammar_symbol_t *grammar_symbol) {
    free(grammar_symbol->value);
    grammar_symbol->value = NULL;
    free(grammar_symbol);
    grammar_symbol = NULL;
}

void iks_grammar_symbol_set(iks_grammar_symbol_t *grammar_symbol, int token_type, int code_line_number, char *value) {
    grammar_symbol->token_type = token_type;
    grammar_symbol->code_line_number = code_line_number;
    grammar_symbol->value = value;
}

void iks_grammar_symbol_print(iks_grammar_symbol_t *grammar_symbol) {
    printf("%s\n",grammar_symbol->value);
}

void symbol_table_append(char *identifier, iks_grammar_symbol_t *symbol, iks_dict_t *symbol_table) {
    iks_dict_t *new_entry;
    new_entry = new_iks_dict();
    if (iks_dict_is_empty(symbol_table)) {
      symbol_table->item = new_iks_dict_item();
      iks_dict_item_set(symbol_table->item,identifier,(void *)symbol);
      symbol_table->next=new_entry;
      symbol_table->prev=new_entry;
      new_entry->next=symbol_table;
      new_entry->prev=symbol_table;
      //new_entry->item = new_iks_dict_item();
      //iks_grammar_symbol_t *symbol;
      //symbol = new_iks_grammar_symbol();
      //iks_dict_item_set(new_entry->item,NULL,(void *)symbol);
    }
    else {
      new_entry->item = new_iks_dict_item();
      iks_dict_item_set(new_entry->item,identifier,(void *)symbol);
      iks_dict_append(symbol_table,new_entry);
    }
}

void symbol_table_init() {
    //symbol_table = new_iks_dict();
    //iks_grammar_symbol_t *symbol;
    //symbol = new_iks_grammar_symbol();
    //symbol_table->item = new_iks_dict_item();
    //iks_dict_item_set(symbol_table->item,"empty",(void *)symbol);
}

void symbol_table_delete(iks_dict_t *dict) {
    if (!iks_dict_is_empty(dict)) {
        iks_dict_t *temp;
        temp = dict->next;
        do {
            temp = temp->next;
						if (temp->prev->item) {
							iks_grammar_symbol_delete((iks_grammar_symbol_t*)temp->prev->item);					
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


void symbol_table_print(iks_dict_t *symbol_table) {
    //printf("imprimindo: %X\n",symbol_table);
    iks_dict_t *temp;
    temp = symbol_table;
    int i=0;
    do {
        if (temp->item) {
        if (temp->item->value) {
          iks_grammar_symbol_t *s;
          s = temp->item->value;
          printf("symbol: %s\n\ttoken_type: %d\n\tline: %d\n\tidentifier: %s\n\tsymbol_table: %X\n\tdecl_type: %d\n\tiks_size: %d\n\t iks_type: %d\n", \
            iks_dict_item_key_get(temp->item),\
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

iks_grammar_symbol_t *search_symbol_global(iks_grammar_symbol_t *symbol, iks_stack_t *scope) {
    iks_grammar_symbol_t *ret=NULL;
    iks_stack_t *it_scope;
    it_scope = scope;
		scope_t *scp;
    iks_dict_t *symbol_table;
    do {
      //symbol_table = iks_stack_top(it_scope);
      scp = iks_stack_top(it_scope);
			symbol_table = scp->st;
      //printf("global looking for %s at: %X\n",symbol->value,symbol_table);
      ret = search_symbol_local(symbol,symbol_table); 
      it_scope = it_scope->below;
    } while ((ret==NULL) && (it_scope != it_scope->below)); 
    if (ret==NULL) { //look at global
      //ret = search_symbol_local(symbol,iks_stack_top(it_scope)); 
      scp = iks_stack_top(it_scope);
			symbol_table = scp->st;
      ret = search_symbol_local(symbol,symbol_table); 
    }
    return ret;
}

iks_grammar_symbol_t *search_symbol_local(iks_grammar_symbol_t *symbol, iks_dict_t *symbol_table) {
    iks_grammar_symbol_t *ret =NULL;
    //printf("local looking at: %X\n",symbol_table);
    if (!iks_dict_is_empty(symbol_table)) {
      iks_dict_t *temp;
      temp = symbol_table;
      do {
          if (temp->item) {
          if (temp->item->value) {
            iks_grammar_symbol_t *s;
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


int exist_symbol_local(iks_grammar_symbol_t *symbol, iks_dict_t *symbol_table) {
    int ret = 0;
    //printf("local looking at: %X\n",symbol_table);
    if (!iks_dict_is_empty(symbol_table)) {
      iks_dict_t *temp;
      temp = symbol_table;
      do {
          if (temp->item) {
          if (temp->item->value) {
            iks_grammar_symbol_t *s;
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


//int decl_symbol(iks_grammar_symbol_t *s,int iks_type, int decl_type, void *symbol_table, iks_grammar_symbol_t *function_with_param) {
int decl_symbol(iks_grammar_symbol_t *s,int iks_type, int decl_type, scope_t *scope, iks_grammar_symbol_t *function_with_param) {
  int ret=1;
  s->iks_type = iks_type;
  
	//iks_dict_t *symbol_table = (iks_dict_t*) iks_stack_top(scope->st);
	iks_dict_t *symbol_table = scope->st;

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
  s->symbol_table = (iks_dict_t*)symbol_table;
	s->scope_type = scope->type;

	if (s->decl_type==IKS_DECL_VAR) {
		if (scope->type==IKS_SCOPE_LOCAL) {
			s->addr_offset = scope->next_addr + 16; //RA offset
		}
		else {
			s->addr_offset = scope->next_addr;
		}
		scope->next_addr += s->iks_size;
	}
  
  if (!exist_symbol_local(s,s->symbol_table)) {
    symbol_table_append(s->value,s,s->symbol_table);
    if (function_with_param) {
      iks_list_append(function_with_param->params,(void*)s);
    }
  } else {
    ret=0;
    fprintf(stderr,"line %d: identificador '%s' já declarado\n",s->code_line_number,s->value);
  }
  
  return ret;
}

int update_vector_symbol(iks_grammar_symbol_t *s,unsigned int dimen_counter,iks_list_t *dimens) {
  //s->iks_size = s->iks_size * atoi(lit->value);
  
  if(s != NULL && dimens != NULL) {
  	s->decl_type = IKS_DECL_VECTOR;
		s->num_dimen = dimen_counter;
		s->dimens = dimens;
		
		iks_list_t *d = dimens;
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

int symbol_is_decl_type(iks_grammar_symbol_t *s,int decl_type) {
  int ret=1;
  if (!(s->decl_type==decl_type)) {
    ret=0;
  }
  return ret;
}


int iks_error(iks_grammar_symbol_t *s, int error_type) {
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


int verify_function_args(iks_grammar_symbol_t *s, iks_list_t *args) {
  int ret=0;
  iks_grammar_symbol_t *s1,*s2;
  iks_list_t *l1,*l2;
  int sl1,sl2,diff;
  l1 = s->params->next;
  l2 = args->next;
  sl1 = iks_list_size(s->params->next);
  sl2 = iks_list_size(args->next);
  diff = sl1-sl2;
  if (diff!=0) {
    if (sl1>sl2) {
      fprintf(stderr,"faltam %d argumentos em '%s'\n",diff,s->value);
      ret=IKS_ERROR_MISSING_ARGS;
    }   
    else {
      fprintf(stderr,"sobram %d argumentos em '%s'\n",diff*-1,s->value);
      ret=IKS_ERROR_EXCESS_ARGS;
    }   
  }
  else if (sl1!=0){
    do {
       s1 = l1->item;
       s2 = l2->item;
       if (s1->iks_type!=s2->iks_type) {
        fprintf(stderr,"tipos incompativeis entre '%s' e '%s'\n",s1->value,s2->value);
        ret=IKS_ERROR_WRONG_TYPE_ARGS;
        break;
       }
       l1 = l1->next;
       l2 = l2->next;
    } while(l1 != s->params);
  }
  return ret;
}


int symbol_is_iks_type(iks_grammar_symbol_t *s,int iks_type) {
  int ret=1;
  //printf("%d vs %d\n",s->iks_type,iks_type);
  if (!(s->iks_type==iks_type)) {
    ret=0;
  }
  return ret;
}


