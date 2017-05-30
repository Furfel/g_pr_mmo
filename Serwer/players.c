#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "players.h"
#include "game.h"
#include <stdio.h>
#include <sys/time.h>

unsigned int PreparePlayerMap(char* out_buffer, unsigned short center_x, unsigned short center_y, Tile** tiles, Player* player) {
	if(tiles == 0 || out_buffer == 0) return 0;
	short start_x = center_x-PLAYER_VIEW_RADIUS;
	short start_y = center_y-PLAYER_VIEW_RADIUS;
	short end_x = center_x+PLAYER_VIEW_RADIUS;
	short end_y = center_y+PLAYER_VIEW_RADIUS;
	short x,y,z;
	unsigned int index = 0;
	out_buffer[index]=player->xMilis; ++index;
	out_buffer[index]=player->yMilis; ++index;
	out_buffer[index]=player->direction; ++index;
	#ifdef _DONTSENDMAP_
		out_buffer[index]=(unsigned char)player->x; ++index;
		out_buffer[index]=(unsigned char) player->y; ++index;	
	#endif
	#ifndef _DONTSENDMAP_
	for(y=start_y;y<=end_y;++y)
		for(x=start_x;x<=end_x;++x) {
			if(x<0||y<0||x>=MapWidth||y>=MapHeight) {
				out_buffer[index]=0; ++index;
			} else {
				out_buffer[index]=map[y][x].count; ++index;
				for(z=0;z<map[y][x].count;++z) {
					out_buffer[index]=map[y][x].items[z]; ++index;
				}
			}
	}
	#endif
	for(z=0;z<MAX_THREADS;++z) {
		if(playerPtrs[z]!=NULL && playerPtrs[z]!=0 && playerPtrs[z]!=player) {
			if(playerPtrs[z]->playerThread->alive==THREAD_ALIVE) {
				start_x = playerPtrs[z]->x-center_x;
				start_y = playerPtrs[z]->y-center_y;
				//if(start_x<-10 || start_x>10 || start_y<-10 || start_y>10) continue; //Nie sa obok
				out_buffer[index]=DATA_HEADER_PLAYER; ++index;
				out_buffer[index]=(unsigned char)z; ++index;
				out_buffer[index]=(signed char)start_x; ++index;
				out_buffer[index]=(signed char)start_y; ++index;
				out_buffer[index]=playerPtrs[z]->direction; ++index;
				out_buffer[index]=playerPtrs[z]->xMilis; ++index;
				out_buffer[index]=playerPtrs[z]->yMilis; ++index;
			}
		}
	}
	
	for(z=0;z<MAX_BULLETS;++z) {
		if(1) {
			float bullet_x = bullets[z].x-(float)center_x;
			float bullet_y = bullets[z].y-(float)center_y;
			//if(bullet_x<-10.0f || bullet_x>10.0f || bullet_y<-10.0f || bullet_y>10.0f) continue;
			out_buffer[index]=DATA_HEADER_BULLET; ++index;
			out_buffer[index]=(unsigned char)z; ++index;
			out_buffer[index]=(unsigned char)bullets[z].type; ++index;
			char* angle_bytes = (char*)&bullets[z].angle;
			char* x_bytes = (char*)&bullet_x;
			char* y_bytes = (char*)&bullet_y;
			for(x=0;x<sizeof(float);++x) {
				out_buffer[index]=angle_bytes[x]; ++index;
			}
			for(x=0;x<sizeof(float);++x) {
				out_buffer[index]=x_bytes[x]; ++index;
			}
			for(x=0;x<sizeof(float);++x) {
				out_buffer[index]=y_bytes[x]; ++index;
			}
		}
	}
		
	
	out_buffer[index]=SECTION_END; ++index;
	
	#ifdef _HARDDEBUG_
		printf(CLR_G"Prepared view of %u bytes\n"CLR_N,index);
		DumpMap(start_x,start_y,7,7);
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
	newPlayer->x = PLAYER_START_X;
	newPlayer->y = PLAYER_START_Y;
	newPlayer->xMilis = 0;
	newPlayer->yMilis = 0;
	newPlayer->direction = DIRECTION_DOWN;
	return newPlayer;
}

char IsPlayerBlocking(short x, short y, Player* player) {
	unsigned char z;
	for(z=0;z<MAX_THREADS;++z) {
		if(playerPtrs[z]!=NULL && playerPtrs[z]!=0 && playerPtrs[z]!=player) {
			if(playerPtrs[z]->playerThread->alive==THREAD_ALIVE) {
				if(playerPtrs[z]->x==x && playerPtrs[z]->y==y) return 1;
			}
		}
	}
	return 0;
}

