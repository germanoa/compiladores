#include "iks_stack.h"
#include <stdlib.h>
#include <stdio.h>

static inline void __iks_stack_init(iks_stack_t *stack) {
	  stack->item = NULL; 
    stack->below = stack;
}

int iks_stack_is_empty(iks_stack_t *stack) {
	if( (stack == stack->below) && (stack->item == NULL) ) {
		return 1;
  }
  else return 0;
}

iks_stack_t *new_iks_stack() {
    iks_stack_t *stack;
    stack = malloc(sizeof(iks_stack_t));
    __iks_stack_init(stack);
    return stack;
}

void iks_stack_delete(iks_stack_t *stack) {
    iks_stack_t *temp,*temp2;
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

iks_stack_t *iks_stack_push(iks_stack_t *stack, void *item) {
	if(iks_stack_is_empty(stack)) {
    stack->item = item;
  }
  else {
    iks_stack_t *old_stack_top;
    old_stack_top=new_iks_stack();
    old_stack_top->item = stack->item;
    stack->item = item;
    stack->below = old_stack_top; 
  }
  //printf("stack push: %X -> %X\n",stack, stack->item);
  return stack;
}

iks_stack_t *iks_stack_pop(iks_stack_t *stack) {
	if(!iks_stack_is_empty(stack)) {
    iks_stack_t *temp;
    temp = stack;
    //printf("stack pop sai: %X -> %X\n",stack,stack->item);
    stack = stack->below;
    //printf("stack pop novo top: %X -> %X\n",stack,stack->item);
    free(temp);
  }
  return stack;
}

void *iks_stack_top(iks_stack_t *stack) {
  //printf("stack top: %X -> %X\n",stack,stack->item);
  return stack->item;
}
