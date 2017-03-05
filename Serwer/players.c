#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "players.h"
#include "game.h"
#include <stdio.h>

unsigned int PreparePlayerMap(char* out_buffer, unsigned short center_x, unsigned short center_y, Tile** tiles) {
	if(tiles == 0 || out_buffer == 0) return 0;
	short start_x = center_x-PLAYER_VIEW_RADIUS;
	short start_y = center_y-PLAYER_VIEW_RADIUS;
	short end_x = center_x+PLAYER_VIEW_RADIUS;
	short end_y = center_y+PLAYER_VIEW_RADIUS;
	short x,y,z;
	unsigned int index = 0;
	for(y=start_y;y<=end_y;++y)
		for(x=start_x;x<=end_x;++x) {
			if(x<0||y<0||x>=MapWidth||y>=MapHeight) {
				out_buffer[index]=0; ++index;
			} else {
				out_buffer[index]=tiles[y][x].count; ++index;
				for(z=0;z<tiles[y][x].count;++z) {
					out_buffer[index]=tiles[y][x].items[z]; ++index;
				}
			}
	}
	
	#ifdef _DEBUG_
		printf("Prepared view of %ud bytes\n",index);
	#endif
	
	return index;
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

Player* CreatePlayer(Thread* attachThread, int index) {
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	SetPlayerName(newPlayer,"Sammy");
	newPlayer->playerThread = attachThread;
	newPlayer->index = index;
	return newPlayer;
}

void UpdatePlayer(Player* player, char* commands){
	if(commands!=0) {
		//Jezeli wiadomosc to komenda
		if(commands[0]==0x11) {
			if(commands[1]==0x2) { //Tylko ustawianie imienia
				#ifdef _DEBUG_
					printf("Changing name to: %s\n",commands+2);
				#endif
				SetPlayerName(player,commands+2);
			} else if(commands[1]==0x3) { //Ruszanie
				switch(commands[2]) {
					case 'w': --player->y; break;
					case 's': ++player->y; break;
					case 'a': --player->x; break;
					case 'd': ++player->x; break;
				}
			}
		}
		//Wyzeruj: nie powtarzaj komendy
		commands[0]=0x0;
	}
}

void DestroyPlayer(Player* player) {
	free(player);
}

void* PlayerThreadFunction(void* arg) {
	Thread* this = (Thread*) arg;
	
	//Zrzucamy zalacznik do zmiennych lokalnych i wywalamy go
	PlayerThreadAttachment* attachment = this->attachment;
	int socketf = attachment->socket;
	Player* player = attachment->player;
	int index = attachment->index;
	free(attachment);
	
	char buffer[1024];
	
	//Timeout socketu na 5 sekund
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	setsockopt(socketf,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(struct timeval));
	
	this->safety_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->safety_mutex,NULL);
	this->last_active = time(NULL);
	this->alive = THREAD_ALIVE;
	
	int r;
	unsigned int w;
	
	//Zablokuj watek przed anulowaniem, poniewaz mamy otwarty socket
	pthread_mutex_lock(this->safety_mutex);
	
	while(this->alive == THREAD_ALIVE) {
		//Odczytaj co wysyla klient
		r = read(socketf,buffer,1023);
		
		if(r < 0) printf("(%s) Error reading from socket.\n",player->name);
		else if(r==0) {
			UpdatePlayer(player,0);
			usleep(1000*25);
		} else {
			this->last_active = time(NULL);
			UpdatePlayer(player,buffer);
			w=PreparePlayerMap(buffer,player->x,player->y,map);
			write(socketf, buffer, w);
			usleep(1000*25);
		}
	}
	
	#ifdef _DEBUG_
		printf("(%s) Closing socket\n",player->name);
	#endif
	close(socketf);
	pthread_mutex_unlock(this->safety_mutex);
	//Odblokowany watek mozna anulowac
	
	pthread_exit(NULL);
}

void StartPlayerThread(Thread* thread, int index, int socket, Player** playerptrs) {
	#ifdef _DEBUG_
		printf("Creating player [%d] and passing free thread and socket.\n",index);
	#endif
	
	//Tworzymy gracza w pamieci
	Player* player = CreatePlayer(thread, index);
	
	//Budujemy zalacznik dla watku gracza
	PlayerThreadAttachment* attachment = (PlayerThreadAttachment*)malloc(sizeof(PlayerThreadAttachment));
	attachment->player = player;
	attachment->index = index;
	attachment->socket = socket;
	
	thread->attachment = attachment;
	
	//Dodajemy wskaznik gracza do pomocniczej tablicy
	if(playerptrs[index] != 0) {
		#ifdef _DEBUG_
			printf("Freeing player %d\n",index);
		#endif
		free(playerptrs[index]);
	}
		playerptrs[index]=player;
	#ifdef _DEBUG_
		printf("Starting player #%d thread.\n",index);
	#endif
	
	//Startujemy pthread z parametrem do funkcji, ktorym jest watek
	pthread_t* tmp = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(tmp, NULL, PlayerThreadFunction, thread);
	thread->self = tmp;
}

void InitPlayerPtrArray(Player** playerptrs, size_t size) {
	for(int i=0;i<size;i++)
		playerptrs[i] = 0;
}
