#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "bst.h"
#include "rbt.h"


#define getParent(x) getBSTNODEparent(x)
#define getLeft(x) getBSTNODEleft(x)
#define getRight(x) getBSTNODEright(x)

#define setParent(x, y) setBSTNODEparent(x, y)
#define setLeft(x, y) setBSTNODEleft(x, y)
#define setRight(x, y) setBSTNODEright(x, y)
#define getCount(x) (((RBTVALUE*)getBSTNODE(x))->count)



// TODO:
// - add notions of size vs words
// - stats


/****************** structs ******************/

struct rbt
{
    BST *tree;
    int size;
    int words;
    void (*display)(FILE *,void *);
    int (*comparator)(void *,void *);
};


typedef struct rbtvalue
{
    void *value;
    char color;
    int count;
}RBTVALUE;


/****************** private function ******************/

BSTNODE *getUncle(BSTNODE *node)
{
    if(getLeft(getParent(getParent(node))) == getParent(node))
        return getRight(getParent(getParent(node)));
    else
        return getLeft(getParent(getParent(node)));
}
BSTNODE *getSibling(BSTNODE *node)
{
    if(getLeft(getParent(node)) == node)
        return getRight(getParent(node));
    else
        return getLeft(getParent(node));
}
BSTNODE *getNephew(BSTNODE *node)
{
    if(getLeft(getParent(node)) == node)
        return getRight(getRight(getParent(node)));
    else
        return getLeft(getLeft(getParent(node)));
}
BSTNODE *getNiece(BSTNODE *node)
{
    if(getLeft(getParent(node)) == node)
        return getLeft(getLeft(getParent(node)));
    else
        return getRight(getRight(getParent(node)));
}

char linear(BSTNODE *child, BSTNODE *parent)
{
    if(child == getLeft(parent))
    {
        if(getLeft(getParent(parent)) == parent)
            return 'l';
        else return 0;
    }
    else if(child == getRight(parent))
    {
        if(getRight(getParent(parent)) == parent)
            return 'R';
        else return 0;
    }
    return 0;
}
void rbtDisp(RBT *t, FILE *fp, void *val)
{
    t->display(fp, ((RBTVALUE*)val)->value);
}

char getColor(BSTNODE *node)
{
    if(node == NULL)
        return 'B';
    return ((RBTVALUE*)getBSTNODE(node))->color;
}

void setColor(BSTNODE *node, char color)
{
    if(node == NULL)
        return;
    ((RBTVALUE*)getBSTNODE(node))->color = color;
}

RBTVALUE *newRBTVALUE(void *value)
{
    RBTVALUE *newVal = (RBTVALUE*)(malloc(sizeof(RBTVALUE)));
    newVal->value = value;
    newVal->color = 'R';
    newVal->count = 1;
    return newVal;
}

static void swapper (BSTNODE *a, BSTNODE *b)
{
    RBTVALUE *ra = getBSTNODE(a);
    RBTVALUE *rb = getBSTNODE(b);

    void *vtemp = ra->value;
    ra->value = rb->value;
    rb->value = vtemp;

    int ctemp = ra->count;
    ra->count = rb->count;
    rb->count = ctemp;
}

void rotateRight(RBT *t, BSTNODE *gp)
{

    BSTNODE *p = getLeft(gp);
    BSTNODE *pseudoRoot = getParent(gp);

    if(pseudoRoot != NULL && getLeft(getParent(gp)) == gp)
        setLeft(pseudoRoot, p);
    else if(pseudoRoot != NULL && getRight(getParent(gp)) == gp)
        setRight(pseudoRoot, p);
    else if(getBSTroot(t->tree) == gp)
        setBSTroot(t->tree, p);

    setLeft(gp, getRight(p));
    setRight(p, gp);
}

