#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>

//Broadcast function
void* startNetwork(void* listV){	
	
	UnitList* list =(UnitList*) listV;
	int sockfd;
	
	//create socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
        exit(1);
    }    
	
	//initialize broadcast adress
	struct sockaddr_in braddr;
	memset(&braddr, 0, sizeof(braddr));
	braddr.sin_family = AF_INET;
	braddr.sin_port = htons(55555);
	braddr.sin_addr.s_addr = INADDR_BROADCAST;
	
	if (bind(sockfd, (struct sockaddr *)&braddr, sizeof(braddr)) == -1) {
		perror("bind");
		exit(1);
	}
	
	int broadcast = 1;
	struct timespec loop_delay = {.tv_sec = 1, .tv_nsec = 0};
	//set broadcast options
	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof(broadcast)) == -1) {
		perror("setsockopt (SO_BROADCAST)");
		exit(1);
	}
	
	char loopch=0;
	//set loopbacj options
	if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP,(char *)&loopch, sizeof(loopch)) == -1) {
		perror("setting IP_MULTICAST_LOOP:");
		close(sockfd);
		exit(1);
	}
	
	unsigned char buffer[548];
	memset(buffer,0,548); 
	char alc []= "ALC1";	
	uint32_t version = 0x00010000;
	uint32_t status = 0;
	
	
	for (int i = 0; i < 4; i++) {//save ALC1
		buffer[i] = *((unsigned char *)&alc + i);
	}		
	for (int i = 0; i < 4; i++) {//save version
		buffer[4+i] = *((unsigned char *)&version + (3-i));
	}
	for (int i = 0; i < 4; i++) {//save status
		buffer[8+i] = *((unsigned char *)&status + (3-i));
	}
	while(1){
		for (int i = 0; i < 4; i++) {//save ceiling colour
			buffer[12+i] = *((unsigned char *)&list->units[0]->ceiling + (3-i));
		}
		for (int i = 0; i < 4; i++) {//save walls colour
			buffer[16+i] = *((unsigned char *)&list->units[0]->wall + (3-i));
		}
		for (int i = 0; i < 16; i++) {//save unit name
			buffer[20+i] = list->units[0]->name[i];
		}
		for(int i = 0; i < 256; i++){//save unit icon
			buffer[36+(i*2)] = *((unsigned char *)&list->units[0]->icon[i/16][i%16] + 1);
			buffer[36+(i*2) + 1] = *((unsigned char *)&list->units[0]->icon[i/16][i%16]);
		}			
		clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);//delay
		//broadcast
		if (sendto(sockfd, buffer, 548, 0, (struct sockaddr *)&braddr, sizeof(braddr)) != 548){
            fprintf(stderr, "sendto error");
            exit(1);
        }	
	}			
	return NULL;
}

