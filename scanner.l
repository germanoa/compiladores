%{
#include <string.h>
#include "tokens.h"
#include "comp_dict.h"
#include "comp_grammar.h"
int line_number = 1;
char *identifier;

%}
%%

\/\/[^\n\r]*[\n\r] { }

\/\*[^\*\/]*\*\/ { }

int { return TK_PR_INT; }
float { return TK_PR_FLOAT; }
bool { return TK_PR_BOOL; }
char { return TK_PR_CHAR; }
string { return TK_PR_STRING; }
if { return TK_PR_IF; }
then { return TK_PR_THEN; }
else { return TK_PR_ELSE; }
while { return TK_PR_WHILE; }
do { return TK_PR_DO; }
input { return TK_PR_INPUT; }
output { return TK_PR_OUTPUT; }
return { return TK_PR_RETURN; }

[0-9]+|-[0-9]+ { return TK_LIT_INT; }

([0-9]+|-[0-9]+)\.[0-9]+ { return TK_LIT_FLOAT; }

true { return TK_LIT_TRUE; }

false { return TK_LIT_FALSE; }

'[^']' {
    identifier = malloc(yyleng);
    strncpy(identifier,&yytext[1],yyleng-2);     
    
    comp_grammar_symbol_t *symbol;
    symbol = new_comp_grammar_symbol();
    comp_grammar_symbol_set(symbol,T,line_number,NULL);

    symbol_table_append(identifier,symbol);

    return TK_LIT_CHAR;
}

\"[^\"\n\r]+\" {
    identifier = malloc(yyleng);
    strncpy(identifier,&yytext[1],yyleng-2);     
    
    comp_grammar_symbol_t *symbol;
    symbol = new_comp_grammar_symbol();
    comp_grammar_symbol_set(symbol,T,line_number,NULL);

    symbol_table_append(identifier,symbol);

    return TK_LIT_STRING;
}

([a-z]|[A-Z]|_)+([a-z]|[A-Z]|_|[0-9])* {
    identifier = malloc(yyleng);
    strncpy(identifier,yytext,yyleng);    

    comp_grammar_symbol_t *symbol;
    symbol = new_comp_grammar_symbol();
    comp_grammar_symbol_set(symbol,T,line_number,NULL);

    symbol_table_append(identifier,symbol);
    
    return TK_IDENTIFICADOR;
}

\,|\;|\:|\(|\)|\[|\]|\{|\}|\+|\-|\*|\/|\<|\>|\=|\!|\&|\$ { return yytext[0]; }

\<\= { return TK_OC_LE; }
\>\= { return TK_OC_GE; }
\=\= { return TK_OC_EQ; }
\!\= { return TK_OC_NE; }
\&\& { return TK_OC_AND; }
\|\| { return TK_OC_OR; }

[\n\r] { line_number++; }

" "+ { }

. { return TOKEN_ERRO; }
%%

int yy_line_number_get() {
    return line_number;
}

char *yy_last_token_identifier_get() {
    return identifier;
}
