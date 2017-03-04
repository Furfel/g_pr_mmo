#ifndef NETWORK_H
#define NETWORK_H

/*
 * Funkcje sieciowe. Tutaj nasluchujemy nowych polaczen i negocjujemy
 * dodanie nowego gracza do gry
 * */

#include <sys/types.h>
#include <sys/socket.h>
#include "utils.h"
#include "players.h"

#define ERROR_SOCKET_OPEN -2
#define ERROR_SOCKET_BIND -3

typedef struct {
	Thread* threads; //Tablica watkow do ktorych dodajemy nowe na polaczenie
	size_t size; //Rozmiar tej tablicy
	int socket; //Socket serwera
	Player** playerptrs; //Pomocniczo wskazniki na graczy
} ListenThreadAttachment;

//StartListening(watek sluchania, tablica do ktorej dodajemy, rozmiar tablicy, socket serwera, wskazniki)
void StartListening(Thread* thread, Thread* playerThreads, size_t size, int socket, Player** playerptrs);
//OpenSocket(port serwera, zwrot adresu<?>)
int OpenSocket(int port, char* addr);

#endif
