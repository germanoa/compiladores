#include "comp_tree.h"
#include "comp_list.h"
#include <stdlib.h>
#include <stdio.h>

static inline __comp_tree_init(comp_tree_t *tree) {
	tree->item = NULL;
	tree->children = NULL;
}

comp_tree_t *new_comp_tree() {
    comp_tree_t *tree = NULL;
    
    tree = malloc(sizeof(comp_tree_t));
    if(tree == NULL) {
    	fprintf(stderr, "ERROR: Allocation of new tree failed.\n");
    	return NULL;
    }
    
    __comp_tree_init(tree);
    
    return tree;
}

void comp_tree_add_child(comp_tree_t *tree) {
	if(tree == NULL) {
		fprintf(stderr, "WARNING: Cannot add new child for null tree. Will autoallocate tree and add child.\n");
		tree = new_comp_tree();
	}
	
	if(tree->children == NULL) {
		tree->children = new_comp_list();
		if(tree->children == NULL) {
			fprintf(stderr, "ERROR: Error adding child.\n");
			return;
		}
		
		tree->children->item = new_comp_tree();
		if(tree->children->item == NULL) {
			fprintf(stderr, "ERROR: Error adding child.\n");
			return;
		}
	}
	else {
		comp_list_t *child = new_comp_list();
		if(child == NULL) {
			fprintf(stderr, "ERROR: Error adding child.\n");
			return;
		}
		
		child->item = new_comp_tree();
		if(child->item == NULL) {
			fprintf(stderr, "ERROR: Error adding child.\n");
			return;
		}
		
		comp_list_append(tree->children, child);
	}
}

void comp_tree_remove_child(comp_tree_t *tree, comp_tree_t *child) {
	if(tree == NULL) {
		fprintf(stderr, "ERROR: Cannot remove child from null tree.\n");
		return;
	}
	
	if(child == NULL) {
		fprintf(stderr, "ERROR: Cannot remove null child from tree.\n");
		return;
	}
	
	if(tree->children == NULL) {
		fprintf(stderr, "ERROR: Cannot remove child from childless tree.\n");
		return;
	}
	
	comp_list_t *childEntry = NULL;
	
}
	

void comp_tree_delete(comp_tree_t *tree) {
// TO-DO: DELETE ENTIRE TREE
    free(tree);
    tree = NULL;
}
