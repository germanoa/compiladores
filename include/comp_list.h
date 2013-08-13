typedef struct comp_list_t_struct {
    void *item; //not defined yet
    comp_list_t *prev;
    comp_list_t *next;
} comp_list_t;

// initialize list with item as NULL and next and prev pointing to list itself
static inline void __comp_list_init(comp_list_t *list);
// return 1 if list is empty
static inline int __comp_list_is_empty(comp_list_t *list);
// create a list, alloc mem and return address
comp_list_t *new_comp_list();
// free address memory used by list
void comp_list_delete(comp_list_t *list);
// add node as the last in the list
void comp_list_append(comp_list_t *list, comp_list_t *new_node);
// add node as the first in the list
void comp_list_insert(comp_list_t *list, comp_list_t *new_node);
// remove node
void comp_list_remove(comp_list_t *list, comp_list_t *node_to_remove);
// concat two lists
void comp_list_concat(comp_list_t *list1, comp_list_t *list2);
