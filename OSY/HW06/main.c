#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "list.h"
#include <stdbool.h>

typedef struct{
	int fab;
	int pos;
}fabN;


static char* toolStrings[7] = {"nuzky","vrtacka","ohybacka","svarecka","lakovna","sroubovak","freza"};
static char* productStrings[3] = {"A","B","C"};

static int timeTools [7] = {100,200,150,300,400,250,500};
static int tools[7] = {0,0,0,0,0,0,0};
static int fabs [3][7];
static int currWork = 0;

static fabN* toolUsage[7];
static int lengths[7] = {2,6,1,1,3,2,3};
static fabN toolN0[2] = {{1,1},{0,0}};
static fabN toolN1[6] = {{0,4},{1,3},{2,3},{0,1},{2,1},{1,0}};
static fabN toolN2[1] = {{0,2}};
static fabN toolN3[1] = {{0,3}};
static fabN toolN4[3] = {{0,5},{2,5},{1,4}};
static fabN toolN5[2] = {{1,5},{2,2}};
static fabN toolN6[3] = {{2,4},{1,2},{2,0}};

static pthread_mutex_t factory_mutex;
static pthread_cond_t factory_cond;

bool canWork(int toolNum,int myData [2]){
	bool ret = false;
	int i;
	if(tools[toolNum] == 0){
		return false;
	}
	for(i = 0; i <lengths[toolNum];i++){
		if(fabs[toolUsage[toolNum][i].fab][toolUsage[toolNum][i].pos] > 0){
			myData[0] = toolUsage[toolNum][i].fab;
			myData[1] = toolUsage[toolNum][i].pos;
			ret = true;
			break;
		}		
	}
	return ret;
}

void* worker(void* arg){
	Data* data = (Data*)arg;
	int tool = data->tool; 	
	pthread_mutex_lock(&factory_mutex);
	while(1){
		int myData [2] = {-1,-1};
		while(!(canWork(tool,myData)||data->quit)){
			pthread_cond_wait(&factory_cond,&factory_mutex);
		}
		if(data->quit){
			break;
		}
		tools[tool]--;
		fabs[myData[0]][myData[1]]--;
		currWork++;
		printf("%s %s %s %d\n",data->name,toolStrings[tool],productStrings[myData[0]],myData[1]+1);

		pthread_mutex_unlock(&factory_mutex);

		usleep(timeTools[tool] * 1000000);

		pthread_mutex_lock(&factory_mutex);

		currWork--;
		if(myData[1]+1 == 6){
			printf("done %s\n",productStrings[myData[0]]);
		}
		fabs[myData[0]][myData[1]+1]++;
		tools[tool]++;
		pthread_cond_broadcast(&factory_cond);
	}
	fprintf(stderr,"Worker %s ENDED\n",data->name);
	pthread_mutex_unlock(&factory_mutex);	
	return 0;
}

int toolNum (char* tool){
	if(strcmp(tool,"nuzky") == 0){
		return 0;
	}else if(strcmp(tool,"vrtacka") == 0){
		return 1;
	}else if(strcmp(tool,"ohybacka") == 0){
		return 2;
	}else if(strcmp(tool,"svarecka") == 0){
		return 3;
	}else if(strcmp(tool,"lakovna") == 0){
		return 4;
	}else if(strcmp(tool,"sroubovak") == 0){
		return 5;
	}else if (strcmp(tool,"freza") == 0){
		return 6;
	}
	return -1;
}

bool noWork(List* workers){
	int myData [2] = {-1,-1};
	int i;
	if(currWork != 0){
		return false;
	}
	Node* node = workers->start;
	for(i = 0; i < workers->nItems; i++){
		if(node->data->active){
			if(canWork(node->data->tool,myData)){
				return false;
			}
		}
		node = node->next;
	}
	return true;
}

int main (int argc, char* argv[]){

	pthread_mutex_init(&factory_mutex, NULL);
	pthread_cond_init(&factory_cond,NULL);

	toolUsage[0]=toolN0;
	toolUsage[1]=toolN1;
	toolUsage[2]=toolN2;
	toolUsage[3]=toolN3;
	toolUsage[4]=toolN4;
	toolUsage[5]=toolN5;
	toolUsage[6]=toolN6;

	List* workersID = createList();
	char* command;
	char* name;
	char* tool;
	char* secPart;
	int ret,i,j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 7; j++){
			fabs[i][j] = 0;
		}
	}
	while(1)
	{
		ret = scanf("%ms",&command);
		if(ret == EOF)
		{
			free(command);
			break;
		}
		if(strcmp(command,"start") == 0)
		{
			ret = scanf("%ms",&name);
			if(ret > 0)
			{
				ret = scanf("%ms",&tool);
				if(ret > 0)
				{	
					fprintf(stderr,"Start %s\n",name);
					Data* data = malloc(sizeof(Data));
					data->name = name;
					data->tool = toolNum(tool);
					data->quit = false;
					data->active = true;
					pthread_create(&data->thread_id, NULL, worker,data);
					push(data,workersID);
				}
				free(tool);
			}
		}else{
			ret = scanf("%ms",&secPart);
			if(strcmp(command,"add")==0){
				fprintf(stderr,"Add %s\n",secPart);
				pthread_mutex_lock(&factory_mutex);
				tools[toolNum(secPart)]++;
				pthread_cond_broadcast(&factory_cond);
				pthread_mutex_unlock(&factory_mutex);
			}else if(strcmp(command,"make")==0){
				fprintf(stderr,"Make com: %s\n",secPart);
				pthread_mutex_lock(&factory_mutex);
				fabs[(*secPart - 17) -'0'][0]++;
				pthread_cond_broadcast(&factory_cond);
				pthread_mutex_unlock(&factory_mutex);
			}else if(strcmp(command,"end")==0){
				fprintf(stderr,"Worker removed: %s\n",secPart);

				Data* worker = findWorker(workersID,secPart);
				if(worker == NULL){
					break;
				}

                pthread_mutex_lock(&factory_mutex);
                worker->quit = true;
				worker->active = false;
                pthread_cond_broadcast(&factory_cond);
                pthread_mutex_unlock(&factory_mutex);

               // pthread_join(worker->thread_id, NULL);				
			}else if(strcmp(command,"remove")==0){
				fprintf(stderr,"Tool removed: %s\n",secPart);
				pthread_mutex_lock(&factory_mutex);
				tools[toolNum(secPart)]--;
				pthread_cond_broadcast(&factory_cond);
                pthread_mutex_unlock(&factory_mutex);
			}
			free(secPart);
		}
		free(command);
	}

	pthread_mutex_lock(&factory_mutex);
    while (!noWork(workersID)){
        pthread_cond_wait(&factory_cond, &factory_mutex);
	}

	int k;
	Node* endWorker = workersID->start;
	for(k = 0; k < workersID->nItems; k++){
		endWorker->data->quit = true;
		endWorker->data->active = false;
		endWorker = endWorker->next;
	}
    pthread_cond_broadcast(&factory_cond);   
    pthread_mutex_unlock(&factory_mutex);
	
	endWorker = workersID->start;
	for(k = 0; k < workersID->nItems; k++){
		pthread_join(endWorker->data->thread_id,NULL);
		endWorker = endWorker->next;
	}
	destroy(workersID);
	pthread_mutex_destroy(&factory_mutex);
    pthread_cond_destroy(&factory_cond);
	return 0;
}
