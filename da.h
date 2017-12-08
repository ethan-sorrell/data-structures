#ifndef __DA_INCLUDED__
#define __DA_INCLUDED__

#include <stdio.h>

typedef struct da DA;


extern DA *newDA(void (*d)(FILE *,void *)); 
/* constructor - passed a function that knows how to display
 * the value stored in an array slot
 */
extern void insertDA(DA *items,void *value);
/* insert - places the itme in the leftmost unfilled slot
 */
extern void *removeDA(DA *items);
/* remove - removes the rightmost item in the filled region
 * and returns it
 */
extern void unionDA(DA *recipient,DA *donor);
/* union - takes two arrays, appends donor array
 * to recipient array
 */
extern void *getDA(DA *items,int index);
/* get - returns the value at the given index
 */
extern void *setDA(DA *items,int index,void *value);
extern void **extractDA(DA *items);
extern int sizeDA(DA *items);
extern void visualizeDA(FILE *fp,DA *items);
extern void displayDA(FILE *fp,DA *items);

#endif
