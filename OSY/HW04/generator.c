#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

volatile sig_atomic_t done = 0;
 
void term(int signum)
{
    done = 1;
}
int generate(){
	struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);
	while (!done)
    {
		printf("%d %d\n", rand(), rand());
		fflush(stdout);
		sleep(1);
	}
	
	fprintf(stderr,"GEN TERMINATED\n");
	return 0;
}