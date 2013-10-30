#include "iks_tree.h"
#include "iks_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iks_gv.h"
#include "iks_ast.h"

static inline __iks_tree_init(iks_tree_t *tree) {
	tree->item = NULL;
	tree->children = NULL;
}

iks_tree_t *new_iks_tree() {
    iks_tree_t *tree = NULL;
    
    tree = malloc(sizeof(iks_tree_t));
    if(tree == NULL) {
    	fprintf(stderr, "ERROR: Allocation of new tree failed.\n");
    	return NULL;
    }


    __iks_tree_init(tree);

    iks_list_t *l;
    l = new_iks_list();
    tree->children = l;

    return tree;
}

void iks_tree_set_item(iks_tree_t *tree, void *item) {
	if(tree == NULL) {
		fprintf(stderr, "ERROR: Cannot set item for null tree.\n");
		return;
	}
	tree->item = item;
}

void iks_tree_set_string(iks_tree_t *tree, const char *string) {
	if(tree == NULL) {
		fprintf(stderr, "ERROR: Cannot set string for null tree.\n");
		return;
	}
	
	char *_string = malloc((strlen(string) + 1)*sizeof(char));
	strcpy(_string, string);
	tree->item = _string;
}

void iks_tree_depth_print_string(iks_tree_t *tree) {
	// uses auxiliary function to print entire tree in a single line
	// using recursion, but still printing a newline at the end
	void __aux_iks_tree_depth_print_string(iks_tree_t *_tree) {
		if(_tree == NULL)
			return;
			
		printf("%s ", (char*)_tree->item);
		
		if(_tree->children != NULL) { // has children to print
			iks_list_t *childList = _tree->children;
			iks_tree_t *child = NULL;
			
			do { // runs through every child and calls recursion
				child = childList->item;
				
				if(child == NULL)
					fprintf(stderr, "WARNING: Tried to print child with no content. ");
				else __aux_iks_tree_depth_print_string(child);
				
				childList = childList->next;
			} while(childList != _tree->children);
		}
	}
	
	__aux_iks_tree_depth_print_string(tree);
	printf("\n");
}	

void iks_tree_append(iks_tree_t *parent, iks_tree_t *child) {
    iks_list_append(parent->children,(void*)child);
}

void iks_tree_insert(iks_tree_t *parent, iks_tree_t *child) {
    iks_list_t *t;
    t = new_iks_list();
    iks_list_set_item(t, (void*) child);
    iks_list_insert(parent->children,t);
}

int iks_tree_create_child(iks_tree_t *tree) {
	if(tree == NULL) {
		fprintf(stderr, "ERROR: Cannot create child for null tree.\n");
		return 0;
	}
	
	if(tree->children == NULL) {
		tree->children = new_iks_list();
		if(tree->children == NULL) {
			fprintf(stderr, "ERROR: Error creating list entry for child.\n");
			return 0;
		}
		
		tree->children->item = new_iks_tree();
		if(tree->children->item == NULL) {
			fprintf(stderr, "ERROR: Error creating child.\n");
			return 0;
		}
	}
	else { // tree already has children
		iks_list_append(tree->children, (void*)new_iks_tree());
	}
	
	return 1;
}

void iks_tree_create_child_with_item(iks_tree_t *tree, void *item) {
	if(iks_tree_create_child(tree))
		iks_tree_set_item(tree->children->prev->item, item);
}

void iks_tree_create_child_with_string(iks_tree_t *tree, const char *string) {
	if(iks_tree_create_child(tree))
		iks_tree_set_string(tree->children->prev->item, string);
}

int iks_tree_remove(iks_tree_t *tree, iks_tree_t *child) {
	if(tree == NULL)
		return 0;
	
	if(child == NULL) {
		fprintf(stderr, "ERROR: Cannot delete null child from tree.\n");
		return 0;
	}
	
	if(tree->children == NULL)
		return 0;
	
	iks_list_t *childList = tree->children;
	
	do { // checks if tree is father
		childList = childList->next;
	} while(childList->item != child && childList != tree->children);
	
	if(childList->item != child) { // didn't find father yet
		childList = tree->children;
		int found = 0;
		
		do { // tries to find father in tree's children
			childList = childList->next;
			found = iks_tree_remove(childList->item, child);
		} while(!found && childList != tree->children);
		
		if(!found) {
			fprintf(stderr, "ERROR: Could not find child father in tree.\n");
			return 0;
		}
		else return 1;
	}
	else { // found father
		tree->children = iks_list_concat(tree->children, child->children);
		iks_list_remove(tree->children, childList);
		
		free(child);
		child == NULL;
	}
}
	

void iks_tree_delete(iks_tree_t *tree) {
	if((tree->children)->item != NULL) { //has children
		iks_list_t *childList = tree->children;
	
		do { // runs through every child
			childList = childList->next;
			iks_tree_delete(childList->item);
		} while(childList != tree->children);
	}
	
	iks_list_delete(tree->children);
	
  free(tree);
  tree = NULL;
}
