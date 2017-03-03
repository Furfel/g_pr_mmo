#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"

#ifdef _DEBUG_
	#include <stdio.h>
#endif

void InitThreadArray(Thread* threads, size_t size) {
	int i;
	for(i=0;i<size;++i) {
		threads[i].alive=THREAD_DEAD;
		threads[i].self = 0;
		threads[i].attachment = 0;
	}
}

void KillThread(Thread* thread) {
	thread->alive = THREAD_DEAD;
}

void CancelThread(Thread* thread) {
	if(thread->alive == THREAD_ALIVE)
		KillThread(thread);
	pthread_cancel(*(thread->self));
	pthread_join(*(thread->self),NULL);
	free(thread->self);
}
