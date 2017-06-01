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

#define COMMAND 0x11
#define COMMAND_NAME 0x2
#define COMMAND_MOVE 0x3
#define COMMAND_LOGOUT 0xF

#define DATA_HEADER_BULLET 0x40
#define DATA_HEADER_PLAYER 0x50
#define DATA_HEADER_MAP 0x60
#define SECTION_END 0x17

#define CLR_R "\x1b[1;31m"
#define CLR_G "\x1b[1;32m"
#define CLR_B "\x1b[1;34m"
#define CLR_C "\x1b[1;36m"
#define CLR_M "\x1b[1;35m"
#define CLR_Y "\x1b[1;33m"
#define CLR_N "\x1b[0m"

#define warn(caller,msg) (caller==NULL)?printf(CLR_Y "%s" CLR_N "\n",msg) : printf(CLR_Y "%s:" CLR_N " %s\n",caller?caller:"",msg)
#define printerr(caller,msg) (caller==NULL)?printf((const char*)(CLR_R "%s" CLR_N "\n"),msg) : printf((const char*)(CLR_R "%s:" CLR_N " %s\n"),caller?caller:"",msg)
#define printinfo(caller,msg) (caller==NULL)?printf((const char*)(CLR_C "%s" CLR_N "\n"),msg) : printf((const char*)(CLR_C "%s:" CLR_N " %s\n"),caller?caller:"",msg)
#define success(caller,msg) (caller==NULL)?printf(CLR_G "%s" CLR_N "\n",msg) : printf(CLR_G "%s:" CLR_N " %s\n",caller?caller:"",msg)

#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>

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

short SwapShort(short);
double SwapDouble(double);
float SwapFloat(float);
int SwapInt(int);
long SwapLong(long);
void SwapEndian(char*, int);

void Tick(struct timeval* tv, unsigned long* usec);
unsigned long Tock(struct timeval* tv, unsigned long* usec);

pthread_mutex_t destruction_mutex;

#endif
