#include <stdlib.h>
#include <unistd.h>
#include "players.h"

#ifdef _DEBUG_
#include <stdio.h>
#endif
Player* CreatePlayer(Thread* attachThread) {
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	SetPlayerName(newPlayer,"Sammy");
	return newPlayer;
}

void SetPlayerName(Player* player, char* name) {
	int i=0;
	while(i<PLAYER_NAME_LENGTH && name[i]!='\0') {
		player->name[i] = name[i];
		++i;
	}
	if(i<PLAYER_NAME_LENGTH)
		player->name[i]='\0';
	else player->name[i-1]='\0';
}

void UpdatePlayer(Player* player){
	
}

void DestroyPlayer(Player* player) {
	free(player);
}

void* UpdatePlayersThreadFunction(void* arg) {
	#ifdef _DEBUG_
		printf("UpdatePlayers: starting...\n");
	#endif
	UpdatePlayersThreadArgument* this = (UpdatePlayersThreadArgument*) arg;
	int i;
	while(this->self->alive == THREAD_ALIVE) {
		for(i=0;i < this->size;++i){
			Player* player = &(this->players[i]);
			if(player!=0) {
				UpdatePlayer(player);
				#ifdef _DEBUG_
					printf("UpdatePlayers: updating %s",player->name);
				#endif
			}
		}
		usleep(this->sleep);
	}
	#ifdef _DEBUG_
		printf("UpdatePlayers: exiting\n");
	#endif
	free(this);
	pthread_exit(NULL);
}

void StartPlayerThread(Thread* thread, int socket) {
	//Player* player = CreatePlayer(thread);
	char b;
	thread->attachment = &b;
	#ifdef _DEBUG_
		printf("Creating player and passing free thread and socket.\n");
	#endif
}
