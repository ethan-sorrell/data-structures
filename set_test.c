#include "set.h"
#include "integer.h"

int main()
{
    SET *d = newSET(displayINTEGER);
    int u = makeSET(d,newINTEGER(4)); int v = makeSET(d,newINTEGER(8));
    int w = makeSET(d,newINTEGER(3)); int x = makeSET(d,newINTEGER(1));
    int y = makeSET(d,newINTEGER(7)); int z = makeSET(d,newINTEGER(9));
    unionSET(d,u,v);
    unionSET(d,w,x);
    unionSET(d,v,x);
    displaySET(stdout,d);
}
