#include "comp_tree.h"
#include "comp_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void comp_tree_set_string(comp_tree_t *tree, const char *string) {
	if(tree == NULL) {
		fprintf(stderr, "ERROR: Cannot set string for null tree.\n");
		return;
	}
	
	char *_string = malloc((strlen(string) + 1)*sizeof(char));
	strcpy(_string, string);
	tree->item = _string;
}

void comp_tree_depth_print_string(comp_tree_t *tree) {
	// uses auxiliary function to print entire tree in a single line
	// using recursion, but still printing a newline at the end
	void __aux_comp_tree_depth_print_string(comp_tree_t *_tree) {
		if(_tree == NULL)
			return;
			
		printf("%s ", (char*)_tree->item);
		
		if(_tree->children != NULL) { // has children to print
			comp_list_t *childList = _tree->children;
			comp_tree_t *child = NULL;
			
			do { // runs through every child and calls recursion
				child = childList->item;
				
				if(child == NULL)
					fprintf(stderr, "WARNING: Tried to print child with no content. ");
				else __aux_comp_tree_depth_print_string(child);
				
				childList = childList->next;
			} while(childList != _tree->children);
		}
	}
	
	__aux_comp_tree_depth_print_string(tree);
	printf("\n");
}	

int comp_tree_create_child(comp_tree_t *tree) {
	if(tree == NULL) {
		fprintf(stderr, "ERROR: Cannot create child for null tree.\n");
		return 0;
	}
	
	if(tree->children == NULL) {
		tree->children = new_comp_list();
		if(tree->children == NULL) {
			fprintf(stderr, "ERROR: Error creating list entry for child.\n");
			return 0;
		}
		
		tree->children->item = new_comp_tree();
		if(tree->children->item == NULL) {
			fprintf(stderr, "ERROR: Error creating child.\n");
			return 0;
		}
	}
	else { // tree already has children
		comp_list_t *child = new_comp_list();
		if(child == NULL) {
			fprintf(stderr, "ERROR: Error creating list entry for child.\n");
			return 0;
		}
		
		child->item = new_comp_tree();
		if(child->item == NULL) {
			fprintf(stderr, "ERROR: Error creating child.\n");
			return 0;
		}
		
		comp_list_append(tree->children, child);
	}
	
	return 1;
}

void comp_tree_create_child_with_string(comp_tree_t *tree, const char *string) {
	if(comp_tree_create_child(tree))
		comp_tree_set_string(tree->children->prev->item, string);
}

int comp_tree_remove(comp_tree_t *tree, comp_tree_t *child) {
	if(tree == NULL)
		return 0;
	
	if(child == NULL) {
		fprintf(stderr, "ERROR: Cannot delete null child from tree.\n");
		return 0;
	}
	
	if(tree->children == NULL)
		return 0;
	
	comp_list_t *childList = tree->children;
	
	do { // checks if tree is father
		childList = childList->next;
	} while(childList->item != child && childList != tree->children);
	
	if(childList->item != child) { // didn't find father yet
		childList = tree->children;
		int found = 0;
		
		do { // tries to find father in tree's children
			childList = childList->next;
			found = comp_tree_remove(childList->item, child);
		} while(!found && childList != tree->children);
		
		if(!found) {
			fprintf(stderr, "ERROR: Could not find child father in tree.\n");
			return 0;
		}
		else return 1;
	}
	else { // found father
		comp_list_concat(tree->children, child->children);
		comp_list_remove(tree->children, childList);
		
		free(child);
		child == NULL;
	}
}
	

void comp_tree_delete(comp_tree_t *tree) {
	if(tree->children != NULL) {
		comp_list_t *childList = tree->children;
	
		do { // runs through every child
			childList = childList->next;
			comp_tree_delete(childList->item);
		} while(childList != tree->children);
	}
	
	comp_list_delete(tree->children);
	
    free(tree);
    tree = NULL;
}
