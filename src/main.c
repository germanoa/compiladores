#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "tokens.h"

int getLineNumber (void)
{
  /* deve ser implementada */
  return 0;
}

int main (int argc, char **argv)
{
  int token = TOKEN_ERRO;
  while (token = yylex()){
    printf ("token <%d> at %d\n", token, getLineNumber());
  }
  return 0;
}
