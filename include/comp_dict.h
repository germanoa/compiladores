#ifndef COMP_DICT_H
#define COMP_DICT_H

#define DICT_ITEM_MAX_KEY_SIZE 128
#define DICT_ITEM_MAX_VALUE_SIZE 1024

/**
 * Dict item struct
 */
typedef struct comp_dict_item_t comp_dict_item_t;
struct comp_dict_item_t {
    char *key; /*!< item key from dict */
    void *value; /*!< value mapped by key from dict. Not present at specification. */
};

/**
 * Dict struct
 */
typedef struct comp_dict_t comp_dict_t;
struct comp_dict_t {
    comp_dict_item_t *item; /*!< Pointer to dict item */
    comp_dict_t *prev; /*!< Pointer to previous dict item */
    comp_dict_t *next; /*!< Pointer to next dict item */
};

/**
 * initialize dict with prev,next with its same address
 */
static inline void __comp_dict_init(comp_dict_t *dict);
/**
 * return 0 if dict is empty
 */
static inline int __comp_dict_is_empty(struct comp_dict_t *dict);
/**
 * create a dict, alloc mem and return address
 */
comp_dict_t *new_comp_dict();
/**
 * free address memory used by dict
 */
void comp_dict_delete(comp_dict_t *dict);
/**
 * add node as the last in the dict
 */
void comp_dict_append(comp_dict_t *dict, comp_dict_t *new_node);
/**
 * add node as the first in the dict
 */
void comp_dict_insert(comp_dict_t *dict, comp_dict_t *new_node);
/**
 * remove node
 */
void comp_dict_remove(comp_dict_t *dict, comp_dict_t *node_to_remove);
/**
 * concat two dicts
 */
void comp_dict_concat(comp_dict_t *dict1, comp_dict_t *dict2);


/**
 * initialize dict_item
 */
static inline void __comp_dict_item_init(comp_dict_item_t *dict_item);
/**
 * create a dict_item, alloc mem and return address
 */
comp_dict_item_t *new_comp_dict_item();
/**
 * free address memory used by dict
 */
void comp_dict_item_delete(comp_dict_item_t *dict_item);
/**
 * get dict_item key
 */
char *comp_dict_item_key_get(comp_dict_item_t *dict_item);
/**
 * get dict_item value
 */
void *comp_dict_item_value_get(comp_dict_item_t *dict_item);
/**
 * set dict_item key,value. Return 1 if set ok, 0 if not
 */
int comp_dict_item_set(comp_dict_item_t *dict_item, char *key, char *value);
/**
 * print dict_item key,value
 */
void comp_dict_item_print(comp_dict_item_t *dict_item);

#endif /* COMP_DICT_H */
