%{
#include "tokens.h"
#include "comp_dict.h"
#include "comp_grammar.h"
comp_dict_t *symbol_table;
int line_number = 1;
%}
%%

[a-z|A-Z|_]+[a-z|A-Z|_|0-9]* { 
    int line = 9;

    /*comp_grammar_symbol_t *symbol;
    symbol = new_comp_grammar_symbol();
    comp_grammar_symbol_set(symbol,T,line,NULL);

    comp_dict_t *new_entry;
    new_entry = new_comp_dict();
    new_entry->item = new_comp_dict_item();
    comp_dict_item_set(new_entry->item,yytext,(void *)symbol);
    comp_dict_append(symbol_table,new_entry);
*/
    return TK_IDENTIFICADOR;
}

\<\= { return TK_OC_LE; }
\>\= { return TK_OC_GE; }
\=\= { return TK_OC_EQ; }
\!\= { return TK_OC_NE; }
\&\& { return TK_OC_AND; }
\|\| { return TK_OC_OR; }

\,|\;|\:|\(|\)|\[|\]|\{|\}|\+|\-|\*|\/|\<|\>|\=|\!|\&|\$ { return yytext[0]; }

"int" { return TK_PR_INT; }
"float" { return TK_PR_FLOAT; }
"bool" { return TK_PR_BOOL; }
"char" { return TK_PR_CHAR; }
"string" { return TK_PR_STRING; }
"if" { return TK_PR_IF; }
"then" { return TK_PR_THEN; }
"else" { return TK_PR_ELSE; }
"while" { return TK_PR_WHILE; }
"do" { return TK_PR_DO; }
"input" { return TK_PR_INPUT; }
"output" { return TK_PR_OUTPUT; }
"return" { return TK_PR_RETURN; }

[\n\r] { line_number++; }
. { return TOKEN_ERRO; }
%%

int yylinenumber() {
    return line_number;
}
