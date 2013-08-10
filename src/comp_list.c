#include <comp_list.h>
#include <stdlib.h>

static inline __comp_list_init(struct comp_list_t *list) {
    list->prev = list;
    list->next = list;
}

struct comp_list_t *new_comp_list_create() {
    struct comp_list_t *list;
    list = malloc(sizeof(struct comp_list_t));
    __comp_list_init(list);
    return list;
}

void comp_list_delete(struct comp_list_t *list) {
    free(list);
    list = NULL;
}

int comp_list_is_empty(struct comp_list_t *list) {
    return list == list->next;
}
