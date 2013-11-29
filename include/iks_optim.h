/*
  iks_optim.h
*/
#ifndef __IKS_OPTIM_H__
#define __IKS_OPTIM_H__

#include "iks_list.h"
#include "iks_iloc.h"

/*
 * optimization main
 */
void optim_main(iks_list_t *code, int window, int count);

/*
 * CMP two iloc instructions
 */
static int _iloc_cmp(iloc_oper_t *i1, iloc_oper_t *i2);

/*
 * algebric optimization
 */
void alg_optim(iks_list_t *c, int w);

/*
 * ctrl_flow optimization
 */
void ctrl_flow_optim(iks_list_t *c, int w);

/*
 * machine instructions use
 */
void machine_inst(iks_list_t *c, int w);

/*
 * consts pre-calc
 */
void consts_precalc(iks_list_t *c, int w);

/*
 * propagation copy
 */
void propagation_copy_optim(iks_list_t *c, int w);

#endif /* __IKS_OPTIM_H__ */
