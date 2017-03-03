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
	pthread_t* self;
	pthread_mutex_t* safety_mutex;
	unsigned long last_active;
	char alive;
	void* attachment;
} Thread;

void InitThreadArray(Thread* threads,size_t size);
void KillThread(Thread* thread);
void CancelThread(Thread* thread);

#endif
