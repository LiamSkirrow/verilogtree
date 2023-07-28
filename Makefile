# Makefile for verilogtree project, reference: https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

IDIR =src/include
CC=g++
CFLAGS=-I $(IDIR)

ODIR=obj
LDIR =lib

LIBS=-lm

_DEPS = deriveHierarchyTree.h parseUserArgs.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = deriveHierarchyTree.o main.o parseUserArgs.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

verilogtree: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
