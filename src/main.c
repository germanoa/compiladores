#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "comp_grammar.h"
#include "tokens.h"

int getLineNumber (void)
{
    return yy_line_number_get();
}

char *getLastTokenIdentifier (void)
{
    return (char *)yy_last_token_identifier_get();
}

int main (int argc, char **argv)
{
    int token = TOKEN_ERRO;

    symbol_table_init();
    while (token = yylex()){
        //printf ("token <%d> at line %d: %s\n", token, getLineNumber(),getLastTokenIdentifier());
        printf ("token <%d> at %d\n", token, getLineNumber());
    }
    //symbol_table_print();
	
    return 0;
}
