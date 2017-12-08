#include <stdio.h>
#include <stdlib.h>
#include "set.h"
#include "da.h"

// -------
// structs
// -------
typedef struct node
{
    void *value;
    int parent;
    int rank;
}NODE;

struct set
{
    void (*display)(FILE *, void*);
    DA *list;
    int count;
};

// -----------------
// private functions
// -----------------

static void linkSET(SET *d,int index1,int index2)
{
    NODE *nodeVal1 = (NODE*)getDA(d->list, index1);
    NODE *nodeVal2 = (NODE*)getDA(d->list, index2);
    if(nodeVal1->rank > nodeVal2->rank)
        nodeVal2->parent = index1;
    else if(nodeVal1->rank < nodeVal2->rank)
        nodeVal1->parent = index2;
    else
    {
        if(index1 < index2)
        {
            nodeVal2->parent = index1;
            nodeVal2->rank += 1;
        }
        else
        {
            nodeVal1->parent = index2;
            nodeVal1->rank += 1;
        }
    }
}

// ----------------
// public functions
// ----------------

extern SET *newSET(void (*d)(FILE *,void *))
{
    // allocate
    SET *this = malloc(sizeof(SET));
    DA *list = newDA(NULL);

    // set
    this->display = d;
    this->list = list;

    // initialize
    this->count = 0;

    // return
    return this;
}
extern int makeSET(SET *d,void *value)
{
    int index = sizeDA(d->list);

    //DEBUG
    // printf("inserting ");
    // d->display(stdout, value);
    // printf("\n");

    // make node
    NODE *nodeVal = malloc(sizeof(NODE));
    nodeVal->value = value;
    nodeVal->parent = index;
    nodeVal->rank = 0;

    setDA(d->list, index, nodeVal);
    d->count += 1;

    return index;
}
extern int findSET(SET *d,int index)
{
    NODE *nodeVal = (NODE*)getDA(d->list, index);
    int parent = nodeVal->parent;

    if(index != parent)
        nodeVal->parent = findSET(d, parent);
    return nodeVal->parent;
}
extern int unionSET(SET *d,int index1,int index2)
{
    if(findSET(d, index1) == findSET(d, index2))
        return 0;

    linkSET(d, findSET(d,index1), findSET(d, index2));

    d->count -= 1;
    return 1;
}
extern int countSET(SET *d)
{
    return d->count;
}
extern int displaySET(FILE *fp,SET *d)
{
    int i;
    for(i = 0; i < sizeDA(d->list); i++)
    {
        fprintf(fp,"%d:", i);
        NODE *nodeVal = (NODE*)getDA(d->list, i);
        int index = i;

        fprintf(fp, " ");
        d->display(fp, nodeVal->value);
        while(index != nodeVal->parent)
        {
            index = nodeVal->parent;
            nodeVal = (NODE*)getDA(d->list, index);
            fprintf(fp, " ");
            d->display(fp, nodeVal->value);
        }
        fprintf(fp, "\n");
    }
    return 0;
}


