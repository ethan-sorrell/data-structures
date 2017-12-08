#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbt.h"
#include "cda.h"
#include "set.h"
#include "scanner.h"

// -------
// structs
// -------

typedef struct edge
{
    int from;
    int to;
    int weight;
} EDGE;

typedef struct vertex
{
    int val;
    int index;
    CDA *adj;
} VERTEX;

// -----------------
// private functions
// -----------------

VERTEX *makeVERTEX(int val, int index)
{
    VERTEX *v = malloc(sizeof(VERTEX));
    v->val = val;
    v->index = index;
    v->adj = newCDA(NULL);
    return v;
}
void debugDisplay(FILE *fp, void *a)
{
    fprintf(fp, "<val:%d index:%d>", ((VERTEX*)a)->val,((VERTEX*)a)->index);
}
void edgeDisplay(FILE *fp, void *e)
{
    fprintf(fp, "%d", *(int*)e);
}

int compare(void *a, void *b)
{
    // a and b will be vertices
    return ((VERTEX*)a)->val - ((VERTEX*)b)->val;
}

int getWeight(CDA *edgeList, int index)
{
    EDGE *e = getCDA(edgeList, index);
    return e->weight;
}
// void setWeight(CDA *edgeList, int index, int x)
// {
// EDGE *e = getCDA(edgeList, index);
// e->weight = x;
// }

int partition(CDA *A, int start, int end)
{
    int x = getWeight(A, end);
    int i = start - 1;
    EDGE* temp;
    int j;

    for(j = start; j < end; j++)
    {
        if(getWeight(A, j) <= x)
        {
            i += 1;
            temp = getCDA(A, j);
            setCDA(A, j, getCDA(A, i));
            setCDA(A, i, temp);
        }
    }
    temp = getCDA(A, end);
    setCDA(A, end, getCDA(A, i+1));
    setCDA(A, i+1, temp);
    return i+1;
}

void quicksort(CDA *A, int start, int end)
{
    if(start < end)
    {
        int pivot = partition(A, start, end);
        quicksort(A, start  , pivot-1);
        quicksort(A, pivot+1, end    );
    }
}

// ----
// main
// ----

int main(int argc, char **argv)
{
    // read input
    if(argc != 2)
    {
        fprintf(stderr, "Improper input\n");
        return 1;
    }
    // DEBUG
    printf("%s\n", argv[1]);
    FILE *graphFile = fopen(argv[1], "r");

    CDA *edgeList = newCDA(NULL);
    RBT *vertexList = newRBT(debugDisplay, compare);
    SET *adjacencyForest = newSET(edgeDisplay);
    int index = 0;
    int temp;
    VERTEX *v;

    // read edge list and infer vertex list
    char *token = readToken(graphFile);
    int *itoken;
    while(token != NULL)
    {
        EDGE *e = malloc(sizeof(EDGE));
        e->from = atoi(token);
        // DEBUG
        // printf("from %s\n", token);
        itoken = malloc(sizeof(int));
        *itoken = atoi(token);
        temp = makeSET(adjacencyForest, itoken);
        v = makeVERTEX(e->from, temp);
        insertRBT(vertexList, v);

        token = readToken(graphFile);
        e->to = atoi(token);
        // DEBUG
        // printf("to %s\n", token);
        itoken = malloc(sizeof(int));
        *itoken = atoi(token);
        temp = makeSET(adjacencyForest, itoken);
        v = makeVERTEX(e->to, temp);
        insertRBT(vertexList, v);

        token = readToken(graphFile);
        if(*token == ';')
            e->weight = 1;
        else
        {
            e->weight = atoi(token);
            // DEBUG
            printf("weight %s\n", token);
            token = readToken(graphFile);
        }

        insertCDAback(edgeList, e);
        index += 1;
        token = readToken(graphFile);
    }
    // displayRBT(stdout, vertexList);

    // DEBUG 
    int i;
    for(i = 0; i < sizeCDA(edgeList); i++)
    {
        EDGE *walk = ((EDGE*)getCDA(edgeList, i));
        printf("%d(%d)%d\n", walk->to, walk->from, walk->weight);
    }
    printf("end pre-sorted edge list\n");

    // sort edge list
    quicksort(edgeList, 0, index-1);

    /*
    // DEBUG 
    for(i = 0; i < sizeCDA(edgeList); i++)
    {
        EDGE *walk = ((EDGE*)getCDA(edgeList, i));
        printf("%d(%d)%d\n", walk->to, walk->from, walk->weight);
    }
    printf("end sorted edge list\n");
    */

    // run kruskals
    int size = index;
    index = 0;

    CDA *MST = newCDA(NULL);
    VERTEX *searchVertex = makeVERTEX(-2, -2);
    VERTEX *fromVertex;
    VERTEX *toVertex;
    int toIndex;
    int fromIndex;
    // while(index < size)
    for(index = 0; index < size; index++)
    {
        EDGE *e = getCDA(edgeList, index);

        searchVertex->val = e->to;
        toVertex = (VERTEX*) findRBT(vertexList, searchVertex);
        // displayRBT(stdout, vertexList);
        toIndex = toVertex->index;

        searchVertex->val = e->from;
        fromVertex = (VERTEX*) findRBT(vertexList, searchVertex);
        fromIndex = fromVertex->index;

        // DEBUG
        // printf("%d(%d)%d\n", e->to, e->from, e->weight);
        // displaySET(stdout, adjacencyForest);

        if(findSET(adjacencyForest, toIndex) !=
           findSET(adjacencyForest, fromIndex))
        {
                insertCDAback(MST, e);
                insertCDAback(fromVertex->adj, toVertex);
                unionSET(adjacencyForest, toIndex, fromIndex);
        }
    }
    free(searchVertex);

    // find and print total weight
    int weight = 0;
    for(i = 0; i < sizeCDA(MST); i++)
    {
        EDGE *walk = ((EDGE*)getCDA(MST, i));
        printf("%d(%d)%d\n", walk->to, walk->from, walk->weight);

        weight += ((EDGE*)getCDA(MST, i))->weight;
    }
    printf("total weight: %d\n", weight);
    return 0;
}
