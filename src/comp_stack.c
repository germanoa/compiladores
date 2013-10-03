#include "comp_stack.h"
#include <stdlib.h>
#include <stdio.h>

static inline void __comp_stack_init(comp_stack_t *stack) {
	  stack->item = NULL; 
    stack->below = stack;
}

int comp_stack_is_empty(comp_stack_t *stack) {
	if( (stack == stack->below) && (stack->item = NULL) ) {
		return 1;
  }
  else return 0;
}

comp_stack_t *new_comp_stack() {
    comp_stack_t *stack;
    stack = malloc(sizeof(comp_stack_t));
    __comp_stack_init(stack);
    return stack;
}

void comp_stack_delete(comp_stack_t *stack) {
    comp_stack_t *temp,*temp2;
    temp = stack;
    while (temp != temp->below) {
        temp2 = temp;
        temp = temp->below;    
        free(temp2);
        temp2 = NULL;
    }
    free(stack);
    stack = NULL;
}

void comp_stack_push(comp_stack_t *stack, void *item) {
	if(comp_stack_is_empty(stack)) {
    stack->item = item;
  }
  else {
    comp_stack_t *old_stack_top;
    old_stack_top=new_comp_stack();
    old_stack_top->item = stack->item;
    old_stack_top->below = stack->below;
    stack->item = item;
    stack->below = old_stack_top; 
  }
}

void comp_stack_pop(comp_stack_t *stack) {
	if(!comp_stack_is_empty(stack)) {
    comp_stack_t *temp;
    temp = stack;
    stack = stack->below;
    free(temp);
  }
}

void *comp_stack_top(comp_stack_t *stack) {
  return stack->item;
}
