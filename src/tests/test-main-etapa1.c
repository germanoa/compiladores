#include <stdio.h>
#include "parser.h"
#include "comp_grammar.h"

extern char *yytext;
int getLineNumber (void)
{
    return yy_line_number_get();
}

#define print_nome(TOKEN) printf("%d " #TOKEN " [%s]\n", getLineNumber(), yytext);
#define print_nome2(TOKEN) printf("%d %c\n", getLineNumber(), TOKEN);

int main (int argc, char **argv)
{

  symbol_table_init();
  int token = 0;
  while (token = yylex()) {
    switch (token){
    case ',':
    case ';':
    case ':':
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case '+':
    case '-':
    case '*':
    case '/':
    case '<':
    case '>':
    case '=':
    case '!':
    case '&':
    case '$': print_nome2 (token); break;
    case TK_PR_INT: print_nome(TK_PR_INT); break;
    case TK_PR_FLOAT: print_nome(TK_PR_FLOAT); break;
    case TK_PR_BOOL: print_nome (TK_PR_BOOL); break;
    case TK_PR_CHAR: print_nome (TK_PR_CHAR); break;
    case TK_PR_STRING: print_nome (TK_PR_STRING); break;
    case TK_PR_IF: print_nome (TK_PR_IF); break;
    case TK_PR_THEN: print_nome (TK_PR_THEN); break;
    case TK_PR_ELSE: print_nome (TK_PR_ELSE); break;
    case TK_PR_WHILE: print_nome (TK_PR_WHILE); break;
    case TK_PR_DO: print_nome (TK_PR_DO); break;
    case TK_PR_INPUT: print_nome (TK_PR_INPUT); break;
    case TK_PR_OUTPUT: print_nome (TK_PR_OUTPUT); break;
    case TK_PR_RETURN: print_nome (TK_PR_RETURN); break;
    case TK_OC_LE: print_nome (TK_OC_LE); break;
    case TK_OC_GE: print_nome (TK_OC_GE); break;
    case TK_OC_EQ: print_nome (TK_OC_EQ); break;
    case TK_OC_NE: print_nome (TK_OC_NE); break;
    case TK_OC_AND: print_nome (TK_OC_AND); break;
    case TK_OC_OR: print_nome (TK_OC_OR); break;
    case TK_LIT_INT: print_nome (TK_LIT_INT); break;
    case TK_LIT_FLOAT: print_nome (TK_LIT_FLOAT); break;
    case TK_LIT_FALSE: print_nome (TK_LIT_FALSE); break;
    case TK_LIT_TRUE: print_nome (TK_LIT_TRUE); break;
    case TK_LIT_CHAR: print_nome (TK_LIT_CHAR); break;
    case TK_LIT_STRING: print_nome (TK_LIT_STRING); break;
    case TK_IDENTIFICADOR: print_nome (TK_IDENTIFICADOR); break;
    case TOKEN_ERRO:  print_nome (TOKEN_ERRO); break;
    default: printf ("<Invalid Token with code %d>\n", token); return 1; break;
    }
  }
    symbol_table_print();
  return 0;
}
