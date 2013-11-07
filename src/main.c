/*
   main.c

   Arquivo principal do analisador sintático.
*/

#include <stdio.h>
#include "iks_dict.h"
#include "iks_list.h"
#include "iks_tree.h"
#include "iks_graph.h"
#include "iks_grammar.h"
#include "iks_ast.h"
#include "iks_types.h"
#include "iks_gv.h"
#include "main.h"

void yyerror(char* str)
{
    fflush(stderr);
    fprintf(stderr, "ERRO: \"%s\"\t Linha: %d token: %s\n", \
            str, yy_line_number_get(), yy_last_token_identifier_get());
}

void iks_init() {
//  symbol_table_init();
  iks_ast_init();
	scope=new_scope();
}


int main (int argc, char **argv)
{
  iks_init();  
	//printf("\n%d", argc);  
	//printf("\n\n\n%s\n\n\n\n", argv[0]);
  //printf("\n\n\n%s\n\n\n\n", argv[1]);
  //printf("\n\n\n%s\n\n\n\n", argv[2]);
  gv_init("ast_graph.dot");
	
  int result = yyparse();
  gv_close();
  
	code_generator(&ast);
	iks_ast_node_value_t *program = ast->item;
	iloc_print(program->code);  

  //symbol_table_print((iks_dict_t*)iks_stack_top(scope));

  return result;
}
