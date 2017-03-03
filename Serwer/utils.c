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
			printf("CancelThread: Locking safety mutex.\n");
		#endif
		pthread_mutex_lock(thread->safety_mutex);
		pthread_mutex_unlock(thread->safety_mutex);
		pthread_mutex_destroy(thread->safety_mutex);
	}
	#ifdef _DEBUG_
		printf("CancelThread: Cancelling thread.\n");
	#endif
	pthread_cancel(*(thread->self));
	pthread_join(*(thread->self),NULL);
	free(thread->self);
	thread->attachment = 0;
	#ifdef _DEBUG_
		printf("CancelThread: Freed!\n");
	#endif
}
