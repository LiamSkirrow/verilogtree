# Makefile for verilogtree project, reference: https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

IDIR =src/include
CC=g++
CFLAGS=-I $(IDIR)

build: src/*.cc
	$(CC) -o verilogtree src/*.cc $(CFLAGS)
