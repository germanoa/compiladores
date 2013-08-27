/*
   main.c

   Arquivo principal do analisador sintático.
*/
#include "main.h"

void yyerror (char const *mensagem)
{
  fprintf (stderr, "%s\n", mensagem);
}

int main (int argc, char **argv)
{
  int resultado = yyparse();
  return resultado;
}

