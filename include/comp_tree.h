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

// set tree item with string
void comp_tree_set_string(comp_tree_t *tree, const char *string);

// print, depth first, entire tree contents as strings, separated by spaces
void comp_tree_depth_print_string(comp_tree_t *tree);

// alloc mem to child tree and append to children list
// returns 1 if successful; 0 otherwise
int comp_tree_create_child(comp_tree_t *tree);

// create child and set its item with string
void comp_tree_create_child_with_string(comp_tree_t *tree, const char *string);

// delete child tree and remove from children list
void comp_tree_delete_child(comp_tree_t *tree, comp_tree_t *child);

// free address memory used by tree and its subtrees
void comp_tree_delete(comp_tree_t *tree);

#endif /* COMP_TREE_H */
