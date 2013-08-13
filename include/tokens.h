/*
  tokens.h

  Lista dos tokens, com valores constantes associados.  Este arquivo
  será posterioremente substituído automaticamente. Portanto, não
  acrescente nada a ele, caso contrário seu analisador léxico não
  funcionará. Os valores das constantes são arbitrários, mas não podem
  ser alterados. Cada valor deve ser distinto e fora da escala ASCII.
  Assim, eles não entram em conflito entre si e com os tokens
  representados pelo próprio valor ASCII de caracteres isolados.
*/

/* Palavras Reservadas (PR) */
#define TK_PR_INT         256   /* int       */
#define TK_PR_FLOAT       257   /* float     */
#define TK_PR_BOOL        258   /* bool      */
#define TK_PR_CHAR        259   /* char      */
#define TK_PR_STRING      260   /* string    */
#define TK_PR_IF          261   /* if        */
#define TK_PR_THEN        262   /* then      */
#define TK_PR_ELSE        263   /* else      */
#define TK_PR_WHILE       264   /* while     */
#define TK_PR_DO          265   /* do        */
#define TK_PR_INPUT       266   /* input     */
#define TK_PR_OUTPUT      267   /* output    */
#define TK_PR_RETURN      268   /* return    */

/* Operadores Compostos (OC) */
#define TK_OC_LE    270   /* <=        */
#define TK_OC_GE    271   /* >=        */
#define TK_OC_EQ    272   /* ==        */
#define TK_OC_NE    273   /* !=        */
#define TK_OC_AND   274   /* &&        */
#define TK_OC_OR    275   /* ||        */

/* Literais (LIT) */
#define TK_LIT_INT        281
#define TK_LIT_FLOAT      282
#define TK_LIT_FALSE      283
#define TK_LIT_TRUE       284
#define TK_LIT_CHAR       285
#define TK_LIT_STRING     286

/* Indentificador */
#define TK_IDENTIFICADOR  290

/* Erro */
#define TOKEN_ERRO        291

/* Fim de tokens.h */
