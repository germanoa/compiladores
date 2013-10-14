#define IKS_SUCCESS            0 //caso não houver nenhum tipo de erro

/* Verificação de declarações */
#define IKS_ERROR_UNDECLARED  1 //identificador não declarado
#define IKS_ERROR_DECLARED    2 //identificador já declarado

/* Uso correto de identificadores */
#define IKS_ERROR_VARIABLE    3 //identificador deve ser utilizado como variável
#define IKS_ERROR_VECTOR      4 //identificador deve ser utilizado como vetor
#define IKS_ERROR_FUNCTION    5 //identificador deve ser utilizado como função

/* Tipos e tamanho de dados */
#define IKS_ERROR_WRONG_TYPE  6 //tipos incompatíveis
#define IKS_ERROR_STRING_TO_X 7 //coerção impossível do tipo string
#define IKS_ERROR_CHAR_TO_X   8 //coerção impossível do tipo char

/* Argumentos e parâmetros */
#define IKS_ERROR_MISSING_ARGS    9  //faltam argumentos 
#define IKS_ERROR_EXCESS_ARGS     10 //sobram argumentos 
#define IKS_ERROR_WRONG_TYPE_ARGS 11 //argumentos incompatíveis

/* Verificação de tipos em comandos */
#define IKS_ERROR_WRONG_PAR_INPUT  12 //parâmetro não é identificador
#define IKS_ERROR_WRONG_PAR_OUTPUT 13 //parâmetro não é literal string ou expressão
#define IKS_ERROR_WRONG_PAR_RETURN 14 //parâmetro não é expressão compatível com tipo do retorno
