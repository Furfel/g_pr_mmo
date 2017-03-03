#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "utils.h"

/*
 * Tu trzymamy naszego psa wartownika i strukture Thread ktora ma imitowac sobie Jave ^^
 * Wartownik zabija Thready ktore sa za stare (czyli np graczowi sie klient zawiesil
 * wiec usuwamy go z gry po okolo THREAD_MAX_IDLE
 * */

typedef struct {
	Thread* threads;
	size_t size;
	useconds_t sleep;
	unsigned int* alive_threads;
} WatchdogThreadAttachment;

void StartWatchdog(Thread* watchdogThread, Thread* threads, size_t size, useconds_t sleep, unsigned int* alive_return);

#endif
