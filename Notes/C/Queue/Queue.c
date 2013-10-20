#include <stdlib.h>
#include "Queue.h"

void add(queue_t *head, int data){

	queue_t* newNode = malloc(sizeof(queue_t));

	while(head->next != NULL){
		head = head->next;
	}

	newNode->data = data;

	head->next = newNode;

}

int poll(queue_t **head){

	if(!(*head)){
		return;
	}

	queue_t* node = (*head); 
	int ret = node->data;
	*head = node->next;

	free(node);
	return ret;
}
int peek(queue_t* head){

	return head->data;

}
void initQueue(queue_t *head, int data){

	head->data = data;
	head->next = NULL;

}