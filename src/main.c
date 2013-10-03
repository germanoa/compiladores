/*
   main.c

   Arquivo principal do analisador sintático.
*/

#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "comp_grammar.h"
#include "iks_ast.h"
#include "iks_types.h"
#include "gv.h"
#include "main.h"

void yyerror(char* str)
{
    fflush(stderr);
    fprintf(stderr, "ERRO: \"%s\"\t Linha: %d token: %s\n", \
            str, yy_line_number_get(), yy_last_token_identifier_get());
}

void iks_init() {
  symbol_table_init();
  iks_ast_init();
  scope=new_comp_stack();
}


int main (int argc, char **argv)
{
  iks_init();  
  //printf("\n\n\n%s\n\n\n\n", argv[0]);
  //printf("\n\n\n%s\n\n\n\n", argv[1]);
  //printf("\n\n\n%s\n\n\n\n", argv[2]);
  gv_init("output/etapa3/saida.dot");
  
  int resultado = yyparse();
  
  gv_close();

  symbol_table_print();
  
  return resultado;
}
