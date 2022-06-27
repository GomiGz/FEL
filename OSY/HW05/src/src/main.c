#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "command.h"

#define TYPE pthread_t, pthread
#include "array.h"
#undef TYPE

#define TYPE struct command, command
#include "linkedList.h"
#undef TYPE

static int runProducerInner();
static void * runProducer(void * arg);
static void * runConsumer(void * arg);

static pthread_mutex_t mutex_list;
static pthread_mutex_t mutex_stdout;
static sem_t sem_used;
// static sem_t sem_free;
static linkedList_command commands;

int runProducerInner()
{
    while (true)
    {
        struct command command;

        int result = scanf("%u %ms", &command.x, &command.word);
        if (result == EOF)
            break;

        if (result != 2)
            return 1;

        //sem_wait(&sem_free);
        pthread_mutex_lock(&mutex_list);
        linkedList_command_addLast(&commands, command);
        pthread_mutex_unlock(&mutex_list);
        sem_post(&sem_used);
    }

    return 0;
}

void * runProducer(void * arg)
{
    int const returnValue = runProducerInner();

   // sem_wait(&sem_free);
    sem_post(&sem_used); // post with an empty list to report end of input

    return (void *)(intptr_t)returnValue;
}

void * runConsumer(void * arg)
{
    int const consumerNumber = (int)(intptr_t)arg;
    bool hasCommand;

    do
    {
        sem_wait(&sem_used);
        pthread_mutex_lock(&mutex_list);

        struct command command;
        hasCommand = linkedList_command_tryRemoveFirst(&commands, &command);
            
        pthread_mutex_unlock(&mutex_list);
        //sem_post(hasCommand ? &sem_free : &sem_used);        
        
        if (hasCommand)
        {
            pthread_mutex_lock(&mutex_stdout);            
            printf("Thread %d:", consumerNumber);
            for (int i = 0; i < command.x; ++i)
                printf(" %s", command.word);
            printf("\n");
            pthread_mutex_unlock(&mutex_stdout);

            free(command.word);
        }
        else
            sem_post(&sem_used);
    }
    while (hasCommand);

    return 0;
}

int main(int argc, char ** argv)
{
    int n = 1;
   
    if (argc > 1)
        sscanf(argv[1], "%d", &n);

    if (n < 1 || n > sysconf(_SC_NPROCESSORS_ONLN))
        return 1;

    commands = linkedList_command_empty();

    array_pthread consumers;
    array_pthread_new(&consumers, n);

    pthread_mutex_init(&mutex_stdout, NULL);
    pthread_mutex_init(&mutex_list, NULL);
    sem_init(&sem_used, 0, 0);
   // sem_init(&sem_free, 0, n);

    pthread_t producer;
    pthread_create(&producer, NULL, runProducer, NULL);

    for (int i = 0; i < n; ++i)
        pthread_create(&consumers.data[i], NULL, runConsumer, (void *)(intptr_t)(i + 1));
    
    void * returnValue = 0;
    pthread_join(producer, &returnValue);

    for (int i = 0; i < n; ++i)
        pthread_join(consumers.data[i], NULL);

   // sem_destroy(&sem_free);
    sem_destroy(&sem_used);    
    pthread_mutex_destroy(&mutex_list);
    pthread_mutex_destroy(&mutex_stdout);
    
    array_pthread_delete(&consumers);

    return (int)(intptr_t)returnValue;
}
