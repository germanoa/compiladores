#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "tokens.h"

int getLineNumber (void)
{
    /* deve ser implementada */
    return 0;
}

int main (int argc, char **argv)
{
    int token = TOKEN_ERRO;
    while (token = yylex()){
        printf ("token <%d> at %d\n", token, getLineNumber());
    }
	
	/* Tree tests
	comp_tree_t *tree = new_comp_tree();
	printf("New tree.\n");
	comp_tree_set_string(tree, "1");
	puts(tree->item);
	
	comp_tree_create_child(tree);
	comp_tree_set_string(tree->children->item, "2");
	puts(((comp_tree_t*)((tree->children)->item))->item);
	
	comp_tree_create_child(tree);
	comp_tree_set_string(tree->children->next->item, "4");
	puts(((comp_tree_t*)((tree->children->next)->item))->item);
	
	comp_tree_create_child(tree->children->item);
	comp_tree_set_string(((comp_tree_t*)((tree->children)->item))->children->item, "3");
	puts(((comp_tree_t*)((((comp_tree_t*)((tree->children))->item))->children)->item)->item);
	
	comp_tree_depth_print_string(tree);
	printf("Out.\n");
	*/
	
    return 0;
}
