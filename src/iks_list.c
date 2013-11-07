#include "iks_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static inline void __iks_list_init(iks_list_t *list) {
	list->item = NULL; 
  list->prev = list;
  list->next = list;
}

int iks_list_is_empty(iks_list_t *list) {
	if( (list == list->next) && (list->item == NULL) ) {
		return 1;
	}
    else return 0;
}

iks_list_t *new_iks_list() {
    iks_list_t *list;
    list = malloc(sizeof(iks_list_t));
    __iks_list_init(list);
    return list;
}

void iks_list_delete(iks_list_t *list) {
    if (!iks_list_is_empty(list)) {
        iks_list_t *temp;
        temp = list->next;
        do {
            temp = temp->next;    
            free(temp->prev);
            temp->prev = NULL;
        } while(temp != list);
    }
    free(list);
    list = NULL; /* http://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free */
}

void iks_list_append(iks_list_t *list, void *item) {
		if(iks_list_is_empty(list)) {
			list->item = item;
		}
		else {
    	iks_list_t *node_to_append;
    	node_to_append = new_iks_list();
			iks_list_set_item(node_to_append,(void*)item);			    

    	list->prev->next = node_to_append;
    	node_to_append->prev = list->prev;
    	node_to_append->next = list;
    	list->prev = node_to_append;
		}
}

void iks_list_insert(iks_list_t *list, void *item) {
		if(iks_list_is_empty(list)) {
			list->item = item;
		}
		else {
    	iks_list_t *node_to_insert;
    	node_to_insert = new_iks_list();
			iks_list_set_item(node_to_insert,(void*)item);			    

  	  list->next->prev = node_to_insert;
    	node_to_insert->prev = list;
   	 	node_to_insert->next = list->next;
    	list->next = node_to_insert;
		}
}

void iks_list_remove(iks_list_t *list, iks_list_t *node_to_remove) {
    iks_list_t *temp;
    temp = list->next;
    while(temp != list) {
        if (temp == node_to_remove) {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            break;
        }
        temp = temp->next;
    }
}

iks_list_t *iks_list_concat(iks_list_t *list1, iks_list_t *list2) {
  iks_list_t *new_list;
  if (!iks_list_is_empty(list2)) {
    if (iks_list_is_empty(list1)) {
      free(list1);
      new_list=list2;
    }
    else {
      list1->prev->next = list2;
      list2->prev->next = list1;
      iks_list_t *temp_list1_prev;
      temp_list1_prev = list1->prev;
      list1->prev = list2->prev;
      list2->prev = temp_list1_prev;
      new_list = list1;
    }
  }
  else {
    free(list2);
    new_list=list1;
  }
  return new_list;
}

void iks_list_set_item(iks_list_t *list, void *item) {
    //malloc not here because we dont know item size (void)
    list->item = item;
}

void iks_list_print(iks_list_t *list) {
    iks_list_t *temp;
    temp = list;
    do {
        printf("%d,",(int)temp->item);
        temp = temp->next;    
    } while(temp != list);
    printf("\n");
}

int iks_list_size(iks_list_t *list) {
  int ret=0;
	if (!iks_list_is_empty(list)) {
  	iks_list_t *temp;
  	temp = list;
  	do {
  	    ret+=1;
  	    temp = temp->next;    
  	} while(temp != list);
	}
  return ret;
}
