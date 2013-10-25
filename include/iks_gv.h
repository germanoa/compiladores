/*
  iks_gv.h

  Funções públicas do módulo gv. Verifique a implementação para
  maiores detalhes de como utilizar cada uma. 

  Atenção: não altere este arquivo
*/

#ifndef __IKS_GV_H__
#define __IKS_GV_H__

void gv_init (const char *filename);
void gv_close (void);
void gv_declare (const int tipo, const void *pointer, char *name);
void gv_connect (const void *p1, const void *p2);

#endif /* __IKS_GV_H__ */
