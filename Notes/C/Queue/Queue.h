#ifndef QUEUE_H
#define QUEUE_H value

typedef struct queueType
{
	struct queueType* next;
	int data;
}queue_t;

void initQueue(queue_t *head, int data);
void add(queue_t *head, int data);
int poll(queue_t **head);
int peek(queue_t* head);

#endif