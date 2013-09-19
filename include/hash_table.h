#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HASH_WIDTH 3210

typedef struct sHASH_NODE
{
  char *content;
  int type;
  struct sHASH_NODE *next;
}

typedef struct{
  HASH_NODE *hashArray[HASH_WIDTH];
} HASH_TABLE;

int hashCreate(HASH_TABLE *hashTable);

int hashDestroy(HASH_TABLE *hashTable);

void hashPrint(HASH_TABLE *hashTable);

HASH_NODE *hashInsert(HASH_TABLE *hashTable, int nodeType, char *nodeContent);

HASH_NODE *hashSeek(HASH_TABLE *hashTable, int nodeType, char *nodeContent);

#endif
