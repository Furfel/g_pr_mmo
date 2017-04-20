#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "network.h"
#include "utils.h"
#include "watchdog.h"
#include "players.h"
#include "game.h"

#define WATCHDOG_SLEEP 1000*1000 //Ile ma spac watchdog
#define PORT 9528

int main() {
	
	char addr[20]; //Teoretycznie mial tu byc zwrocony nasz adres
	
	int socket = OpenSocket(PORT,addr); //Otworz socket - network.h
	if(socket == ERROR_SOCKET_OPEN) {
		printerr(NULL,"Error opening socket");
		return 1;
	} else if(socket == ERROR_SOCKET_BIND) {
		printerr(NULL,"Error binding socket");
		return 1;
	}
	
	printf(CLR_G "Successfully created socket on %s:%d\n" CLR_N,addr,PORT);

	ReadMap(&map,"mapa");
	
	Thread gracze[MAX_THREADS]; //Watki graczy
	Thread watchdog; //Watek wartownika
	Thread listenThread; //Watek sluchania nowych klientow
	Thread gameThread; //Watek trwania gry
	//-> players.h Player* playerPtrs[MAX_THREADS]; //Wskazniki dla graczy (pomocniczo, moze sie przydac aby nie bylo wyciekow)
	
	InitThreadArray(gracze, MAX_THREADS); //Zerowanie watkow graczy - utils.h
	InitPlayerPtrArray(playerPtrs, MAX_THREADS); //Zerowanie wskaznikow graczy - players.h
	InitBullets();
	
	#ifdef _DEBUG_
		printinfo(NULL,"StartWatchdog");
	#endif
	int clients;
	StartWatchdog(&watchdog, gracze, MAX_THREADS, WATCHDOG_SLEEP, &clients); //Uruchamiam wartownika na tablicy gracze - watchdog.h
	
	#ifdef _DEBUG_
		printinfo(NULL,"StartListener");
	#endif
	StartListening(&listenThread, gracze, MAX_THREADS, socket, playerPtrs); //Uruchamiam nasluchiwanie nowych polaczen - network.h
	
	StartGame(&gameThread);
	
	char key = 0;
	while(key!='q' && key!='Q') {
		if(key=='\n' || key=='\r' || key==' ') {key=(char)getchar(); continue;}
		printf(CLR_M "Console:"CLR_N" Q-Quit, C-connected clients, P# - position of player\n");
		printf(CLR_M "Console:"CLR_N" Dx y w h-dump map rect\n");
		key = (char)getchar();
		if(key=='C' || key=='c')
			printf("Clients: %d\n",clients);
		if(key=='P' || key=='p') {
			key = (char)getchar();
			int p = key-'0';
			if(playerPtrs[p]==0)
				printerr(NULL,"No player in this slot");
			else
				printf("Player #%d %s [%d,%d]\n",p,playerPtrs[p]->name,playerPtrs[p]->x,playerPtrs[p]->y);
		} else if(key=='d' || key=='D') {
			int dump_x,dump_y,dump_w,dump_h;
			scanf("%d%d%d%d",&dump_x,&dump_y,&dump_w,&dump_h);
			DumpMap(dump_x,dump_y,dump_w,dump_h);
		}
	}
	
	CancelThread(&watchdog);
	CancelThread(&listenThread);
	CancelThread(&gameThread);
	
	close(socket);
	
	pthread_exit(NULL);
	return 0;
}
