#ifndef __IKS_STACK_H__
#define __IKS_STACK_H__

/**
 * Stack struct
 */
typedef struct iks_stack_t iks_stack_t;
struct iks_stack_t {
    void *item; /*!< Item has no type defined. Be moderate. */
    iks_stack_t *below; /*!< Pointer to  */
};

/**
 * Initialize stack with item as NULL and next and prev pointing to stack itself
 * just for internal use.
 */
static inline void __iks_stack_init(iks_stack_t *stack);
/**
 * Return 1 if stack is empty
 */
int iks_stack_is_empty(iks_stack_t *stack);
/**
 * Create a stack, alloc mem and return address
 */
iks_stack_t *new_iks_stack();
/**
 * Free address memory used by stack
 */
void iks_stack_delete(iks_stack_t *stack);
/**
 * push item to stack
 */
iks_stack_t *iks_stack_push(iks_stack_t *stack, void *item);
/**
 * pop item from stack
 */
iks_stack_t *iks_stack_pop(iks_stack_t *stack);
/**
 * return pointer item from stack top
 */
void *iks_stack_top(iks_stack_t *stack);

#endif /* __IKS_STACK_H__ */
