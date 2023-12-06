# Makefile for verilogtree project, reference: https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

# don't forget to .gitignore the object files dir

# test whether the header files are being tracked properly

IDIR=src/include
SRC=src/*.cc
CC=g++
CFLAGS=-I$(IDIR)
# $(IDIR)
DEPS = src/include/*.h
#src/include/deriveHierarchyTree.h src/include/parseUserArgs.h
ODIR=src/obj/
OBJ = main.o deriveHierarchyTree.o parseUserArgs.o

# compile (but don't link) each changed C++ file
%.o: src/%.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# link all the object files
verilogtree: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	cp verilogtree ./tests
	
.PHONY: clean

clean:
	rm -rf $(ODIR)*.o verilogtree tests/verilogtree
