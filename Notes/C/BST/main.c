#include <stdlib.h>
#include <stdio.h>
#include "BST.h"

int main(int argc, char const *argv[])
{
	node_t* root = malloc(sizeof(node_t));

	root->data = 10;

	add(root, 5);
	add(root, 15);
	add(root, 20);
	add(root, 1);
	add(root, 7);
	add(root, 8);
	add(root, 9);
	add(root, 2);
	add(root, 50);
	add(root, 35);
	add(root, 42);
	add(root, 43);
	add(root, 101);
	
	printInOrder(root);

	free(root);

	return 0;
}