void MovePlayer(Player* player, char direction) {
	short yDest, xDest;
	switch(direction) {
		case 'w': {
			yDest = player->y-1;
			xDest = player->x;
			if(IsBlockingXY(xDest, yDest)) return;
			LockChunk(xDest, yDest);
				if(!IsPlayerBlocking(xDest,yDest,player)) {
					--player->y;
					player->yMilis=MILIS_DELTA;
				}
			UnlockChunk(xDest,yDest);
			player->direction=DIRECTION_UP;
			} break;
		case 's': {
			yDest = player->y+1;
			xDest = player->x;
			if(IsBlockingXY(player->x, player->y+1)) return;
			LockChunk(xDest, yDest);
				if(!IsPlayerBlocking(xDest,yDest,player)) {
					++player->y;
					player->yMilis=-MILIS_DELTA;
				}
			UnlockChunk(xDest, yDest);
			player->direction=DIRECTION_DOWN;
			} break;
		case 'a': {
			yDest = player->y;
			xDest = player->x-1;
			if(IsBlockingXY(player->x-1, player->y)) return;
			LockChunk(xDest, yDest);
				if(!IsPlayerBlocking(xDest,yDest,player)) {
					--player->x;
					player->xMilis=MILIS_DELTA;
				}
			UnlockChunk(xDest, yDest);
			player->direction=DIRECTION_LEFT;
			} break;
		case 'd': {
			yDest = player->y;
			xDest = player->x+1;
			if(IsBlockingXY(xDest, yDest)) return;
			LockChunk(xDest, yDest);
				if(!IsPlayerBlocking(xDest,yDest,player)) {
					++player->x;
					player->xMilis=-MILIS_DELTA;
				}
			UnlockChunk(xDest, yDest);
			player->direction=DIRECTION_RIGHT;
			} break;
	}
}

void UpdatePlayer(Player* player, char* commands){
	if(commands!=0) {
		//Jezeli wiadomosc to komenda
		if(commands[0]==COMMAND) {
			if(commands[1]==COMMAND_NAME) { //Tylko ustawianie imienia
				#ifdef _DEBUG_
					printf(CLR_B"(%s)"CLR_N" Changing name to: %s\n",player->name,commands+2);
				#endif
				SetPlayerName(player,commands+2);
			} else if(commands[1]==COMMAND_MOVE) { //Ruszanie
				if(player->xMilis==0 && player->yMilis==0)
					MovePlayer(player, commands[2]);
			} else if(commands[1]==COMMAND_LOGOUT) {
				player->playerThread->alive=THREAD_DEAD;
				#ifdef _DEBUG_
					printf(CLR_Y"(%s)"CLR_N" Logged out, thread becoming dead\n",player->name);
				#endif
			} else if(commands[1]==0x8) {
				float angle;
				char* Pangle = (char*)&angle;
				int i=0;
				for(i=0;i<sizeof(float);++i) Pangle[i]=commands[2+i];
				#ifdef _DEBUG_
					printf(CLR_C"(%s)"CLR_N" Received bullet shot %.2f deg\n",player->name,angle);
				#endif
				CreateBullet((float)player->x, (float)player->y, angle);
			}
		}
		
		if(player->xMilis>0) player->xMilis-=MILIS_INC;
		else if(player->xMilis<0) player->xMilis+=MILIS_INC;
		if(player->yMilis>0) player->yMilis-=MILIS_INC;
		else if(player->yMilis<0) player->yMilis+=MILIS_INC;
		//Wyzeruj: nie powtarzaj komendy
		commands[0]=0x0;
	}
}

void DestroyPlayer(Player* player) {
	playerPtrs[player->index]=0;
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
	unsigned int no_read;
	unsigned int w;
	unsigned long sleep_u;
	
	//Zablokuj watek przed anulowaniem, poniewaz mamy otwarty socket
	pthread_mutex_lock(this->safety_mutex);
	
	while(this->alive == THREAD_ALIVE) {
		//Odczytaj co wysyla klient
		r = read(socketf,buffer,1023);
		
		if(r < 0) printerr(player->name,"Error reading from socket");
		else if(r==0) {
			Tick(&tv,&sleep_u);
			UpdatePlayer(player,0);
			sleep_u = Tock(&tv,&sleep_u);
			#ifdef _HARDDEBUG_
				warn(player->name,"No read");
			#endif
			no_read++;
			usleep(1000*25-sleep_u);
			if(no_read>=MAX_NOREAD) this->alive=THREAD_DEAD;
		} else {
			no_read=0;
			Tick(&tv,&sleep_u);
			this->last_active = time(NULL);
			UpdatePlayer(player,buffer);
			w=PreparePlayerMap(buffer,player->x,player->y,map,player);
			write(socketf, buffer, w);
			sleep_u = Tock(&tv,&sleep_u);
			#ifdef _HARDDEBUG_
				printf(CLR_C"(%s)"CLR_N" Sleeping for %lu usec\n",player->name,1000*25-sleep_u);
			#endif
			usleep(1000*25-sleep_u);
		}
	}
	
	#ifdef _DEBUG_
		warn(player->name,"Closing socket");
	#endif
	close(socketf);
	pthread_mutex_unlock(this->safety_mutex);
	//Odblokowany watek mozna anulowac
	
	pthread_exit(NULL);
}

void StartPlayerThread(Thread* thread, int index, int socket, Player** playerptrs) {
	#ifdef _DEBUG_
		printf(CLR_C"Creating player [%d] and passing free thread and socket.\n"CLR_N,index);
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
			printf(CLR_Y"Freeing player %d\n"CLR_N,index);
		#endif
		free(playerptrs[index]);
	}
		playerptrs[index]=player;
	#ifdef _DEBUG_
		printf(CLR_C"Starting player #%d thread.\n"CLR_N,index);
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
