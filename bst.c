// Author: Ethan Sorrell
// TODO:
// - DisplayBST() :: print = sign on leaves
// - StatisticsBST() :: not currently functioning properly

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "queue.h"


#define getVal(x) (((BSTVAL*)x->value)->value)
/******************** public structs ********************/
struct bst
{
    void (*display)(FILE *, void *);
    int (*comparator)(void *, void *);
    void (*swapper)(BSTNODE *, BSTNODE *);
    BSTNODE *root;
    int size;
};

struct bstnode
{
    void (*swapper)(BSTNODE *, BSTNODE *);
    void *value;
    BSTNODE *parent;
    BSTNODE *left;
    BSTNODE *right;
};


/******************** private structs ********************/
typedef struct statsObj{
    int size;
    int min;
    int max;
} statsObj;

typedef struct
{
    void *value;
}BSTVAL;

/******************** private helper methods ********************/

BSTVAL *newBSTVAL(void *value)
{
    BSTVAL *newVal = (BSTVAL*)malloc(sizeof(BSTVAL));
    newVal->value = value;
    return newVal;
}

BSTNODE *treeSearch(BST *t, BSTNODE *node, void *value)
{
    // DEBUG
    // if(node) t->display(stdout, *(void**)getVal(node));
    if(!node || !t->comparator(*(void**)value, *(void**)getVal(node)))
        return node;
    else if(t->comparator(*(void**)getVal(node), *(void**)value) > 0)
        return treeSearch(t, node->left, value);
    else
        return treeSearch(t, node->right, value);
}
/******************** public node methods ********************/
extern void *getBSTNODE(BSTNODE *n)
{
    if(n)
        return getVal(n);
    else
        return NULL;
}
extern void setBSTNODE(BSTNODE *n, void *value)
{
    getVal(n) = value;
}
extern BSTNODE *getBSTNODEleft(BSTNODE *n)
{
    if(n)
        return n->left;
    else
        return NULL;
}
extern void setBSTNODEleft(BSTNODE *n,BSTNODE *replacement)
{
    n->left = replacement;
    if(replacement != NULL)
        replacement->parent = n;
}
extern BSTNODE *getBSTNODEright(BSTNODE *n)
{
    if(n)
        return n->right;
    else
        return NULL;
}
extern void setBSTNODEright(BSTNODE *n,BSTNODE *replacement)
{
    n->right = replacement;
    if(replacement != NULL)
        replacement->parent = n;
}
extern BSTNODE *getBSTNODEparent(BSTNODE *n)
{
    if(n)
        return n->parent;
    else
        return NULL;
}
extern void setBSTNODEparent(BSTNODE *n,BSTNODE *replacement)
{
    n->parent = replacement;
}

