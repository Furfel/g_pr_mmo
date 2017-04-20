#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "watchdog.h"
#include "utils.h"

#ifdef _DEBUG_
	#include <stdio.h>
#endif

void* WatchdogFunction(void* arg) {
	#ifdef _DEBUG_
		printf(CLR_C"Watchdog:"CLR_N" Starting...\n");
	#endif
	
	Thread* this = (Thread*)arg;
	WatchdogThreadAttachment* attachment = this->attachment;
		useconds_t sleep = attachment->sleep;
		unsigned int* alive_threads_ptr = attachment->alive_threads;
		Thread* threads = attachment->threads;
		size_t size = attachment->size;
	free(attachment);
	
	#ifdef _DEBUG_
		int debug_iters = 0;
		#define MIN_DEBUG_ITERS 15
	#endif
	
	unsigned long current_time;
	int i;
	unsigned int alive_threads;
	while(this->alive == THREAD_ALIVE) {
		current_time = time(NULL) - THREAD_MAX_IDLE;
		alive_threads = 0;
		for(i=0;i<size;++i) {
			if(threads[i].alive == THREAD_ALIVE) {
				if (threads[i].last_active < current_time) {
					#ifdef _DEBUG_
						printf(CLR_Y"Watchdog:"CLR_N" cancelling thread %d\n",i);
					#endif
					CancelThread(&threads[i]);
				} else {
					++alive_threads;
				}
			}
		}
		if(alive_threads_ptr != 0)
			*(alive_threads_ptr) = alive_threads;
		#ifdef _DEBUG_
			if(debug_iters == MIN_DEBUG_ITERS) {
				printf(CLR_C"Watchdog:"CLR_N" reporting alive threads = %d\n",alive_threads);
				debug_iters=0;
			} else ++debug_iters;
		#endif
		usleep(sleep);
	}
	#ifdef _DEBUG_
		printf(CLR_Y"Watchdog:"CLR_N" exiting\n");
	#endif
	pthread_exit(NULL);
}

void StartWatchdog(Thread* watchdogThread, Thread* threads, size_t size, useconds_t sleep,unsigned int* alive_threads) {
	#ifdef _DEBUG_
		printf(CLR_C"StartWatchdog:"CLR_N" Setting up thread parameters\n");
	#endif
	WatchdogThreadAttachment* attachment = (WatchdogThreadAttachment*)malloc(sizeof(WatchdogThreadAttachment));
	attachment->threads = threads;
	attachment->size = size;
	attachment->sleep = sleep;
	attachment->alive_threads = alive_threads;
	
	watchdogThread->attachment = attachment;
	watchdogThread->alive = THREAD_ALIVE;
	watchdogThread->safety_mutex = 0;
	#ifdef _DEBUG_
		printf(CLR_C"StartWatchdog:"CLR_N" Creating Watchdog thread\n");
	#endif
	pthread_t* tmp = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(tmp, NULL, WatchdogFunction, (void*)watchdogThread);
	watchdogThread->self = tmp;
}
