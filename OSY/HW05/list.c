#include "list.h"

List* createList(){
	List* list = malloc(sizeof(List));
	list->start = NULL;
	list->end = NULL;
	list->nItems = 0;
	return list;
}

void push(Data* d, List* list){
	Node* newNode = malloc(sizeof(Node));
	newNode->data = d;
	if(list->nItems == 0){
		list->start = newNode;
	}else{
		list->end->next = newNode;
	}
	list->end = newNode;
	list->nItems++;	
}

Data* pull(List* list){
	if(list->nItems == 0){
		return NULL;
	}
	Node* retNode = list->start;
	list->start= list->start->next;
	Data* retData = retNode->data;
	list->nItems--;
	if(list->nItems == 0){
		list->end = NULL;
	}
	free(retNode);
	return retData;
}

int listSize(List* list){
	return  list->nItems;
}

void destroy(List* list){
	Node* destroy;
	Node* next = list->start;
	int i;
	for(i = 0; i < list->nItems; i++){
		destroy = next;
		next = next->next;
		free(destroy->data->word);
		free(destroy->data);
		free(destroy);
	}
	free(list);
	list = NULL;
}
void freeData(Data* data){
	free(data->word);
	free(data);
}