# Makefile for verilogtree project

make-verilogtree: src/main.cc src/parseUserArgs.cc
	g++ src/main.cc src/parseUserArgs.cc -o verilogtree
