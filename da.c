#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "da.h"

struct da{
  void (*print)(FILE *, void *);
  void **items;
  int size;
  int capacity;
  void *return_item;
};

DA *newDA(void (*d)(FILE *, void*)){
  DA *dynaArray = malloc(sizeof(DA));
  dynaArray->items = malloc(sizeof(void*));
  assert(dynaArray->items != 0);
  dynaArray->size = 0;
  dynaArray->capacity = 1;
  dynaArray->print = d;
  return dynaArray;

}
void insertDA(DA *items,void *value){
  assert(items->items != NULL);
  if(items->size == items->capacity){
    items->capacity *= 2;
    //malloc(sizeof(void*) * items->capacity);
    items->items = realloc(items->items, sizeof(void*) * items->capacity);
  }
  items->items[items->size] = value;
  items->size += 1;
}
void *removeDA(DA *items){
  assert(items->items != 0);
  items->return_item = items->items[items->size-1];
  items->size -= 1;
  if(items->size < 0.25 * items->capacity){
    items->capacity /= 2;
    if(items->capacity == 0)
        items->capacity = 1;
    items->items = realloc(items->items, sizeof(void*) * items->capacity);
  }
  return items->return_item;
}

void unionDA(DA *recipient,DA *donor){
  int i;
  for(i = 0; i < donor->size; i++){
    insertDA(recipient, getDA(donor, i));
  }
  free(donor);
  donor = newDA(recipient->print);
}

void *getDA(DA *items,int index){
  assert(index >= 0 && index < items->size);
  return items->items[index];
}

void *setDA(DA *items,int index,void *value){
  assert(index >= 0 && index <= items->size);
  //if index = size
  if(index == items->size){
    insertDA(items, value);
    return NULL;
  }
  else{
  void *returnVal = items->items[index];
  items->items[index] = value;
  return returnVal;
  }
}

void **extractDA(DA *items){
  if(items->size == 0)
              return 0;
  void (*print)(FILE *, void *) = items->print;
  void **returnArray = realloc(items->items, items->size);
  free(items);
  items = newDA(print);
  assert(items != 0);
  return returnArray;
}
int sizeDA(DA *items){
  return items->size;
}
void visualizeDA(FILE *fp,DA *items){
  int i;
  fprintf(fp, "[");
  for(i = 0; i < items->size; i++){
    items->print(fp, items->items[i]);
    if(i < items->size - 1)
      fprintf(fp, ",");
  }
  fprintf(fp, "][%d]",items->capacity - items->size);
}
void displayDA(FILE *fp,DA *items){
  int i;
  fprintf(fp, "[");
  for(i = 0; i < items->size; i++){
    items->print(fp, items->items[i]);
    if (i < items->size - 1)
      fprintf(fp, ",");
  }
  fprintf(fp, "]");
}
