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
		freeData(destroy->data);
		free(destroy);
	}
	free(list);
	list = NULL;
}
void freeData(Data* data){
	free(data->name);
	free(data);
}

void printData(Data* data){
	printf("Name: %s\nTool: %d\n",data->name,data->tool);
}
void copyData(Data* newData, Data* oldData){
	newData->name = malloc(strlen(oldData->name)+1);
	strcpy(newData->name,oldData->name);
	newData->tool = oldData->tool;
	newData->thread_id = oldData->thread_id;
}

void removeByName(List** list, char* name){
	List* newList = createList();
	Node* node = (*list)->start;
	int i;
	for(i = 0; i < (*list)->nItems; i++){
		if(strcmp(node->data->name,name) != 0){
			Data* newData = malloc(sizeof(Data));
			copyData(newData,node->data);
			push(newData,newList);
		}
		node = node->next;
	}
	destroy((*list));
	*list = newList;
}

Data* findWorker(List* list, char* name){
	int i;
	Node* node = list->start;
	for(i = 0; i < list->nItems; i++){
		if(strcmp(node->data->name,name) == 0){
			return node->data;
		}
		node = node->next;
	}
	return NULL;
}
