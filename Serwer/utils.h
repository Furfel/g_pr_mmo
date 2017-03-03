#ifndef UTILS_H
#define UTILS_H

#define THREAD_ALIVE 1
#define THREAD_DEAD 0
#define THREAD_MAX_IDLE 2500;

#define WELCOME_BYTE 0x7
#define WELCOME_MESSAGE "\x7Welcome to gameserver!"
#define WELCOME_MESSAGE_LENGTH strlen(WELCOME_MESSAGE)

#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

typedef struct {
	pthread_t* self;
	unsigned long last_active;
	char alive;
	void* attachment;
} Thread;

void InitThreadArray(Thread* threads,size_t size);
void KillThread(Thread* thread);
void CancelThread(Thread* thread);

#endif