/******************** public tree methods ********************/
extern BST *newBST(
    void (*display)(FILE *,void *),
    int (*comparator)(void *,void *),
    void (*swapper)(BSTNODE *,BSTNODE *))
{
    BST *tree = (BST*) malloc(sizeof(BST));
    tree->display = display;
    tree->comparator = comparator;
    tree->swapper = swapper;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

extern void setBSTroot(BST *t,BSTNODE *replacement)
{
    // TODO
    // dealing with size will be complicated
    t->root = replacement;
}
extern BSTNODE *getBSTroot(BST *t)
{
    return t->root;
}

extern BSTNODE *insertBST(BST *t,void *value)
{
    t->size += 1;

    BSTNODE *newNode = (BSTNODE*)malloc(sizeof(BSTNODE));
    newNode->swapper = t->swapper;
    newNode->value = newBSTVAL(value);
    newNode->left = NULL;
    newNode->right = NULL;

    BSTNODE *walk = t->root;
    BSTNODE *parent = NULL;
    while(walk){
        parent = walk;
        if(t->comparator(*(void**)getVal(walk), *(void**)value) == 0)
            break;
        else if(t->comparator(*(void**)getVal(walk), *(void**)value) > 0){
            walk = walk->left;
        }else{
            walk = walk->right;
        }
    }

    newNode->parent = parent;
    if(parent == NULL){
        t->root = newNode;
        return newNode;
    }else if(t->comparator(*(void**)getVal(parent), *(void**)value) > 0){
        parent->left = newNode;
    }else
        parent->right = newNode;

    return newNode;
}

extern BSTNODE *findBST(BST *t,void *value)
{
    // what if treeSearch returns null node?
    BSTNODE *found = treeSearch(t, t->root, value);

    if(found)
        return found;
    else
        return NULL;
}

extern BSTNODE *deleteBST(BST *t,void *value)
{
    BSTNODE *found = findBST(t, value);
    if(found == NULL)
    {
        t->display(stdout, value);
        printf(" not found\n");
        return 0;
    }
    BSTNODE *leaf = swapToLeafBST(t, found);
    pruneLeafBST(t, leaf);
    return leaf;
}

extern BSTNODE *swapToLeafBST(BST *t,BSTNODE *node)
{
    /*
    if(getBSTroot(t) == node)
    {
        if(node->left != NULL)
        {
            setBSTroot(t, node->left);
            t->swapper(node, node->left);
            return swapToLeafBST(t, node->left);
        }
        else if(node->right != NULL)
        {
            setBSTroot(t, node->right);
        }
        else
            printf("this shouldn't happen\n");
    }
    */
    BSTNODE *walk = node;
    if(node->left != NULL){
        walk = node->left;
        while(walk->right != NULL)
            walk = walk->right;
        t->swapper(node, walk);
        return swapToLeafBST(t, walk);
    }else if(node->right != NULL){
        walk = node->right;
        while(walk->left != NULL)
            walk = walk->left;
        t->swapper(node, walk);
        return swapToLeafBST(t, walk);
    }
    return node;
}

extern void pruneLeafBST(BST *t,BSTNODE *leaf)
{
    t->size -= 1;
    if(t->root == leaf){
        t->root = NULL;
    }else if(leaf->parent->left == leaf){
        leaf->parent->left = NULL;
    }else{
        leaf->parent->right = NULL;
    }
}

extern int sizeBST(BST *t)
{
    return t->size;
}

extern void statisticsBST(FILE *fp,BST *t)
{
    QUEUE *nodeBuffer = newQUEUE(NULL);
    int inc_level = -1;
    int min = -1;
    int max = -1;
    int currDepth = 0;

    enqueue(nodeBuffer, &inc_level);
    if(t->root->left)
        enqueue(nodeBuffer, t->root->left);
    if(t->root->right)
        enqueue(nodeBuffer, t->root->right);
    while(sizeQUEUE(nodeBuffer) > 0)
    {
        BSTNODE *node = dequeue(nodeBuffer);
        if( (int *) node == &inc_level)
        {
            if(sizeQUEUE(nodeBuffer) == 0)
                break;
            currDepth += 1;
            enqueue(nodeBuffer, &inc_level);
            continue;
        }

        // enqueue
        if(node->left)
            enqueue(nodeBuffer, node->left);
        if(node->right)
            enqueue(nodeBuffer, node->right);

        if(!node->left && !node->right)
        {
            if(min == -1)
            {
                min = currDepth;
                max = currDepth;
            }
            if(currDepth > max)
            {
                max = currDepth;
            }
        }
    }
    fprintf(fp,"%d %d\n", min, max);
}

extern void displayBST(FILE *fp,BST *t)
{
    QUEUE *nodeBuffer = newQUEUE(NULL);
    int inc_level = -1;
    int treeDepth = 0;


    if(t->root == NULL)
    {
        fprintf(fp, "EMPTY\n");
        return;
    }

    enqueue(nodeBuffer, &inc_level);
    if(t->root->left)
        enqueue(nodeBuffer, t->root->left);
    if(t->root->right)
        enqueue(nodeBuffer, t->root->right);

    // init loop
    fprintf(fp, "%d: ", treeDepth);
    if(t->root->right == NULL && t->root->left == NULL)
        fprintf(fp, "=");
    t->display(fp, *(int**)getVal(t->root));
    fprintf(fp,"(");
    t->display(fp, *(int**)getVal(t->root));
    fprintf(fp,")-");

    while(sizeQUEUE(nodeBuffer) > 0)
    {
        BSTNODE *node = dequeue(nodeBuffer);
        //printf("\nsize %d\n",sizeQUEUE(nodeBuffer));

        if( (int *) node == &inc_level)
        {
            if(sizeQUEUE(nodeBuffer) == 0)
            {
                fprintf(fp, "\n");
                continue;
            }

            treeDepth += 1;
            fprintf(fp, "\n%d:", treeDepth);
            enqueue(nodeBuffer, &inc_level);
            continue;
        }

        // enqueue
        if(node->left)
            enqueue(nodeBuffer, node->left);
        if(node->right)
            enqueue(nodeBuffer, node->right);

        // print node
        printf(" ");
        if(!node->right && !node->left)
            fprintf(fp, "=");
        t->display(fp, *(int**)getVal(node));
        fprintf(fp, "(");
        t->display(fp, *(int**)getVal(node->parent));
        fprintf(fp, ")-");
        if(node->parent->left == node)
            fprintf(fp, "l");
        else
            fprintf(fp, "r");
    }
}
