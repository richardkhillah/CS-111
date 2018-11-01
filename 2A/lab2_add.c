// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include "common.h"
#include "add_member.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <signal.h>

#define SEC_TO_NS_CFACTOR 1000000000


long long counter;

void add(long long *pointer, long long value) {
	long long sum = *pointer + value;

	if (yield)
		sched_yield();
	*pointer = sum;
}

void add_CAS(long long *pointer, long long value) {
	long long temp;
    long long new;
    do {
        temp = *pointer;
        new = temp + value;
        if (yield)
            sched_yield();

    } while(__sync_val_compare_and_swap(pointer, temp, new) != temp);
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int spinLock = 0;

void* thread_routine() {
	/* add 1  */
	int i;
	for(i = 0; i < numIterations; i++) {
		switch (sync_type) {
			case NONE:
				add(&counter, 1);
				break;
			case MUTEX: {
				if (pthread_mutex_lock(&lock) != 0)
					fatal_error2("Error locking pthread mutex lock");
				add(&counter, 1);
				if (pthread_mutex_unlock(&lock) != 0)
					fatal_error2("Error unlcoking pthread mutex lock");
				break;
			}
			case SPIN:{
				while(__sync_lock_test_and_set(&spinLock, 1));

				add(&counter, 1);

				__sync_lock_release(&spinLock);
				break;
			}
			case CAS: {
				add_CAS(&counter, 1);
				break;
			}
			/* No need for default case since get_options() has ensured sync_type options */
		}
	}

	/* add -1 */
	for(i = 0; i < numIterations; i++) {
		add(&counter, -1);

		switch (sync_type) {
			case NONE:
				add(&counter, -1);
				break;
			case MUTEX: {
				if (pthread_mutex_lock(&lock) != 0)
					fatal_error2("Error locking pthread mutex lock");
				add(&counter, -1);
				if (pthread_mutex_unlock(&lock) != 0)
					fatal_error2("Error unlcoking pthread mutex lock");
				break;
			} // end case MUTEX
			case SPIN:{
				while(__sync_lock_test_and_set(&spinLock, 1));

				add(&counter, 1);

				__sync_lock_release(&spinLock);
				break;
			} // end case SPIN
			case CAS: {
				add_CAS(&counter, -1);
				break;
			}	
			/* No need for default case since get_options() has ensured sync_type options */
		} // end switch
	} // end for
	return NULL;
}

int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	get_options(argc, argv);

	signal(SIGSEGV, handle_sig);
	tag();

	/* Initial long long counter to zero */
	counter = 0;

	/* create threadpool based on numThreads. Default = 1 */
	pthread_t *threadPool;
	threadPool = (pthread_t *)malloc(numThreads * sizeof(pthread_t));
	if(threadPool == NULL)
		fatal_error2("Error allocating thread pool");

	/* Get time before setting threads off to run thread_routine */
	struct timespec time_start, time_end;
	clock_gettime(CLOCK_MONOTONIC, &time_start);

	/* set threads off running thread_routine */
	int i;
	for(i = 0; i < numThreads; i++) {
		if(pthread_create(threadPool + i, NULL, thread_routine, NULL) != 0)
			fatal_error2("Error creating a thread.");

	}

	/* rejoin threads to main */
	for(i = 0; i < numThreads; i++) {
		if(pthread_join(threadPool[i], NULL) != 0)
			fatal_error2("Error joining a thread.");

	}

	/* end time - time after threads have been rejoined to main */
	clock_gettime(CLOCK_MONOTONIC, &time_end);	

	/**
	 * Calculate performance metrics: elapsed time, number operations
	 * and average time each operation took to run, then report to console.
	 * Start with less precise time spec granularity then account for high 
	 * granularity time spec. 
	 */
	long long time_elapsed = (time_end.tv_sec - time_start.tv_sec) * SEC_TO_NS_CFACTOR;
	time_elapsed += time_end.tv_nsec;
	time_elapsed -= time_start.tv_nsec;

	long long numOperations = numThreads * numIterations * 2;
	long long time_average = time_elapsed / numOperations;

	printf(",%d,%d,%lld,%lld,%lld,%lld\n", numThreads, numIterations, numOperations, time_elapsed, time_average, counter);

	free(threadPool);

	return 0;
}