#include <stdio.h>

extern FILE *yyin;

int iloc_parser(int argc, char **argv)
{
  FILE *file;

  if (argc==2) {
    file = fopen(argv[1],"r");
  }
  else {
    printf("are you crazy??? just ONE param!\n");
    return 1;
  }

  yyin=file;

  int result = yyparse();
  
  return result;
}
