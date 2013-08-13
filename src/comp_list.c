#include "comp_list.h"
#include <stdlib.h>

static inline void __comp_list_init(comp_list_t *list) {
    list->prev = list;
    list->next = list;
}

static inline int __comp_list_is_empty(comp_list_t *list) {
    return list == list->next;
}

comp_list_t *new_comp_list_create() {
    comp_list_t *list;
    list = malloc(sizeof(comp_list_t));
    __comp_list_init(list);
    return list;
}

void comp_list_delete(comp_list_t *list) {
    free(list);
    list = NULL;
}

void comp_list_append(comp_list_t *list, comp_list_t *new_node) {
    list->prev->next = new_node;
    new_node->prev = list->prev;
    new_node->next = list;
    list->prev = new_node;
}

void comp_list_insert(comp_list_t *list, comp_list_t *new_node) {
    list->next->prev = new_node;
    new_node->prev = list;
    new_node->next = list->next;
    list->next = new_node;
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
