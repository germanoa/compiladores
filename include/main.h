/*
  main.h

  Cabeçalho principal do analisador sintático
*/
#ifndef __MAIN_H
#define __MAIN_H
#include <stdio.h>
#include "iks_dict.h"
#include "iks_list.h"
#include "iks_tree.h"
#include "iks_graph.h"
#include "iks_ast.h"
#include "iks_gv.h"

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
#define IKS_SIMBOLO_INT    1
#define IKS_SIMBOLO_FLOAT  2
#define IKS_SIMBOLO_CHAR   3
#define IKS_SIMBOLO_STRING 4
#define IKS_SIMBOLO_BOOL   5

#endif
