struct comp_tree_t {
    void *item;
    struct comp_list_t *prev;
    struct comp_list_t *next;
};

// initialize list with prev,next with its same address
static inline __comp_list_init(struct comp_list_t *list);

// create a list, alloc mem and return address
struct comp_list_t *new_comp_list();
// free address memory used by list
void comp_list_delete(struct comp_list_t *list);
// add item as the last in the list
void comp_list_append(struct comp_list_t *list, void *item);
// add item as the first in the list
void comp_list_insert(struct comp_list_t *list, void *item);
// ?? delete by item or by index ??
void comp_list_remove(struct comp_list_t *list, void *item);

// return 0 if list is empty
int comp_list_is_empty(struct comp_list_t *list);
