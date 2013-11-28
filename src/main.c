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
#include "iks_iloc.h"
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
  int result;

  if (argc==2) {
    result = iloc_parser(argc,argv);
  }
  else {
    iks_init();  

    gv_init("ast_graph.dot");	
    result = yyparse();
    gv_close();
    
    code_generator(&ast);
    iks_ast_node_value_t *program = ast->item;
    program_iloc = program->code;
  }

  iloc_print(program_iloc);  


  return result;
}
