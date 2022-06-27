/*
Author: Martin Sustek
Date: 31.10.2017
*/

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "generator.h"

static void errorControl(int num){
	if(num < 0){
		exit(2);
	}
}

int main(int argc, char* argv[]){
	int retValGen;
	int retValNsd;
	int pipefd[2];	
	int pipeSucc = 0;
	
	pipeSucc = pipe(pipefd);
	if(pipeSucc != 0){
		close(pipefd[0]);
		close(pipefd[1]);
		return 2;
	}
	pid_t pid1 = fork();
	if(pid1 == -1){
		return 2;
	}
	if(pid1 == 0){
		errorControl(close(pipefd[0]));
		errorControl(dup2(pipefd[1],1));
		errorControl(close(pipefd[1]));
		return generate();
	}
	pid_t pid2 = fork();
	if(pid2 == -1){
		return 2;
	}
	if (pid2 == 0){		
		errorControl(close(pipefd[1]));
		errorControl(dup2(pipefd[0],0));
		errorControl(close(pipefd[0]));
		return execl("nsd","nsd",(char*)NULL);
		
	}
	errorControl(close(pipefd[0]));
	errorControl(close(pipefd[1]));
	sleep(5);
	kill(pid1,SIGTERM);
	//Waiting for other processes
	waitpid(pid1,&retValGen,0);
	waitpid(pid2,&retValNsd,0);
	//printf("%d %d\n",WEXITSTATUS(retValGen), WEXITSTATUS(retValNsd));
	if((WEXITSTATUS(retValNsd) != 0)|| (WEXITSTATUS(retValGen) != 0)){
		printf("ERROR\n");
		return 1;
	}else{
		printf("OK\n");
		return 0;
	}
}