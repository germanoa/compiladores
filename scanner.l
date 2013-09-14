%{
#include <string.h>
#include "comp_dict.h"
#include "comp_grammar.h"
#include "parser.h"

extern YYSTYPE yylval;

int line_number = 1;
char *identifier;

typedef struct yyval_t yyval_t;
struct yyval_t {
    comp_grammar_symbol_t symbol;
};

yyval_t yyval;


void push_to_symbol_table(int type) {
    identifier = malloc(yyleng);
    int deslocamento=0;
    if (type==IKS_SIMBOLO_LITERAL_CHAR || type==IKS_SIMBOLO_LITERAL_STRING) {
        deslocamento=1; /* para eliminar '' e "" */
    }
    strncpy(identifier,&yytext[deslocamento],yyleng-1-deslocamento);     
    
    comp_grammar_symbol_t *symbol;
    symbol = new_comp_grammar_symbol();
    comp_grammar_symbol_set(symbol,type,line_number,NULL);

    symbol_table_append(identifier,symbol);
    yyval.symbol = *symbol;
}

%}


%x COMMENT
%%
\/\/[^\n\r]* { }

\/\* { BEGIN COMMENT; }

<COMMENT>.*"*/" { BEGIN 0; }
<COMMENT>[^\n\r] { }
<COMMENT>[\n\r] { line_number++; }


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

[0-9]+ {
    push_to_symbol_table(TK_LIT_INT);
    return TK_LIT_INT;
}

([0-9]+|-[0-9]+)\.[0-9]+ {
    push_to_symbol_table(TK_LIT_FLOAT);
    return TK_LIT_FLOAT;
}

true { return TK_LIT_TRUE; }

false { return TK_LIT_FALSE; }

'[^']' {
    push_to_symbol_table(TK_LIT_CHAR);
    return TK_LIT_CHAR;
}

\"[^\"\n\r]*\" {
    push_to_symbol_table(TK_LIT_STRING);
    return TK_LIT_STRING;
}


([a-z]|[A-Z]|_)+([a-z]|[A-Z]|_|[0-9])* {
    push_to_symbol_table(TK_IDENTIFICADOR);
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

[ \t]+ {  }

. { return TOKEN_ERRO; }
%%

int yy_line_number_get() {
    return line_number;
}

char *yy_last_token_identifier_get() {
    return identifier;
}

