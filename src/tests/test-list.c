#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "tokens.h"

int main (int argc, char **argv)
{
    comp_list_t *lista,*l1,*l2,*l3,*l4;
    lista = new_comp_list();
    lista->item = 0;
    l1 = new_comp_list();
    l2 = new_comp_list();
    l3 = new_comp_list();
    l4 = new_comp_list();
    l1->item = 1;
    l2->item = 2;
    l3->item = 3;
    l4->item = 4;
    comp_list_print(lista);
    comp_list_append(lista,l1);
    comp_list_print(lista);
    comp_list_print(lista);
    comp_list_append(lista,l2);
    comp_list_concat(lista,l3);
    comp_list_insert(lista,l4);
    comp_list_delete(lista);
    comp_list_print(lista);
    comp_list_remove(lista,l2);
    comp_list_print(lista);

    return 0;
}
