
typedef struct {
    char *key;
    void *value;
    struct comp_dict_item_t *next;
} comp_dict_item_t;

typedef struct {
    struct comp_dict_item_t *items_header;
} comp_dict_t;

