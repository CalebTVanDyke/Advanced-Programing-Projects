all: main

main: main.o BST.o
	gcc -g -o main main.o BST.o

main.o: main.c
	gcc -g -c main.c

BST.o: BST.c BST.h
	gcc -g -c BST.c