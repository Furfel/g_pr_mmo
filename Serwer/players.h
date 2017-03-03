#ifndef PLAYERS_H
#define PLAYERS_H

/*
 * Tutaj znajduja sie definicje graczy. Tworzy sie ich na podlaczenie do gry
 * oraz poprzez uzyskanie poprawnego handshake'a od klienta. Tutaj tez
 * siedzi caly kod z odswiezaniem itp
 * */

#include "utils.h"
#include <unistd.h>

#define PLAYER_NAME_LENGTH 30
#define PLAYER_START_X 4
#define PLAYER_START_Y 4

typedef struct {
	char name[PLAYER_NAME_LENGTH];
	Thread* playerThread;
	int x;
	int y;
	int index;
} Player;

Player* playerPtrs[MAX_THREADS];

typedef struct {
	Player* players;
	size_t size;
	useconds_t sleep;
	Thread* self;
} UpdatePlayersThreadArgument;

typedef struct {
	Player* player;
	int socket;
	int index;
} PlayerThreadAttachment;

void DestroyPlayer(Player* player);
void StartPlayerThread(Thread* thread, int index, int socket);
void InitPlayerPtrArray(Player** playerptrs, size_t size);

#endif
