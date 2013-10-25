#ifndef __IKS_DICT_H__
#define __IKS_DICT_H__

#define DICT_ITEM_MAX_KEY_SIZE 128
#define DICT_ITEM_MAX_VALUE_SIZE 1024

/**
 * Dict item struct
 */
typedef struct iks_dict_item_t iks_dict_item_t;
struct iks_dict_item_t {
    char *key; /*!< item key from dict */
    void *value; /*!< value mapped by key from dict. Not present at specification. */
};

/**
 * Dict struct
 */
typedef struct iks_dict_t iks_dict_t;
struct iks_dict_t {
    iks_dict_item_t *item; /*!< Pointer to dict item */
    iks_dict_t *prev; /*!< Pointer to previous dict item */
    iks_dict_t *next; /*!< Pointer to next dict item */
};

/**
 * initialize dict with prev,next with its same address
 */
static inline void __iks_dict_init(iks_dict_t *dict);
/**
 * return 0 if dict is empty
 */
inline int iks_dict_is_empty(struct iks_dict_t *dict);
/**
 * create a dict, alloc mem and return address
 */
iks_dict_t *new_iks_dict();
/**
 * free address memory used by dict
 */
void iks_dict_delete(iks_dict_t *dict);
/**
 * add node as the last in the dict
 */
void iks_dict_append(iks_dict_t *dict, iks_dict_t *new_node);
/**
 * add node as the first in the dict
 */
void iks_dict_insert(iks_dict_t *dict, iks_dict_t *new_node);
/**
 * remove node
 */
void iks_dict_remove(iks_dict_t *dict, iks_dict_t *node_to_remove);
/**
 * concat two dicts
 */
void iks_dict_concat(iks_dict_t *dict1, iks_dict_t *dict2);
/**
 * set dict item
 */
void iks_dict_set(iks_dict_t *dict, iks_dict_item_t *item);
/**
 * print dict key as char and address from pointer value
 */
void iks_dict_print(iks_dict_t *dict);

/**
 * find dict item by key
 */
iks_dict_t* iks_dict_find(iks_dict_t *dict, char *key);

/**
 * initialize dict_item
 */
static inline void __iks_dict_item_init(iks_dict_item_t *dict_item);
/**
 * create a dict_item, alloc mem and return address
 */
iks_dict_item_t *new_iks_dict_item();
/**
 * free address memory used by dict
 */
void iks_dict_item_delete(iks_dict_item_t *dict_item);
/**
 * get dict_item key
 */
char *iks_dict_item_key_get(iks_dict_item_t *dict_item);
/**
 * get dict_item value
 */
void *iks_dict_item_value_get(iks_dict_item_t *dict_item);
/**
 * set dict_item key,value. Return 1 if set ok, 0 if not
 */
int iks_dict_item_set(iks_dict_item_t *dict_item, char *key, char *value);
/**
 * print dict_item key,value
 */
void iks_dict_item_print(iks_dict_item_t *dict_item);

#endif /* __IKS_DICT_H__ */
