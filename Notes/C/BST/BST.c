#include "BST.h"
#include <stdlib.h>
#include <stdio.h>


int ADDED_DUPLICATE = 0;

void add(node_t* root, int data){
	
	node_t *currNode = root;
	node_t *newNode = malloc(sizeof(node_t));
	newNode->data = data;

	while(1){

		if(data < currNode->data){
			if(currNode->left == NULL){
				currNode->left = newNode;
				return;
			}
			currNode = currNode->left;
		}
		else if(data > currNode->data){
			if(currNode->right == NULL){
				currNode->right = newNode;
				return;
			}
			currNode = currNode->right;
		}
		else{
			ADDED_DUPLICATE = 1;
			return;
		}
	}
	currNode->data = data;
}
void printInOrder(node_t* root){
	if(root == NULL) return;
	printInOrder(root->left);
	printf("%d ", root->data);
	printInOrder(root->right);

}