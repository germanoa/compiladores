#ifndef COMP_LIST_H
#define COMP_LIST_H

/**
 * List struct
 */
typedef struct comp_list_t comp_list_t;
struct comp_list_t {
    void *item; /*!< Item has no type defined. Be moderate. */
    //int item; /*!< Not present at specification. Defined to tests. */
    comp_list_t *prev; /*!< Pointer to previous list node */
    comp_list_t *next; /*!< Pointer to next list node */
};

/**
 * Initialize list with item as NULL and next and prev pointing to list itself
 * just for internal use.
 */
static inline void __comp_list_init(comp_list_t *list);
/**
 * Return 1 if list is empty
 */
static inline int __comp_list_is_empty(comp_list_t *list);
/**
 * Create a list, alloc mem and return address
 */
comp_list_t *new_comp_list();
/**
 * Free address memory used by list
 */
void comp_list_delete(comp_list_t *list);
/**
 * add node as the last in the list
 */
void comp_list_append(comp_list_t *list, comp_list_t *new_node);
/**
 * add node as the first in the list
 */
void comp_list_insert(comp_list_t *list, comp_list_t *new_node);
/*
 * remove node
 */
void comp_list_remove(comp_list_t *list, comp_list_t *node_to_remove);
/*
 * concat two lists
 */
void comp_list_concat(comp_list_t *list1, comp_list_t *list2);
/*
 * set item
 */
void comp_list_set_item(comp_list_t *list, void *item);
/*
 * print list items
 */
void comp_list_print(comp_list_t *list) ;

#endif /* COMP_LIST_H */
