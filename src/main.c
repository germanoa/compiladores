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
#include "main.h"

void yyerror(char* str)
{
    fflush(stderr);
    fprintf(stderr, "ERRO: \"%s\"\t Linha: %d token: %s\n", \
            str, yy_line_number_get(), yy_last_token_identifier_get());
}


int main (int argc, char **argv)
{
  symbol_table_init();
  
  iks_ast_init();
  
  gv_init("saida.odt");
  
  int resultado = yyparse();
  
  gv_close();
  
  return resultado;
}
