#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "utils.h"

/*
 * Tu trzymamy naszego psa wartownika i strukture Thread ktora ma imitowac sobie Jave ^^
 * Wartownik zabija Thready ktore sa za stare (czyli np graczowi sie klient zawiesil
 * wiec usuwamy go z gry po okolo THREAD_MAX_IDLE
 * */

typedef struct {
	Thread* threads; //Watki jakie watchdog ma przegladac
	size_t size; //Rozmiar tablicy threads
	useconds_t sleep; //Ile ma odczekac po sprawdzeniu
	unsigned int* alive_threads; //Zwraca informacyjnie ile watkow
} WatchdogThreadAttachment;

//StartWatchdog(Watek wartownika, Watki do pilnowania, rozmiar tablicy watkow, ile spac, zwrot z informacja)
void StartWatchdog(Thread* watchdogThread, Thread* threads, size_t size, useconds_t sleep, unsigned int* alive_return);

#endif
