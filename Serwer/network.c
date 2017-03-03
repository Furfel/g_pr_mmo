#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "network.h"
#include "utils.h"
#include "players.h"

void* ListenerFunction(void* arg) {
	#ifdef _DEBUG_
		printf("Listener: starting...\n");
	#endif
	
	Thread* this = (Thread*)arg;
	Thread* threads = ((ListenThreadAttachment*)this->attachment)->threads;
	size_t size = ((ListenThreadAttachment*)this->attachment)->size;
	int socket = ((ListenThreadAttachment*)this->attachment)->socket;
	free(this->attachment);
	
	#ifdef _DEBUG_
		printf("Listener: Max number of threads = %lu\n",size);
	#endif
	
	while(this->alive == THREAD_ALIVE) {
		struct sockaddr_in client_addr;
		int clilen = sizeof(client_addr);
		int newsocket = accept(socket,(struct sockaddr*) &client_addr, &clilen);
		if(newsocket < 0) {
			char* address = inet_ntoa(client_addr.sin_addr);
			printf("Listener: error accepting connection [%s]\n",address);
		} else {
			char* address = inet_ntoa(client_addr.sin_addr);
			printf("Successfully accepted connection from [%s]\n",address);
			Thread* freet = 0;
			for(int i=0;i<size && freet == 0;i++)
				if(threads[i].attachment == 0) freet = &threads[i];
			if(freet == 0) { //Znaczy ze nie ma miejsca xd
				#ifdef _DEBUG_
					printf("No more room for player!\n");
				#endif
				write(newsocket,WELCOME_NOSPACE_MESSAGE,WELCOME_NOSPACE_LENGTH);
				close(newsocket); //Zamykamy bo nie ma miejsca
			} else {
				#ifdef _DEBUG_
					printf("Welcoming player and passing to new thread.\n");
				#endif
				write(newsocket,WELCOME_MESSAGE,WELCOME_MESSAGE_LENGTH); //Witamy na serwerze, jest miesjce!
				StartPlayerThread(freet,newsocket);
			}
			
		}
	}
	
	#ifdef _DEBUG_
		printf("Listener: exiting\n");
	#endif
	
	pthread_exit(NULL);
	
}

void StartListening(Thread* thread, Thread* threads, size_t size, int socket) {
	ListenThreadAttachment* attachment = (ListenThreadAttachment*)malloc(sizeof(ListenThreadAttachment));
	attachment->socket = socket;
	attachment->size = size;
	attachment->threads = threads;
	
	thread->attachment = attachment;
	thread->alive = THREAD_ALIVE;
	pthread_t tmp;
	pthread_create(&tmp, NULL, ListenerFunction, thread);
	thread->self = &tmp;
}

int OpenSocket(int port, char* addr) {
	struct sockaddr_in server_address;
	int socketf;
	socketf = socket(AF_INET,SOCK_STREAM,0);
	if(socketf<0)
		return ERROR_SOCKET_OPEN;
	bzero((char*)&server_address,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);
	int r;
	r = bind(socketf,(struct sockaddr*) &server_address,sizeof(server_address));
	if(r<0)
		return ERROR_SOCKET_BIND;
	listen(socketf,4);
	if(addr!=0) {
		char* tmp_addr = inet_ntoa(server_address.sin_addr);
		strcpy(addr,tmp_addr);
	}
	return socketf;
}
