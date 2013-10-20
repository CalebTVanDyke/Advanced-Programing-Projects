#ifndef BST_H
#define BST_H 

typedef struct nodeType{

	struct nodeType* left;
	struct nodeType* right;
	int data;

}node_t;

void add(node_t* node, int toAdd);

void printInOrder(node_t* root);


#endif