void rotateLeft(RBT *t, BSTNODE *gp)
{
    BSTNODE *p = getRight(gp);
    BSTNODE *pseudoRoot = getParent(gp);

    if(pseudoRoot != NULL && getLeft(getParent(gp)) == gp)
        setLeft(pseudoRoot, p);
    else if(pseudoRoot != NULL && getRight(getParent(gp)) == gp)
        setRight(pseudoRoot, p);
    else if(getBSTroot(t->tree) == gp)
        setBSTroot(t->tree, p);

    setRight(gp, getLeft(p));
    setLeft(p, gp);
}

BSTNODE *doubleLeftRotate(RBT *t, BSTNODE *node)
{
    // passed the grandparent of walk
    // returns the new value for walk (x in lusth pseudocode)

    // DEBUG
    // displayBST(stdout, t->tree);
    BSTNODE *gp = node;
    BSTNODE *parent = getRight(gp);
    BSTNODE *walk = getLeft(parent);
    BSTNODE *pseudoRoot = getParent(gp);

    if(pseudoRoot != NULL && getLeft(getParent(gp)) == gp)
        setLeft(pseudoRoot, walk);
    else if(pseudoRoot != NULL && getRight(getParent(gp)) == gp)
        setRight(pseudoRoot, walk);
    else if(getBSTroot(t->tree) == gp)
        setBSTroot(t->tree, walk);

    setRight(gp, getLeft(walk));
    setLeft(parent, getRight(walk));
    setLeft(walk, gp);
    setRight(walk, parent);
    setColor(gp, 'R');
    setColor(walk, 'B');
    return parent;
}

BSTNODE *doubleRightRotate(RBT *t, BSTNODE *node)
{
    // passed the grandparent of walk
    // returns the new value for walk (x in lusth pseudocode)

    // DEBUG
    // displayBST(stdout, t->tree);
    BSTNODE *gp = node;
    BSTNODE *parent = getLeft(gp);
    BSTNODE *walk = getRight(parent);
    BSTNODE *pseudoRoot = getParent(gp);

    if(pseudoRoot != NULL && getLeft(getParent(gp)) == gp)
        setLeft(pseudoRoot, walk);
    else if(pseudoRoot != NULL && getRight(getParent(gp)) == gp)
        setRight(pseudoRoot, walk);
    else if(getBSTroot(t->tree) == gp)
        setBSTroot(t->tree, walk);

    setLeft(gp, getRight(walk));
    setRight(parent, getLeft(walk));
    setRight(walk, gp);
    setLeft(walk, parent);
    setColor(gp, 'R');
    setColor(walk, 'B');
    return parent;
}

void insertFixupRBT(RBT *t, BSTNODE *node)
{
    BSTNODE *walk = node;
    BST *tree = t->tree;
    while(1)
    {
        if(getBSTroot(tree) == walk)
            break;
        if(getColor(getParent(walk)) == 'B')
            break;

        if(getColor(getUncle(walk)) == 'R')
        {
            setColor(getParent(walk), 'B');
            setColor(getUncle(walk), 'B');
            setColor(getParent(getParent(walk)), 'R');
            walk = getParent(getParent(walk));
        }
        else
        {
            if(!linear(walk, getParent(walk)))
            {
                if(getLeft(getParent(getParent(walk))) == getParent(walk))
                    walk = doubleRightRotate(t, getParent(getParent(walk)));
                else
                    walk = doubleLeftRotate(t, getParent(getParent(walk)));
                break;
            }
            setColor(getParent(walk), 'B');
            setColor(getParent(getParent(walk)), 'R');

            if(getLeft(getParent(getParent(walk))) == getParent(walk))
                rotateRight(t, getParent(getParent(walk)));
            else
                rotateLeft(t, getParent(getParent(walk)));
            break;
        }
    }
    setColor(getBSTroot(tree), 'B');
}

