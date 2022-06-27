#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct{
	char* name;
	int tool;
	pthread_t thread_id;
	bool quit;
	bool active;
}Data;

typedef struct _node{
	Data* data;
	struct _node* next;
}Node;

typedef struct{
	Node* start;
	Node* end;
	int nItems;
}List;

List* createList();
void push(Data* d, List* list);
Data* pull(List* list);
int listSize(List* list);
void destroy(List* list);
void freeData(Data* data);
void removeByName(List** list, char* name);
Data* findWorker(List* list, char* name);
