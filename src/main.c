/*
   main.c

   Arquivo principal do analisador sintático.
*/

#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"

void yyerror (char const *mensagem)
{
  fprintf (stderr, "%s\n", mensagem);
}

int main (int argc, char **argv)
{
  if(argc != 2)
  {
    fprintf(stderr, "Call with the file name... \n");
    exit(1);
  }

  if(!(yyin = fopen(argv[1], "r")))
  {
    fprintf(stderr, "Cannot open file\n");
    exit(1);
  }

  int resultado = yyparse();
  return resultado;
}
