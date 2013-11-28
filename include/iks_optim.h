/*
  iks_optim.h
*/
#ifndef __IKS_OPTIM_H__
#define __IKS_OPTIM_H__

#include "iks_list.h"
#include "iks_iloc.h"

/*
 * CMP two iloc instructions
 */
static int _iloc_cmp(iloc_t *i1, iloc_t *i2);

/*
 * algebric optimization
 */
void alg_optim(iloc_t *i1);

/*
 * ctrl_flow optimization
 */

/*
 * machine instructions use
 */
iloc_t *machine_inst(iloc_t *i);

/*
 * consts pre-calc
 */
iloc_t *consts_precalc(iloc_t *i);

/*
 * propagation copy
 */


#endif /* __IKS_OPTIM_H__ */
