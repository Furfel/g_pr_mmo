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
#define INVENTORY_SIZE 10
#define PLAYER_START_X 12
#define PLAYER_START_Y 12
#define PLAYER_VIEW_RADIUS 3
#define MILIS_DELTA 32
#define MILIS_INC 2
#define DIRECTION_DOWN 1
#define DIRECTION_UP 2
#define DIRECTION_LEFT 4
#define DIRECTION_RIGHT 3
#define PLAYER_LIFE_MAX 100

#define MAX_NOREAD 250

typedef struct {
	char name[PLAYER_NAME_LENGTH]; //Imie gracza
	Thread* playerThread; //Referencja do watku gracza
	short x; //Pozycja x
	short y; //Pozycja y
	int index; //Informacyjnie index w tablicy playerPtrs i main.c: players
	char direction;
	signed char xMilis;
	signed char yMilis;
	char inventory[INVENTORY_SIZE];
	char life;
} Player;

Player* playerPtrs[MAX_THREADS]; //Wskazniki dla graczy (pomocniczo, moze sie przydac aby nie bylo wyciekow)

typedef struct {
	Player* player; //Referencja do gracza
	int socket; //Unikatowy socket gracza/klienta (nie serwera)
	int index; //Informacyjnie index w tablicy playerPtrs i main.c: players
} PlayerThreadAttachment;

void DestroyPlayer(Player* player);
void StartPlayerThread(Thread* thread, int index, int socket, Player** playerptrs);
void InitPlayerPtrArray(Player** playerptrs, size_t size);
void Take(Player* player);
void HurtPlayer(Player* player, int amount);

#endif
