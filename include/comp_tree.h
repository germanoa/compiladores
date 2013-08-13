#ifndef COMP_TREE_H
#define COMP_TREE_H

#include "comp_list.h"

typedef struct comp_tree_t_struct {
    void *item;
    comp_list_t *children;
} comp_tree_t;

// initialize tree with item and children as NULL
static inline __comp_tree_init(comp_tree_t *tree);

// create a tree, alloc mem and return address
comp_tree_t *new_comp_tree();

// alloc mem to child tree and append to children list
void comp_tree_add_child(comp_tree_t *tree);

// delete child tree and remove from children list
void comp_tree_remove_child(comp_tree_t *tree, comp_tree_t *child);

// free address memory used by tree
void comp_tree_delete(comp_tree_t *tree);

#endif /* COMP_TREE_H */
