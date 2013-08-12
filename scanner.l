%{
#include "tokens.h"
%}
%%
int { return TK_PR_INT; }
. { return TOKEN_ERRO; }
%%
