#ifndef COMP_STACK_H
#define COMP_STACK_H

/**
 * Stack struct
 */
typedef struct comp_stack_t comp_stack_t;
struct comp_stack_t {
    void *item; /*!< Item has no type defined. Be moderate. */
    comp_stack_t *below; /*!< Pointer to  */
};

/**
 * Initialize stack with item as NULL and next and prev pointing to stack itself
 * just for internal use.
 */
static inline void __comp_stack_init(comp_stack_t *stack);
/**
 * Return 1 if stack is empty
 */
int comp_stack_is_empty(comp_stack_t *stack);
/**
 * Create a stack, alloc mem and return address
 */
comp_stack_t *new_comp_stack();
/**
 * Free address memory used by stack
 */
void comp_stack_delete(comp_stack_t *stack);
/**
 * push item to stack
 */
void comp_stack_push(comp_stack_t *stack, void *item);
/**
 * pop item from stack
 */
void comp_stack_pop(comp_stack_t *stack);
/**
 * return pointer item from stack top
 */
void *comp_stack_top(comp_stack_t *stack);

#endif /* COMP_STACK_H */
