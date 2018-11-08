// NAME: Richard Khillah
// EMAIL: RKhillah@ucla.edu
// ID: 604853262

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>

#include <pthread.h>
#include <string.h>
#include <sched.h>
#include <time.h>
#include <signal.h>

#include "SortedList.h"
#include "utils.h"
#include "helper.h"

#define SEC_TO_NS_CFACTOR 1000000000

SortedList_t* heads;
SortedListElement_t* elements;
pthread_mutex_t* locks;
int* spinLocks;

long long* thread_times;

long long getElapsedTime(struct timespec start, struct timespec end) {
	long long elapsedtime = (end.tv_sec - start.tv_sec) * SEC_TO_NS_CFACTOR;
    elapsedtime += end.tv_nsec;
    elapsedtime -= start.tv_nsec;

    return elapsedtime;
}

int hash(const char* key) {
	int i = 0;
	int id = 0;
	while(key[i] != '\0') {
		id += key[i++];
	}

	return abs(id) % numLists;
}

void * thread_routine(void* arg) {
	int tid = *((int *)arg);
	int num_ops = numThreads * numIterations;
	struct timespec beforeLock, afterLock;

	for(int i = tid; i < num_ops; i+=numThreads) {
		int listId = hash(elements[i].key); // get the list elemenet belongs
		SortedList_t* head = heads+listId; // get the correct list
		switch(sync_type) {
			case NONE: {
				SortedList_insert(head, elements+i);
				break;
			}
			case MUTEX: {
				clock_gettime(CLOCK_MONOTONIC, &beforeLock);
				if(pthread_mutex_lock(locks+listId) != 0) {
					fatal_error("Error getting the lock", 0, 2);
				}
				clock_gettime(CLOCK_MONOTONIC, &afterLock);
				thread_times[tid] += getElapsedTime(beforeLock, afterLock);

				SortedList_insert(head, elements+i);

				if(pthread_mutex_unlock(locks+listId) != 0) {
					fatal_error("Error releasing the lock", 0, 2);
				}
				break;
			}
			case SPIN: {
				clock_gettime(CLOCK_MONOTONIC, &beforeLock);
				while(__sync_lock_test_and_set(spinLocks+listId, 1));
				clock_gettime(CLOCK_MONOTONIC, &afterLock);
				thread_times[tid] += getElapsedTime(beforeLock, afterLock);
				
				SortedList_insert(head, elements+i);

				__sync_lock_release(spinLocks+listId);
				break;
			}
		}
	}

	int total_length = 0;
	for(int i = 0; i < numLists; i++) {
		SortedList_t* head = heads+i;
		switch(sync_type) {
			case NONE: {
				total_length += SortedList_length(head);
				break;
			}
			case MUTEX: {
				clock_gettime(CLOCK_MONOTONIC, &beforeLock);
				if(pthread_mutex_lock(locks+i) != 0) {
					fatal_error("Error getting the lock", 0, 2);
				}
				clock_gettime(CLOCK_MONOTONIC, &afterLock);
				thread_times[tid] += getElapsedTime(beforeLock, afterLock);

				total_length += SortedList_length(head);

				if(pthread_mutex_unlock(locks+i) != 0) {
					fatal_error("Error releasing the lock", 0, 2);
				}
				break;
			}
			case SPIN: {
				clock_gettime(CLOCK_MONOTONIC, &beforeLock);
				while(__sync_lock_test_and_set(spinLocks+i, 1));
				clock_gettime(CLOCK_MONOTONIC, &afterLock);
				thread_times[tid] += getElapsedTime(beforeLock, afterLock);

				total_length += SortedList_length(head);

				__sync_lock_release(spinLocks+i);
				break;
			}
		}

	}
	
	// delete items
	for(int n = tid; n < num_ops; n+=numThreads) {
		int listId = hash(elements[n].key); // get the list elemenet belongs
		SortedList_t* head = heads + listId; // get the correct list
		switch(sync_type) {
			case NONE: {
				SortedListElement_t *el = SortedList_lookup(head, elements[n].key);
				if(el == NULL) {
					fatal_error("Element not found. NULL element lookup.", 0, 2);
				}
				SortedList_delete(el);
				break;
			}
			case MUTEX: {
				clock_gettime(CLOCK_MONOTONIC, &beforeLock);
				if(pthread_mutex_lock(locks+listId) != 0) {
					fatal_error("Error getting the lock", 0, 2);
				}
				clock_gettime(CLOCK_MONOTONIC, &afterLock);
				thread_times[tid] += getElapsedTime(beforeLock, afterLock);

				SortedListElement_t *el = SortedList_lookup(head, elements[n].key);
				if(el == NULL) {
					fatal_error("Element not found. NULL element lookup.", 0, 2);
				}

				SortedList_delete(el);
				
				if(pthread_mutex_unlock(locks+listId) != 0) {
					fatal_error("Error releasing the lock", 0, 2);
				}
				break;
			}
			case SPIN: {
				clock_gettime(CLOCK_MONOTONIC, &beforeLock);
				while(__sync_lock_test_and_set(spinLocks+listId, 1));
				clock_gettime(CLOCK_MONOTONIC, &afterLock);
				thread_times[tid] += getElapsedTime(beforeLock, afterLock);

				SortedListElement_t *el = SortedList_lookup(head, elements[n].key);
				if(el == NULL) {
					fatal_error("Element not found. NULL element lookup.", 0, 2);
				}
				SortedList_delete(el);

				__sync_lock_release(spinLocks+listId);
				break;
			}
		}
	}

	return NULL;
}

