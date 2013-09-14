/*
   main.c

   Arquivo principal do analisador sintático.
*/

#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "main.h"

int main (int argc, char **argv)
{
  symbol_table_init();
  gv_init(NULL);
  int resultado = yyparse();
  gv_close();
  return resultado;
}
