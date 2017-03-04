#ifndef UTILS_H
#define UTILS_H

#define THREAD_ALIVE 1
#define THREAD_DEAD 0
#define THREAD_MAX_IDLE 3;
#define MAX_THREADS 5 //Maksymalna ilosc podlaczonych graczy

#define WELCOME_BYTE 0x7
#define WELCOME_NOSPACE 0x15
#define WELCOME_MESSAGE "\x7Welcome to gameserver!"
#define WELCOME_MESSAGE_LENGTH strlen(WELCOME_MESSAGE)
#define WELCOME_NOSPACE_MESSAGE "\x15No more room!"
#define WELCOME_NOSPACE_LENGTH strlen(WELCOME_NOSPACE_MESSAGE)

#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

typedef struct {
	pthread_t* self; //Watek itself
	pthread_mutex_t* safety_mutex; //Blokada dla watkow graczy (inaczej nie uzywane)
	unsigned long last_active; //Ostatnio aktywny dla graczy (inaczej nie uzywane)
	char alive; //Czy watek ma sie krecic: while(thread->alive == THREAD_ALIVE)
	void* attachment; //Zalacznik (przesylamy tu dodatkowe dane dla pracy watku)
} Thread; //Podstawowa struktura watku

void InitThreadArray(Thread* threads,size_t size);
void KillThread(Thread* thread);
void CancelThread(Thread* thread);

#endif
