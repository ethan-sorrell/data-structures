#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cda.h"

struct cda{
  void (*print)(FILE*, void*);
  void **items;
  int head;
  int tail;
  int capacity;
  int size;
  void *return_item;
};

static void downsize(CDA *items){
  void **newItems = malloc(sizeof(void*) * items->capacity / 2);
  int i = 0;
  for(i = 0; i < items->size; i++){
    newItems[i] = getCDA(items, i);
  }
  items->head = 0;
  items->tail = items->size-1;
  if(items->size == 0)
    items->tail = 0;
  if(items->capacity > 1) items->capacity /= 2;
  items->items = newItems;
}

static void upsize(CDA *items){
  void **newItems = malloc(sizeof(void*) * items->capacity * 2);
  int i = 0;
  for(i = 0; i < items->size; i++){
    newItems[i] = getCDA(items, i);
  }
  items->head = 0;
  items->tail = items->size-1;
  items->items = newItems;
  items->capacity *= 2;
}

/************************** public methods ***************************/

CDA *newCDA(void (*d)(FILE *,void *)){
  CDA *items = malloc(sizeof(CDA));
  items->print = d;
  items->items = malloc(sizeof(void*));
  assert(items->items != 0);
  items->head = 0;
  items->tail = 0;
  items->capacity = 1;
  items->size = 0;
  return items;
}

void insertCDAfront(CDA *items,void *value){
  assert(items->items != 0);
  if(items->size == items->capacity){
    upsize(items);
  }
  items->head -= 1; items->size += 1;
  if(items->head < 0)
    items->head = items->capacity - 1;
  if(items->size == 1)
    items->head = 0;
  items->items[items->head] = value;
}

void insertCDAback(CDA *items,void *value){
  assert(items->items != 0);
  if(items->size == items->capacity){
    upsize(items);
  }
  items->tail += 1; items->size += 1;
  if(items->tail >= items->capacity)
    items->tail = 0;
  if(items->size == 1)
    items->tail = 0;
  items->items[items->tail] = value;
}

void *removeCDAfront(CDA *items){
  assert(items->items != 0);
  items->return_item = items->items[items->head];
  items->head += 1;
  if(items->head >= items->capacity){
    items->head = 0;
  }
  if(items->size != 0 )
    items->size -= 1;
  if(items->size < 0.25 * items->capacity ){
    downsize(items);
  }
  return items->return_item;
}

void *removeCDAback(CDA *items){
  assert(items->items != 0);
  items->return_item = items->items[items->tail];
  items->tail -= 1;
  if(items->tail <= 0){
    items->tail = items->capacity - 1;
  }
  if(items->size != 0 )
    items->size -= 1;
  if(items->size < 0.25 * items->capacity){
    downsize(items);
  }
  return items->return_item;
}
void unionCDA(CDA *recipient,CDA *donor){
  while(donor->size > 0){
    insertCDAback(recipient, removeCDAfront(donor));
  }
  donor = newCDA(donor->print);
}

void *getCDA(CDA *items,int index){
  assert(index >= 0 && index < items->size);
  if(items->head + index >= items->capacity){
    int run_over = index + items->head - items->capacity;
    return items->items[run_over];
  }else{
    return items->items[items->head + index];
  }
}
void *setCDA(CDA *items,int index,void *value){
  assert(index >= -1 && index <= items->size);
  //special cases
  if(index == items->size){
    insertCDAback(items, value);
    return NULL;
  }else if(index == -1){
    insertCDAfront(items, value);
    return NULL;
  }
  //normal case
  if(items->head + index >= items->capacity){
    int run_over = index + items->head - items->capacity - 1;
    void *return_item = items->items[run_over];
    items->items[run_over] = value;
    return return_item;
  }else{
    void *return_item = items->items[items->head + index];
    items->items[items->head + index] = value;
    return return_item;
  }
}

void **extractCDA(CDA *items){
  // shrink to fit
  void **return_items = malloc(sizeof(void*) * items->size);
  int i, j = 0;
  for(i = 0; i < items->size; i++){
    return_items[j] = getCDA(items, i);
    j++;
  }
  // new array of size 1
  void (*print)(FILE*, void*) = items->print;
  free(items);
  items = newCDA(print);
  // return array 
  return return_items;
}
int sizeCDA(CDA *items){
  return items->size;
}
void visualizeCDA(FILE *fp,CDA *items){
  int i;
  fprintf(fp, "(");
  for(i = 0; i < items->size; i++){
    items->print(fp, getCDA(items, i));
    if(i < items->size - 1)
      fprintf(fp, ",");
  }

  fprintf(fp, ")(%d)", items->capacity - items->size);
}
void displayCDA(FILE *fp,CDA *items){
  int i;
  fprintf(fp,"(");
  for(i = 0; i < items->size; i++){
    items->print(fp, getCDA(items, i));
    if(i < items->size - 1)
      fprintf(fp, ",");
  }
  fprintf(fp, ")");
}
