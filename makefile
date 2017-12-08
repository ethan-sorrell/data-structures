OBJS = cda.o queue.o bst.o kruskal.o rbt.o scanner.o set.o da.o
OPTS = -Wall -Wextra -std=c99 -g

kruskal : $(OBJS)
	gcc $(OBJS) $(OPTS) -lm -o kruskal

.PHONY : all try

test : kruskal
	./kruskal g2

cda.o : cda.c cda.h
	gcc $(OPTS) -c cda.c

bst.o : bst.c bst.h
	gcc $(OPTS) -c bst.c

da.o : da.c da.h
	gcc $(OPTS) -c da.c

rbt.o : rbt.c rbt.h
	gcc $(OPTS) -c rbt.c

queue.o : queue.c queue.h 
	gcc $(OPTS) -c queue.c

scanner.o : scanner.c scanner.h
	gcc $(OPTS) -c scanner.c

set.o : set.c set.h
	gcc $(OPTS) -c set.c

kruskal.o : kruskal.c
	gcc $(OPTS) -c kruskal.c

clean :
	rm -f $(OBJS)
	rm -f kruskal
