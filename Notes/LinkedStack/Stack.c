#include "start.h"
#include <stdlib.h>

int isEmpty(node_t* S){
	return !S;
}

/*
	Non-destructively get top of stack
	barf if the stack is empty
*/
int getTop(node_t* S){
	if(isEmpty(S)){
		fprintf(stderr, "Barf\n");
	}
	return S->data;
}
/*
	Pushs int onto top of stack
*/
void push(node_t **S, int thing){
	node_t *newNode = malloc(sizeof(node_t));
	newNode->data = thing;
	newNode->next = *S;
	*S = newNode;
}
/*
	Pops the top item from stack
*/
int pop(node_t **S){
	if(isEmpty(*S)){
		fprintf(stderr, "Stack underflow\n");
	}

	node_t * top = *S;
	*S = top->next;

	int ret = top->data;
	free(top);
	return ret;
}
