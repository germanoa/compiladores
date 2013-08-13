typedef struct comp_dict_item_t comp_dict_item_t;
struct comp_dict_item_t {
    char *key;
    void *value; //not defined yet
};

typedef struct comp_dict_t comp_dict_t;
struct comp_dict_t {
    comp_dict_item_t *item;
    comp_dict_t *prev;
    comp_dict_t *next;
};
