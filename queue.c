#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "cda.h"
#include "queue.h"

struct queue{
  CDA *array;
  void (*print)(FILE*, void*);
};

QUEUE *newQUEUE(void (*d)(FILE *,void *)){
  QUEUE *items = malloc(sizeof(QUEUE));
  assert(items != 0);
  items->print = d;
  items->array = newCDA(d);
  return items;
}
void enqueue(QUEUE *items, void *value){
  insertCDAback(items->array, value);
}
void *dequeue(QUEUE *items){
  assert(sizeQUEUE(items) > 0);
  return removeCDAfront(items->array);
}
void *peekQUEUE(QUEUE *items){
  assert(sizeQUEUE(items) > 0);
  return getCDA(items->array, 0);
}
int sizeQUEUE(QUEUE *items){
  return sizeCDA(items->array);
}
void displayQUEUE(FILE *fp,QUEUE *items){
  int i;
  fprintf(fp, "<");
  for(i = 0; i <= sizeCDA(items->array) - 1; i++){
    items->print(fp, getCDA(items->array, i));
    if(i < sizeCDA(items->array) - 1)
      fprintf(fp, ",");
  }
  fprintf(fp, ">");
}
void visualizeQUEUE(FILE *fp,QUEUE *items){
  displayCDA(fp, items->array);
}


