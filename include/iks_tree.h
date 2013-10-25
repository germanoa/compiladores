#ifndef __IKS_TREE_H__
#define __IKS_TREE_H__

#include "iks_list.h"

typedef struct iks_tree_t_struct {
    void *item;
    iks_list_t *children;
} iks_tree_t;

// initialize tree with item and children as NULL
static inline __iks_tree_init(iks_tree_t *tree);

// create a tree, alloc mem and return address
iks_tree_t *new_iks_tree();

// set tree item with void value
void iks_tree_set_item(iks_tree_t *tree, void *item);

// set tree item with string
void iks_tree_set_string(iks_tree_t *tree, const char *string);

// print, depth first, entire tree contents as strings, separated by spaces
void iks_tree_depth_print_string(iks_tree_t *tree);

// append new node to tree
void iks_tree_append(iks_tree_t *parent, iks_tree_t *child);

// insert new node to tree
void iks_tree_insert(iks_tree_t *parent, iks_tree_t *child);

// alloc mem to child tree and append to children list
// returns 1 if successful; 0 otherwise
int iks_tree_create_child(iks_tree_t *tree);

// create child and set its item
void iks_tree_create_child_with_item(iks_tree_t *tree, void *item);

// create child and set its item with string
void iks_tree_create_child_with_string(iks_tree_t *tree, const char *string);

// delete child node. tree inherits node's children
// returns 1 if successful; 0 otherwise
int iks_tree_remove(iks_tree_t *tree, iks_tree_t *child);

// free address memory used by tree and its subtrees
void iks_tree_delete(iks_tree_t *tree);

#endif /* __IKS_TREE_H__ */
