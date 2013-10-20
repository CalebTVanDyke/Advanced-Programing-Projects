#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

int main(int argc, char const *argv[])
{
	
	queue_t* head = malloc(sizeof(queue_t));
	initQueue(head, 11);
	add(head, 10);
	add(head,  9);
	add(head,  8);
	add(head,  7);
	add(head,  6);
	add(head,  5);
	add(head,  4);
	add(head,  3);
	add(head,  2);
	add(head,  1);
	add(head,  0);
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));
	add(head,  100);
	printf("%d\n", poll(&head));
	printf("%d\n", poll(&head));

	free(head);
	return 0;
}