void init() {
	/* Create a circular list */
	heads = (SortedList_t *)malloc(numLists * sizeof(SortedList_t));
	if( heads == NULL ) {
		fatal_error("Error initializing list", 0, 1);
	}

	locks = (pthread_mutex_t*)malloc(numLists*(sizeof(pthread_mutex_t)));
	spinLocks = (int*)malloc(numLists*sizeof(int));
	if(locks == NULL || spinLocks == NULL) {
		fatal_error("Error initializing locks", NULL, 1);
	}

	for(int i = 0; i < numLists; i++) {
		pthread_mutex_init(locks+i, NULL);
		spinLocks[i] = 0;

		SortedList_t* head = heads+i;
		head->next = head;
	    head->prev = head;
	    head->key = NULL;
	}

	/* initialize thread_times to 0 using calloc. this ensure:
	 *   1. threadtimes are recoreded accurately
	 *   2. in the non-sync cases, the non case is not affected
	 */
	thread_times = (long long*)calloc(numThreads, sizeof(long long));
	if (thread_times == NULL) {
		fatal_error("Error creating thread_times for threads", 0, 1);
	}

	/* generate random elements with random lengths for each thread */
	int numElements = numThreads * numIterations;
	elements = (SortedListElement_t *)malloc(numElements * sizeof(SortedListElement_t));
	if( elements == NULL ) {
		fatal_error("Error initializing SortedList elements", 0, 1);
	}

	for (int i = 0; i < numElements; i++) {
		int random_element_len = 3 + (rand() % 8);
		char* key = (char *)malloc(random_element_len * sizeof(char));
		if(key == NULL) {
			fatal_error("Error creating random element", 0, 1);
		}

		for(int j = 0; j < random_element_len - 1; j++) {
			key[j] = (char)(rand() % 255 + 1);
		}

		key[random_element_len-1] = '\0';
		elements[i].key = key; // = &key[0]; //??
	}
}

int main(int argc, char* argv[]) {
	set_program_name(argv[0]);
	get_options(argc, argv);
	signal(SIGSEGV, handle_sig);

	/* Allocate memory for the threadPool, thread_ids (tids), and thread_times */
	pthread_t* threadPool = (pthread_t *)malloc(numThreads * sizeof(pthread_t));
	if(threadPool == NULL) {
		fatal_error("Error creating thread pool", 0, 1);
	}

	int* tids = (int *)malloc(numThreads * sizeof(int));
	if(tids == NULL) {
		fatal_error("Error creating thread tid list", 0, 1);
	}

	init();

	// Get start time
	struct timespec time_start, time_end;
	clock_gettime(CLOCK_MONOTONIC, &time_start);

	for (int k = 0; k < numThreads; k++) {
		tids[k] = k;
		if(pthread_create(threadPool + k, NULL, thread_routine, tids + k) != 0) {
			fatal_error("Error creating threads", 0, 2);
		}
	}

	for(int n = 0; n < numThreads; n++) {
		if (pthread_join(threadPool[n], NULL) != 0) {
			fatal_error("Error joining threads", 0, 2);
		}
	}

	// Get end time
	clock_gettime(CLOCK_MONOTONIC, &time_end);

	int total_list_length = 0;
	for(int i = 0; i < numLists; i++){
		total_list_length += SortedList_length(heads+i);
	}
	if(total_list_length != 0) {
		fatal_error("List is not list 0", 0, 2);
	}

	long long time_elapsed = (time_end.tv_sec - time_start.tv_sec) * SEC_TO_NS_CFACTOR;
	time_elapsed += time_end.tv_nsec;
	time_elapsed -= time_start.tv_nsec;

	long long numOperations = numThreads * numIterations * 3;
	long long time_average = time_elapsed / numOperations;
	long long total_lock_acquisition_time = 0;
	
	/* add up total lock acquisition time and divide by number of operations
	 * to compute average wait-time for lock
	 */
	for (int i = 0; i < numThreads; i++) {
		total_lock_acquisition_time += thread_times[i];
	}
	total_lock_acquisition_time /= numOperations;

	tag();
	printf(",%d,%d,%d,%lld,%lld,%lld,%lld\n", numThreads, numIterations, numLists, numOperations, time_elapsed, time_average, total_lock_acquisition_time);


	free(spinLocks);
	free(locks);
	free(heads);
	free(thread_times);
	free(threadPool);
	free(tids);
	free(elements);
	//free(head);
	return 0;
}
