# Makefile for verilogtree project

make-verilogtree: src/main.cc src/parseUserArgs.cc src/deriveHierarchyTree.cc
	g++ src/main.cc src/parseUserArgs.cc src/deriveHierarchyTree.cc -o verilogtree

clean:
	rm $(find . -name *.o) verilogtree
