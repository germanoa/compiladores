#include <comp_list.h>
#include <stdlib.h>
#include <stdio.h>

static inline void __comp_list_init(comp_list_t *list) {
    list->prev = list;
    list->next = list;
}

static inline int __comp_list_is_empty(comp_list_t *list) {
    return list == list->next;
}

comp_list_t *new_comp_list() {
    comp_list_t *list;
    list = malloc(sizeof(comp_list_t));
    __comp_list_init(list);
    return list;
}

void comp_list_delete(comp_list_t *list) {
    if (!__comp_list_is_empty) {
        comp_list_t *temp;
        temp = list->next;
        do {
            temp = temp->next;    
            free(temp->prev);
            temp->prev = NULL;
        } while(temp != list);
    }
    free(list);
    list = NULL; /* http://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free */
}

void comp_list_append(comp_list_t *list, comp_list_t *new_node) {
    comp_list_t *node_to_append;
    /* to append the same list any times we need a new one, but
       when active this, we cant find the list by reference, just by item value */
    //node_to_append = new_comp_list();
    //node_to_append->item = new_node->item;

    node_to_append = new_node;
    
    list->prev->next = node_to_append;
    node_to_append->prev = list->prev;
    node_to_append->next = list;
    list->prev = node_to_append;
}

void comp_list_insert(comp_list_t *list, comp_list_t *new_node) {
    comp_list_t *node_to_insert;
    /* same as append */
    //node_to_insert = new_comp_list();
    //node_to_insert->item = new_node->item;

    node_to_insert = new_node;

    list->next->prev = node_to_insert;
    node_to_insert->prev = list;
    node_to_insert->next = list->next;
    list->next = node_to_insert;
}

void comp_list_remove(comp_list_t *list, comp_list_t *node_to_remove) {
    comp_list_t *temp;
    temp = list->next;
    while(temp != list) {
        if (temp == node_to_remove) {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            break;
        }
        temp = temp->next;
    }
}

void comp_list_concat(comp_list_t *list1, comp_list_t *list2) {
    list1->prev->next = list2;
    list2->prev->next = list1;
    comp_list_t *temp_list1_prev;
    temp_list1_prev = list1->prev;
    list1->prev = list2->prev;
    list2->prev = temp_list1_prev;
}

void comp_list_print(comp_list_t *list) {
    comp_list_t *temp;
    temp = list;
    do {
        printf("%d,",temp->item);
        temp = temp->next;    
    } while(temp != list);
    printf("\n");
}
