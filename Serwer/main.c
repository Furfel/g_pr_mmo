#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "network.h"
#include "utils.h"
#include "watchdog.h"

#define MAX_THREADS 5 //Maksymalna ilosc podlaczonych graczy
#define WATCHDOG_SLEEP 1000*1000 //Ile ma spac watchdog
#define PORT 9528

pthread_t threads[MAX_THREADS];
pthread_t waitThread;

int main() {
	
	char addr[20];
	
	int socket = OpenSocket(PORT,addr);
	if(socket == ERROR_SOCKET_OPEN) {
		printf("Error opening socket\n");
		return 1;
	} else if(socket == ERROR_SOCKET_BIND) {
		printf("Error binding socket\n");
		return 1;
	}
	
	printf("Successfully created socket on %s:%d\n",addr,PORT);
	
	Thread gracze[MAX_THREADS];
	Thread watchdog;
	Thread listenThread;
	
	InitThreadArray(gracze,MAX_THREADS);
	
	#ifdef _DEBUG_
		printf("StartWatchdog\n");
	#endif
	int clients;
	StartWatchdog(&watchdog,gracze,MAX_THREADS,WATCHDOG_SLEEP,&clients);
	
	#ifdef _DEBUG_
		printf("StartListener\n");
	#endif
	StartListening(&listenThread,gracze,MAX_THREADS,socket);
	
	char key = 0;
	while(key!='q' && key!='Q') {
		if(key=='\n' || key=='\r') {key=(char)getchar(); continue;}
		printf("Console: Q-Quit, C-connected clients, \n");
		key = (char)getchar();
		if(key=='C' || key=='c')
			printf("Clients: %d\n",clients);
	}
	
	KillThread(&watchdog);
	KillThread(&listenThread);
	CancelThread(&listenThread);
	
	close(socket);
	
	pthread_exit(NULL);
	return 0;
}
