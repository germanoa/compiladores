%{
  #include "parser.h"
%}

%option noyywrap

%%


add { return add; }
sub { return sub; }
mult { return mult; }
div { return _div; }
inv { return inv; }
addI { return addI; }
subI { return subI; }
multI { return multI; }
divI { return divI; }
rdivI { return rdivI; }
and { return and; }
andI { return andI; }
or { return or; }
orI { return orI; }
xor { return xor; }
xorI { return xorI; }
lshift { return lshift; }
rshift { return rshift; }
lshiftI { return lshiftI; }
rshifI { return rshifI; }
load { return load; }
loadAI { return loadAI; }
loadA0 { return loadA0; }
loadI { return loadI; }
cload { return cload; }
cloadAI { return cloadAI; }
cloadA0 { return cloadA0; }
store { return store; }
storeAI { return storeAI; }
storeA0 { return storeA0; }
cstore { return cstore; }
cstoreAI { return cstoreAI; }
cstoreA0 { return cstoreA0; }
i2i { return i2i; }
c2c { return c2c; }
c2i { return c2i; }
i2c { return i2c; }
cmp_LT { return cmp_LT; }
cmp_LE { return cmp_LE; }
cmp_EQ { return cmp_EQ; }
cmp_GE { return cmp_GE; }
cmp_GT { return cmp_GT; }
cmp_NE { return cmp_NE; }
cbr { return cbr; }
jump { return jump; }
jumpI { return jumpI; }
nop { return nop; }
tbl { return tbl; }

r[0-9]+ { return r; }
l[0-9]+ { return l; }

, { return comma; }
=> { return to; }

%%

//int ilocparserwrap() {
//  return 0;
//}


//void yyerror(char *str) {
void ilocparsererror(char *str) {
  fflush(stderr);
  fprintf(stderr,"error\n");
}
