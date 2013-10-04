#include "start.h"
#include <stdio.h>


int main(int argc, char const *argv[])
{
	node_t *stack = NULL;

	int i, l, r;
	int answer = 0;

	for (i = 0; i < argc; ++i)
	{
		switch(argv[i][0]){
			case '+': 
				l = pop(&stack);
				r = pop(&stack);
				push(&stack, l+r);
				break;
			case '-': 
				l = pop(&stack);
				r = pop(&stack);
				push(&stack, l-r);
				break;
			case '*': 
				l = pop(&stack);
				r = pop(&stack);
				push(&stack, l*r);
				break;
			case '/': 
				l = pop(&stack);
				r = pop(&stack);
				push(&stack, l/r);
				break;
			case '%': 
				l = pop(&stack);
				r = pop(&stack);
				push(&stack, l%r);
				break;
			default:
				push(&stack, atol(argv[i]));

		}
	}
	printf("answer: %d\n", getTop(stack));

	return 0;
}