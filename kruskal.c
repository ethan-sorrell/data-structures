#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "queue.h"
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
    int visited;
} EDGE;

typedef struct vertex
{
    int val;
    int index;
    CDA *edgeList;
} VERTEX;

// -----------------
// private functions
// -----------------

void reverseEdge(EDGE *e)
{
    int temp;
    temp = e->from;
    e->from = e->to;
    e->to = temp;
}

VERTEX *makeVERTEX(int val, int index)
{
    VERTEX *v = malloc(sizeof(VERTEX));
    v->val = val;
    v->index = index;
    v->edgeList = newCDA(NULL);
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

int getIndex(RBT *vertexList, VERTEX *searchVertex, int vertexNumber)
{
    searchVertex->val = vertexNumber;
    VERTEX *foundVertex = (VERTEX*) findRBT(vertexList, searchVertex);
    if(foundVertex != NULL)
        return foundVertex->index;
    else
    {
        fprintf(stderr, "Vertex %d not found\n", vertexNumber);
        assert(0);
    }
}

int getWeight(CDA *edgeList, int index)
{
    EDGE *e = getCDA(edgeList, index);
    return e->weight;
}

int getOrigin(CDA *edgeList, int index)
{
    EDGE *e = getCDA(edgeList, index);
    return e->from;
}

int getTermination(CDA *edgeList, int index)
{
    EDGE *e = getCDA(edgeList, index);
    return e->to;
}
// void setWeight(CDA *edgeList, int index, int x)
// {
// EDGE *e = getCDA(edgeList, index);
// e->weight = x;
// }
int partitionOrigin(CDA *A, int start, int end)
{
    int x = getOrigin(A, end);
    int i = start - 1;
    EDGE* temp;
    int j;

    for(j = start; j < end; j++)
    {
        if(getOrigin(A, j) < x || (getOrigin(A, j) == x && getTermination(A, j) < getTermination(A, end)) )
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
int partitionTermination(CDA *A, int start, int end)
{
    int x = getTermination(A, end);
    int i = start - 1;
    EDGE* temp;
    int j;

    for(j = start; j < end; j++)
    {
        if(getTermination(A, j) <= x)
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

int partitionWeight(CDA *A, int start, int end)
{
    int x = getWeight(A, end);
    int i = start - 1;
    EDGE* temp;
    int j;

    for(j = start; j < end; j++)
    {
        if(getWeight(A, j) < x)
        {
            i += 1;
            temp = getCDA(A, j);
            setCDA(A, j, getCDA(A, i));
            setCDA(A, i, temp);
        }
        else if(getWeight(A, j) == x && getOrigin(A, j) < getOrigin(A, end))
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

void quicksortOrigin(CDA *A, int start, int end)
{
    if(start < end)
    {
        int pivot = partitionOrigin(A, start, end);
        quicksortOrigin(A, start  , pivot-1);
        quicksortOrigin(A, pivot+1, end    );
    }
}

void quicksortTermination(CDA *A, int start, int end)
{
    if(start < end)
    {
        int pivot = partitionTermination(A, start, end);
        quicksortTermination(A, start  , pivot-1);
        quicksortTermination(A, pivot+1, end    );
    }
}

void quicksortWeight(CDA *A, int start, int end)
{
    if(start < end)
    {
        int pivot = partitionWeight(A, start, end);
        quicksortWeight(A, start  , pivot-1);
        quicksortWeight(A, pivot+1, end    );
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
    // printf("%s\n", argv[1]);
    if(strcmp(argv[1], "-v") == 0)
    {
        printf("Ethan Sorrell\n");
        return 0;
    }
    FILE *graphFile = fopen(argv[1], "r");

    CDA *edgeList = newCDA(NULL);
    RBT *vertexList = newRBT(debugDisplay, compare);
    SET *adjacencyForest = newSET(edgeDisplay);
    SET *levelForest = newSET(edgeDisplay);
    int index = 0;
    int temp;
    int i;
    VERTEX *v;

    // read edge list and infer vertex list
    char *token = readToken(graphFile);
    int *itoken;
    while(token != NULL)
    {
        EDGE *e = malloc(sizeof(EDGE));
        e->visited = 0;
        e->from = atoi(token);
        // DEBUG
        // printf("from %s\n", token);
        itoken = malloc(sizeof(int));
        *itoken = atoi(token);
        temp = makeSET(adjacencyForest, itoken);
        temp = makeSET(levelForest, itoken);
        v = makeVERTEX(e->from, temp);
        insertRBT(vertexList, v);

        token = readToken(graphFile);
        e->to = atoi(token);
        // DEBUG
        // printf("to %s\n", token);
        itoken = malloc(sizeof(int));
        *itoken = atoi(token);
        temp = makeSET(adjacencyForest, itoken);
        temp = makeSET(levelForest, itoken);
        v = makeVERTEX(e->to, temp);
        insertRBT(vertexList, v);

        // FIXME
        if(e->from > e->to)
        {
            temp = e->from;
            e->from = e->to;
            e->to = temp;
        }

        token = readToken(graphFile);
        if(*token == ';')
            e->weight = 1;
        else
        {
            e->weight = atoi(token);

            // DEBUG
            // printf("weight %s\n", token);
            token = readToken(graphFile);
        }

        insertCDAback(edgeList, e);
        index += 1;
        token = readToken(graphFile);
    }
    // displayRBT(stdout, vertexList);

    // DEBUG 
    /*
    for(i = 0; i < sizeCDA(edgeList); i++)
    {
        EDGE *walk = ((EDGE*)getCDA(edgeList, i));
        printf("%d(%d)%d\n", walk->to, walk->from, walk->weight);
    }
    printf("end pre-sorted edge list\n");
    */

    // sort edge list by origin then weight
    quicksortOrigin(edgeList, 0, index-1);
    quicksortWeight(edgeList, 0, index-1);

    // DEBUG
    /*
    printf("\nbegin sorted edge list\n");
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
    int lastWeight;
    CDA *eligbleEdges = newCDA(NULL);
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
            insertCDAback(fromVertex->edgeList, e);
            insertCDAback(toVertex->edgeList, e);
            unionSET(adjacencyForest, toIndex, fromIndex);
        }
    }

    // Display MST

    // TODO
    // empty graph?

    CDA *levelEdges = newCDA(NULL);
    QUEUE *sortedVertices = getNodeList(vertexList);
    QUEUE *treeVertices = newQUEUE(NULL);
    EDGE *walk = ((EDGE*)getCDA(MST, 0));
    int rep = -1;
    int level = 0;
    int levelFlag = 0;
    // int levelRep = getIndex(vertexList, searchVertex, walk->to);
    int treeRep = makeSET(adjacencyForest, &rep);
    int weight = 0;



    //TODO: initialize loop
    
    while(sizeQUEUE(sortedVertices) > 0)
    {
        v = dequeue(sortedVertices);

        if(sizeCDA(v->edgeList) <= 0)
            continue;

        if(findSET(adjacencyForest, treeRep)
           != findSET(adjacencyForest, v->index))
            // we have found a new tree
        {
            unionSET(adjacencyForest, treeRep, v->index);
            level = 0;
            weight = 0;
            walk = ((EDGE*)getCDA(v->edgeList, 0));

            // Init loop
            printf("%d : %d", level++, walk->from);
            printf("\n%d :", level++);

            for(i = 0; i < sizeCDA(v->edgeList); i++)
            {
                walk = ((EDGE*)getCDA(v->edgeList, i));
                if(walk->to == v->val)
                    reverseEdge(walk);
            }
            quicksortTermination(v->edgeList, 0, sizeCDA(v->edgeList)-1);

            enqueue(treeVertices, NULL);

            // loop through v's edge list
            for(i = 0; i < sizeCDA(v->edgeList); i++)
            {
                walk = ((EDGE*)getCDA(v->edgeList, i));
                if(walk->visited == 1)
                    continue;
                searchVertex->val = walk->to;
                toVertex = findRBT(vertexList, searchVertex);
                enqueue(treeVertices, toVertex);

                printf(" %d(%d)%d", walk->to, walk->from, walk->weight);
                weight += walk->weight;
                walk->visited = 1;
            }

            while(sizeQUEUE(treeVertices) > 0)
            {
                v = dequeue(treeVertices);

                if(v == NULL)
                {
                    if(sizeQUEUE(treeVertices) == 0)
                        continue;
                    // print all edges from level
                    quicksortTermination(levelEdges, 0, sizeCDA(levelEdges)-1);
                    while(sizeCDA(levelEdges) > 0)
                    {
                        walk = ((EDGE*)removeCDAfront(levelEdges));
                        printf(" %d(%d)%d", walk->to, walk->from, walk->weight);
                    }
                    levelFlag = 1;
                    enqueue(treeVertices, NULL);
                    continue;
                }

                // fix edge orientation
                for(i = 0; i < sizeCDA(v->edgeList); i++)
                {
                    walk = ((EDGE*)getCDA(v->edgeList, i));
                    if(walk->to == v->val)
                        reverseEdge(walk);
                }

                for(i = 0; i < sizeCDA(v->edgeList); i++)
                {
                    walk = ((EDGE*)getCDA(v->edgeList, i));
                    
                    if(walk->visited == 1)
                        continue;

                    searchVertex->val = walk->to;
                    toVertex = findRBT(vertexList, searchVertex);
                    enqueue(treeVertices, toVertex);

                    if(levelFlag == 1)
                    {
                        levelFlag = 0;
                        printf("\n%d :", level++);
                    }
                    insertCDAback(levelEdges, walk);
                    // printf(" %d(%d)%d", walk->to, walk->from, walk->weight);
                    weight += walk->weight;
                    walk->visited = 1;

                }
            }
            printf("\ntotal weight: %d\n----\n", weight);
        }
    }
    /*
    if(findSET(levelForest, fromVertex->index) != findSET(levelForest, rep))
        // we have finished a level

    for(i = 0; i < sizeCDA(MST); i++)
    {
        walk = ((EDGE*)getCDA(MST, i));
        searchVertex->val = walk->from;
        fromVertex = findRBT(vertexList, searchVertex);
        searchVertex->val = walk->to;
        toVertex = findRBT(vertexList, searchVertex);

        if(findSET(adjacencyForest, treeRep)
           != findSET(adjacencyForest, fromIndex))
        {
            // we have found a new tree
            if(sizeCDA(fromVertex->edgeList) <= 1)
                continue;

            quicksortOrigin(fromVertex->edgeList, 0, sizeCDA(edgeList)-1);
            quicksortWeight(fromVertex->edgeList, 0, sizeCDA(edgeList)-1);


            // new tree
            rep = fromIndex;
            level = 0;
            levelRep = toIndex;
            treeRep = rep;
            weight = 0;

            printf("%d : %d", level++, walk->from);
            printf("\n%d :", level++);
        }
        else
        {
            unionSET(levelForest, rep, fromIndex);
            levelRep = toIndex;

            printf("\n%d :", level++);
        }



            else
            unionSET(levelForest, toIndex, levelRep);

    }
    printf("\ntotal weight: %d\n----\n", weight);
    */

    /*
    // find and print total weight
    int weight = 0;
    for(i = 0; i < sizeCDA(MST); i++)
    {
        walk = ((EDGE*)getCDA(MST, i));
        printf("%d(%d)%d\n", walk->to, walk->from, walk->weight);

        weight += ((EDGE*)getCDA(MST, i))->weight;
    }
    printf("total weight: %d\n", weight);
    */
    free(searchVertex);
    return 0;
}
