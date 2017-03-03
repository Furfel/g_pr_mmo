#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "players.h"

#ifdef _DEBUG_
#include <stdio.h>
#endif

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

Player* CreatePlayer(Thread* attachThread, int index) {
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	SetPlayerName(newPlayer,"Sammy");
	newPlayer->playerThread = attachThread;
	newPlayer->index = index;
	return newPlayer;
}

void UpdatePlayer(Player* player, char* commands){
	if(commands!=0) {
		if(commands[0]==0x11) {
			if(commands[1]==0x2) { //Tylko ustawianie imienia
				#ifdef _DEBUG_
					printf("Changing name to: %s\n",commands+2);
				#endif
				SetPlayerName(player,commands+2);
			} else if(commands[1]==0x3) {
				switch(commands[2]) {
					case 'w': --player->y; break;
					case 's': ++player->y; break;
					case 'a': --player->x; break;
					case 'd': ++player->x; break;
				}
			}
		}
		commands[0]=0x0;
	}
}

void DestroyPlayer(Player* player) {
	free(player);
}

void* PlayerThreadFunction(void* arg) {
	Thread* this = (Thread*) arg;
	PlayerThreadAttachment* attachment = this->attachment;
	int socketf = attachment->socket;
	Player* player = attachment->player;
	int index = attachment->index;
	free(attachment);
	
	char buffer[1024];
	
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	setsockopt(socketf,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(struct timeval));
	
	this->safety_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->safety_mutex,NULL);
	this->last_active = time(NULL);
	this->alive = THREAD_ALIVE;
	
	int r;
	
	pthread_mutex_lock(this->safety_mutex);
	while(this->alive == THREAD_ALIVE) {
		r = read(socketf,buffer,1023);
		if(r < 0) printf("(%s) Error reading from socket.\n",player->name);
		else if(r==0) {
			UpdatePlayer(player,0);
			usleep(1000*25);
		} else {
			this->last_active = time(NULL);
			UpdatePlayer(player,buffer);
			usleep(1000*20);
		}
	}
	#ifdef _DEBUG_
		printf("(%s) Closing socket\n",player->name);
	#endif
	close(socketf);
	pthread_mutex_unlock(this->safety_mutex);
	
	pthread_exit(NULL);
}

void StartPlayerThread(Thread* thread, int index, int socket) {
	#ifdef _DEBUG_
		printf("Creating player [%d] and passing free thread and socket.\n",index);
	#endif
	PlayerThreadAttachment* attachment = (PlayerThreadAttachment*)malloc(sizeof(PlayerThreadAttachment));
	Player* player = CreatePlayer(thread, index);
	attachment->player = player;
	attachment->index = index;
	attachment->socket = socket;
	thread->attachment = attachment;
	if(playerPtrs[index] != 0) {
		#ifdef _DEBUG_
			printf("Freeing player %d\n",index);
		#endif
		free(playerPtrs[index]);
	}
		playerPtrs[index]=player;
	#ifdef _DEBUG_
		printf("Starting player #%d thread.\n",index);
	#endif
	
	pthread_t* tmp = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(tmp, NULL, PlayerThreadFunction, thread);
	thread->self = tmp;
}

void InitPlayerPtrArray(Player** playerptrs, size_t size) {
	for(int i=0;i<size;i++)
		playerptrs[i] = 0;
}
