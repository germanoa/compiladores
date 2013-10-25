#ifndef __IKS_HASH_H__
#define __IKS_HASH_H__

#define HASH_WIDTH 3210

typedef struct iks_iks_hash_node_t iks_iks_hash_node_t_t;
struct iks_iks_hash_node_t
{
  char *content;
  int type;
  struct iks_iks_hash_node_t *next;
}

typedef struct iks_iks_hash_table_t iks_iks_hash_table_t_t;
struct iks_iks_hash_table_t{
  iks_iks_hash_node_t *hash[HASH_WIDTH];
};

int iks_hash_create(iks_hash_table_t *hash);

int iks_hash_destroy(iks_hash_table_t *hash);

void iks_hash_print(iks_hash_table_t *hash);

iks_hash_node_t *iks_hash_insert(iks_hash_table_t *hash, int type, char *content);

iks_hash_node_t *iks_hash_seek(iks_hash_table_t *hash, int type, char *content);

#endif /* __IKS_HASH_H__ */
