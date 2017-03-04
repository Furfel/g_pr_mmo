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
	char name[PLAYER_NAME_LENGTH]; //Imie gracza
	Thread* playerThread; //Referencja do watku gracza
	int x; //Pozycja x
	int y; //Pozycja y
	int index; //Informacyjnie index w tablicy playerPtrs i main.c: players
} Player;

typedef struct {
	Player* player; //Referencja do gracza
	int socket; //Unikatowy socket gracza/klienta (nie serwera)
	int index; //Informacyjnie index w tablicy playerPtrs i main.c: players
} PlayerThreadAttachment;

void DestroyPlayer(Player* player);
void StartPlayerThread(Thread* thread, int index, int socket, Player** playerptrs);
void InitPlayerPtrArray(Player** playerptrs, size_t size);

#endif
