# Makefile for verilogtree project, reference: https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

IDIR=src/include
SRC=src/*.cc
CC=g++
CFLAGS=-I$(IDIR)
# $(IDIR)
DEPS = src/include/*.h
#src/include/deriveHierarchyTree.h src/include/parseUserArgs.h
ODIR=src/obj/
OBJ = src/obj/main.o src/obj/deriveHierarchyTree.o src/obj/parseUserArgs.o
# TODO:
# would be nice to make the above line use *.o instead of having to manually 
# list the object files individually

all: create-dir verilogtree

# create the object file directory
create-dir:
	@mkdir -p src/obj

# compile (but don't link) each changed C++ file
$(ODIR)%.o: src/%.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# link all the object files
verilogtree: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	@cp verilogtree ./tests
	
.PHONY: clean

clean:
	rm -rf $(ODIR)*.o verilogtree tests/verilogtree
