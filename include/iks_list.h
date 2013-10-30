#ifndef __IKS_LIST_H__
#define __IKS_LIST_H__

/**
 * List struct
 */
typedef struct iks_list_t iks_list_t;
struct iks_list_t {
    void *item; /*!< Item has no type defined. Be moderate. */
    //int item; /*!< Not present at specification. Defined to tests. */
    iks_list_t *prev; /*!< Pointer to previous list node */
    iks_list_t *next; /*!< Pointer to next list node */
};

/**
 * Initialize list with item as NULL and next and prev pointing to list itself
 * just for internal use.
 */
static inline void __iks_list_init(iks_list_t *list);
/**
 * Return 1 if list is empty
 */
int iks_list_is_empty(iks_list_t *list);
/**
 * Create a list, alloc mem and return address
 */
iks_list_t *new_iks_list();
/**
 * Free address memory used by list
 */
void iks_list_delete(iks_list_t *list);
/**
 * add node as the last in the list
 */
void iks_list_append(iks_list_t *list, void *item);
/**
 * add node as the first in the list
 */
void iks_list_insert(iks_list_t *list, void *item);
/*
 * remove node
 */
void iks_list_remove(iks_list_t *list, iks_list_t *node_to_remove);
/*
 * concat two lists
 */
iks_list_t *iks_list_concat(iks_list_t *list1, iks_list_t *list2);
/*
 * set item
 */
void iks_list_set_item(iks_list_t *list, void *item);
/*
 * print list items
 */
void iks_list_print(iks_list_t *list) ;

/*
 * print list items
 */
int iks_list_size(iks_list_t *list) ;


#endif /* __IKS_LIST_H__ */
