/*
  iks_ast.h

  Este arquivo contém as constantes para os tipos dos nós da AST.
*/
#ifndef __IKS_AST_H
#define __IKS_AST_H
#define IKS_AST_PROGRAMA             0
#define IKS_AST_FUNCAO               1
#define IKS_AST_IF_ELSE              2
#define IKS_AST_DO_WHILE             3
#define IKS_AST_WHILE_DO             4
#define IKS_AST_INPUT                5
#define IKS_AST_OUTPUT               6
#define IKS_AST_ATRIBUICAO           7
#define IKS_AST_BLOCO                8
#define IKS_AST_IDENTIFICADOR        9
#define IKS_AST_LITERAL             10
#define IKS_AST_ARIM_SOMA           11
#define IKS_AST_ARIM_SUBTRACAO      12
#define IKS_AST_ARIM_MULTIPLICACAO  13
#define IKS_AST_ARIM_DIVISAO        14
#define IKS_AST_ARIM_INVERSAO       15 // - (operador unário -)
#define IKS_AST_LOGICO_E            16 // &&
#define IKS_AST_LOGICO_OU           17 // ||
#define IKS_AST_LOGICO_COMP_DIF     18 // !=
#define IKS_AST_LOGICO_COMP_IGUAL   19 // ==
#define IKS_AST_LOGICO_COMP_LE      20 // <=
#define IKS_AST_LOGICO_COMP_GE      21 // >=
#define IKS_AST_LOGICO_COMP_L       22 // <
#define IKS_AST_LOGICO_COMP_G       23 // >
#define IKS_AST_LOGICO_COMP_NEGACAO 24 // ! 
#define IKS_AST_VETOR_INDEXADO      25 // para var[exp] quando o índice exp é acesso no vetor var
#endif
