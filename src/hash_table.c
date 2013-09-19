#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hash_table.h>

int hashCreate(HASH_TABLE *hashTable)
{
  int i;
  
  for(i = 0; i<HASH_WIDTH; i++)
    hashTable->hashArray[i] = NULL;
  
  return 0;
}

int hashDestroy(HASH_TABLE *hashTable)
{
  int i;
  HASH_NODE *hashNode;

  for(i = 0; i < HASH_WIDTH, i++)
  {
    for(hashNode = hashTable->hashArray[i]; hashNode != NULL; hashNode = hashNode->next)
    {
      free(hashNode);
    }
  }
  
  return 0;
}

void hashPrint(HASH_TABLE *hashTable)
{
  HASH_NODE *hashNode;

  for(int i = 0; i < HASH_WIDTH; i++){
    for(hashNode = hashTable->hashArray[i]; hashNode != NULL; hashNode = hashNode->next)
    {
      switch(hashNode->type){
        case TK_LIT_INT:
          printf("\nPos:[%d] - Tipo: TK_LIT_INT Valor: %s ", i, hashNode->content);
          break;
        case TK_LIT_FLOAT:
          printf("\nPos:[%d] - Tipo: TK_LIT_FLOAT Valor: %s ", i, hashNode->content);
          break;
        case TK_LIT_TRUE:
          printf("\nPos:[%d] - Tipo: TK_LIT_TRUEValor: %s ", i, hashNode->content);
          break;
        case TK_LIT_FALSE:
          printf("\nPos:[%d] - Tipo: TK_LIT_FALSE Valor: %s ", i, hashNode->content);
          break;
        case TK_LIT_CHAR:
          printf("\nPos[%d] - Tipo: TK_LIT_CHAR Valor: %s ", i, hashNode->content);
          break;
        case TK_LIT_STRING:
          printf("\nPos[%d] - Tipo: TK_LIT_STRING Valor: %s ", i, hashNode->content);
          break;
        default:
          printf("\nPos[%d] - Tipo: Indentifier: %s", i, node->content);
          break;
      }
    }
  }
}

int getHashCode(char *nodeContent)
{
  int hashAddress = 1;
  int i;

  for(i = 0; nodeContent[i] != '\0'; i++)
    hashAddress = (hashAddress * nodeContent[i]) % HASH_WIDTH + 1;
  
  return hashAddress;
}

HASH_NODE *hashInsert(HASH_TABLE *hashTable, int nodeType, char *nodeContent)
{
  int hashAddress;
  HASH_NODE *hashNode, *hashNewNode;

  if(hashSeek(hashTable, nodeType, nodeContent) != NULL)
  {
    //fprintf(stderr, "hashInsert Debug");
    return(hashSeek(hashTable, noteType, nodeContent));
  }

  hashAdress = getHashCode(nodeContent);
  hashNode = hashTable->hashArray[hashAdress];
  
  hashNewNode = malloc(sizeof(HASH_NODE));
  hashNewNode->type = nodeType;
  
  strcpy(hashNewNode->content, nodeContent);
  
  hashNewNode->next = hashNode;
  hashTable->hashArray[hashAddress] = hashNewNode;

  return hashNewNode;
}

HASH_NODE *hashSeek(HASH_TABLE *hashTable, int nodeType, char *nodeContent)
{
  HASH_NODE *hashNode;
  int hashAddress;
  int seekFlag = 0;

  hashAddress = getHashCode(nodeContent);
  hashNode = hashTable->hashArray[hashAddress];

  while((seekFlag == 0) && hashNode != NULL)
  {
    if(hashNode->content == nodeType && (strcmp(hashNode->content, nodeContent) == 0))
      seekFlag = 1;
    else
      hashNode = hashNode->next;
  }
  
  if(seekFlag)
    return hashNode;
  else
    return 0;
}
