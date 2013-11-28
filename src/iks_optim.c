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

void alg_optim(iloc_t *i1) {

}

iloc_t *machine_inst(iloc_t *i) {

  return i;
}

iloc_t *consts_precalc(iloc_t *i) {

  return i;
}

