#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "list.h"

static List* list;
static pthread_mutex_t list_mutex;
static pthread_mutex_t print_mutex;
static sem_t semafor;


void* producent(){
	int X;
	char* text;
	Data* data;
	while(1){
		int ret = scanf("%u %ms",&X,&text); 
		if(ret == EOF){
			sem_post(&semafor);
			pthread_exit(NULL);
		}
		if(ret != 2){
			int a = 1;
			sem_post(&semafor);
			pthread_exit((void *)(intptr_t)a);
		}
		data = malloc(sizeof(Data));
		data->num = X;
		data->word = text;
		pthread_mutex_lock(&list_mutex);
		push(data,list);
		pthread_mutex_unlock(&list_mutex);
		sem_post(&semafor);
	}
}


void* konzument(void* N){
	Data * data;
	int num = *(int*)N;
	int i;
	while(1){
		sem_wait(&semafor);
		pthread_mutex_lock(&list_mutex);
		data = pull(list);
		pthread_mutex_unlock(&list_mutex);
		if (data == NULL){
			sem_post(&semafor);
			fprintf(stderr,"Thread %d ended\n",num);
			pthread_exit(NULL);
		}else{
			pthread_mutex_lock(&print_mutex);
			printf("Thread %d:",num);
			for(i = 0; i < data->num;i++){
				printf(" %s",data->word);
			}
			printf("\n");
			pthread_mutex_unlock(&print_mutex);	
			freeData(data);
		}		
	}
}

int main(int argc, char * argv[]){
	int n = 1;
	if(argc > 1){
		sscanf(argv[1], "%d", &n);
	}
	if((n < 1) || (n > sysconf(_SC_NPROCESSORS_ONLN))){
		return 1;
	}
	
	pthread_mutex_init(&list_mutex, NULL);
	pthread_mutex_init(&print_mutex, NULL);
	sem_init(&semafor,0,0);
	int i;
	pthread_t tid_konzument [n];
	pthread_t tid_producent;
	
	list = createList();
	
	pthread_create(&tid_producent, NULL, producent,NULL);
	
	for(i = 0; i < n; i++){
		pthread_create(&tid_konzument[i], NULL, konzument,&i);
	}
	
	void * returnValue = 0;
	pthread_join(tid_producent,&returnValue);
	
	for(i =0; i < n; i++){
		pthread_join(tid_konzument[i],NULL);
	}
	destroy(list);
	pthread_mutex_destroy(&list_mutex);
	pthread_mutex_destroy(&print_mutex);
	sem_destroy(&semafor);
	//printf("ret: %d\n",(int)(intptr_t)returnValue);
	return (int)(intptr_t)returnValue;
}
 