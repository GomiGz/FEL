#include <stdlib.h>

typedef struct{
	int num;
	char* word;
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