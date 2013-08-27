/*
  main.h

  Cabeçalho principal do analisador sintático
*/
#ifndef __MAIN_H
#define __MAIN_H
#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"

/*
  Constantes a serem utilizadas como valor de retorno no caso de
  sucesso (IKS_SYNTAX_SUCESS) e erro (IKS_SYNTAX_ERRO) do analisador
  sintático.
*/
#define IKS_SYNTAX_SUCESSO 0
#define IKS_SYNTAX_ERRO    1

/*
  Constantes a serem utilizadas para diferenciar os lexemas que estão
  registrados na tabela de símbolos.
*/
#define IKS_SIMBOLO_INDEFINIDO     0
#define IKS_SIMBOLO_LITERAL_INT    1
#define IKS_SIMBOLO_LITERAL_FLOAT  2
#define IKS_SIMBOLO_LITERAL_CHAR   3
#define IKS_SIMBOLO_LITERAL_STRING 4
#define IKS_SIMBOLO_LITERAL_BOOL   5
#define IKS_SIMBOLO_IDENTIFICADOR  6

#endif