static void deleteFixupRBT(RBT *t, BSTNODE *x)
{
    while(1)
    {
        if(getBSTroot(t->tree) == x)
            break;
        if(getColor(x) == 'R')
            break;
        if(getColor(getSibling(x)) == 'R')
        {
            setColor(getParent(x), 'R');
            setColor(getSibling(x), 'B');
            // rotate sibling to parent
            if(getLeft(getParent(x)) == getSibling(x))
                rotateRight(t, getParent(x));
            else if (getRight(getParent(x)) == getSibling(x))
                rotateLeft(t, getParent(x));
        }
        else if(getColor(getNephew(x)) == 'R')
        {
            setColor(getSibling(x), getColor(getParent(x)));
            setColor(getParent(x), 'B');
            setColor(getNephew(x), 'B');
            // rotate sibling to parent
            if(getLeft(getParent(x)) == getSibling(x))
                rotateRight(t, getParent(x));
            else if(getRight(getParent(x)) == getSibling(x))
                rotateLeft(t, getParent(x));

            break;
        }
        else if(getColor(getNiece(x)) == 'R')
        {
            // nephew should be black
            setColor(getNiece(x), 'B');
            setColor(getSibling(x), 'R');
            // rotate niece to sibling
            if(getLeft(getSibling(x)) == getNiece(x))
                rotateRight(t, getSibling(x));
            else if(getRight(getSibling(x)) == getNiece(x))
                rotateLeft(t, getSibling(x));
            // should have red nephew now 
        }
        else
        {
            // sibling, niece, and nephew should be black
            setColor(getSibling(x), 'R');
            x = getParent(x);
        }
    }
    setColor(x, 'B');
}

/*********************** public methods ***********************/
RBT *newRBT(void (*display)(FILE *,void *),           //display
            int (*comparator)(void *,void *))         //comparator
{
    RBT *newTree = (RBT*)malloc(sizeof(RBT));
    newTree->tree = newBST(display, comparator, swapper);
    newTree->comparator = comparator;
    newTree->display = display;
    newTree->size = 0;
    return newTree;
}

extern void insertRBT(RBT *t,void *value)
{
    t->words += 1;

    RBTVALUE *newVal = newRBTVALUE(value);
    BSTNODE *node = findBST(t->tree, newVal);
    if(node != NULL)
    {
        // node already exists
        getCount(node) += 1;
        return;
    }

    // ELSE:
    // node does not already exist
    t->size += 1;
    BSTNODE *newNode = insertBST(t->tree, newVal);
    setBSTNODE(newNode, newVal);

    insertFixupRBT(t, newNode);
}

extern void *findRBT(RBT *t,void *value)
{
    RBTVALUE *rbtVal = newRBTVALUE(value);
    BSTNODE *node = findBST(t->tree, rbtVal);
    if(node == NULL)
        return 0;
    // FIXME
    return ((RBTVALUE*)getBSTNODE(node))->value;
}

extern void deleteRBT(RBT *t,void *value)
{
    t->words -= 1;

    RBTVALUE *rbtVal = newRBTVALUE(value);
    BSTNODE *node = findBST(t->tree, rbtVal);
    if(node == NULL)
    {
        printf("Value ");
        t->display(stdout, value);
        printf(" not found.\n");
        return;
    }
    if(getCount(node) > 1)
    {
        getCount(node) -= 1;
        return;
    }
    else
    {
        t->size -= 1;
        node = swapToLeafBST(t->tree, node);
        deleteFixupRBT(t, node);
        pruneLeafBST(t->tree, node);
    }

}

