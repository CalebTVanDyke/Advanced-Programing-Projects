#ifndef STACK_H
#define STACK_H


/*
	Struct for our linked list of integers
*/
typedef struct node {

	node_t *next;
	int data;

} node_t;

/*
	Is the stack empty
*/
int isEmpty(node_t* S);

/*
	Non-destructively get top of stack
	barf if the stack is empty
*/
int getTop(node_t* S);
/*
	Pushs int onto top of stack
*/
void push(node_t **S, int thing);
/*
	Pops the top item from stack
*/
int pop(node_t **S);


#endif