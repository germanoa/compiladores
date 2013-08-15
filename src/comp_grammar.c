#include <comp_grammar.h>
#include <stdlib.h>
#include <stdio.h>

static inline void __comp_grammar_symbol_init(comp_grammar_symbol_t *grammar_symbol) {
    grammar_symbol->type = ND;
    grammar_symbol->code_line_number = 0;
    grammar_symbol->value = NULL;
}

comp_grammar_symbol_t *new_comp_grammar_symbol() {
    comp_grammar_symbol_t *grammar_symbol;
    grammar_symbol = malloc(sizeof(comp_grammar_symbol_t));
    __comp_grammar_symbol_init(grammar_symbol);
    return grammar_symbol;
}

void comp_grammar_symbol_delete(comp_grammar_symbol_t *grammar_symbol) {
    free(grammar_symbol->value);
    grammar_symbol->value = NULL; /* http://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free */
    free(grammar_symbol);
    grammar_symbol = NULL; /* http://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free */
}

void comp_grammar_symbol_set(comp_grammar_symbol_t *grammar_symbol, symbol_type type, int code_line_number, void *value) {
    grammar_symbol->type = type;
    grammar_symbol->code_line_number = code_line_number;
    grammar_symbol->value = value;
}

void comp_grammar_symbol_print(comp_grammar_symbol_t *grammar_symbol) {
    printf("%s\n",(char *)grammar_symbol->value);
}