extern int sizeRBT(RBT *t)
{
    return t->size;
}
extern int wordsRBT(RBT *t)
{
    return t->words;
}
extern void statisticsRBT(FILE *fp,RBT *rbt)
{
    fprintf(fp,"Words/Phrases: %d\n", wordsRBT(rbt));
    fprintf(fp,"Nodes: %d\n", sizeRBT(rbt));
    int minDepth = 9999999;
    int maxDepth = 1;

    {
        BST *t = rbt->tree;

        QUEUE *nodeBuffer = newQUEUE(NULL);
        int inc_level = -1;
        int treeDepth = 1;

        if(getBSTroot(t) == NULL)
        {
            fprintf(fp, "Minimum depth: 1\nMaximum depth: 1\n");
            return;
        }

        enqueue(nodeBuffer, &inc_level);
        if(getLeft(getBSTroot(t)))
            enqueue(nodeBuffer, getLeft(getBSTroot(t)));
        if(getRight(getBSTroot(t)))
            enqueue(nodeBuffer, getRight(getBSTroot(t)));

        // init loop
        while(sizeQUEUE(nodeBuffer) > 0)
        {
            BSTNODE *node = dequeue(nodeBuffer);

            if( (int *) node == &inc_level)
            {
                if(sizeQUEUE(nodeBuffer) == 0)
                {
                    continue;
                }
                treeDepth += 1;
                enqueue(nodeBuffer, &inc_level);
                continue;
            }
            // enqueue
            if(getLeft(node))
                enqueue(nodeBuffer, getLeft(node));
            if(getRight(node))
                enqueue(nodeBuffer, getRight(node));

            // check node
            if(!getRight(node) || !getLeft(node))
            {
                if(treeDepth > maxDepth)
                    maxDepth = treeDepth;
                if(treeDepth < minDepth)
                    minDepth = treeDepth;
            }
        }
        fprintf(fp, "Minimum depth: %d\nMaximum depth: %d\n", minDepth, maxDepth);
    }
}

extern void displayRBT(FILE *fp,RBT *rbt)
{
    BST *t = rbt->tree;

    QUEUE *nodeBuffer = newQUEUE(NULL);
    int inc_level = -1;
    int treeDepth = 0;

    if(getBSTroot(t) == NULL)
    {
        fprintf(fp, "EMPTY\n");
        return;
    }

    enqueue(nodeBuffer, &inc_level);
    if(getLeft(getBSTroot(t)))
        enqueue(nodeBuffer, getLeft(getBSTroot(t)));
    if(getRight(getBSTroot(t)))
        enqueue(nodeBuffer, getRight(getBSTroot(t)));

    // init loop
    fprintf(fp, "%d: ", treeDepth);
    if(!getRight(getBSTroot(t)) && !getLeft(getBSTroot(t)))
        fprintf(fp, "=");
    rbtDisp(rbt, fp, getBSTNODE(getBSTroot(t)));
    if(getCount(getBSTroot(t)) > 1)
        fprintf(fp, "-%d", getCount(getBSTroot(t)));
    fprintf(fp, "-%c", getColor(getBSTroot(t)));
    fprintf(fp, "(");
    rbtDisp(rbt, fp, getBSTNODE(getBSTroot(t)));
    if(getCount(getBSTroot(t)) > 1)
        fprintf(fp, "-%d", getCount(getBSTroot(t)));
    fprintf(fp, "-%c", getColor(getBSTroot(t)));
    fprintf(fp, ")-");

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
        if(getLeft(node))
        {
            // DEBUG
            // printf("enqueue-ing: ");
            // rbtDisp(rbt, fp, getBSTNODE(getLeft(node)));
            // printf(" from: ");
            // rbtDisp(rbt, fp, getBSTNODE(node));
            // printf("\n");
            enqueue(nodeBuffer, getLeft(node));
        }
        if(getRight(node))
        {
            // DEBUG
            // printf("enqueue-ing: ");
            // rbtDisp(rbt, fp, getBSTNODE(getRight(node)));
            // printf(" from: ");
            // rbtDisp(rbt, fp, getBSTNODE(node));
            // printf("\n");
            enqueue(nodeBuffer, getRight(node));
        }

        // print node
        fprintf(fp, " ");
        if(!getRight(node) && !getLeft(node))
            fprintf(fp, "=");
        rbtDisp(rbt, fp, getBSTNODE(node));
        if(getCount(node) > 1)
            fprintf(fp, "-%d", getCount(node));
        fprintf(fp, "-%c", getColor(node));
        fprintf(fp, "(");
        rbtDisp(rbt, fp, getBSTNODE(getParent(node)));
        if(getCount(getParent(node)) > 1)
            fprintf(fp, "-%d", getCount(getParent(node)));
        fprintf(fp, "-%c", getColor(getParent(node)));
        fprintf(fp, ")-");
        if(getLeft(getParent(node)) == node)
            fprintf(fp, "l");
        else
            fprintf(fp, "r");
    }
}
