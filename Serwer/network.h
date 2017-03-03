#ifndef NETWORK_H
#define NETWORK_H

/*
 * Funkcje sieciowe. Tutaj nasluchujemy nowych polaczen i negocjujemy
 * dodanie nowego gracza do gry
 * */

#include <sys/types.h>
#include <sys/socket.h>
#include "utils.h"

#define ERROR_SOCKET_OPEN -2
#define ERROR_SOCKET_BIND -3

typedef struct {
	Thread* threads;
	size_t size;
	int socket;
} ListenThreadAttachment;

void StartListening(Thread* thread, Thread* playerThreads, size_t size, int socket);
int OpenSocket(int port, char* addr);

#endif
