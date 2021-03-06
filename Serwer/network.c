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
	
	Thread* this = (Thread*)arg; //Od teraz this to bedzie ten watek
	
	//Kopiujemy zmienne przeslane w zalaczniku
	Thread* threads = ((ListenThreadAttachment*)this->attachment)->threads;
	size_t size = ((ListenThreadAttachment*)this->attachment)->size;
	int socket = ((ListenThreadAttachment*)this->attachment)->socket;
	Player** playerptrs = ((ListenThreadAttachment*)this->attachment)->playerptrs;
	free(this->attachment); //Usuwamy zalacznik z pamieci bo niepotrzebny
	
	#ifdef _DEBUG_
		printf(CLR_B"Listener:"CLR_N" Max number of threads = %lu\n",size);
	#endif
	
	while(this->alive == THREAD_ALIVE) {
		struct sockaddr_in client_addr; //Miejsce na adres klienta
		int clilen = sizeof(client_addr); //rozmiar powyzszego
		int newsocket = accept(socket,(struct sockaddr*) &client_addr, &clilen); //Czekamy na nowe polaczenie
		if(newsocket < 0) { //Nie udalo sie polaczyc nikomu
			char* address = inet_ntoa(client_addr.sin_addr);
			printf(CLR_R"Listener:"CLR_N" error accepting connection [%s]\n",address);
		} else { //Udalo sie polaczyc komus
			char* address = inet_ntoa(client_addr.sin_addr);
			printf(CLR_G"Successfully accepted connection from [%s]"CLR_N"\n",address);
			
			//Szukamy wolnego miejsca dla gracza
			Thread* freet = 0;
			int i;
			for(i=0;i<size && freet == 0;i++)
				if(threads[i].attachment == 0) freet = &threads[i];
			
			if(freet == 0) { //Znaczy ze nie ma miejsca xd
				#ifdef _DEBUG_
					warn(address,"No more room for player");
				#endif
				write(newsocket,WELCOME_NOSPACE_MESSAGE,WELCOME_NOSPACE_LENGTH); //Wysylamy ze nie ma miejsca
				close(newsocket); //Zamykamy bo nie ma miejsca
			} else { //Jest miesjce!
				#ifdef _DEBUG_
					success(address,"Welcoming player and passing to new thread.");
				#endif
				write(newsocket, WELCOME_MESSAGE, WELCOME_MESSAGE_LENGTH); //Witamy na serwerze, jest miesjce!
				StartPlayerThread(freet, i, newsocket, playerptrs); //Rozpoczynamy nowy watek dla gracza
			}
			
		}
	}
	
	#ifdef _DEBUG_
		printinfo("Listener","Exiting");
	#endif
	
	pthread_exit(NULL);
	
}

void StartListening(Thread* thread, Thread* threads, size_t size, int socket, Player** playerptrs) {
	//Budujemy zalacznik przekazany do watku
	ListenThreadAttachment* attachment = (ListenThreadAttachment*)malloc(sizeof(ListenThreadAttachment));
	attachment->socket = socket;
	attachment->size = size;
	attachment->threads = threads;
	attachment->playerptrs = playerptrs;
	
	//Ustawiamy zalacznik w watku i parametry watku
	thread->attachment = attachment;
	thread->alive = THREAD_ALIVE;
	thread->safety_mutex = 0;
	pthread_t* tmp = (pthread_t*)malloc(sizeof(pthread_t));
	
	//Rozpoczynamy watek i przekazujemy watek do funkcji watku
	pthread_create(tmp, NULL, ListenerFunction, thread);
	thread->self = tmp;
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
