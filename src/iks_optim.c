/*
  iks_optim.c
*/

#include "iks_list.h"
#include "iks_iloc.h"
#include <stdlib.h>
#include <stdio.h>

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
  while(w--) {
    iloc_t *i = c->item;
    iloc_oper_t *o = i->opers->item;
    char *t1,*t2;
    switch (o->opcode) {
      // sub r1,r1 => r2
      case op_sub:
         t1 = o->src_operands->item;
         t2 = o->src_operands->next->item;
        if (!strncmp(t1,t2,2)) {
          free(t1);
          free(t2);
          t1=int_to_char(0);
          i->opers->item = new_iloc_oper(op_loadI,
                            t1,
                            NULL,
                            NULL,
                            o->dst_operands->item,
                            NULL,
                            NULL);
        }
        break;
      // addI r0, 0 => r1
      case op_addI:
        t1 = o->src_operands->next->item;
        if (!strncmp(t1,"0",1)) {
          i->opers->item = new_iloc_oper(op_load,
                            o->src_operands->item,
                            NULL,
                            NULL,
                            o->dst_operands->item,
                            NULL,
                            NULL);
        }
        break;
      // multI r0, 1 => r1
      case op_multI:
        t1 = o->src_operands->next->item;
        if (!strncmp(t1,"1",1)) {
          free(t1);
          i->opers->item = new_iloc_oper(op_load,
                            o->src_operands->item,
                            NULL,
                            NULL,
                            o->dst_operands->item,
                            NULL,
                            NULL);
        }
        break;
    }

    c = c->next;
  }
}

void machine_inst(iks_list_t *c, int w) {
  while(w--) {
    iloc_t *i = c->item;
    iloc_oper_t *o = i->opers->item;
    char *t1,*t2;
    if(o->dst_operands->item) {
    if( (!strncmp(o->src_operands->item,o->dst_operands->item,2)) &&
        (!strncmp(o->src_operands->next->item,"1",1)) ) {
      switch (o->opcode) {
        // addI r0, 1 => r0 ---> inc r0 
        case op_addI:
          i->opers->item = new_iloc_oper(op_inc,
                              o->src_operands->item,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL);
          break;
        // subI r0, 1 => r0 ---> dec r0 
        case op_subI:
          i->opers->item = new_iloc_oper(op_dec,
                              o->src_operands->item,
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL);
          break;
      }
    }
    }
    c = c->next;
  }
}

void const_precalc(iks_list_t *c, int w) {
  int found_const=0;
  char *t1,*t2,*t3;
  while(w--) {
    iloc_t *i = c->item;
    iloc_oper_t *o = i->opers->item;
    if(o->dst_operands->item) {
      if(!strncmp(t2,o->dst_operands->item)) {
        found_const=0;
      }
      if(found_const) {
        int vai=0;
        if(!strncmp(t2,o->src_operands->item,2)) {
          t3=o->src_operands->next->item;
          vai=1;
        }
        else if(!strncmp(t2,o->src_operands->next->item,2))  {
          t3=o->src_operands->item;
          vai=1;
}
          if(vai) {
            switch (o->opcode) {
              case op_add:
                  i->opers->item = new_iloc_oper(op_addI,
                                    t3,
                                    t1,
                                    NULL,
                                    o->dst_operands->item,
                                    NULL,
                                    NULL);
                  break;
              case op_sub:
                  i->opers->item = new_iloc_oper(op_subI,
                                    t3,
                                    t1,
                                    NULL,
                                    o->dst_operands->item,
                                    NULL,
                                    NULL);
                  break;
              case op_mult:
                  i->opers->item = new_iloc_oper(op_multI,
                                    t3,
                                    t1,
                                    NULL,
                                    o->dst_operands->item,
                                    NULL,
                                    NULL);
                  break;
              case op_div:
                  i->opers->item = new_iloc_oper(op_divI,
                                    t3,
                                    t1,
                                    NULL,
                                    o->dst_operands->item,
                                    NULL,
                                    NULL);
                  break;
            }
          }
      }
    }
    // loadI 2 => r2
    // op r2,r3 => r4 ---> opI 2,r3 => r4
    if(o->opcode == op_loadI) {
      t1 = o->src_operands->item;
      t2 = o->dst_operands->item;
      found_const=1;
    }
    c = c->next;
  }
}

void propagation_copy_optim(iks_list_t *c, int w) {

}

void ctrl_flow_optim(iks_list_t *c, int w) {

}

void optim_main(iks_list_t *code, int window, int count) {

  if (!iks_list_is_empty(code) && window!=0) {

    while(count--) {

      int s = iks_list_size(code);

      iks_list_t *it;
      it = code;
      while(s--) {
        //printf("nova janela\n");
        machine_inst(it, window); 
        const_precalc(it, window); 
        alg_optim(it, window); 
        propagation_copy_optim(it, window);
        ctrl_flow_optim(it, window);

        it = it->next;
      }

    }

  }   

}
