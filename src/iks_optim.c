/*
  iks_optim.c
*/

#include "iks_list.h"
#include "iks_iloc.h"

static int _iloc_cmp(iloc_oper_t *o1, iloc_oper_t *o2) {

  if ( (o1->opcode == o2->opcode) &&
    (!strncmp(o1->src_operands->item,o2->src_operands->item)) &&
    (!strncmp(o1->src_operands->next->item,o2->src_operands->next->item)) &&
    (!strncmp(o1->src_operands->next->next->item,o2->src_operands->next->next->item)) &&
    (!strncmp(o1->dst_operands->item,o2->dst_operands->item)) &&
    (!strncmp(o1->dst_operands->next->item,o2->dst_operands->next->item)) &&
    (!strncmp(o1->dst_operands->next->next->item,o2->dst_operands->next->next->item)) ) {
    return 0;

  } else {
    return 1;
  }

}

void alg_optim(iks_list_t *c, int w) {

}

void machine_inst(iks_list_t *c, int w) {

}

void const_precalc(iks_list_t *c, int w) {

}

void propagation_copy_optim(iks_list_t *c, int w) {

}

void ctrl_flow_optim(iks_list_t *c, int w) {

}

void optim_main(iks_list_t *code, int window, int count) {

  if (!iks_list_is_empty(code) && window!=0) {

    while(count--) {

      int s = iks_list_size(code);
      if(s<window) {
        window=s;
      }

      int n_windows = s / window;    
      int resto = s % window;


      iks_list_t *it;
      it = code;
      while(n_windows--) {
        const_precalc(it, window); 
        alg_optim(it, window); 
        propagation_copy_optim(it, window);
        ctrl_flow_optim(it, window);
        machine_inst(it, window); 

        //prepara proxima janela      
        int temp=window;
        while(temp--) {
          it = it->next;
        }
      }

      if (resto>0) {
        const_precalc(it, resto); 
        alg_optim(it, resto); 
        propagation_copy_optim(it, resto);
        ctrl_flow_optim(it, resto);
        machine_inst(it, resto); 
      }

    }

  }   

}
