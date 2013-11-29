#include <stdio.h>


//extern FILE *yyin;
extern FILE *ilocparserin;

int iloc_parser(const char *filename)
{
  FILE *file;
  file = fopen(filename,"r");

  //yyin=file;
  ilocparserin=file;

  //int result = yyparse();
  int result = ilocparserparse();
  
  return result;
}
