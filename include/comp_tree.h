#include "comp_list.h"

typedef struct comp_tree_t_struct {
    void *item;
    comp_list_t *children;
} comp_tree_t;

// initialize tree with item and children as NULL
static inline __comp_tree_init(comp_tree_t *tree);

// create a tree, alloc mem and return address
comp_tree_t *new_comp_tree();
// free address memory used by tree
void comp_tree_delete(comp_tree_t *tree);
