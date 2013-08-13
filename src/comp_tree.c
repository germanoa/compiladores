#include "comp_tree.h"
#include <stdlib.h>

static inline __comp_tree_init(comp_tree_t *tree) {
	item = NULL;
    tree->children = NULL;
}

comp_tree_t *new_comp_tree_create() {
    comp_tree_t *tree;
    tree = malloc(sizeof(comp_tree_t));
    __comp_tree_init(tree);
    return tree;
}

void comp_tree_delete(comp_tree_t *tree) {
// TO-DO: DELETE ENTIRE TREE
    free(tree);
    tree = NULL;
}
