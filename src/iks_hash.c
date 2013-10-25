#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iks_hash.h>

int iks_hash_create(iks_hash_table_t *hash){
  int i;
  
  for(i = 0; i<HASH_WIDTH; i++)
    hash->hash[i] = NULL;
  
  return 0;
}


int iks_hash_destroy(iks_hash_table_t *hash){
  int i;
  iks_hash_node_t *node;

  for(i = 0; i < HASH_WIDTH, i++)
  {
    for(node = hash->hash[i]; node != NULL; node = node->next)
    {
      free(node);
    }
  }
  
  return 0;
}


void iks_hash_print(iks_hash_table_t *hash){
  iks_hash_node_t *node;

  for(int i = 0; i < HASH_WIDTH; i++){
    for(node = hash->hash[i]; node != NULL; node = node->next)
    {
      switch(node->type){
        case TK_LIT_INT:
          printf("\nPos:[%d] - Tipo: TK_LIT_INT Valor: %s ", i, node->content);
          break;
        case TK_LIT_FLOAT:
          printf("\nPos:[%d] - Tipo: TK_LIT_FLOAT Valor: %s ", i, node->content);
          break;
        case TK_LIT_TRUE:
          printf("\nPos:[%d] - Tipo: TK_LIT_TRUEValor: %s ", i, node->content);
          break;
        case TK_LIT_FALSE:
          printf("\nPos:[%d] - Tipo: TK_LIT_FALSE Valor: %s ", i, node->content);
          break;
        case TK_LIT_CHAR:
          printf("\nPos[%d] - Tipo: TK_LIT_CHAR Valor: %s ", i, node->content);
          break;
        case TK_LIT_STRING:
          printf("\nPos[%d] - Tipo: TK_LIT_STRING Valor: %s ", i, node->content);
          break;
        default:
          printf("\nPos[%d] - Tipo: Indentifier: %s", i, node->content);
          break;
      }
    }
  }
}

int iks_hash_get_code(char *content){
  int address = 1;
  int i;

  for(i = 0; content[i] != '\0'; i++)
    address = (address * content[i]) % HASH_WIDTH + 1;
  
  return address;
}


iks_hash_node_t *iks_hash_insert(iks_hash_table_t *hash, int type, char *content){
  int address;
  iks_hash_node_t *node, *hashNewNode;

  if(iks_hash_seek(hash, type, content) != NULL)
  {
    //fprintf(stderr, "iks_hash_insert Debug");
    return(iks_hash_seek(hash, noteType, content));
  }

  hashAdress = iks_hash_get_code(content);
  node = hash->hash[hashAdress];
  
  hashNewNode = malloc(sizeof(iks_hash_node_t));
  hashNewNode->type = type;
  
  strcpy(hashNewNode->content, content);
  
  hashNewNode->next = node;
  hash->hash[address] = hashNewNode;

  return hashNewNode;
}


iks_hash_node_t *iks_hash_seek(iks_hash_table_t *hash, int type, char *content){
  iks_hash_node_t *node;
  int address;
  int seek_flag = 0;

  address = iks_hash_get_code(content);
  node = hash->hash[address];

  while((seek_flag == 0) && node != NULL)
  {
    if(node->content == type && (strcmp(node->content, content) == 0))
      seek_flag = 1;
    else
      node = node->next;
  }
  
  if(seek_flag)
    return node;
  else
    return 0;
}
