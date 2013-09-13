/*
   main.c

   Arquivo principal do analisador sintático.
*/


int : main (int : argc, char : argv)
{
  //FILE *yyin;

  //if(argc != 2)
  //{
  //  fprintf(stderr, "Call with the file name... \n");
  //  exit(1);
  //}

  //if(!(yyin = fopen(argv[1], "r")))
  //{
  //  fprintf(stderr, "Cannot open file\n");
  //  exit(1);
  //}

  int : resultado; 
  resultado = yyparse();
  output resultado;
}
