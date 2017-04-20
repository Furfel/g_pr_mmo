#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"
#include <sys/time.h>

#ifdef _DEBUG_
	#include <stdio.h>
#endif

void InitThreadArray(Thread* threads, size_t size) {
	int i;
	for(i=0;i<size;++i) {
		threads[i].alive=THREAD_DEAD;
		threads[i].self = 0;
		threads[i].attachment = 0;
		threads[i].safety_mutex = 0;
	}
}

void KillThread(Thread* thread) {
	thread->alive = THREAD_DEAD;
}

void CancelThread(Thread* thread) {
	if(thread->alive == THREAD_ALIVE)
		KillThread(thread);
	if(thread->safety_mutex!=0) {
		#ifdef _DEBUG_
			printf(CLR_R "CancelThread:" CLR_N " Locking safety mutex.\n");
		#endif
		pthread_mutex_lock(thread->safety_mutex);
		pthread_mutex_unlock(thread->safety_mutex);
		pthread_mutex_destroy(thread->safety_mutex);
	}
	#ifdef _DEBUG_
		printf(CLR_R "CancelThread:" CLR_N " Cancelling thread.\n");
	#endif
	pthread_cancel(*(thread->self));
	pthread_join(*(thread->self),NULL);
	free(thread->self);
	thread->attachment = 0;
	#ifdef _DEBUG_
		printf(CLR_R "CancelThread:" CLR_N " Freed!\n");
	#endif
}

short SwapShort(short in) {
	short tmp = in;
	SwapEndian((char*)&tmp,sizeof(short));
	return tmp;
}

double SwapDouble(double in) {
	double tmp = in;
	SwapEndian((char*)&tmp,sizeof(double));
	return tmp;
}

float SwapFloat(float in) {
	float tmp = in;
	SwapEndian((char*)&tmp,sizeof(float));
	return tmp;
}

int SwapInt(int in) {
	int tmp = in;
	SwapEndian((char*)&tmp,sizeof(int));
	return tmp;
}

long SwapLong(long in) {
	long tmp = in;
	SwapEndian((char*)&tmp,sizeof(long));
	return tmp;
}

void SwapEndian(char* bytes, int n) {
	char tmpbytes[n];
	int i=0;
	for(i=0;i<n;++i) tmpbytes[i]=bytes[i];
	for(i=0;i<n;++i) bytes[n-1-i]=tmpbytes[i];
}

void Tick(struct timeval* tv, unsigned long* usec) {
	gettimeofday(tv,NULL);
	*usec = tv->tv_sec * 1000000 + tv->tv_usec;
}

unsigned long Tock(struct timeval* tv, unsigned long* usec) {
	gettimeofday(tv,NULL);
	return (tv->tv_sec*1000000+tv->tv_usec)-(*usec);
}
